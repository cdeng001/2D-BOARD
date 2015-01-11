
//Using SDL, SDL_image, standard IO, strings, and file streams
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include "math.h"
#include "Consts.h"
#include "Monster.h"
#include "LTexture.h"
#include "Tile.h"
#include "Dot.h"
#include "Icon.h"
#include "Button.h"
#include "ButtonFunctions.h"
#include "Menu.h"
#include "SelectScreen.h"
#include "Team.h"
#include "Globals.h"


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia(Tile* tiles[]);

//Frees media and shuts down SDL
void close(Tile* tiles[]);

//Box collision detector
bool checkCollision(SDL_Rect a, SDL_Rect b);

//Checks collision box against set of tiles
bool touchesWall(SDL_Rect box, Tile* tiles[]);

//Sets tiles from tile map
bool setTiles(Tile *tiles[]);

//Sets monsters on the map
bool setMonsters();

//Handles events for mouse
void mouseHandle(SDL_Event e, Tile* tiles[], int gameState);

//Handle events for keyboard
void keyBoardHandle(SDL_Event e, Tile* tiles[], SDL_Rect &camera, Monster* &target, int &gameState);

//Clears the bottom sub-screen
void displayBlank();
void populateStatLoc();

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface != NULL)
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}


	//Return success
	return mTexture != NULL;
}
#endif

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedTextSmall(std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFontSmall, textureText.c_str(), textColor);
	if (textSurface != NULL)
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}


	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::queryTexture(int x, int y, int access)
{
	SDL_QueryTexture(mTexture, &(SDL_GetWindowSurface(gWindow)->format->format),&access, &mWidth, &mHeight);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

Tile::Tile(int x, int y, int tileType)
{
	//Get the offsets
	mBox.x = x*TILE_WIDTH;
	mBox.y = y*TILE_HEIGHT;

	row = y;
	col = x;

	//Set the collision box
	mBox.w = TILE_WIDTH;
	mBox.h = TILE_HEIGHT;

	//Get the tile type
	mType = tileType;
}

void Tile::render(SDL_Rect& camera)
{
	//If the tile is on screen
	if (checkCollision(camera, mBox))
	{
		//Show the tile
		gTileTexture.render(mBox.x - camera.x, mBox.y - camera.y, &gTileClips[mType]);
	}
}

int Tile::getType()
{
	return mType;
}

void Tile::setRow(int r)
{
	this->row = r;
	return;
}

void Tile::setCol(int c)
{
	this->col = c;
	return;
}

int Tile::getRow()
{
	return this->row;
}

int Tile::getCol()
{
	return this->col;
}

bool Tile::checkClick(int x, int y)
{
	return ((x >= mBox.x) && x <= (mBox.x + mBox.w)) && ((y >= mBox.y) && y <= (mBox.y + mBox.h));
}

//Functions dealing with highlight implementtion
/**/SDL_Rect setUpRect( SDL_Rect mBox, SDL_Rect camera)
{
	SDL_Rect nBox;
	nBox.x = mBox.x - camera.x;
	nBox.y = mBox.y - camera.y;
	nBox.w = mBox.w;
	nBox.h = mBox.h;

	return nBox;
}
/**/
/**/void drawHighlight(SDL_Rect mBox, SDL_Rect camera, SDL_Color color)
{
	SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, color.a);
	//set up new box for camera and movement
	SDL_Rect nBox(setUpRect(mBox, camera));
	SDL_RenderFillRect(gRenderer, &nBox);
}
/**/
/**/void Tile::highlightTile(int s, SDL_Rect &camera)
{
	//initialize colors, open is green and occupied is red
	if (checkCollision(mBox, camera))
	{
		SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_ADD);

		if (s == HIGHLIGHT_RANGE_MOVEMENT)
		{
			SDL_Color color = { 0, 255, 0, 200 };
			drawHighlight(mBox, camera, color);
		}
		else if (s == HIGHLIGHT_SELECT)
		{
			SDL_Color color = { 239, 232, 20, 100 };
			drawHighlight(mBox, camera, color);
		}
		else if (s == HIGHLIGHT_RANGE_ATTACK)
		{
			SDL_Color color = { 255, 0, 127, 200 };
			drawHighlight(mBox, camera, color);
		}
		else if (s == HIGHLIGHT_HOVER)
		{
			SDL_Color color = { 36, 233, 109, 100 };
			drawHighlight(mBox, camera, color);
		}
		else if (s == HIGHLIGHT_NOT_PASSABLE)
		{
			SDL_Color color = { 255, 0, 0, 200 };
			drawHighlight(mBox, camera, color);
		}
		else if (s == HIGHLIGHT_DEAD)
		{
			gOverlay.setAlpha(50);
			gOverlay.render(mBox.x - camera.x, mBox.y-camera.y, &gOverlays[DEATH_OVERLAY]);
			SDL_Color color = { 128, 128, 128, 200 };
			drawHighlight(mBox, camera, color);
		}
		else if (s == HIGHLIGHT_MELEE_TARGET)
		{
			gOverlay.setAlpha(200);
			gOverlay.render(mBox.x - camera.x, mBox.y - camera.y, &gOverlays[MELEE_ATTACK_MARK]);
			SDL_Color color = { 255, 0, 0, 200 };
			drawHighlight(mBox, camera, color);
		}
		else if (s == HIGHLIGHT_RANGED_TARGET)
		{
			gOverlay.setAlpha(200);
			gOverlay.render(mBox.x - camera.x, mBox.y - camera.y, &gOverlays[RANGE_ATTACK_MARK]);
			SDL_Color color = { 255, 0, 0, 200 };
			drawHighlight(mBox, camera, color);
		}
		else if (s == HIGHLIGHT_MAGIC_TARGET)
		{
			gOverlay.setAlpha(200);
			gOverlay.render(mBox.x - camera.x, mBox.y - camera.y, &gOverlays[MAGIC_ATTACK_MARK]);
			SDL_Color color = { 255, 0, 0, 200 };
			drawHighlight(mBox, camera, color);
		}
		SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);
	}
}
// end functions dealing with highlight implementation

