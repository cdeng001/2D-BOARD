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
#include "Globals.h"

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


void Icon::render(SDL_Rect& camera)
{
	//Show the tile
	gOverlay.setAlpha(255);
	gOverlay.render(mBox.x, mBox.y, &gOverlays[mType]);
}

void Icon::showHoverText(int mouse_x, int mouse_y)
{
	if (this->checkClick(mouse_x, mouse_y))
	{
		LTexture gTextTexture;

		gTextTexture.setBlendMode(SDL_BLENDMODE_ADD);
		SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_ADD);
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 50);
		SDL_Rect nBox;
		TTF_SizeText(gFont, onHoverText.c_str(), &nBox.w, &nBox.h);
		nBox.x = mouse_x + nBox.w / 2;
		nBox.y = mouse_y;
		nBox.w += 5;
		SDL_RenderFillRect(gRenderer, &nBox);

		SDL_Color textColor = { 0, 0, 0 };
		gTextTexture.loadFromRenderedText(gFont, onHoverText, textColor);
		gTextTexture.render(nBox.x + 5, nBox.y);

		gTextTexture.free();

	}
}
