#include "Button.h"
#include "SDL.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <iostream>
#include "Consts.h"
#include "stdlib.h"

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