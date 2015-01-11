#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

//The tile
class Tile
{
public:
	//Initializes position and type
	Tile(int x, int y, int tileType);

	//Shows the tile
	virtual void render(SDL_Rect& camera);
	virtual bool checkClick(int x, int y);

	//Get the tile type
	int getType();

	//Set row and col
	void setRow(int r);
	void setCol(int c);

	int getRow();
	int getCol();

	//Get the collision box
	SDL_Rect getBox();

	//Highlight tile
	void highlightTile(int s, SDL_Rect &camera);

protected:
	//The attributes of the tile
	SDL_Rect mBox;

	//Tile location on grid
	int row, col;

	//The tile type
	int mType;
};
