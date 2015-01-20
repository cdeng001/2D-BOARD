#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include "Consts.h"
#include "Monster.h"
#include "Button.h"
#include "LTexture.h"
#include "Menu.h"
#include <vector>

enum player{ PLAYER1, PLAYER2 };

class SelectScreen: public Menu
{
public:
	SelectScreen(int a);
	virtual ~SelectScreen();
	virtual void menuDisplay(SDL_Rect &menuCamera, SDL_Rect &windowCamera, int scrolling_offset);
	virtual void getButtonFunction();
	std::string getMonster(int i, Button &b);

	Button* getButton(int i);
	player getSelectedPlayer();
	void togglePlayer();
	
	void setHover(int i);
	int getHover();

	void addList(int i);
	bool checkList(int i);
	void removeList(int ID);

	Button getRemoveButton(int i);

private:
	Button* characterSet[TOTAL_MONSTER_SPRITES];
	player current;
	std::vector<int> list;
	int curr_hover;
	std::vector<Button> teamRemove;

};
