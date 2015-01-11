#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "Tile.h"

class Icon : public Tile
{
public:
	Icon();
	Icon(int x, int y, int type);

	virtual void render(SDL_Rect& camera);
	void setHoverText(int type);
	void showHoverText(int mouse_x, int mouse_y);
	void setLoc(int x, int y);

protected:
	std::string onHoverText;
};