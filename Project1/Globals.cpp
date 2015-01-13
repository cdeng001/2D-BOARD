#include "Globals.h"

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Mouse location
int mouse_x, mouse_y;
int scrolling_offset = 0;

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
LTexture gStartTexture;

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
SDL_Rect manaBlock;

//render font
TTF_Font* gFont = NULL;
TTF_Font* gFontSmall = NULL;
TTF_Font* gFontPhase = NULL;

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

	if (!gStartTexture.loadFromFile("image/START.png"))
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
	gFont = TTF_OpenFont("font/Prototype.ttf", FONT_SIZE);
	if (gFont == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	gFontSmall = TTF_OpenFont("font/Prototype.ttf", FONT_SIZE - 6);
	if (gFontSmall == NULL)
	{
		printf("Failed to load small font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	gFontPhase = TTF_OpenFont("font/Blox2.ttf", FONT_SIZE * 3);
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
	gStartTexture.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;
	TTF_CloseFont(gFontPhase);
	gFontPhase = NULL;
	TTF_CloseFont(gFontSmall);
	gFontSmall = NULL;

	Mix_FreeChunk(gDiablo_Select);
	gDiablo_Select = NULL;
	Mix_FreeChunk(gMalthael_Select);
	gMalthael_Select = NULL;
	Mix_FreeChunk(gMephisto_Select);
	gMephisto_Select = NULL;
	Mix_FreeChunk(gBaal_Select);
	gBaal_Select = NULL;

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
				tiles[i] = new Tile(c - 1, r - 1, tileType);
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
	clipTarget(gMonsterClips, Illidan_Stormrage, 0, 0, TILE_WIDTH, TILE_HEIGHT);
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

	clipTarget(gMonsterClips, Jim_Raynor, 0, 200, TILE_WIDTH, TILE_HEIGHT);
	clipTarget(gMonsterClips, Tychus_Findley, 40, 200, TILE_WIDTH, TILE_HEIGHT);
	clipTarget(gMonsterClips, Annabeth_Terra, 80, 200, TILE_WIDTH, TILE_HEIGHT);
	clipTarget(gMonsterClips, Artanis, 120, 200, TILE_WIDTH, TILE_HEIGHT);
	clipTarget(gMonsterClips, Zeratul, 160, 200, TILE_WIDTH, TILE_HEIGHT);
	clipTarget(gMonsterClips, Tassadar, 200, 200, TILE_WIDTH, TILE_HEIGHT);
	clipTarget(gMonsterClips, Sarah_Kerrigan, 240, 200, TILE_WIDTH, TILE_HEIGHT);
	clipTarget(gMonsterClips, Zagara, 280, 200, TILE_WIDTH, TILE_HEIGHT);

	clipTarget(gMonsterClips, Thestalos, 0, 240, TILE_WIDTH, TILE_HEIGHT);
	clipTarget(gMonsterClips, Mobius, 40, 240, TILE_WIDTH, TILE_HEIGHT);
	clipTarget(gMonsterClips, Granmarg, 80, 240, TILE_WIDTH, TILE_HEIGHT);
	clipTarget(gMonsterClips, Zaborg, 120, 240, TILE_WIDTH, TILE_HEIGHT);
	clipTarget(gMonsterClips, Raiza, 160, 240, TILE_WIDTH, TILE_HEIGHT);
	clipTarget(gMonsterClips, Caius, 200, 240, TILE_WIDTH, TILE_HEIGHT);
	clipTarget(gMonsterClips, Dark_Magician, 240, 240, TILE_WIDTH, TILE_HEIGHT);
	clipTarget(gMonsterClips, Blue_Eyes, 280, 240, TILE_WIDTH, TILE_HEIGHT);

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

	clipTarget(gAvatarClips, Jim_Raynor, 0, 500, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Tychus_Findley, 100, 500, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Annabeth_Terra, 200, 500, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Artanis, 300, 500, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Zeratul, 400, 500, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Tassadar, 500, 500, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Sarah_Kerrigan, 600, 500, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Zagara, 700, 500, AVATAR_WIDTH, AVATAR_HEIGHT);

	clipTarget(gAvatarClips, Thestalos, 0, 600, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Mobius, 100, 600, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Granmarg, 200, 600, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Zaborg, 300, 600, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Raiza, 400, 600, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Caius, 500, 600, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Dark_Magician, 600, 600, AVATAR_WIDTH, AVATAR_HEIGHT);
	clipTarget(gAvatarClips, Blue_Eyes, 700, 600, AVATAR_WIDTH, AVATAR_HEIGHT);
}
/**/
/**/void clipStatTemp()
{
	clipTarget(gStatClips, ATTACK, 0, 0, 200, 24);
	clipTarget(gStatClips, HEALTH, 0, 24, 200, 24);
	clipTarget(gStatClips, RANGE, 0, 48, 200, 24);
	clipTarget(gStatClips, SPEED, 0, 72, 200, 24);
	clipTarget(gStatClips, MANA, 0, 96, 200, 24);
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

void checkClickMonster(int x, int y, Monster* &target, int gameState, int p)
{
	if (p != PLAYER1_END && p != PLAYER2_END && p != AI_PHASE1 && p != AI_PHASE2)
	{
		for (int i = 0; i < TEAM_SIZE; i++)
		{
			if (team1.getMonster(i)->checkClick(x, y))
			{
				if (!(team1.getMonster(i)->checkDead()) && p == PLAYER1_STANDBY)
				{
					target = team1.getMonster(i);
				}
			}
			if (team2.getMonster(i)->checkClick(x, y))
			{
				if (!(team2.getMonster(i)->checkDead()) && p == PLAYER2_STANDBY)
				{
					target = team2.getMonster(i);
				}
			}
		}
	}
	if (p == PLAYER1_BATTLE || p == PLAYER2_BATTLE)
	{
		for (int i = 0; i < TEAM_SIZE; i++)
		{
			if ((team1.getMonster(i) != target) || (team2.getMonster(i) != target))
			{
				if (team1.getMonster(i)->checkClick(x, y))
				{
					if ((team1.getMonster(i)->checkInRange(target, p)) && p == PLAYER2_BATTLE )
					{
						std::cout << i << std::endl;
						team1.damageMonster(i, target);
					}
				}
				if (team2.getMonster(i)->checkClick(x, y))
				{
					if ((team2.getMonster(i)->checkInRange(target, p)) && p == PLAYER1_BATTLE )
					{
						std::cout << i << std::endl;
						team2.damageMonster(i, target);
					}
				}
			}
		}
	}

	return;
}

void mouseHandle(SDL_Event e, Tile* tiles[], Monster* &target, int &gameState, SelectScreen &ss, int p, SDL_Rect win)
{
	//deal with turn player
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
				if ((ss.getButton(i))->checkClick(mouse_x, mouse_y, win))
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
							team2.addMonster(ss.getButton(i)->getType(), (LEVEL_WIDTH / TILE_WIDTH) - 1, team2.getCurr());
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
					if (p == PLAYER1_STANDBY || p == PLAYER2_STANDBY)
					{
						target = NULL;
					}
					else if (p == PLAYER1_MOVEMENT || p == PLAYER2_MOVEMENT)
					{
						for (int j = 0; j < TEAM_SIZE; j++)
						{
							if ((p == PLAYER1_MOVEMENT) && (team1.getMonster(j) == target))
							{
								target->moveUnit(tiles[i]->getCol(), tiles[i]->getRow());
							}
							else if ((p == PLAYER2_MOVEMENT) && (team2.getMonster(j) == target))
							{
								target->moveUnit(tiles[i]->getCol(), tiles[i]->getRow());
							}
						}
						
					}
				}
			}
			checkClickMonster(mouse_x, mouse_y, target, gameState, p);
		}
	}
}

