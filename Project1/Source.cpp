
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
		Button* buttonSet[TOTAL_TRANSITION_BUTTONS];

		populateStatLoc();
		populateButton(buttonSet);
		populateIcon(iconSet);

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
			bool p1s = false, p1m = false, p1b = false, p2s = false, p2m = false, p2b = false;
			bool first = false;

			//Event handler
			SDL_Event e;

			//The dot that will be moving around on the screen
			Dot dot;

			//keeps track of the state of the game
			int gameState = MENU_SCREEN;
			
			bool turn_end = false;
			int p;              //keeps track of the phase while in game;
			int turnCount = 0;  //keeps track of the number of turns that have passed.

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
						mouseHandle(e, tileSet, target, gameState, selectMenu, p, select_window, buttonSet, first);
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

					if (p == PLAYER1_STANDBY || p == PLAYER2_STANDBY)
					{
						if (p == PLAYER1_STANDBY && first){ p1s = true; scrolling_offset = 0; }
						else if (p == PLAYER2_STANDBY && first){ p2s = true; scrolling_offset = 0; }
						turn_end = true;
					}
					else if (p == PLAYER1_MOVEMENT || p == PLAYER2_MOVEMENT)
					{
						if (p == PLAYER1_MOVEMENT && first){ p1m = true; scrolling_offset = 0; }
						else if (p == PLAYER2_MOVEMENT && first){ p2m = true; scrolling_offset = 0; }
					}
					else if (p == PLAYER1_BATTLE || p == PLAYER2_BATTLE)
					{
						if (p == PLAYER1_BATTLE && first){ p1b = true; scrolling_offset = 0; }
						else if (p == PLAYER2_BATTLE && first){ p2b = true; scrolling_offset = 0; }
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
								if (team1.getMonster(i)->checkInRange(target, p) && p == PLAYER1_MOVEMENT)
								{
									team1.getMonster(i)->highlightTile(HIGHLIGHT_NOT_PASSABLE, camera);
								}
								if (team2.getMonster(i)->checkInRange(target, p)&& p == PLAYER2_MOVEMENT)
								{
									team2.getMonster(i)->highlightTile(HIGHLIGHT_NOT_PASSABLE, camera);
								}
							}
							else if (p == PLAYER1_BATTLE || p == PLAYER2_BATTLE)
							{
								for (int j = 0; j < TEAM_SIZE; j++)
								{
									if (team1.getMonster(j) == target)
									{
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
										}
										else if ((team1.getMonster(i)->checkInRange(target, p)))
										{
											team1.getMonster(i)->highlightTile(HIGHLIGHT_NOT_PASSABLE, camera);
										}
									}
									else if (team2.getMonster(j) == target)
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
											else if ((team2.getMonster(i)->checkInRange(target, p)))
											{
												team2.getMonster(i)->highlightTile(HIGHLIGHT_NOT_PASSABLE, camera);
											}
										}
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
						displayBlank(p);
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
						transitionAnimate(scrolling_offset, gameStart, 3);
						scrolling_offset += 5;
					}

					if (p1s && p == PLAYER1_STANDBY)
					{
						transitionAnimate(scrolling_offset, p1s, 2);
						scrolling_offset += 5;
						first = false;
					}
					else if (p1m && p == PLAYER1_MOVEMENT)
					{
						transitionAnimate(scrolling_offset, p1m, 0);
						scrolling_offset += 5;
						first = false;
					}
					else if (p2s && p == PLAYER2_STANDBY)
					{
						transitionAnimate(scrolling_offset, p2s, 4);
						scrolling_offset += 5;
						first = false;
					}
					else if (p2m && p == PLAYER2_MOVEMENT)
					{
						transitionAnimate(scrolling_offset, p2m, 6);
						scrolling_offset += 5;
						first = false;
					}
					else if (p1b && p == PLAYER1_BATTLE)
					{
						transitionAnimate(scrolling_offset, p1b, 1);
						scrolling_offset += 5;
						first = false;
					}
					else if (p2b && p == PLAYER2_BATTLE)
					{
						transitionAnimate(scrolling_offset, p2b, 5);
						scrolling_offset += 5;
						first = false;
					}

					if ((p == PLAYER1_END || p == PLAYER2_END) && (turn_end))
					{
						target = NULL;
						
						if (p == PLAYER1_END)
						{ 
							team1.setUnusedAP(team1.getCurrentAP()); 
						}
						else if (p == PLAYER2_END)
						{ 
							team2.setUnusedAP(team2.getCurrentAP());
							turnCount++;
						}

						if (turnCount % 2 == 0 && turnCount>0)
						{
							if (team1.getCurrentAP() < team1.getMaxAP())
							{
								team1.setCurrentAP(team1.getCurrentAP() + 1);
								team1.setUnusedAP(team1.getCurrentAP());
							}
							if (team2.getCurrentAP() < team2.getMaxAP())
							{
								team2.setCurrentAP(team2.getCurrentAP() + 1);
								team2.setUnusedAP(team2.getCurrentAP());
							}
						}
						turn_end = false;
					}
				}
				
				mouseMotion(tileSet, camera, target, hover, gameState, selectMenu, select_window, p);

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}

		//Free resources and close SDL
		close(tileSet, buttonSet);
	}

	return 0;
}