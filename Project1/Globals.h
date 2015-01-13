#pragma once

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

bool init();

void loadMusic();

bool loadMedia(Tile* tiles[]);

void close(Tile* tiles[]);

bool checkCollision(SDL_Rect a, SDL_Rect b);

//Clipping Functions
/**/bool setTiles(Tile* tiles[]);
/**/
/**/void clipTarget(SDL_Rect gRect[], int name, int x, int y, int w, int h);
/**/
/**/bool setMonsters();
/**/
/**/void clipAvatars();
/**/
/**/void clipStatTemp();
/**/
/**/void clipOverlays();
/**/
/**/void clipButtons();
//End of clipping functions

void getTaunt(int i);

void checkClickMonster(int x, int y, Monster* &target, int gameState);

void mouseHandle(SDL_Event e, Tile* tiles[], Monster* &target, int &gameState, SelectScreen &ss);

void mouseMotion(Tile* tile[], SDL_Rect& camera, Monster* &target, Monster* &hover, int gameState, SelectScreen &ss);

void mouseWheelHandle(SDL_Event e, SDL_Rect &window_screen, SelectScreen &ss);

void keyBoardHandle(SDL_Event e, Tile* tiles[], SDL_Rect &camera, Monster* &target, int &gameState, SelectScreen &ss);

bool touchesWall(SDL_Rect box, Tile* tiles[]);

//populate the rects
int getY(int a);

void populateStatLoc();

void populateIcon(Icon* iconSet[]);

void displayBlank();

void readGameState(int gameState, Monster* &target, Tile* tile[], SDL_Rect &camera);

void transitionAnimate(int scrolling_offset, bool &start);

//The window we'll be rendering to
extern SDL_Window* gWindow;

//The window renderer
extern SDL_Renderer* gRenderer;

//Mouse location
extern int mouse_x, mouse_y;
extern int scrolling_offset;

//Scene textures
extern LTexture gDotTexture;
extern LTexture gTileTexture;
extern LTexture gMonsterTexture;
extern LTexture gLargeMonsterTexture;
extern LTexture gLowerSubscreen;
extern LTexture gStatTemplate;
extern LTexture gOverlay;
extern LTexture gButtonTexture;
extern LTexture gMenuBackground;
extern LTexture gScrollingBG;
extern LTexture gSelectOverlay;
extern LTexture gSelectWindow;
extern LTexture gStartTexture;

//Clip arrays
extern SDL_Rect gTileClips[TOTAL_TILE_SPRITES];
extern SDL_Rect gMonsterClips[TOTAL_MONSTER_SPRITES];
extern SDL_Rect gAvatarClips[TOTAL_MONSTER_SPRITES];
extern SDL_Rect gStatClips[TOTAL_STATS];
extern SDL_Rect gOverlays[TOTAL_OVERLAYS];
extern SDL_Rect gButtonClips[TOTAL_BUTTONS];

//stat menu rects
extern SDL_Rect nameBlock;
extern SDL_Rect attackBlock;
extern SDL_Rect healthBlock;
extern SDL_Rect rangeBlock;
extern SDL_Rect speedBlock;
extern SDL_Rect manaBlock;

//render font
extern TTF_Font* gFont;
extern TTF_Font* gFontSmall;

//create team of monsters
extern Team team1;
extern Team team2;

extern Icon* iconSet[TOTAL_ICONS];
extern Button* buttonSet[TOTAL_MENU_OPTIONS];

//sound
extern Mix_Music *gMusic;

//The sound effects that will be used
extern Mix_Chunk *gDiablo_Select;
extern Mix_Chunk *gMephisto_Select;
extern Mix_Chunk *gBaal_Select;
extern Mix_Chunk *gMalthael_Select;