SDL_Rect Tile::getBox()
{
	return mBox;
}

Dot::Dot()
{
	//Initialize the collision box
	mBox.x = 0;
	mBox.y = 0;
	mBox.w = DOT_WIDTH;
	mBox.h = DOT_HEIGHT;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;
}

void Dot::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY -= DOT_VEL; break;
		case SDLK_DOWN: mVelY += DOT_VEL; break;
		case SDLK_LEFT: mVelX -= DOT_VEL; break;
		case SDLK_RIGHT: mVelX += DOT_VEL; break;
		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY += DOT_VEL; break;
		case SDLK_DOWN: mVelY -= DOT_VEL; break;
		case SDLK_LEFT: mVelX += DOT_VEL; break;
		case SDLK_RIGHT: mVelX -= DOT_VEL; break;
		}
	}
}

void Dot::move(Tile *tiles[])
{
	//Move the dot left or right
	mBox.x += mVelX;

	//If the dot went too far to the left or right or touched a wall
	if ((mBox.x < 0) || (mBox.x + DOT_WIDTH > LEVEL_WIDTH) || touchesWall(mBox, tiles))
	{
		//move back
		mBox.x -= mVelX;
	}

	//Move the dot up or down
	mBox.y += mVelY;

	//If the dot went too far up or down or touched a wall
	if ((mBox.y < 0) || (mBox.y + DOT_HEIGHT > LEVEL_HEIGHT) || touchesWall(mBox, tiles))
	{
		//move back
		mBox.y -= mVelY;
	}
}

void Dot::setCamera(SDL_Rect& camera, int &mouse_x, int &mouse_y)
{
	int x_0 = camera.x, y_0 = camera.y;

	//Center the camera over the dot
	camera.x = (mBox.x + DOT_WIDTH / 2) - SUBSCREEN_WIDTH / 2;
	camera.y = (mBox.y + DOT_HEIGHT / 2) - SUBSCREEN_HEIGHT / 2;

	//Keep the camera in bounds
	if (camera.x < 0)
	{
		camera.x = 0;
	}
	if (camera.y < 0)
	{
		camera.y = 0;
	}
	if (camera.x > LEVEL_WIDTH - camera.w)
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if (camera.y > LEVEL_HEIGHT - camera.h)
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}

	mouse_x += camera.x - x_0;
	mouse_y += camera.y - y_0;
}

void Dot::render(SDL_Rect& camera)
{
	//Show the dot
	gDotTexture.render(mBox.x - camera.x, mBox.y - camera.y);
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

void Icon::render(SDL_Rect& camera)
{
		//Show the tile
		gOverlay.setAlpha(255);
		gOverlay.render(mBox.x , mBox.y , &gOverlays[mType]);
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
		gTextTexture.loadFromRenderedText(onHoverText, textColor);
		gTextTexture.render(nBox.x + 5, nBox.y);

		gTextTexture.free();

	}
}

void Button::render( int i)
{
	LTexture gTextTexture;
	SDL_Color textColor;
	if (buttonSet[i]->Tile::checkClick(mouse_x, mouse_y))
	{
		gButtonTexture.render(buttonSet[i]->getBox().x, buttonSet[i]->getBox().y, &gButtonClips[HOVER_BUTTON]);
		textColor = { 238, 187, 21 };
	}
	else
	{
		gButtonTexture.render(buttonSet[i]->getBox().x, buttonSet[i]->getBox().y, &gButtonClips[UP_BUTTON]);
		textColor = { 164, 164, 164 };
	}
	
	gTextTexture.loadFromRenderedText(buttonName, textColor);
	gTextTexture.render(mBox.x + mBox.w / 4, mBox.y + mBox.h / 4);

	gTextTexture.free();
}

void Button::render(SDL_Rect camera, int y)
{
	SDL_Rect nBox(camera);
	nBox.x = 0;
	nBox.y = 0;

	if (checkCollision(nBox, mBox))
	{
		//Show the tile
		gMonsterTexture.render(mBox.x - camera.x, y, &gMonsterClips[mType]);
	}
}

void Menu::menuDisplay(SDL_Rect &menuCamera)
{
	gMenuBackground.render(0, 0, &menuCamera);
	
	for (int i = 0; i < TOTAL_MENU_OPTIONS; i++)
	{
		buttonSet[i]->render( i);
	}
	return;
}

Menu::Menu() 
{
	for (int i = 0; i < TOTAL_MENU_OPTIONS; i++)
	{
		buttonSet[i] = new Button();
		getButtonFunction(i);
		buttonSet[i]->setLoc(100 + (i * 350), 500);
	}
}

void Menu::getButtonFunction(int i)
{
	switch (i)
	{
	case FROM_MENU_START:
		buttonSet[i]->setActionFunction(changeToCharacterSelection);
		buttonSet[i]->setName("Start");
		break;
	case FROM_MENU_QUIT:
		buttonSet[i]->setActionFunction(changeToQuit);
		buttonSet[i]->setName("Quit");
		break;
	default:
		break;
	}
}

SelectScreen::SelectScreen(int a) :
Menu(a), current(PLAYER1), curr_hover(INT_MAX), listSize(0)
{
	for (int i = 0; i < TEAM_SIZE * 2; i++)
	{
		list[i] = NULL;
	}

	buttonSet[SELECT_BUTTON] = new Button();
	buttonSet[SELECT_BUTTON]->setLoc(SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, ((SCREEN_HEIGHT-SCREEN_HEIGHT / 4) - BUTTON_HEIGHT / 2)-5);
	buttonSet[SELECT_BUTTON]->setActionFunction(changeToStart);
	buttonSet[SELECT_BUTTON]->setName("Start");
	getButtonFunction();
}

