#include "Globals.h"

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Mouse location
int mouse_x, mouse_y;

//Scene textures
LTexture gDotTexture;
LTexture gTileTexture;
LTexture gMonsterTexture;
LTexture gLargeMonsterTexture;
LTexture gLowerSubscreen;
LTexture gStatTemplate;
LTexture gOverlay;
LTexture gButtonTexture;
LTexture gMenuBackground;
LTexture gScrollingBG;
LTexture gSelectOverlay;
LTexture gSelectWindow;

//Clip arrays
SDL_Rect gTileClips[TOTAL_TILE_SPRITES];
SDL_Rect gMonsterClips[TOTAL_MONSTER_SPRITES];
SDL_Rect gAvatarClips[TOTAL_MONSTER_SPRITES];
SDL_Rect gStatClips[TOTAL_STATS];
SDL_Rect gOverlays[TOTAL_OVERLAYS];
SDL_Rect gButtonClips[TOTAL_BUTTONS];

//stat menu rects
SDL_Rect nameBlock;
SDL_Rect attackBlock;
SDL_Rect healthBlock;
SDL_Rect rangeBlock;
SDL_Rect speedBlock;

//render font
TTF_Font* gFont = NULL;
TTF_Font* gFontSmall = NULL;

//create team of monsters
Team team1;
Team team2;

Icon* iconSet[TOTAL_ICONS];
Button* buttonSet[TOTAL_MENU_OPTIONS];

//sound
//The music that will be played
Mix_Music *gMusic = NULL;

//The sound effects that will be used
Mix_Chunk *gDiablo_Select = NULL;
Mix_Chunk *gMephisto_Select = NULL;
Mix_Chunk *gBaal_Select = NULL;
Mix_Chunk *gMalthael_Select = NULL;