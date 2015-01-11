#include "Menu.h"
#include "SDL.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include "LTexture.h"
#include "Consts.h"
#include "stdlib.h"
#include "Button.h"
#include "ButtonFunctions.h"
#include "SelectScreen.h"
#include <iostream>



SelectScreen::~SelectScreen()
{
	for (int i = 0; i < TOTAL_MONSTER_SPRITES; ++i)
	{
		if (characterSet[i] == NULL)
		{
			delete characterSet[i];
			characterSet[i] = NULL;
		}
	}
	
}

std::string SelectScreen::getMonster(int i, Button &b)
{
	b.setType(i);
	switch (i)
	{
	case Illidan_Stormrage:
		return "Illidan Stormrage, Lord of Outland";
		break;
	case Akama:
		return "Akama, the Lost One";
		break;
	case Magni_Bronzebeard:
		return "Magni Bronzebeard, King of Khaz Modan";
		break;
	case Tyrande_Whisperwind:
		return "Tyrande Whisperwind, High Priestess of Elune";
		break;
	case Lich_King:
		return "Bolvar Fordragon, the Lich King";
		break;
	case Maiev_Shadowsong:
		return "Huntress Maiev Shadowsong";
		break;
	case Baine_Bloodhoof:
		return "Baine Bloodhoof, Son of Caine";
		break;
	case Sylvanas_Windrunner:
		return "Sylvanas Windrunner, The Dark Lady";
		break;
	case Glenn_Greymane:
		return "King Glenn Greymane";
		break;
	case Garrosh_Hellscream:
		return "Garrosh Hellscream, Sha of Pride";
		break;
	case Varian_Wrynn:
		return "King of Stromwind, Varian Wrynn";
		break;
	case Prophet_Velen:
		return "Divine Prophet Velen";
		break;
	case Goblin_Trade_Prince:
		return "Goblin Trade Prince";
		break;
	case Voljin:
		return "Vol'jin of Darkspear";
		break;
	case Lorthemar_Theron:
		return "Lorthemar Theron, Regent Lord of Quel'Thalas";
		break;
	case Gelbin_Mekkatorque:
		return "High Tinker Gelbin Mekkatorque,";
		break;
	case Diablo:
		return "Diablo, Lord of Hatred";
		break;
	case Mephisto:
		return "Mephisto, Lord of Terror";
		break;
	case Baal:
		return "Baal, Lord of Destruction";
		break;
	case King_Leoric:
		return "King Leoric, the Skeleton King";
		break;
	case Imperius:
		return "Imperius, Archangel of Valor";
		break;
	case Tyrael:
		return "Tyreal, Aspect of Wisdom";
		break;
	case Malthael:
		return "Malthael, The Reaper";
		break;
	case Izual:
		return "Izual, The Betrayer";
		break;
	case Mephiston:
		return "Mephiston, Lord of Death";
		break;
	case Swarm_Lord:
		return "Hive Fleet Behemoth: Swarm Lord";
		break;
	case Deathleaper:
		return "Hive Fleet Leviathan: Deathleaper";
		break;
	case Typhus:
		return "Typhus the Traveller";
		break;
	case Kharn:
		return "Kharn, Champion of Khorn";
		break;
	case Ahriman:
		return "Ahriman of Thousand Suns";
		break;
	case Lucius:
		return "Lucius the Eternal";
		break;
	case Kaela_Meinsha_Khaine:
		return "Kaela Meinsha Khaine, Avatar of War";
		break;
	case Astrubael_Vect:
		return "Astrubael Vect the Dark Archon";
		break;
	case O_Shovah:
		return "O\'Shovah, Commander Farsight";
		break;
	case Imotekh_Stormlord:
		return "Imotekh the Stormlord";
		break;
	case Snagrod:
		return "Ork Warlord Snagrod";
		break;
	case Sebastian_Yarrick:
		return "Commissar Sebastian Yarrick";
		break;
	case Darnath_Lysander:
		return "Darnath Lysander";
		break;
	case Logan_Grimnar:
		return "Logan Grimnar, the Fangfather";
		break;
	case Kaldor_Draigo:
		return "Lord Kaldor Draigo";
		break;
	default:
		return " ";
		break;
	}
}

Button* SelectScreen::getButton(int i)
{
	if (i < TOTAL_MONSTER_SPRITES)
	{
		return characterSet[i];
	}
	return NULL;
}

player SelectScreen::getSelectedPlayer()
{
	return this->current;
}

void SelectScreen::togglePlayer()
{
	if (current == PLAYER1)
	{
		current = PLAYER2;
		std::cout << "player2 choose" << std::endl;
	}
	else 
	{
		current = PLAYER1;
		std::cout << "player1 choose" << std::endl;
	}
}

void SelectScreen::setHover(int i)
{
	this->curr_hover = i;
	return;
}

int SelectScreen::getHover()
{
	return this->curr_hover;
}

void SelectScreen::addList(int i)
{
	if (listSize < TEAM_SIZE * 2)
	{
		list[listSize] = i;
		listSize++;
	}
}

bool SelectScreen::checkList(int x)
{
	for (int i = 0; i < listSize; i++)
	{
		if (list[i] == x)
		{
			return true;
		}
	}
	return false;
}