#include "Monster.h"
#include "Consts.h"
#include "stdlib.h"
#include "iostream"
#include <cmath>


Monster::Monster():
Tile(0, 0, 0), health(0), attack(0), speed(0), range(0), damageType(0), weakness(0), current_health(0)
{
	first = " ";
	last = " ";
}

Monster::Monster(int x, int y, int h, int a, int s, int r, int dT, int w, std::string n, int type):
Tile(x, y, type), health(h), attack(a), speed(s), range(r), damageType(dT), weakness(w), current_health(h)
{
	first = n.substr(0, n.find(' '));
	if (n.find(' ') != -1)
	{
		last = n.substr(n.find(' '));
	}
	else
	{
		last = " ";
	}
}


Monster::~Monster()
{
}

int Monster::get_health()
{
	return this->health;
}

int Monster::get_attack()
{
	return this->attack;
}

int Monster::get_speed()
{
	return this->speed;
}

int Monster::get_range()
{
	return this->range;
}

int Monster::get_damageType()
{
	return this->damageType;
}

bool Monster::is_weakness()
{
	bool check = false;
	return check;
}

std::string Monster::get_name()
{
	return this->first + " " + last;
}


SDL_Rect Monster::getBox()
{
	return this->mBox;
}

bool Monster::showRange(Tile* tile[], SDL_Rect &camera)
{
	for (int i = 0; i < TOTAL_TILES; i++)
	{
		int check = abs(tile[i]->getCol() - ( col + 1 ) ) + abs(tile[i]->getRow() - ( row + 1 ) );
		if (check <= range)
		{
			tile[i]->highlightTile(HIGHLIGHT_RANGE_ATTACK, camera);
		}
	}
	return true;
}

bool Monster::showSpeed(Tile* tile[], SDL_Rect& camera, int name)
{
	for (int i = 0; i < TOTAL_TILES; i++)
	{
		int check = abs(tile[i]->getCol() - (col + 1)) + abs(tile[i]->getRow() - (row + 1));
		if (check <= speed)
		{
			tile[i]->highlightTile(name, camera);
		}
	}
	return true;
}

void Monster::doDamageTo(Monster* enemy)
{
	if (!(enemy->checkDead()))
	{
		int newHealth = enemy->current_health - attack;
		if (newHealth <= 0)
		{
			enemy->current_health = 0;
			std::cout << enemy->get_name() << " has been slain by " << this->get_name() << "." << std::endl;
		}
		else
		{
			enemy->current_health = newHealth;
			std::cout << this->get_name() << " does " << this->get_attack() << " damage to " << enemy->get_name() << "." << std::endl;
		}
	}
}

bool Monster::checkDead()
{
	if (current_health <= 0)
	{
		current_health = 0;
		return true;
	}
	return false;
}
