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

Menu::Menu(int a)
{

}

Menu::~Menu()
{

}


void Menu::menuDisplay(SDL_Rect &menuCamera)
{
	gMenuBackground.render(0, 0, &menuCamera);

	for (int i = 0; i < TOTAL_MENU_OPTIONS; i++)
	{
		buttonSet[i]->render(i);
	}
	return;
}

Menu::Menu()
{
	for (int i = 0; i < TOTAL_MENU_OPTIONS; i++)
	{
		buttonSet[i] = new Button();
		getButtonFunction(i);
		buttonSet[i]->setLoc(100 + (i * 350), 500);
	}
}

void Menu::getButtonFunction(int i)
{
	switch (i)
	{
	case FROM_MENU_START:
		buttonSet[i]->setActionFunction(changeToCharacterSelection);
		buttonSet[i]->setName("Start");
		break;
	case FROM_MENU_QUIT:
		buttonSet[i]->setActionFunction(changeToQuit);
		buttonSet[i]->setName("Quit");
		break;
	default:
		break;
	}
}

