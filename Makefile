#--------------------------------------------------------------------
# Clear the implicit built in rules
#--------------------------------------------------------------------
.SUFFIXES:



#--------------------------------------------------------------------
# define project information
#--------------------------------------------------------------------
PROGRAM := $(notdir $(CURDIR))

ifndef TARGET
TARGET := native
else
PREFIX := $(TARGET)-
endif
AS      := $(PREFIX)as
CC      := $(PREFIX)gcc
CXX     := $(PREFIX)g++
AR      := $(PREFIX)ar
OBJCOPY := $(PREFIX)objcopy
LD      := $(CXX)


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

#M4FILES  := $(foreach dir,$(RESOURCE_DIR),$(wildcard $(dir)/*.m4))
#M4OUTS   := $(subst $(RESOURCE_DIR), $(BUILD_DIR), $(basename$(M4FILES)))



#--------------------------------------------------------------------
# project dependencies; include files, libraries, etc
#--------------------------------------------------------------------
INCLUDES := 
LIBDIRS  := 
LIBS     := -lSDL_ttf -lSDL_gfx -lSDL_image -ljpeg -lpng -lz -lSDL  \
            -lfreetype

ifeq ($(TARGET), powerpc-gekko)
INCLUDES := $(INCLUDES) -I$(DEVKITPRO)/libogc/include
LIBDIRS  := $(LIBDIRS) -L$(DEVKITPRO)/libogc/lib/wii
LIBS     := $(LIBS) -lfat -lwiiuse -lbte -lwiikeyboard -logc -lm
endif



#--------------------------------------------------------------------
# code generation options
#--------------------------------------------------------------------
ifeq ($(TARGET), powerpc-gekko)
MACHDEP  := -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float
endif

ifeq ($(DEBUG), true)
CFLAGS   := -DDEBUG -g -O0 $(MACHDEP)
else
CFLAGS   := -g -O2 -Wall $(MACHDEP)
endif

ifeq ($(TARGET), powerpc-gekko)
CFLAGS   := $(CFLAGS) -DHAVE_STDIO_H
else
CFLAGS   := $(CFLAGS) -DPC
endif

CXXFLAGS := $(CFLAGS)
LDFLAGS  := -g $(MACHDEP) -Wl



#--------------------------------------------------------------------
# PHONY targets that don't actually create a file with the target's
# name
#--------------------------------------------------------------------
.PHONY: all package dist binaries resources

all: package

package: binaries resources $(TARGET_DIST)
	cp $(TARGET_BUILD)/$(PROGRAM) $(TARGET_DIST)
	cp $(COMMON_BUILD)/* $(TARGET_DIST) -R

binaries: $(TARGET_BUILD)/$(PROGRAM)

resources: $(XCF_OUT) $(TTF_OUT)



#--------------------------------------------------------------------
# other targets
#--------------------------------------------------------------------
$(TARGET_BUILD)/$(PROGRAM): $(CPP_OUT)
	$(LD) $(CPP_OUT) $(LDFLAGS) $(LIBDIRS) $(LIBS) -o $@


$(CPP_OUT): $(CPP_OUT_PAT): $(CPP_SRC_PAT) $(TARGET_BUILD)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<


$(XCF_OUT): $(XCF_OUT_PAT): $(XCF_SRC_PAT) $(COMMON_BUILD)/graphics
	gimp --no-interface -n \
           --batch \
              '(let \
                  ( (img (car (gimp-xcf-load 1 "$<" "$<"))) ) \
                  (file-png-save-defaults 1 img (car (gimp-image-merge-visible-layers img 0)) "$@" "$@") \
               )' \
           --batch '(gimp-quit TRUE)'


$(TTF_OUT): $(TTF_OUT_PAT): $(TTF_SRC_PAT) $(COMMON_BUILD)/fonts
	cp $< $@


$(BUILD_DIR) $(TARGET_BUILD) $(COMMON_BUILD) $(COMMON_BUILD)/graphics $(COMMON_BUILD)/fonts $(DIST_DIR) $(TARGET_DIST) $(TARGET_DIST)/graphics $(TARGET_DIST)/fonts:
	mkdir -p $@



#--------------------------------------------------------------------
# dependency targets -- This MUST follow all other targets to create
# .o files
#--------------------------------------------------------------------
include $(subst $(SOURCECODE_DIR), $(TARGET_BUILD), $(addsuffix .d, $(basename $(SOURCECODE))))

$(TARGET_BUILD)/%.d: $(SOURCECODE_DIR)/%.c* $(TARGET_BUILD)
	$(CXX) $(CFLAGS) -M $< | sed 's,\(.*\)\.o:,$(TARGET_BUILD)/\1.o $(TARGET_BUILD)/\1.d:,1' > $@