void SelectScreen::menuDisplay(SDL_Rect &menuCamera, SDL_Rect &windowCamera, int scrolling_offset)
{
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_Rect nBox;
	nBox.x = 0;
	nBox.y = 0;
	nBox.w = SCREEN_WIDTH;
	nBox.h = SCREEN_HEIGHT;
	SDL_RenderFillRect(gRenderer, &nBox);

	gSelectWindow.render(100, 10, &windowCamera);
	gScrollingBG.render(scrolling_offset, 0, &menuCamera);
	gScrollingBG.render(scrolling_offset + gScrollingBG.getWidth(), 0, &menuCamera);
	
	for (int i = 0; i < TOTAL_MONSTER_SPRITES; i++)
	{
		characterSet[i]->render(windowCamera, characterSet[i]->getRow() );
		if (checkList(i))
		{
			gOverlay.render((characterSet[i]->getBox().x - windowCamera.x), characterSet[i]->getRow(), &gOverlays[X_MARK]);
		}
	}

	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	nBox.x = 0;
	nBox.y = 0;
	nBox.w = SCREEN_WIDTH;
	nBox.h = 10;
	SDL_RenderFillRect(gRenderer, &nBox);

	gSelectOverlay.render(0, 0, &menuCamera);
	buttonSet[SELECT_BUTTON]->render(SELECT_BUTTON);
	
	for (int i = 0; i < TEAM_SIZE; i++)
	{
		if (team1.getMonster(i) != NULL)
		{
			team1.getMonster(i)->renderAt(menuCamera, 20, 50 + (TILE_HEIGHT + 10)*i);
		}
		else
		{
			gOverlay.render(20, 50 + (TILE_HEIGHT + 10)*i, &gOverlays[QUESTION_MARK]);
		}
		if (team2.getMonster(i) != NULL)
		{
			team2.getMonster(i)->renderAt(menuCamera, (SCREEN_WIDTH - 20) - TILE_WIDTH, (TILE_HEIGHT + 10)*(i + 1));
		}
		else
		{
			gOverlay.render((SCREEN_WIDTH - 20) - TILE_WIDTH, (TILE_HEIGHT + 10)*(i + 1), &gOverlays[QUESTION_MARK]);
		}
	}
	
	if (curr_hover < TOTAL_MONSTER_SPRITES)
	{

		gLargeMonsterTexture.render(50, (SCREEN_HEIGHT - AVATAR_HEIGHT) - 25, & gAvatarClips[curr_hover]);
		LTexture gTextTexture;

		SDL_Color textColor = { 0, 0, 0 };
		gTextTexture.loadFromRenderedText(characterSet[curr_hover]->getName(), textColor);
		gTextTexture.render(100 + AVATAR_WIDTH, (SCREEN_HEIGHT - AVATAR_HEIGHT) - 25);

		gTextTexture.free();

	}
}

void SelectScreen::getButtonFunction()
{
	int x = 0;
	int h_offset = 50;
	int w_offset = 150;
	enum { WOW = 0, DIABLO = 1, WH = 2, SC = 4};
	while (x < TOTAL_MONSTER_SPRITES)
	{
		for (int j = 0; j < (SCREEN_HEIGHT / TILE_HEIGHT) ; j++)
		{
			for (int k = 0; k < ((SCREEN_WIDTH / TILE_WIDTH)/2); k++)
			{
				if (x >= TOTAL_MONSTER_SPRITES){ return; }
				Button* temp = new Button();
				temp->setLoc(k * TILE_WIDTH + w_offset, j * TILE_HEIGHT + h_offset);
				temp->setRow(j * TILE_HEIGHT + h_offset);
				temp->setName( getMonster(x, *temp) );
				temp->setSize(TILE_WIDTH, TILE_HEIGHT);
				characterSet[x] = temp;
				x++;
			}
			if (j == DIABLO || j == WH || j == SC )
			{
				h_offset += 60;
			}
			if (x >= TOTAL_MONSTER_SPRITES){ return; }
		}
	}
	
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Initialize SDL_mixer
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
			success = false;
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				else
				{
					//Initialize SDL_ttf
					if (TTF_Init() == -1)
					{
						printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
						success = false;
					}
				}
			}
		}
	}

	return success;
}

void loadMusic()
{
	gDiablo_Select = Mix_LoadWAV("sound/diablotaunt.wav");
	if (gDiablo_Select == NULL)
	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}
	gBaal_Select = Mix_LoadWAV("sound/baaltaunt.wav");
	if (gDiablo_Select == NULL)
	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}
	gMephisto_Select = Mix_LoadWAV("sound/mephistotaunt.wav");
	if (gDiablo_Select == NULL)
	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}
	gMalthael_Select = Mix_LoadWAV("sound/malthaeltaunt.wav");
	if (gDiablo_Select == NULL)
	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}
}

