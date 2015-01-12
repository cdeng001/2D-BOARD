#include "ButtonFunctions.h"
#include <string>
#include <iostream>
#include "Monster.h"
#include "Consts.h"
#include "Globals.h"

void changeToStart(int &gameState)
{
	gameState = IDLE;
	return;
}

void changeToPause(int &gameState)
{
	gameState = PAUSE_SCREEN;
	return;
}

void changeToMenu(int &gameState)
{
	gameState = MENU_SCREEN;
	return;
}

void changeToCharacterSelection(int &gameState)
{
	gameState = CHARACTER_SELECTION;
	return;
}

void changeToQuit(int &gameState)
{
	gameState = GAME_QUIT;
	return;
}

