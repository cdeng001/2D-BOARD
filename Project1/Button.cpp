#include "Button.h"
#include "SDL.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <iostream>
#include "Consts.h"
#include "stdlib.h"
#include "Globals.h"

Button::Button() :
Tile(0, 0, 0), buttonAction(NULL), buttonName("")
{
	this->mBox.w = BUTTON_WIDTH;
	this->mBox.h = BUTTON_HEIGHT;
}

void Button::setLoc(int x, int y)
{
	mBox.x = x;
	mBox.y = y;
}

bool Button::checkClick(int x, int y, int &gameState)
{
	if ( Tile::checkClick(x,y) )
	{
		buttonAction(gameState);
		return true;
	}
	return false;
}

bool Button::checkClick(int x, int y)
{
	return Tile::checkClick(x, y);
}

void Button::setActionFunction(void(*functPtr)(int &gameState))
{
	this->buttonAction = functPtr;
}

void Button::setName(const std::string &name)
{
	buttonName = name;
	return;
}

void Button::setType(int i)
{
	this->mType = i;
	return;
}

int Button::getType()
{
	return this->mType;
}

void Button::setSize(int w, int h)
{
	mBox.w = w;
	mBox.h = h;
}

std::string Button::getName()
{
	return this->buttonName;
}


void Button::render(int i)
{
	LTexture gTextTexture;
	SDL_Color textColor;
	if (buttonSet[i]->Tile::checkClick(mouse_x, mouse_y))
	{
		gButtonTexture.render(buttonSet[i]->getBox().x, buttonSet[i]->getBox().y, &gButtonClips[HOVER_BUTTON]);
		textColor = { 238, 187, 21 };
	}
	else
	{
		gButtonTexture.render(buttonSet[i]->getBox().x, buttonSet[i]->getBox().y, &gButtonClips[UP_BUTTON]);
		textColor = { 164, 164, 164 };
	}

	gTextTexture.loadFromRenderedText(gFont, buttonName, textColor);
	gTextTexture.render(mBox.x + mBox.w / 4, mBox.y + mBox.h / 4);

	gTextTexture.free();
}

void Button::render(SDL_Rect camera, int y)
{
	SDL_Rect nBox(camera);
	nBox.x = 0;
	nBox.y = 0;

	if (checkCollision(nBox, mBox))
	{
		//Show the tile
		gMonsterTexture.render(mBox.x - camera.x, y, &gMonsterClips[mType]);
	}
}

bool Button::checkClick(int x, int y, SDL_Rect camera)
{
	if (x < camera.x || y < camera.y || y > camera.x + camera.w || y > camera.y + camera.h)
	{
		return false;
	}
	return Tile::checkClick(x, y);
}