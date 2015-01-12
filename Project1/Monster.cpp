#include "Monster.h"
#include "Consts.h"
#include "stdlib.h"
#include "iostream"
#include "Globals.h"
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
	if (last == " ")
	{
		return first + last;
	}
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


void Monster::render(SDL_Rect& camera)
{
	//If the tile is on screen
	if (checkCollision(camera, mBox))
	{
		//Show the tile
		gMonsterTexture.render(mBox.x - camera.x, mBox.y - camera.y, &gMonsterClips[mType]);
	}
}

void Monster::displayMenu()
{
	LTexture gTextTexture;
	SDL_Color textColor = { 255, 255, 255 };

	gLargeMonsterTexture.render(20, SUBSCREEN_HEIGHT + 20, &gAvatarClips[mType]);

	//display first name
	gTextTexture.loadFromRenderedTextSmall(first, textColor);
	gTextTexture.render(20, SUBSCREEN_HEIGHT + AVATAR_HEIGHT + 30);
	gTextTexture.free();

	//display last name
	gTextTexture.loadFromRenderedTextSmall(last, textColor);
	gTextTexture.render(20, SUBSCREEN_HEIGHT + AVATAR_HEIGHT + 45);
	gTextTexture.free();

	//Render text
	textColor = { 0, 0, 0 };

	//place templates
	gStatTemplate.render(healthBlock.x, healthBlock.y, &gStatClips[HEALTH]);
	gStatTemplate.render(attackBlock.x, attackBlock.y, &gStatClips[ATTACK]);
	gStatTemplate.render(rangeBlock.x, rangeBlock.y, &gStatClips[RANGE]);
	gStatTemplate.render(speedBlock.x, speedBlock.y, &gStatClips[SPEED]);

	//place stat values
	std::string healthDisplay = std::to_string(current_health) + " / " + std::to_string(health);
	gTextTexture.loadFromRenderedText(healthDisplay, textColor);
	gTextTexture.render(healthBlock.x + 50, healthBlock.y);

	std::string attackDisplay = std::to_string(attack);
	gTextTexture.loadFromRenderedText(attackDisplay, textColor);
	gTextTexture.render(attackBlock.x + 50, attackBlock.y);

	std::string rangeDisplay = std::to_string(range);
	gTextTexture.loadFromRenderedText(rangeDisplay, textColor);
	gTextTexture.render(rangeBlock.x + 50, rangeBlock.y);

	std::string speedDisplay = std::to_string(speed);
	gTextTexture.loadFromRenderedText(speedDisplay, textColor);
	gTextTexture.render(speedBlock.x + 50, speedBlock.y);

	gTextTexture.free();
	return;
}

void Monster::displayAction(SDL_Rect &camera)
{
	LTexture gTextTexture;
	SDL_Color textColor = { 255, 255, 255 };

	gLargeMonsterTexture.render(20, SUBSCREEN_HEIGHT + 20, &gAvatarClips[mType]);

	//display first name
	gTextTexture.loadFromRenderedTextSmall(first, textColor);
	gTextTexture.render(20, SUBSCREEN_HEIGHT + AVATAR_HEIGHT + 30);
	gTextTexture.free();

	//display last name
	gTextTexture.loadFromRenderedTextSmall(last, textColor);
	gTextTexture.render(20, SUBSCREEN_HEIGHT + AVATAR_HEIGHT + 45);
	gTextTexture.free();

	iconSet[damageType]->render(camera);
	return;
}

bool Monster::checkInRange(Monster* target, int r)
{
	bool inRange = false;
	//check if it is same monster
	if ((target->getCol() == (col)) && (target->getRow() == (row)))
	{
		return inRange;
	}

	if (current_health <= 0)
	{
		return inRange;
	}

	int check = abs(target->getCol() - (col)) + abs(target->getRow() - (row));
	if (r = MOVEMENT)
	{
		if (check <= target->speed)
		{
			inRange = true;
		}
	}
	else if (r = BATTLE)
	{
		if (check <= target->range)
		{
			inRange = true;
		}
	}

	return inRange;
}

bool Monster::moveUnit(int x, int y)
{
	bool success = true;

	int check = abs(x - (col + 1)) + abs(y - (row + 1));

	for (int i = 0; i < TEAM_SIZE; i++)
	{
		if ((team1.getMonster(i)->getCol() + 1 == x) && (team1.getMonster(i)->getRow() + 1 == y))
		{
			return false;
		}
		if ((team2.getMonster(i)->getCol() + 1 == x) && (team2.getMonster(i)->getRow() + 1 == y))
		{
			return false;
		}
	}

	if (check <= speed)
	{
		col = x - 1;
		row = y - 1;
		mBox.x = (x - 1) * TILE_WIDTH;
		mBox.y = (y - 1) * TILE_HEIGHT;
	}
	else
	{
		success = false;
	}

	return success;
}

void Monster::renderAt(SDL_Rect& camera, int x, int y)
{
	//If the tile is on screen
	if (checkCollision(camera, mBox))
	{
		//Show the tile
		gMonsterTexture.render(x, y, &gMonsterClips[mType]);
	}
}
