#ifndef __consts__
#define __consts__


//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

const int SUBSCREEN_WIDTH = 640;
const int SUBSCREEN_HEIGHT = 480;

const int LOWER_SCREEN_WIDTH = 640;
const int LOWER_SCREEN_HEIGHT = SCREEN_HEIGHT - SUBSCREEN_HEIGHT;

const int SELECT_WINDOW_WIDTH = SCREEN_WIDTH - 200;
const int SELECT_WINDOW_HEIGHT = 400;

//The dimensions of the level
const int LEVEL_WIDTH = 1280 / 2;
const int LEVEL_HEIGHT = 960 ;

//Tile constants
const int TILE_WIDTH = 40;
const int TILE_HEIGHT = 40;

const int TOTAL_TILES = (LEVEL_HEIGHT/TILE_HEIGHT) * (LEVEL_WIDTH/TILE_WIDTH);
const int TOTAL_TILE_SPRITES = 12;

//Monster constants
const int AVATAR_WIDTH = 100;
const int AVATAR_HEIGHT = 100;

const int TEAM_SIZE = 6;

const int TOTAL_STATS = 5;
const int HEALTH = 0;
const int ATTACK = 1;
const int RANGE = 2;
const int SPEED = 3;
const int NAME = 4;

//Font stuff
const int FONT_SIZE = 18;

//The different tile sprites
const int TILE_RED = 2;
const int TILE_GREEN = 1;
const int TILE_BLUE = 0;
const int TILE_CENTER = 3;
const int TILE_TOP = 4;
const int TILE_TOPRIGHT = 5;
const int TILE_RIGHT = 6;
const int TILE_BOTTOMRIGHT = 7;
const int TILE_BOTTOM = 8;
const int TILE_BOTTOMLEFT = 9;
const int TILE_LEFT = 10;
const int TILE_TOPLEFT = 11;

//Character sprites
enum {
	//world of warcraft
	Illidan_Stormrage,
	Akama,
	Magni_Bronzebeard,
	Tyrande_Whisperwind,
	Lich_King,
	Maiev_Shadowsong,
	Baine_Bloodhoof,
	Sylvanas_Windrunner,
	Glenn_Greymane,
	Garrosh_Hellscream,
	Varian_Wrynn,
	Prophet_Velen,
	Goblin_Trade_Prince,
	Voljin,
	Lorthemar_Theron,
	Gelbin_Mekkatorque,

	//Diablo
	Diablo,
	Mephisto,
	Baal,
	King_Leoric,
	Imperius,
	Tyrael,
	Malthael,
	Izual,

	//WarHammer 40k
	Mephiston,
	Swarm_Lord,
	Deathleaper,
	Typhus,
	Kharn,
	Ahriman,
	Lucius,
	Kaela_Meinsha_Khaine,
	Astrubael_Vect,
	O_Shovah,
	Imotekh_Stormlord,
	Snagrod,
	Sebastian_Yarrick,
	Darnath_Lysander,
	Logan_Grimnar,
	Kaldor_Draigo,

	TOTAL_MONSTER_SPRITES
};


// stat screen locations
const int BLOCK_OFFSET = 15;

//overlay constants                     associated string constants
const int TOTAL_OVERLAYS = 18;
const int MELEE_ATTACK_MARK = 16;
const int RANGE_ATTACK_MARK = 17;
const int DEATH_OVERLAY = 12;
const int SKULL_OVERLAY = 13;
const int MELEE_ICON = 0;               const std::string melee_type = "Melee";
const int RANGED_ICON = 1;              const std::string ranged_type = "Ranged";
const int FIRE_ICON = 2;                const std::string fire_type = "Fire";
const int ICE_ICON = 3;                 const std::string ice_type = "Ice";
const int POISON_ICON = 4;              const std::string poison_type = "Poison";
const int THUNDER_ICON = 5;             const std::string thunder_type = "Thunder";
const int HEAL_OVERLAY = 14;
const int MAGIC_ATTACK_MARK = 15;
const int ARMOR_DEFENSE = 8;            const std::string armor_count = "Armor: ";
const int MAGIC_DEFENSE = 9;            const std::string resist_count = "Resist: ";
const int DARK_ICON = 6;                const std::string dark_type = "Dark";
const int HOLY_ICON = 7;                const std::string holy_type = "Holy";
const int QUESTION_MARK = 10;
const int X_MARK = 11;

const int TOTAL_ICONS = 12;

//damage types
const int MELEE_DAMAGE_TYPE = 0;
const int RANGED_DAMAGE_TYPE = 1;
const int FIRE_DAMAGE_TYPE = 2;
const int ICE_DAMAGE_TYPE = 3;
const int POISON_DAMAGE_TYPE = 4;
const int LIGHTING_DAMAGE_TYPE = 5;
const int DARK_DAMAGE_TYPE = 6;
const int HOLY_DAMAGE_TYPE = 7;

//Highlight constants
const int HIGHLIGHT_SELECT = 0;
const int HIGHLIGHT_RANGE_MOVEMENT = 1;
const int HIGHLIGHT_RANGE_ATTACK = 2;
const int HIGHLIGHT_POTENTIAL_TARGET = 3;
const int HIGHLIGHT_NOT_PASSABLE = 4;
const int HIGHLIGHT_HOVER = 5;
const int HIGHLIGHT_DEAD = 6;
const int HIGHLIGHT_MELEE_TARGET = 7;
const int HIGHLIGHT_RANGED_TARGET = 8;
const int HIGHLIGHT_MAGIC_TARGET = 9;

//game states
const int IDLE = 0;
const int MOVEMENT = 1;
const int BATTLE = 2;
const int MENU_SCREEN = 3;
const int PAUSE_SCREEN = 4;
const int CHARACTER_SELECTION = 5;
const int GAME_QUIT = 6;


//Menu functions
const int TOTAL_MENU_OPTIONS = 2;
const int FROM_MENU_START = 1;
const int FROM_MENU_QUIT = 0;

//Button names and values
const int BUTTON_WIDTH = 94;
const int BUTTON_HEIGHT = 42;
enum button_name{
	UP_BUTTON,
	HOVER_BUTTON,
	SELECT_BUTTON,

	TOTAL_BUTTONS
};
#endif