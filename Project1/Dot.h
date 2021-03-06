#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

//The dot that will move around on the screen
class Dot
{
public:
	//The dimensions of the dot
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;

	//Maximum axis velocity of the dot
	static const int DOT_VEL = 10;

	//Initializes the variables
	Dot();

	//Takes key presses and adjusts the dot's velocity
	void handleEvent(SDL_Event& e);

	//Moves the dot and check collision against tiles
	void move(Tile *tiles[]);

	//Centers the camera over the dot
	void setCamera(SDL_Rect& camera, int &mouse_x, int &mouse_y);

	//Shows the dot on the screen
	void render(SDL_Rect& camera);

private:
	//Collision box of the dot
	SDL_Rect mBox;

	//The velocity of the dot
	int mVelX, mVelY;
};