bool loadMedia(Tile* tiles[])
{
	//Loading success flag
	bool success = true;

	//Load dot texture
	if (!gDotTexture.loadFromFile("image/dot.bmp"))
	{
		printf("Failed to load dot texture!\n");
		success = false;
	}

	//Load tile texture
	if (!gTileTexture.loadFromFile("image/tiles.png"))
	{
		printf("Failed to load tile set texture!\n");
		success = false;
	}

	if (!gScrollingBG.loadFromFile("image/scrollingBG.png"))
	{
		printf("Failed to load scrolling texture!\n");
		success = false;
	}

	if (!gMonsterTexture.loadFromFile("image/avatars.bmp"))
	{
		printf("Failed to load character avatars!\n");
		success = false;
	}

	if (!gLargeMonsterTexture.loadFromFile("image/avatar_large.png"))
	{
		printf("Failed to load character avatars!\n");
		success = false;
	}

	if (!gLowerSubscreen.loadFromFile("image/lower_subscreen.png"))
	{
		printf("Failed to load lower subscreen!\n");
		success = false;
	}

	if (!gStatTemplate.loadFromFile("image/stat_template.png"))
	{
		printf("Failed to load stat templates!\n");
		success = false;
	}

	if (!gOverlay.loadFromFile("image/overlays.png"))
	{
		printf("Failed to load overlays!\n");
		success = false;
	}

	if (!gButtonTexture.loadFromFile("image/button_images.png"))
	{
		printf("Failed to load button images!\n");
		success = false;
	}

	if (!gMenuBackground.loadFromFile("image/menu_image.png"))
	{
		printf("Failed to load menu image!\n");
		success = false;
	}

	if (!gSelectOverlay.loadFromFile("image/select_overlay.png"))
	{
		printf("Failed to load select overlay!\n");
		success = false;
	}

	if (!gSelectWindow.loadFromFile("image/window_screen.png"))
	{
		printf("Failed to load select window!\n");
		success = false;
	}

	//Load tile map
	if (!setTiles(tiles))
	{
		printf("Failed to load tile set!\n");
		success = false;
	}

	//Open the font
	gFont = TTF_OpenFont("Prototype.ttf", FONT_SIZE);
	if (gFont == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	gFontSmall = TTF_OpenFont("Prototype.ttf", FONT_SIZE - 6);
	if (gFontSmall == NULL)
	{
		printf("Failed to load small font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	setMonsters();
	loadMusic();
	return success;
}

void close(Tile* tiles[])
{
	//Deallocate tiles
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		if (tiles[i] == NULL)
		{
			delete tiles[i];
			tiles[i] = NULL;
		}
	}

	for (int i = 0; i < TOTAL_ICONS; i++)
	{
		if (iconSet[i] == NULL)
		{
			delete iconSet[i];
			iconSet[i] = NULL;
		}
	}

	for (int i = 0; i < TOTAL_MENU_OPTIONS; i++)
	{
		if (buttonSet[i] == NULL)
		{
			delete buttonSet[i];
			buttonSet[i] = NULL;
		}
	}

	//Free loaded images
	gDotTexture.free();
	gTileTexture.free();
	gMonsterTexture.free();
	gLargeMonsterTexture.free();
	gLowerSubscreen.free();
	gStatTemplate.free();
	gOverlay.free();
	gMenuBackground.free();
	gScrollingBG.free();
	gSelectOverlay.free();
	gSelectWindow.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	Mix_FreeChunk(gDiablo_Select);
	gDiablo_Select = NULL;

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

//Clipping Functions
/**/bool setTiles(Tile* tiles[])
{
	//Success flag
	bool tilesLoaded = true;

	//The tile offsets
	int x = 0, y = 0;
	
	//The row and col counters
	int r = 1, c = 1;

	//Open the map
	std::ifstream map("lazy.map");

	//If the map couldn't be loaded
	if (!map.good())
	{
		printf("Unable to load map file!\n");
		tilesLoaded = false;
	}
	else
	{
		//Initialize the tiles
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map >> tileType;

			//If the was a problem in reading the map
			if (map.fail())
			{
				//Stop loading map
				printf("Error loading map: Unexpected end of file!\n");
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				tiles[i] = new Tile(c-1, r-1, tileType);
				tiles[i]->setRow(r);
				tiles[i]->setCol(c);
			}
			//If we don't recognize the tile type
			else
			{
				//Stop loading map
				printf("Error loading map: Invalid tile type at %d!\n", i);
				tilesLoaded = false;
				break;
			}

			//Move to next tile spot
			x += TILE_WIDTH;
			c += 1;

			//If we've gone too far
			if (x >= LEVEL_WIDTH)
			{
				//Move back
				x = 0;
				c = 1;

				//Move to the next row
				y += TILE_HEIGHT;
				r += 1;
			}
		}

		//Clip the sprite sheet
		if (tilesLoaded)
		{
			gTileClips[TILE_RED].x = 0;
			gTileClips[TILE_RED].y = 0;
			gTileClips[TILE_RED].w = TILE_WIDTH;
			gTileClips[TILE_RED].h = TILE_HEIGHT;

			gTileClips[TILE_GREEN].x = 0;
			gTileClips[TILE_GREEN].y = 40;
			gTileClips[TILE_GREEN].w = TILE_WIDTH;
			gTileClips[TILE_GREEN].h = TILE_HEIGHT;

			gTileClips[TILE_BLUE].x = 0;
			gTileClips[TILE_BLUE].y = 80;
			gTileClips[TILE_BLUE].w = TILE_WIDTH;
			gTileClips[TILE_BLUE].h = TILE_HEIGHT;

			gTileClips[TILE_TOPLEFT].x = 40;
			gTileClips[TILE_TOPLEFT].y = 0;
			gTileClips[TILE_TOPLEFT].w = TILE_WIDTH;
			gTileClips[TILE_TOPLEFT].h = TILE_HEIGHT;

			gTileClips[TILE_LEFT].x = 40;
			gTileClips[TILE_LEFT].y = 40;
			gTileClips[TILE_LEFT].w = TILE_WIDTH;
			gTileClips[TILE_LEFT].h = TILE_HEIGHT;

			gTileClips[TILE_BOTTOMLEFT].x = 40;
			gTileClips[TILE_BOTTOMLEFT].y = 80;
			gTileClips[TILE_BOTTOMLEFT].w = TILE_WIDTH;
			gTileClips[TILE_BOTTOMLEFT].h = TILE_HEIGHT;

			gTileClips[TILE_TOP].x = 120;
			gTileClips[TILE_TOP].y = 0;
			gTileClips[TILE_TOP].w = TILE_WIDTH;
			gTileClips[TILE_TOP].h = TILE_HEIGHT;

			gTileClips[TILE_CENTER].x = 120;
			gTileClips[TILE_CENTER].y = 40;
			gTileClips[TILE_CENTER].w = TILE_WIDTH;
			gTileClips[TILE_CENTER].h = TILE_HEIGHT;

			gTileClips[TILE_BOTTOM].x = 120;
			gTileClips[TILE_BOTTOM].y = 120;
			gTileClips[TILE_BOTTOM].w = TILE_WIDTH;
			gTileClips[TILE_BOTTOM].h = TILE_HEIGHT;

			gTileClips[TILE_TOPRIGHT].x = 120;
			gTileClips[TILE_TOPRIGHT].y = 0;
			gTileClips[TILE_TOPRIGHT].w = TILE_WIDTH;
			gTileClips[TILE_TOPRIGHT].h = TILE_HEIGHT;

			gTileClips[TILE_RIGHT].x = 120;
			gTileClips[TILE_RIGHT].y = 40;
			gTileClips[TILE_RIGHT].w = TILE_WIDTH;
			gTileClips[TILE_RIGHT].h = TILE_HEIGHT;

			gTileClips[TILE_BOTTOMRIGHT].x = 120;
			gTileClips[TILE_BOTTOMRIGHT].y = 80;
			gTileClips[TILE_BOTTOMRIGHT].w = TILE_WIDTH;
			gTileClips[TILE_BOTTOMRIGHT].h = TILE_HEIGHT;
		}
	}

	//Close the file
	map.close();

	//If the map was loaded fine
	return tilesLoaded;
}
/**/
/**/void clipTarget(SDL_Rect gRect[], int name, int x, int y, int w, int h)
{
	gRect[name].x = x;
	gRect[name].y = y;
	gRect[name].w = w;
	gRect[name].h = h;
}
/**/
/**/bool setMonsters()
{
		clipTarget(gMonsterClips ,Illidan_Stormrage, 0, 0, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Akama, 40, 0, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Magni_Bronzebeard, 80, 0, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Tyrande_Whisperwind, 120, 0, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Lich_King, 0, 40, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Maiev_Shadowsong, 40, 40, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Baine_Bloodhoof, 80, 40, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Sylvanas_Windrunner, 120, 40, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Glenn_Greymane, 0, 80, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Garrosh_Hellscream, 40, 80, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Varian_Wrynn, 80, 80, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Prophet_Velen, 120, 80, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Goblin_Trade_Prince, 0, 120, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Voljin, 40, 120, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Lorthemar_Theron, 80, 120, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Gelbin_Mekkatorque, 120, 120, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Diablo, 280, 160, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Mephisto, 240, 160, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Baal, 200, 160, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, King_Leoric, 160, 160, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Imperius, 0, 160, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Tyrael, 40, 160, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Malthael, 80, 160, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Izual, 120, 160, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Sebastian_Yarrick, 160, 0, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Darnath_Lysander, 160, 40, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Logan_Grimnar, 160, 80, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Kaldor_Draigo, 160, 120, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Swarm_Lord, 200, 0, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Deathleaper, 200, 40, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Kaela_Meinsha_Khaine, 200, 80, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Astrubael_Vect, 200, 120, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Typhus, 240, 0, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Kharn, 240, 40, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Ahriman, 240, 80, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Lucius, 240, 120, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, O_Shovah, 280, 0, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Imotekh_Stormlord, 280, 40, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Mephiston, 280, 80, TILE_WIDTH, TILE_HEIGHT);
		clipTarget(gMonsterClips, Snagrod, 280, 120, TILE_WIDTH, TILE_HEIGHT);

	return true;
}
/**/
/**/void clipAvatars()
{
	clipTarget(gAvatarClips, Magni_Bronzebeard, 0, 0, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Tyrande_Whisperwind, 100, 0, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Prophet_Velen, 200, 0, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Varian_Wrynn, 300, 0, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Gelbin_Mekkatorque, 0, 100, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Baine_Bloodhoof, 100, 100, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Sylvanas_Windrunner, 200, 100, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Lorthemar_Theron, 300, 100, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Garrosh_Hellscream, 0, 200, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Voljin, 100, 200, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Goblin_Trade_Prince, 200, 200, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Glenn_Greymane, 300, 200, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Maiev_Shadowsong, 0, 300, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Akama, 100, 300, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Lich_King, 200, 300, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Illidan_Stormrage, 300, 300, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Diablo, 700, 400, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Mephisto, 600, 400, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Baal, 500, 400, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, King_Leoric, 400, 400, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Imperius, 0, 400, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Tyrael, 100, 400, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Malthael, 200, 400, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Izual, 300, 400, AVATAR_WIDTH, AVATAR_HEIGHT);

	clipTarget(gAvatarClips, Sebastian_Yarrick, 400, 0, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Darnath_Lysander, 400, 100, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Logan_Grimnar, 400, 200, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Kaldor_Draigo, 400, 300, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Swarm_Lord, 500, 0, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Deathleaper, 500, 100, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Kaela_Meinsha_Khaine, 500, 200, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Astrubael_Vect, 500, 300, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Typhus, 600, 0, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Kharn, 600, 100, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Ahriman, 600, 200, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Lucius, 600, 300, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, O_Shovah, 700, 0, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Imotekh_Stormlord, 700, 100, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Mephiston, 700, 200, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Snagrod, 700, 300, AVATAR_WIDTH, AVATAR_HEIGHT);
}
/**/
/**/void clipStatTemp()
{
	clipTarget(gStatClips, ATTACK, 0, 0, 200, 24);
	clipTarget(gStatClips, HEALTH, 0, 24, 200, 24);
	clipTarget(gStatClips, RANGE, 0, 48, 200, 24);
	clipTarget(gStatClips, SPEED, 0, 72, 200, 24);
}
/**/
/**/void clipOverlays()
{
	clipTarget(gOverlays, MELEE_ATTACK_MARK, 40, 0, 40, 40);
	clipTarget(gOverlays, RANGE_ATTACK_MARK, 40, 40, 40, 40);
	clipTarget(gOverlays, SKULL_OVERLAY, 0, 0, 40, 40);
	clipTarget(gOverlays, DEATH_OVERLAY, 0, 40, 40, 40);

	clipTarget(gOverlays, MELEE_ICON, 80, 0, 40, 40);
	clipTarget(gOverlays, RANGED_ICON, 80, 40, 40, 40);
	clipTarget(gOverlays, FIRE_ICON, 120, 0, 40, 40);
	clipTarget(gOverlays, ICE_ICON, 120, 40, 40, 40);

	clipTarget(gOverlays, POISON_ICON, 160, 40, 40, 40);
	clipTarget(gOverlays, THUNDER_ICON, 160, 0, 40, 40);
	clipTarget(gOverlays, HEAL_OVERLAY, 200, 0, 40, 40);
	clipTarget(gOverlays, MAGIC_ATTACK_MARK, 0, 80, 40, 40);

	clipTarget(gOverlays, ARMOR_DEFENSE, 40, 80, 40, 40);
	clipTarget(gOverlays, MAGIC_DEFENSE, 80, 80, 40, 40);
	clipTarget(gOverlays, DARK_ICON, 120, 80, 40, 40);
	clipTarget(gOverlays, HOLY_ICON, 160, 80, 40, 40);

	clipTarget(gOverlays, QUESTION_MARK, 200, 80, 40, 40);
	clipTarget(gOverlays, X_MARK, 200, 120, 40, 40);
}
/**/
/**/void clipButtons()
{
	clipTarget(gButtonClips, UP_BUTTON, 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT);
	clipTarget(gButtonClips, HOVER_BUTTON, BUTTON_WIDTH + 1, 0, BUTTON_WIDTH, BUTTON_HEIGHT);
}
//End of clipping functions

void getTaunt(int i)
{
	switch (i)
	{
	case Illidan_Stormrage:
		break;
	case Akama:
		break;
	case Magni_Bronzebeard:
		break;
	case Tyrande_Whisperwind:
		break;
	case Lich_King:
		break;
	case Maiev_Shadowsong:
		break;
	case Baine_Bloodhoof:
		break;
	case Sylvanas_Windrunner:
		break;
	case Glenn_Greymane:
		break;
	case Garrosh_Hellscream:
		break;
	case Varian_Wrynn:
		break;
	case Prophet_Velen:
		break;
	case Goblin_Trade_Prince:
		break;
	case Voljin:
		break;
	case Lorthemar_Theron:
		break;
	case Gelbin_Mekkatorque:
		break;
	case Diablo:
		Mix_PlayChannel(-1, gDiablo_Select, 0);
		break;
	case Mephisto:
		Mix_PlayChannel(-1, gMephisto_Select, 0);
		break;
	case Baal:
		Mix_PlayChannel(-1, gBaal_Select, 0);
		break;
	case King_Leoric:
		break;
	case Imperius:
		break;
	case Tyrael:
		break;
	case Malthael:
		Mix_PlayChannel(-1, gMalthael_Select, 0);
		break;
	case Izual:
		break;
	case Mephiston:
		break;
	case Swarm_Lord:
		break;
	case Deathleaper:
		break;
	case Typhus:
		break;
	case Kharn:
		break;
	case Ahriman:
		break;
	case Lucius:
		break;
	case Kaela_Meinsha_Khaine:
		break;
	case Astrubael_Vect:
		break;
	case O_Shovah:
		break;
	case Imotekh_Stormlord:
		break;
	case Snagrod:
		break;
	case Sebastian_Yarrick:
		break;
	case Darnath_Lysander:
		break;
	case Logan_Grimnar:
		break;
	case Kaldor_Draigo:
		break;
	default:
		break;
	}
}

void checkClickMonster(int x, int y, Monster* &target, int gameState)
{
	if (gameState == IDLE)
	{
		for (int i = 0; i < TEAM_SIZE; i++)
		{
			if (team1.getMonster(i)->checkClick(x, y))
			{
				if (!(team1.getMonster(i)->checkDead()))
				{
					target = team1.getMonster(i);
				}
			}
			if (team2.getMonster(i)->checkClick(x, y))
			{
				if (!(team2.getMonster(i)->checkDead()))
				{
					target = team2.getMonster(i);
				}
			}
		}
	}
	else if (gameState == BATTLE)
	{
		for (int i = 0; i < TEAM_SIZE; i++)
		{
			if ((team1.getMonster(i) != target) || (team2.getMonster(i) != target))
			{
				if (team1.getMonster(i)->checkClick(x, y))
				{
					if (team1.getMonster(i)->checkInRange(target, gameState))
					{
						//target->doDamageTo(team1.getMonster(i));
						team1.damageMonster(i, target);
					}
				}
				if (team2.getMonster(i)->checkClick(x, y))
				{
					if (team2.getMonster(i)->checkInRange(target, gameState))
					{
						//target->doDamageTo(team2.getMonster(i));
						team2.damageMonster(i, target);
					}
				}
			}
		}
	}
	
	return;
}

void mouseHandle(SDL_Event e, Tile* tiles[], Monster* &target, int &gameState, SelectScreen &ss)
{
	if (e.button.button == SDL_BUTTON_LEFT)
	{
		if (gameState == MENU_SCREEN)
		{
			for (int i = 0; i < TOTAL_MENU_OPTIONS; i++)
			{
				buttonSet[i]->checkClick(mouse_x, mouse_y, gameState);
			}
			return;
		}
		else if (gameState == CHARACTER_SELECTION)
		{
			for (int i = 0; i < TOTAL_MONSTER_SPRITES; i++)
			{
				if ((ss.getButton(i))->checkClick(mouse_x, mouse_y))
				{
					if (!(ss.checkList(i)))
					{
						getTaunt(i);
						if (ss.getSelectedPlayer() == PLAYER1)
						{
							if (!(team1.checkFull()))
							{
								ss.addList(i);
							}
							team1.addMonster(ss.getButton(i)->getType(), 0, team1.getCurr());
						}
						else if (ss.getSelectedPlayer() == PLAYER2)
						{
							if (!(team2.checkFull()))
							{
								ss.addList(i);
							}
							team2.addMonster(ss.getButton(i)->getType(), (SCREEN_WIDTH / TILE_WIDTH) - 1, team2.getCurr());
						}
						
					}
				}
				else if (buttonSet[SELECT_BUTTON]->checkClick(mouse_x, mouse_y))
				{
					if (team1.getCurr() == TEAM_SIZE && team2.getCurr() == TEAM_SIZE)
					{
						buttonSet[SELECT_BUTTON]->checkClick(mouse_x, mouse_y, gameState);
					}
				}
			}
		}
		else if (gameState == PAUSE_SCREEN)
		{
			return;
		}
		else
		{
			for (int i = 0; i < TOTAL_TILES; i++)
			{
				if (tiles[i]->checkClick(mouse_x, mouse_y))
				{
					if (gameState == IDLE)
					{
						target = NULL;
					}
					else if (gameState == MOVEMENT)
					{
						target->moveUnit(tiles[i]->getCol(), tiles[i]->getRow());
					}
				}
			}
			checkClickMonster(mouse_x, mouse_y, target, gameState);
		}
	}
}

void mouseMotion(Tile* tile[], SDL_Rect& camera, Monster* &target, Monster* &hover, int gameState, SelectScreen &ss)
{
	//initialize rectangle to represent sub-screen
	bool mouseOverMonster = false;
	//determine location and dimension of sub-screen
	
	
	if (gameState < MENU_SCREEN)
	{
		displayBlank();
		for (int i = 0; i < TEAM_SIZE; i++)
		{
			if (team1.getMonster(i)->checkClick(mouse_x, mouse_y))
			{
				team1.getMonster(i)->displayMenu();
				hover = team1.getMonster(i);
				mouseOverMonster = true;
			}
			else if (team2.getMonster(i)->checkClick(mouse_x, mouse_y))
			{
				team2.getMonster(i)->displayMenu();
				hover = team2.getMonster(i);
				mouseOverMonster = true;
			}
		}

		//if the mouse is not over a target monster
		if (!mouseOverMonster)
		{
			hover = NULL;
			//if there is a current monster clicked
			if (target != NULL)
			{
				target->displayAction(camera);
			}
		}

		//check icon hovers
		for (int i = 0; i < TOTAL_ICONS; i++)
		{
			if (target != NULL)
			{
				if (i == target->get_damageType())
				{
					iconSet[i]->showHoverText(mouse_x, mouse_y);
				}
			}
		}
	}
	else if (gameState == CHARACTER_SELECTION)
	{
		bool foundHover = false;
		for (int i = 0; i < TOTAL_MONSTER_SPRITES; i++)
		{
			if (ss.getButton(i)->checkClick(mouse_x, mouse_y))
			{
				ss.setHover(i);
				foundHover = true;
			}
		}

		if (!foundHover)
		{
			ss.setHover(INT_MAX);
		}
	}
	
	return;
}

void mouseWheelHandle(SDL_Event e, SDL_Rect &window_screen, SelectScreen &ss)
{
	if (e.wheel.y < 0)
	{
		window_screen.y -= 1;
		if (window_screen.y < 0)
		{
			window_screen.y = 0;
			return;
		}
		for (int i = 0; i < TOTAL_MONSTER_SPRITES; i++)
		{
			ss.getButton(i)->setLoc(ss.getButton(i)->getBox().x, ss.getButton(i)->getBox().y + 1);
			ss.getButton(i)->setRow(ss.getButton(i)->getRow() + 1);
		}
	}
	else
	{
		window_screen.y += 1;
		if (window_screen.y > gSelectWindow.getHeight()-1)
		{
			window_screen.y = gSelectWindow.getHeight() - 1;
			return;
		}
		for (int i = 0; i < TOTAL_MONSTER_SPRITES; i++)
		{
			ss.getButton(i)->setLoc(ss.getButton(i)->getBox().x, ss.getButton(i)->getBox().y - 1);
			ss.getButton(i)->setRow(ss.getButton(i)->getRow() - 1);
		}
	} 
}

void keyBoardHandle(SDL_Event e, Tile* tiles[], SDL_Rect &camera, Monster* &target, int &gameState, SelectScreen &ss)
{
	switch (e.key.keysym.sym)
	{
	case '1':
		if (gameState == MOVEMENT)
		{
			gameState = IDLE;
		}
		else if (target != NULL)
		{
			gameState = MOVEMENT;
		}
		break;
	case '2':
		if (gameState == BATTLE)
		{
			gameState = IDLE;
		}
		else if (target != NULL)
		{
			gameState = BATTLE;
		}
		break;
	case SDLK_SPACE:
		if (gameState == CHARACTER_SELECTION)
		{
			ss.togglePlayer();
		}
	case SDLK_p:
		if (gameState < MENU_SCREEN)
		{
			gameState = PAUSE_SCREEN;
		}
		else if (gameState == PAUSE_SCREEN)
		{
			gameState = IDLE;
		}
	default:
		break;
	}
}

bool touchesWall(SDL_Rect box, Tile* tiles[])
{
	//Go through the tiles
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		//If the tile is a wall type tile
		if ((tiles[i]->getType() >= TILE_CENTER) && (tiles[i]->getType() <= TILE_TOPLEFT))
		{
			//If the collision box touches the wall tile
			if (checkCollision(box, tiles[i]->getBox()))
			{
				return true;
			}
		}
	}

	//If no wall tiles were touched
	return false;
}

//populate the rects
int getY(int a)
{
	return  BLOCK_OFFSET * (a - 1) + SUBSCREEN_HEIGHT + FONT_SIZE * (a - 1);
}

void populateStatLoc()
{
	int standard_x = 40 + AVATAR_WIDTH;
	int standard_w = 200;

	nameBlock.x = standard_x;
	nameBlock.y = getY(1);
	nameBlock.w = standard_w;
	nameBlock.h = FONT_SIZE;

	healthBlock.x = standard_x;
	healthBlock.y = getY(2);
	healthBlock.w = standard_w;
	healthBlock.h = FONT_SIZE;

	attackBlock.x = standard_x;
	attackBlock.y = getY(3);
	attackBlock.w = standard_w;
	attackBlock.h = FONT_SIZE;

	rangeBlock.x = standard_x;
	rangeBlock.y = getY(4);
	rangeBlock.w = standard_w;
	rangeBlock.h = FONT_SIZE;

	speedBlock.x = standard_x;
	speedBlock.y = getY(5);
	speedBlock.w = standard_w;
	speedBlock.h = FONT_SIZE;
}

void populateIcon(Icon* iconSet[])
{
	for (int i = 0; i < TOTAL_ICONS; i++)
	{
		iconSet[i] = new Icon(0, 0, i);
		iconSet[i]->setLoc(300, 525);
		iconSet[i]->setHoverText(i);
	}
}

void displayBlank()
{
	SDL_Rect temp;
	temp.x = 0;
	temp.y = 0;
	temp.w = LOWER_SCREEN_WIDTH;
	temp.h = LOWER_SCREEN_HEIGHT;
	gLowerSubscreen.render(0, SUBSCREEN_HEIGHT, &temp);
	//std::cout << gLowerSubscreen.getHeight() << "," << gLowerSubscreen.getWidth() << std::endl;
}

void readGameState(int gameState, Monster* &target, Tile* tile[], SDL_Rect &camera)
{
	if (gameState == MOVEMENT)
	{
		if (target != NULL)
		{
			target->showSpeed(tile, camera, HIGHLIGHT_RANGE_MOVEMENT);
		}
	}
	else if (gameState == BATTLE)
	{
		if (target != NULL)
		{
			target->showRange(tile, camera);
		}
	}
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//The level tiles
		Tile* tileSet[TOTAL_TILES];

		//clip avatars
		clipAvatars();
		clipStatTemp();
		clipOverlays();
		clipButtons();
		populateStatLoc();
		populateIcon(iconSet);
		//populateTeams(team1, team2);

		//Load media
		if (!loadMedia(tileSet))
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The dot that will be moving around on the screen
			Dot dot;
			int gameState = MENU_SCREEN;
			int scrolling_offset = 0;
			Monster* target = NULL;
			Monster* hover = NULL;

			//Level camera
			SDL_Rect camera = { 0, 0, SUBSCREEN_WIDTH, SUBSCREEN_HEIGHT };
			SDL_Rect menu_camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
			SDL_Rect select_window = { 0, 0, SELECT_WINDOW_WIDTH, SELECT_WINDOW_HEIGHT };

			Menu mainMenu;
			SelectScreen selectMenu(0);

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					if (e.type == SDL_MOUSEMOTION)
					{
						mouse_x = e.motion.x + camera.x;
						mouse_y = e.motion.y + camera.y;
					}

					if (e.type == SDL_MOUSEBUTTONDOWN)
					{
						mouseHandle(e, tileSet, target, gameState, selectMenu);
					}

					if (e.type == SDL_MOUSEWHEEL)
					{
						mouseWheelHandle(e, select_window, selectMenu);
					}

					if (e.type == SDL_KEYDOWN)
					{
						keyBoardHandle(e, tileSet, camera, target, gameState, selectMenu);
					}

					//Handle input for the dot
					dot.handleEvent(e);
				}

				if (gameState == GAME_QUIT){
					quit = true;
					break;
				}

				if ((target == NULL)&&(gameState < MENU_SCREEN)) {gameState = IDLE;}

				if (gameState == MENU_SCREEN)
				{
					mainMenu.menuDisplay(menu_camera);
				}
				else if (gameState == CHARACTER_SELECTION)
				{
					selectMenu.menuDisplay(menu_camera, select_window ,scrolling_offset);
					scrolling_offset--;
					if (scrolling_offset < -gScrollingBG.getWidth())
					{
						scrolling_offset = 0;
					}
				}
				else
				{
					//Move the dot
					dot.move(tileSet);
					dot.setCamera(camera, mouse_x, mouse_y);

					//Clear screen
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderer);

					//Render level
					for (int i = 0; i < TOTAL_TILES; ++i)
					{
						tileSet[i]->render(camera);
					}

					readGameState(gameState, target, tileSet, camera);

					for (int i = 0; i < TEAM_SIZE; ++i)
					{
						team1.getMonster(i)->render(camera);
						team2.getMonster(i)->render(camera);
						if (gameState != IDLE)
						{
							if (gameState == MOVEMENT)
							{
								if (team1.getMonster(i)->checkInRange(target, MOVEMENT))
								{
									team1.getMonster(i)->highlightTile(HIGHLIGHT_NOT_PASSABLE, camera);
								}
								if (team2.getMonster(i)->checkInRange(target, MOVEMENT))
								{
									team2.getMonster(i)->highlightTile(HIGHLIGHT_NOT_PASSABLE, camera);
								}
							}
							else if (gameState == BATTLE)
							{
								if (team1.getMonster(i)->checkInRange(target, BATTLE))
								{
									if (target->get_damageType() == MELEE_DAMAGE_TYPE)
									{
										team1.getMonster(i)->highlightTile(HIGHLIGHT_MELEE_TARGET, camera);
									}
									else if (target->get_damageType() == RANGED_DAMAGE_TYPE)
									{
										team1.getMonster(i)->highlightTile(HIGHLIGHT_RANGED_TARGET, camera);
									}
									else
									{
										team1.getMonster(i)->highlightTile(HIGHLIGHT_MAGIC_TARGET, camera);
									}
								}
								if (team2.getMonster(i)->checkInRange(target, BATTLE))
								{
									if (target->get_damageType() == MELEE_DAMAGE_TYPE)
									{
										team2.getMonster(i)->highlightTile(HIGHLIGHT_MELEE_TARGET, camera);
									}
									else if (target->get_damageType() == RANGED_DAMAGE_TYPE)
									{
										team2.getMonster(i)->highlightTile(HIGHLIGHT_RANGED_TARGET, camera);
									}
									else
									{
										team2.getMonster(i)->highlightTile(HIGHLIGHT_MAGIC_TARGET, camera);
									}
								}
							}
						}
					}

					for (int i = 0; i < TEAM_SIZE; i++)
					{
						if (team1.getMonster(i)->checkDead())
						{
							team1.getMonster(i)->highlightTile(HIGHLIGHT_DEAD, camera);
						}
						if (team2.getMonster(i)->checkDead())
						{
							team2.getMonster(i)->highlightTile(HIGHLIGHT_DEAD, camera);
						}
					}

					if (target != NULL)
					{
						target->highlightTile(HIGHLIGHT_SELECT, camera);
					}

					if (hover != NULL)
					{
						hover->highlightTile(HIGHLIGHT_HOVER, camera);
					}
					
					//Render dot
					dot.render(camera);

					if (gameState == PAUSE_SCREEN)
					{
						displayBlank();
						SDL_Rect nBox;
						nBox.x = 0;
						nBox.y = 0;
						nBox.w = SCREEN_WIDTH;
						nBox.h = SCREEN_HEIGHT;
						SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_ADD);
						SDL_SetRenderDrawColor(gRenderer, 164, 164, 164, 50);
						SDL_RenderFillRect(gRenderer, &nBox);

						SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);
					}
				}
				
				mouseMotion(tileSet, camera, target, hover, gameState, selectMenu);
				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}

		//Free resources and close SDL
		close(tileSet);
	}

	return 0;
}