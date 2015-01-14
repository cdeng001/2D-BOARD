#pragma once

//Forward declare button functions
void changeToStart(int &gameState);
void changeToPause(int &gameState);
void changeToMenu(int &gameState);
void changeToCharacterSelection(int &gameState);
void changeToQuit(int &gameState);

void p1_move(int &phase);
void p1_battle(int &phase);
void p1_end(int &phase);
void p2_move(int &phase);
void p2_battle(int &phase);
void p2_end(int &phase);