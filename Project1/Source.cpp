
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
#include "ImageAnimate.h"

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
			bool gameStart = false;

			//Event handler
			SDL_Event e;

			//The dot that will be moving around on the screen
			Dot dot;
			int gameState = MENU_SCREEN;
			int p;
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
						mouseHandle(e, tileSet, target, gameState, selectMenu, p, select_window);
					}

					if (e.type == SDL_MOUSEWHEEL)
					{
						mouseWheelHandle(e, select_window, selectMenu);
					}

					if (e.type == SDL_KEYDOWN)
					{
						keyBoardHandle(e, tileSet, camera, target, gameState, selectMenu, p);
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
					if (!gameStart)
					{
						p = PLAYER1_STANDBY;
					}

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

					readGameState(p, target, tileSet, camera);

					for (int i = 0; i < TEAM_SIZE; ++i)
					{
						team1.getMonster(i)->render(camera);
						team2.getMonster(i)->render(camera);
						if (gameState == IDLE)
						{
							if (p == PLAYER1_MOVEMENT||p == PLAYER2_MOVEMENT)
							{
								if (team1.getMonster(i)->checkInRange(target, p)) 
								{
									team1.getMonster(i)->highlightTile(HIGHLIGHT_NOT_PASSABLE, camera);
								}
								if (team2.getMonster(i)->checkInRange(target, p))
								{
									team2.getMonster(i)->highlightTile(HIGHLIGHT_NOT_PASSABLE, camera);
								}
							}
							else if (p == PLAYER1_BATTLE || p == PLAYER2_BATTLE)
							{
								if ((team1.getMonster(i)->checkInRange(target, p)))
								{
									if (p == PLAYER2_BATTLE)
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
									team1.getMonster(i)->highlightTile(HIGHLIGHT_NOT_PASSABLE, camera);
								}
								if ((team2.getMonster(i)->checkInRange(target, p)))
								{
									if (p == PLAYER1_BATTLE)
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
									team1.getMonster(i)->highlightTile(HIGHLIGHT_NOT_PASSABLE, camera);
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
					if (!gameStart)
					{
						transitionAnimate(scrolling_offset, gameStart);
						scrolling_offset += 5;
					}
				}
				
				mouseMotion(tileSet, camera, target, hover, gameState, selectMenu, select_window);

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}

		//Free resources and close SDL
		close(tileSet);
	}

	return 0;
}