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
#include "Globals.h"



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
	case Jim_Raynor:
		return "James Eugene \"Jim\" Raynor";
		break;
	case Tychus_Findley:
		return "Tychus J. Findley";
		break;
	case Annabeth_Terra:
		return "November Annabeth \"Nova\" Terra";
		break;
	case Artanis:
		return "Artanis, Praetor of Defense";
		break;
	case Zeratul:
		return "Zeratul the Dark Templar";
		break;
	case Tassadar:
		return "Tassadar, Savior of the Templar";
		break;
	case Sarah_Kerrigan:
		return "Sarah Kerrigan, The Queen of Blades";
		break;
	case Zagara:
		return "Broodmother Zagara";
		break;
	case Thestalos:
		return "Thestalos the Firestorm Monarch";
		break;
	case Mobius:
		return "Mobius the Frost Monarch";
		break;
	case Granmarg:
		return "Granmarg the Rock Monarch";
		break;
	case Zaborg:
		return "Zaborg the Thunder Monarch";
		break;
	case Raiza:
		return "Raiza the Storm Monarch";
		break;
	case Caius:
		return "Caius the Shadow Monarch";
		break;
	case Dark_Magician:
		return "The Dark Magician";
		break;
	case Blue_Eyes:
		return "Blue Eyes White Dragon";
		break;
	case Jinzo:
		return "Jinzo";
		break;
	case Dark_Paladin:
		return "Dark Paladin";
		break;
	case Neos:
		return "Elemental Hero Neos";
		break;
	case Reign_Beaux:
		return "Reign-Beaux, Overlord of the Dark World";
		break;
	case Utopia:
		return "Number 39: Utopia";
		break;
	case Stardust:
		return "Stardust Dragon";
		break;
	case Dark_Armed_Dragon:
		return "Dark Armed Dragon";
		break;
	case Gottoms:
		return "XX-Saber Gottoms";
		break;
	case Cain:
		return "First Apostle: Cain the Destroyer";
		break;
	case Hilder:
		return "Second Apostle: Hilder the Crying Eye";
		break;
	case Isis:
		return "Third Apostle: Isis the Preditor";
		break;
	case Kasillas:
		return "Fourth Apostle: Kasillas the Conqueror";
		break;
	case Siroco:
		return "Fifth Apostle: Siroko of Intangible";
		break;
	case Diregie:
		return "Sixth Apostle: Diregie the Plague";
		break;
	case Anton:
		return "Seventh Apostle: Flame Eater Anton";
		break;
	case Lotus:
		return "Eighth Apostle: Gangling Lotus";
		break;
	case Luke:
		return "Ninth Apostle: Luke the Constructor";
		break;
	case Bakal:
		return "Tenth Apostle: Dragon King Bakal";
		break;
	case Michael:
		return "Eleventh Apostle: Michael of Holy Eye";
		break;
	case Ozma:
		return "Twelfth Apostle: Ozma of Chaos";
		break;
	case Sighard:
		return "Sieghart the Castellan of Light";
		break;
	case Gaunis:
		return "Rift Master Gaunis";
		break;
	case Bwanga:
		return "Bwanga";
		break;
	case Agonzo:
		return "Agonzo";
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
		std::cout << "player2" << std::endl;
	}
	else 
	{
		current = PLAYER1;
		std::cout << "player1" << std::endl;
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
	if (list.size() < TEAM_SIZE * 2)
	{
		list.push_back(i);
	}
}

bool SelectScreen::checkList(int x)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (list.at(i) == x)
		{
			return true;
		}
	}
	return false;
}


