#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include "Tile.h"

class Button : public Tile
{
public:
	Button();
	virtual void render(int i);
	virtual void render(SDL_Rect camera, int y);
	void setLoc(int x, int y);
	void setSize(int w, int h);
	virtual bool checkClick(int x, int y, int &gameState);
	virtual bool checkClick(int x, int y);
	virtual bool checkClick(int x, int y, SDL_Rect camera);
	void setActionFunction(void(*functPtr)(int &gameState));
	
	void setName(const std::string &name);
	std::string getName();

	int getType();
	void setType(int i);

protected:
	//should use functor to create unique button functions
	void(*buttonAction) (int &gameState);
	std::string buttonName;
};