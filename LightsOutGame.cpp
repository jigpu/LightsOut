#include <iostream>
#include <stdlib.h>
#include "LightsOutGame.hpp"


using namespace std;


void LightsOutGame::toggleLight(int x, int y) {
	if (x >= width || x < 0 || y >= height || y < 0)
		return; //Assume the caller was just lazy
	
	lights->getTile(x,y)->object = !lights->getTile(x,y)->object;
}


LightsOutGame::LightsOutGame(int width, int height) {
	this->width = width;
	this->height = height;
	lights = new RectangleMap<bool>(width, height, 10, 10);
	
	for (int x=0; x<width; x++)
		for (int y=0; y<height; y++)
			lights->setTile(x, y, new Tile<bool>());
	
	//Initialize the board to some solvable state.
	for (int x=0; x<width; x++)
		for (int y=0; y<height; y++)
			if (rand() % 2 == 0)
				pressButton(x,y);
}


LightsOutGame::~LightsOutGame() {
	for (int x=0; x<width; x++) {
		for (int y=0; y<height; y++) {
			delete lights->getTile(x, y);
		}
	}
}


int LightsOutGame::getHeight() {
	return height;
}


int LightsOutGame::getWidth() {
	return width;
}


void LightsOutGame::getMoveHint(int* suggestedX, int* suggestedY) {
	for (int y=0; y<height; y++) {
		for (int x=0; x<width; x++) {
			if (lights->getTile(x,y)->object) {
				if (y+1 != height) {
					//"Chase the lights"
					*suggestedX = x;
					*suggestedY = y+1;
					return;
				}
				else {
					//Fixing the top
					*suggestedY = 0;
					switch (x) {
						case 0: *suggestedX = 1; return;
						case 1: *suggestedX = 0; return;
						case 2: *suggestedX = 3; return;	
					}
				}
			}
		}
	}
	
	//Unsolvable
	//How on earth we got here after the constructor ensured
	//the board was solvable is beyond me.
	cout << "ERROR: BOARD NOT SOLVABLE! :( Goodbye." << endl;
	exit(-1);
}


void LightsOutGame::pressButton(int x, int y) {
	if (x >= width || x < 0 || y >= height || y < 0)
		return; //Should throw an invalid move exception...
	
	toggleLight(x,y);
	toggleLight(x-1,y);
	toggleLight(x+1,y);
	toggleLight(x,y-1);
	toggleLight(x,y+1);
}


bool LightsOutGame::winningState() {
	for (int y=0; y<height; y++) {
		for (int x=0; x<width; x++) {
			if (lights->getTile(x,y)->object)
				return false;
			
		}
	}
	return true;
}


void LightsOutGame::paint() {
	for (int y=-1; y<height; y++) {
		if (y < 0) {
			cout << "  ";
		}
		else {
			cout << y << " ";
		}
		
		for (int x=0; x<width; x++) {
			if (y < 0) {
				cout << (char)('A' + x) << " ";
			}
			else {
				if (lights->getTile(x,y)->object)
					cout << "* ";
				else
					cout << "  ";
			}
		}
		cout << endl;
	}
}
