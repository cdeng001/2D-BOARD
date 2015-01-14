#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "Tile.h"

class Monster: public Tile
{
public:
	Monster(int x, int y, int h, int a, int s, int r, int dT, int w, std::string n, int type);
	Monster();
	virtual ~Monster();
	virtual void render(SDL_Rect& camera);
	void renderAt( int a, int b);
	

	virtual int get_health();
	virtual int get_attack();
	virtual int get_speed();
	virtual int get_range();
	virtual int get_damageType();
	virtual bool is_weakness();
	virtual std::string get_name();

	virtual SDL_Rect getBox();
	virtual void displayMenu();
	virtual void displayAction(SDL_Rect &camera);

	bool moveUnit(int x, int y);
	bool doDamageTo(Monster* enemy);
	bool checkDead();
	bool heal();

	bool showRange(Tile* tile[], SDL_Rect& camera);
	bool showSpeed(Tile* tile[], SDL_Rect& camera);
	bool checkInRange(Monster* target, int r);

protected:
	int health, attack, speed, range, mana;
	int current_health, current_mana;
	int damageType, weakness;
	std::string first, last;
};