SelectScreen::SelectScreen(int a) :
Menu(a), current(PLAYER1), curr_hover(INT_MAX)
{

	buttonSet[SELECT_BUTTON] = new Button();
	buttonSet[SELECT_BUTTON]->setLoc(SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, ((SCREEN_HEIGHT - SCREEN_HEIGHT / 4) - BUTTON_HEIGHT / 2) - 5);
	buttonSet[SELECT_BUTTON]->setActionFunction(changeToStart);
	buttonSet[SELECT_BUTTON]->setName("Start");
	getButtonFunction();

	for (int i = 0; i < TEAM_SIZE * 2; i++)
	{
		Button temp;
		if (i < TEAM_SIZE)
		{
			temp.setLoc(20, 50 + (TILE_HEIGHT + 10)*i);
			temp.setSize(TILE_WIDTH, TILE_HEIGHT);
			temp.setType(i);
			teamRemove.push_back(temp);
		}
		else
		{
			temp.setLoc((SCREEN_WIDTH - 20) - TILE_WIDTH, (TILE_HEIGHT + 10)*((i-TEAM_SIZE) + 1));
			temp.setSize(TILE_WIDTH, TILE_HEIGHT);
			temp.setType(i-TEAM_SIZE);
			teamRemove.push_back(temp);
		}
	}
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
		characterSet[i]->render(windowCamera, characterSet[i]->getRow());
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
			team1.getMonster(i)->renderAt( 20, 50 + (TILE_HEIGHT + 10)*i);
		}
		else
		{
			gOverlay.render(20, 50 + (TILE_HEIGHT + 10)*i, &gOverlays[QUESTION_MARK]);
		}
		if (team2.getMonster(i) != NULL)
		{
			team2.getMonster(i)->renderAt( (SCREEN_WIDTH - 20) - TILE_WIDTH, (TILE_HEIGHT + 10)*(i + 1));
		}
		else
		{
			gOverlay.render((SCREEN_WIDTH - 20) - TILE_WIDTH, (TILE_HEIGHT + 10)*(i + 1), &gOverlays[QUESTION_MARK]);
		}
	}

	if (curr_hover < TOTAL_MONSTER_SPRITES)
	{

		gLargeMonsterTexture.render(50, (SCREEN_HEIGHT - AVATAR_HEIGHT) - 25, &gAvatarClips[curr_hover]);
		LTexture gTextTexture;

		SDL_Color textColor = { 0, 0, 0 };
		gTextTexture.loadFromRenderedText(gFont, characterSet[curr_hover]->getName(), textColor);
		gTextTexture.render(100 + AVATAR_WIDTH, (SCREEN_HEIGHT - AVATAR_HEIGHT) - 25);

		gTextTexture.free();

	}
}

void SelectScreen::getButtonFunction()
{
	int x = 0;
	int h_offset = 50;
	int w_offset = 150;
	enum { WOW = 0, DIABLO = 1, WH = 2, SC = 4, YGO = 5, DFO = 7 };
	while (x < TOTAL_MONSTER_SPRITES)
	{
		for (int j = 0; j < (SCREEN_HEIGHT / TILE_HEIGHT); j++)
		{
			for (int k = 0; k < ((SCREEN_WIDTH / TILE_WIDTH) / 2); k++)
			{
				if (x >= TOTAL_MONSTER_SPRITES){ return; }
				Button* temp = new Button();
				temp->setLoc(k * TILE_WIDTH + w_offset, j * TILE_HEIGHT + h_offset);
				temp->setRow(j * TILE_HEIGHT + h_offset);
				temp->setName(getMonster(x, *temp));
				temp->setSize(TILE_WIDTH, TILE_HEIGHT);
				characterSet[x] = temp;
				x++;
			}
			if (j == DIABLO || j == WH || j == SC || j == YGO )
			{
				h_offset += 60;
			}
			else if (j == DFO)
			{
				h_offset += 100;
			}
			if (x >= TOTAL_MONSTER_SPRITES){ return; }
		}
	}

}

Button SelectScreen::getRemoveButton(int i)
{
	return this->teamRemove[i];
}

void SelectScreen::removeList(int ID)
{
	for (int j = 0; j < list.size(); j++)
	{
		if (list.at(j) == ID)
		{
			list.erase(list.begin() + j);
		}
	}
	std::cout << list.size() << std::endl;
}