void mouseMotion(Tile* tile[], SDL_Rect& camera, Monster* &target, Monster* &hover, int gameState, SelectScreen &ss, SDL_Rect win)
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
					iconSet[i]->showHoverText(mouse_x - camera.x, mouse_y - camera.y);
				}
			}
		}
	}
	else if (gameState == CHARACTER_SELECTION)
	{
		bool foundHover = false;
		for (int i = 0; i < TOTAL_MONSTER_SPRITES; i++)
		{
			if (ss.getButton(i)->checkClick(mouse_x, mouse_y, win))
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
			window_screen.y += 1;
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
		if (window_screen.y > gSelectWindow.getHeight() - SELECT_WINDOW_HEIGHT)
		{
			window_screen.y -= 1;
			return;
		}
		for (int i = 0; i < TOTAL_MONSTER_SPRITES; i++)
		{
			ss.getButton(i)->setLoc(ss.getButton(i)->getBox().x, ss.getButton(i)->getBox().y - 1);
			ss.getButton(i)->setRow(ss.getButton(i)->getRow() - 1);
		}
	}
}

void keyBoardHandle(SDL_Event e, Tile* tiles[], SDL_Rect &camera, Monster* &target, int &gameState, SelectScreen &ss, int &p)
{
	switch (e.key.keysym.sym)
	{
	case '1':
		p = p + 1;
		if (p == TOTAL_PHASES)
		{
			p = 0;
		}
		std::cout << p << std::endl;
		break;
	case '2':
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

	manaBlock.x = standard_x;
	manaBlock.y = getY(2);
	manaBlock.w = standard_w;
	manaBlock.h = FONT_SIZE;

	healthBlock.x = standard_x;
	healthBlock.y = getY(1);
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

void readGameState(int p, Monster* &target, Tile* tile[], SDL_Rect &camera)
{
	if (p == PLAYER1_MOVEMENT || p == PLAYER2_MOVEMENT)
	{
		if (target != NULL)
		{
			target->showSpeed(tile, camera, HIGHLIGHT_RANGE_MOVEMENT);
		}
	}
	else if (p == PLAYER1_BATTLE || p == PLAYER2_BATTLE)
	{
		if (target != NULL)
		{
			target->showRange(tile, camera);
		}
	}
}

void transitionAnimate(int scrolling_offset, bool &start)
{
	if (scrolling_offset >= gStartTexture.getWidth())
	{
		start = true;
		int start = SDL_GetTicks();
		while (SDL_GetTicks() - start < 1000){}
		return;
	}
	gStartTexture.render(-gStartTexture.getWidth() + scrolling_offset, 0);

}