#include "Icon.h"
#include "SDL.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include "Consts.h"
#include "stdlib.h"
#include "LTexture.h"
#include "iostream"
#include <cmath>

Icon::Icon() :
Tile(0, 0, 0)
{

}

Icon::Icon(int x, int y, int type) :
Tile(x, y, type)
{

}

void Icon::setHoverText(int type)
{
	switch (type)
	{
	case MELEE_DAMAGE_TYPE:
		this->onHoverText = melee_type;
		break;
	case RANGED_DAMAGE_TYPE:
		this->onHoverText = ranged_type;
		break;
	case FIRE_DAMAGE_TYPE:
		this->onHoverText = fire_type;
		break;
	case ICE_DAMAGE_TYPE:
		this->onHoverText = ice_type;
		break;
	case POISON_DAMAGE_TYPE:
		this->onHoverText = poison_type;
		break;
	case LIGHTING_DAMAGE_TYPE:
		this->onHoverText = thunder_type;
		break;
	case DARK_DAMAGE_TYPE:
		this->onHoverText = dark_type;
		break;
	case HOLY_DAMAGE_TYPE:
		this->onHoverText = holy_type;
		break;
	case ARMOR_DEFENSE:
		this->onHoverText = armor_count;
		break;
	case MAGIC_DEFENSE:
		this->onHoverText = resist_count;
		break;
	case QUESTION_MARK:
		this->onHoverText = "?";
		break;
	case X_MARK:
		this->onHoverText = "X";
		break;
	default:
		break;
	}
	return;
}

void Icon::setLoc(int x, int y)
{
	this->mBox.x = x;
	this->mBox.y = y;
}
