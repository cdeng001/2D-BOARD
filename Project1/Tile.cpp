#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include "math.h"
#include "Consts.h"
#include "Monster.h"
#include "LTexture.h"
#include "Tile.h"
#include "Dot.h"
#include "Icon.h"
#include "Button.h"
#include "ButtonFunctions.h"
#include "Menu.h"
#include "SelectScreen.h"
#include "Team.h"
#include "Globals.h"


Tile::Tile(int x, int y, int tileType)
{
	//Get the offsets
	mBox.x = x*TILE_WIDTH;
	mBox.y = y*TILE_HEIGHT;

	row = y;
	col = x;

	//Set the collision box
	mBox.w = TILE_WIDTH;
	mBox.h = TILE_HEIGHT;

	//Get the tile type
	mType = tileType;
}

void Tile::render(SDL_Rect& camera)
{
	//If the tile is on screen
	if (checkCollision(camera, mBox))
	{
		//Show the tile
		gTileTexture.render(mBox.x - camera.x, mBox.y - camera.y, &gTileClips[mType]);
	}
}

int Tile::getType()
{
	return mType;
}

void Tile::setRow(int r)
{
	this->row = r;
	return;
}

void Tile::setCol(int c)
{
	this->col = c;
	return;
}

int Tile::getRow()
{
	return this->row;
}

int Tile::getCol()
{
	return this->col;
}

bool Tile::checkClick(int x, int y)
{
	return ((x >= mBox.x) && x <= (mBox.x + mBox.w)) && ((y >= mBox.y) && y <= (mBox.y + mBox.h));
}

//Functions dealing with highlight implementtion
/**/SDL_Rect setUpRect(SDL_Rect mBox, SDL_Rect camera)
{
	SDL_Rect nBox;
	nBox.x = mBox.x - camera.x;
	nBox.y = mBox.y - camera.y;
	nBox.w = mBox.w;
	nBox.h = mBox.h;

	return nBox;
}
/**/
/**/void drawHighlight(SDL_Rect mBox, SDL_Rect camera, SDL_Color color)
{
	SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, color.a);
	//set up new box for camera and movement
	SDL_Rect nBox(setUpRect(mBox, camera));
	SDL_RenderFillRect(gRenderer, &nBox);
}
/**/
/**/void Tile::highlightTile(int s, SDL_Rect &camera)
{
	//initialize colors, open is green and occupied is red
	if (checkCollision(mBox, camera))
	{
		SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_ADD);

		if (s == HIGHLIGHT_RANGE_MOVEMENT)
		{
			SDL_Color color = { 0, 255, 0, 200 };
			drawHighlight(mBox, camera, color);
		}
		else if (s == HIGHLIGHT_SELECT)
		{
			SDL_Color color = { 239, 232, 20, 100 };
			drawHighlight(mBox, camera, color);
		}
		else if (s == HIGHLIGHT_RANGE_ATTACK)
		{
			SDL_Color color = { 255, 0, 127, 200 };
			drawHighlight(mBox, camera, color);
		}
		else if (s == HIGHLIGHT_HOVER)
		{
			SDL_Color color = { 36, 233, 109, 100 };
			drawHighlight(mBox, camera, color);
		}
		else if (s == HIGHLIGHT_NOT_PASSABLE)
		{
			SDL_Color color = { 255, 0, 0, 200 };
			drawHighlight(mBox, camera, color);
		}
		else if (s == HIGHLIGHT_DEAD)
		{
			gOverlay.setAlpha(50);
			gOverlay.render(mBox.x - camera.x, mBox.y - camera.y, &gOverlays[DEATH_OVERLAY]);
			SDL_Color color = { 128, 128, 128, 200 };
			drawHighlight(mBox, camera, color);
		}
		else if (s == HIGHLIGHT_MELEE_TARGET)
		{
			gOverlay.setAlpha(200);
			gOverlay.render(mBox.x - camera.x, mBox.y - camera.y, &gOverlays[MELEE_ATTACK_MARK]);
			SDL_Color color = { 255, 0, 0, 200 };
			drawHighlight(mBox, camera, color);
		}
		else if (s == HIGHLIGHT_RANGED_TARGET)
		{
			gOverlay.setAlpha(200);
			gOverlay.render(mBox.x - camera.x, mBox.y - camera.y, &gOverlays[RANGE_ATTACK_MARK]);
			SDL_Color color = { 255, 0, 0, 200 };
			drawHighlight(mBox, camera, color);
		}
		else if (s == HIGHLIGHT_MAGIC_TARGET)
		{
			gOverlay.setAlpha(200);
			gOverlay.render(mBox.x - camera.x, mBox.y - camera.y, &gOverlays[MAGIC_ATTACK_MARK]);
			SDL_Color color = { 255, 0, 0, 200 };
			drawHighlight(mBox, camera, color);
		}
		SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);
	}
}
// end functions dealing with highlight implementation

SDL_Rect Tile::getBox()
{
	return mBox;
}