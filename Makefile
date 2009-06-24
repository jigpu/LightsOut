#--------------------------------------------------------------------
# Clear the implicit built in rules
#--------------------------------------------------------------------
.SUFFIXES:



#--------------------------------------------------------------------
# define project information
#--------------------------------------------------------------------
PROGRAM      := $(notdir $(CURDIR))
VERSION      := 9999

ifndef TARGET
TARGET := native
endif

ifeq ($(TARGET), wii)
ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif
endif



#--------------------------------------------------------------------
# define project directory structure
#--------------------------------------------------------------------
SOURCE_DIR     := source
SOURCECODE_DIR := $(SOURCE_DIR)/code
RESOURCE_DIR   := $(SOURCE_DIR)/resources

BUILD_DIR      := build
TARGET_BUILD   := $(BUILD_DIR)/$(TARGET)
COMMON_BUILD   := $(BUILD_DIR)/common

DIST_DIR       := dist
TARGET_DIST    := $(DIST_DIR)/$(TARGET)



#--------------------------------------------------------------------
# auto-generate a list of input and output objects
#--------------------------------------------------------------------
to_SRC = $(foreach dir, $(dir $(1)), $(wildcard $(dir)*$(suffix $(1))))
to_OUT = $(subst $(dir $(1)), $(dir $(2)), $(addsuffix $(suffix $(2)), $(basename $(3))))

CPP_SRC_PAT := $(SOURCECODE_DIR)/%.cpp
CPP_OUT_PAT := $(TARGET_BUILD)/%.o
CPP_SRC     := $(call to_SRC, $(CPP_SRC_PAT))
CPP_OUT     := $(call to_OUT, $(CPP_SRC_PAT), $(CPP_OUT_PAT), $(CPP_SRC))

XCF_SRC_PAT := $(RESOURCE_DIR)/graphics/%.xcf
XCF_OUT_PAT := $(COMMON_BUILD)/graphics/%.png
XCF_SRC     := $(call to_SRC, $(XCF_SRC_PAT))
XCF_OUT     := $(call to_OUT, $(XCF_SRC_PAT), $(XCF_OUT_PAT), $(XCF_SRC))

TTF_SRC_PAT := $(RESOURCE_DIR)/fonts/%.ttf
TTF_OUT_PAT := $(COMMON_BUILD)/fonts/%.ttf
TTF_SRC     := $(call to_SRC, $(TTF_SRC_PAT))
TTF_OUT     := $(call to_OUT, $(TTF_SRC_PAT), $(TTF_OUT_PAT), $(TTF_SRC))

M4_SRC_PAT  := $(RESOURCE_DIR)/%.m4
M4_OUT_PAT  := $(COMMON_BUILD)/%
M4_SRC      := $(call to_SRC, $(M4_SRC_PAT))
M4_OUT      := $(call to_OUT, $(M4_SRC_PAT), $(M4_OUT_PAT), $(M4_SRC))



#--------------------------------------------------------------------
# code generation options
#--------------------------------------------------------------------
PREFIX := $(TARGET)-

ifeq ($(TARGET), native)
PREFIX :=
endif

ifeq ($(TARGET), wii)
PREFIX := powerpc-gekko-
endif

AS      := $(PREFIX)as
CC      := $(PREFIX)gcc
CXX     := $(PREFIX)g++
AR      := $(PREFIX)ar
OBJCOPY := $(PREFIX)objcopy
LD      := $(CXX)

ifeq ($(TARGET), wii)
MACHDEP  := -DGEKKO -DWII -mrvl -mcpu=750 -meabi -mhard-float
endif

CFLAGS := $(MACHDEP) -Wall -O2

ifdef DEBUG
CFLAGS   := $(CFLAGS) -DDEBUG -g -O0
endif

CXXFLAGS := $(CFLAGS)
LDFLAGS  := -g $(MACHDEP) -Wl



#--------------------------------------------------------------------
# project dependencies; include files, libraries, etc
#--------------------------------------------------------------------
INCLUDES := 
LIBDIRS  := 
LIBS     := -lSDL_ttf -lSDL_gfx -lSDL_image -ljpeg -lpng -lz -lSDL  \
            -lfreetype

ifeq ($(TARGET), wii)
ifdef DEBUG
LIBS     := $(LIBS) -ldb
endif

INCLUDES := $(INCLUDES) -I$(DEVKITPRO)/libogc/include
LIBDIRS  := $(LIBDIRS) -L$(DEVKITPRO)/libogc/lib/wii
LIBS     := $(LIBS) -lfat -lwiiuse -lbte -lwiikeyboard -logc -lm
endif



