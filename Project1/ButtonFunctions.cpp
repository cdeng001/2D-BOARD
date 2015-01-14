#include "ButtonFunctions.h"
#include <string>
#include <iostream>
#include "Monster.h"
#include "Consts.h"
#include "Globals.h"

void changeToStart(int &gameState)
{
	scrolling_offset = 0;
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

void p1_move(int &phase)
{
	phase = PLAYER1_MOVEMENT;
	return;
}

void p1_battle(int &phase)
{
	phase = PLAYER1_BATTLE;
	return;
}

void p1_end(int &phase)
{
	phase = PLAYER1_END;
	return;
}

void p2_move(int &phase)
{
	phase = PLAYER2_MOVEMENT;
	return;
}

void p2_battle(int &phase)
{
	phase = PLAYER2_BATTLE;
	return;
}

void p2_end(int &phase)
{
	phase = PLAYER2_END;
	return;
}