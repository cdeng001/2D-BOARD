#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include "Consts.h"
#include "Button.h"
#include "LTexture.h"

class Menu
{
public:
	Menu();
	Menu(int a);
	virtual ~Menu();
	virtual void menuDisplay(SDL_Rect &menuCamera);
	virtual void getButtonFunction(int i);

private:
};