#--------------------------------------------------------------------
# PHONY targets that don't actually create a file with the target's
# name
#--------------------------------------------------------------------
.PHONY: all clean distclean nuke package dist binaries resources wii native

all: package $(TARGET)

clean:
	rm -rf $(TARGET_BUILD)

distclean:
	rm -rf $(TARGET_DIST)

nuke:
	rm -rf $(BUILD_DIR)
	rm -rf $(DIST_DIR)

package: binaries resources $(TARGET_DIST)/$(PROGRAM) $(TARGET_DIST)
	cp $(COMMON_BUILD)/* $(TARGET_DIST) -R

binaries: $(TARGET_BUILD)/$(PROGRAM) $(TARGET_BUILD)

resources: $(XCF_OUT) $(TTF_OUT) $(M4_OUT)

wii: $(TARGET_DIST)/boot.elf $(TARGET_DIST)/meta.xml $(TARGET_DIST)/icon.png $(TARGET_DIST)

native:



#--------------------------------------------------------------------
# other targets
#--------------------------------------------------------------------
$(TARGET_DIST)/boot.elf: $(TARGET_DIST)/$(PROGRAM) $(TARGET_DIST)
	mv $(TARGET_DIST)/$(PROGRAM) $(TARGET_DIST)/boot.elf


$(TARGET_DIST)/meta.xml: $(SOURCE_DIR)/$(TARGET)/meta.xml.m4 $(TARGET_DIST)
	m4 -DPROGRAM=$(PROGRAM) -DVERSION=$(VERSION) -DRELEASE_DATE=`date -u +%Y%m%d%H%M%S` $< > $@


$(TARGET_DIST)/icon.png: $(SOURCE_DIR)/$(TARGET)/icon.xcf $(TARGET_DIST)
	gimp --no-interface -n \
           --batch \
              '(let \
                  ( (img (car (gimp-xcf-load 1 "$<" "$<"))) ) \
                  (file-png-save-defaults 1 img (car (gimp-image-merge-visible-layers img 1)) "$@" "$@") \
               )' \
           --batch '(gimp-quit TRUE)'


$(TARGET_DIST)/$(PROGRAM): $(TARGET_BUILD)/$(PROGRAM) $(TARGET_DIST)
	cp $(TARGET_BUILD)/$(PROGRAM) $(TARGET_DIST)


$(TARGET_BUILD)/$(PROGRAM): $(CPP_OUT) $(TARGET_BUILD)
	$(LD) $(CPP_OUT) $(LDFLAGS) $(LIBDIRS) $(LIBS) -o $@


$(CPP_OUT): $(CPP_OUT_PAT): $(CPP_SRC_PAT) $(TARGET_BUILD)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<


$(XCF_OUT): $(XCF_OUT_PAT): $(XCF_SRC_PAT) $(COMMON_BUILD)/graphics
	gimp --no-interface -n \
           --batch \
              '(let \
                  ( (img (car (gimp-xcf-load 1 "$<" "$<"))) ) \
                  (file-png-save-defaults 1 img (car (gimp-image-merge-visible-layers img 1)) "$@" "$@") \
               )' \
           --batch '(gimp-quit TRUE)'


$(TTF_OUT): $(TTF_OUT_PAT): $(TTF_SRC_PAT) $(COMMON_BUILD)/fonts
	cp $< $@


$(M4_OUT): $(M4_OUT_PAT): $(M4_SRC_PAT) $(COMMON_BUILD)
	m4 -DPROGRAM=$(PROGRAM) -DVERSION=$(VERSION) -DRELEASE_DATE=`date -u +%Y%m%d%H%M%S` $< > $@


$(BUILD_DIR) $(TARGET_BUILD) $(COMMON_BUILD) $(COMMON_BUILD)/graphics $(COMMON_BUILD)/fonts $(DIST_DIR) $(TARGET_DIST) $(TARGET_DIST)/graphics $(TARGET_DIST)/fonts:
	mkdir -p $@



#--------------------------------------------------------------------
# dependency targets -- This MUST follow all other targets to create
# .o files
#--------------------------------------------------------------------
include $(subst $(SOURCECODE_DIR), $(TARGET_BUILD), $(addsuffix .d, $(basename $(SOURCECODE))))

$(TARGET_BUILD)/%.d: $(SOURCECODE_DIR)/%.c* $(TARGET_BUILD)
	$(CXX) $(CFLAGS) -M $< | sed 's,\(.*\)\.o:,$(TARGET_BUILD)/\1.o $(TARGET_BUILD)/\1.d:,1' > $@



