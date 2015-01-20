#pragma once

#include <string>
#include "Consts.h"
#include "Monster.h"
#include "Button.h"
#include "LTexture.h"
#include "Menu.h"
#include <vector>

class Team
{
public:
	Team();
	~Team();
	int getCurr();
	void addMonster(int i, int x, int y, int val = INT_MAX);
	Monster*& getMonster(int i);
	bool damageMonster(int i, Monster* target);
	bool checkFull();

	int getCurrentAP();
	int getUnusedAP();
	int getMaxAP();
	void setUnusedAP(int i);
	void setCurrentAP(int i);

	int removeMonster(int pos);
	bool teamDead();
private:
	int maxActionPoints, currentActionPoints, unusedActionPoints;
	int cur_capacity;
	Monster* team[TEAM_SIZE];
};