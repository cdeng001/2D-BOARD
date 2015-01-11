#pragma once

#include <string>
#include "Consts.h"
#include "Monster.h"
#include "Button.h"
#include "LTexture.h"
#include "Menu.h"

class Team
{
public:
	Team();
	~Team();
	int getCurr();
	void addMonster(int i, int x, int y);
	Monster*& getMonster(int i);
	void damageMonster(int i, Monster* target);
	bool checkFull();

private:
	int max_capacity;
	int cur_capacity;
	Monster* team[TEAM_SIZE];
};