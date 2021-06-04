#ifndef CONSTANT_H_
#define CONSTANT_H_


// Use Sound ?
//#define SOUND_FMOD_ACTIVATED
//#define SOUND_SDL_ACTIVATED

// Resize ?
//#define HW_SCREEN_RESIZE
#define HW_SCREEN_WIDTH		240
#define HW_SCREEN_HEIGHT	240

#define WINDOW_BPP      16

// Size (# of pixels) of a cell of the map
#define SIZE_BLOC       40

// Time management
#define MAX_SKIPPED_FRAMES 100
#define DEFAULT_GAME_SPEED 8 // the more, the faster
#define BOMBS_SPEED 5  // the less, the faster
#define MONSTER_SPEED 7  // the less, the faster
#define ANIMATION_SPEED 2

// General sprites
#define IMG_MAP_WALL        "sprite/wall.png"
#define IMG_MAP_BOX         "sprite/box.png"
#define IMG_MAP_GOAL        "sprite/objectif.png"

//Menu sprites
#define IMG_MENU_EMPTY          "sprite/menu_empty.png"
#define IMG_MENU_PLAYER_1       "sprite/menu_player_1.png"
#define IMG_MENU_PLAYER_2       "sprite/menu_player_2.png"
#define IMG_MENU_LIVES_0        "sprite/lives_0.png"
#define IMG_MENU_LIVES_1        "sprite/lives_1.png"
#define IMG_MENU_LIVES_2        "sprite/lives_2.png"
#define IMG_MENU_LIVES_3        "sprite/lives_3.png"
#define IMG_MENU_LIVES_4        "sprite/lives_4.png"
#define IMG_MENU_BOMBS_0        "sprite/bombs_0.png"
#define IMG_MENU_BOMBS_1        "sprite/bombs_1.png"
#define IMG_MENU_BOMBS_2        "sprite/bombs_2.png"
#define IMG_MENU_BOMBS_3        "sprite/bombs_3.png"
#define IMG_MENU_BOMBS_4        "sprite/bombs_4.png"
#define IMG_MENU_BOMBS_5        "sprite/bombs_5.png"
#define IMG_MENU_RANGE_0        "sprite/range_0.png"
#define IMG_MENU_RANGE_1        "sprite/range_1.png"
#define IMG_MENU_RANGE_2        "sprite/range_2.png"
#define IMG_MENU_RANGE_3        "sprite/range_3.png"
#define IMG_MENU_RANGE_4        "sprite/range_4.png"
#define IMG_MENU_RANGE_5        "sprite/range_5.png"

// Bombs
#define IMG_BOMB_TTL1       "sprite/bomb1.png"
#define IMG_BOMB_TTL2       "sprite/bomb2.png"
#define IMG_BOMB_TTL3       "sprite/bomb3.png"
#define IMG_BOMB_TTL4       "sprite/bomb4.png"

#define IMG_BONUS_BOMB_RANGE_INC  "sprite/bonus_bomb_range_inc.png"
#define IMG_BONUS_BOMB_RANGE_DEC  "sprite/bonus_bomb_range_dec.png"
#define IMG_BONUS_BOMB_NB_INC     "sprite/bonus_bomb_nb_inc.png"
#define IMG_BONUS_BOMB_NB_DEC     "sprite/bonus_bomb_nb_dec.png"
#define IMG_BONUS_LIFE_INC     "sprite/bonus_life_inc.png"


// Players
#define IMG_PLAYER_LEFT     "sprite/player_left.png"
#define IMG_PLAYER_UP       "sprite/player_up.png"
#define IMG_PLAYER_RIGHT    "sprite/player_right.png"
#define IMG_PLAYER_DOWN     "sprite/player_down.png"
#define IMG_PLAYER_2_LEFT     "sprite/player_left_2.png"
#define IMG_PLAYER_2_UP       "sprite/player_up_2.png"
#define IMG_PLAYER_2_RIGHT    "sprite/player_right_2.png"
#define IMG_PLAYER_2_DOWN     "sprite/player_down_2.png"

//Monsters
#define IMG_MONSTER_LEFT     "sprite/monster_left.png"
#define IMG_MONSTER_UP       "sprite/monster_up.png"
#define IMG_MONSTER_RIGHT    "sprite/monster_right.png"
#define IMG_MONSTER_DOWN     "sprite/monster_down.png"



//flamme
#define IMG_FLAMME_THIN_HAUT         "sprite/ThinUp.png"
#define IMG_FLAMME_THIN_BAS          "sprite/ThinDown.png"
#define IMG_FLAMME_THIN_GAUCHE       "sprite/ThinLeft.png"
#define IMG_FLAMME_THIN_DROITE       "sprite/ThinRight.png"
#define IMG_FLAMME_THIN_HRZ          "sprite/ThinHrz.png"
#define IMG_FLAMME_THIN_VERT         "sprite/ThinVert.png"
#define IMG_FLAMME_THIN_CENTER       "sprite/ThinCenter.png"
#define IMG_FLAMME_THIN_MONSTER_BAS       "sprite/monster_down_TV.png"
#define IMG_FLAMME_THIN_MONSTER_HAUT       "sprite/monster_up_TV.png"
#define IMG_FLAMME_THIN_MONSTER_GAUCHE       "sprite/monster_left_TV.png"
#define IMG_FLAMME_THIN_MONSTER_DROITE       "sprite/monster_right_TV.png"
#define IMG_FLAMME_MEDIUM_HAUT       "sprite/FireUp.png"
#define IMG_FLAMME_MEDIUM_BAS        "sprite/FireDown.png"
#define IMG_FLAMME_MEDIUM_GAUCHE     "sprite/FireLeft.png"
#define IMG_FLAMME_MEDIUM_DROITE     "sprite/FireRight.png"
#define IMG_FLAMME_MEDIUM_HRZ        "sprite/FireHrz.png"
#define IMG_FLAMME_MEDIUM_VERT       "sprite/FireVert.png"
#define IMG_FLAMME_MEDIUM_CENTER     "sprite/FireCenter.png"
#define IMG_FLAMME_MEDIUM_MONSTER_BAS       "sprite/monster_down_FV.png"
#define IMG_FLAMME_MEDIUM_MONSTER_HAUT       "sprite/monster_up_FV.png"
#define IMG_FLAMME_MEDIUM_MONSTER_GAUCHE       "sprite/monster_left_FV.png"
#define IMG_FLAMME_MEDIUM_MONSTER_DROITE       "sprite/monster_right_FV.png"
#define IMG_FLAMME_FIRE_HAUT         "sprite/LargeUp.png"
#define IMG_FLAMME_FIRE_BAS          "sprite/LargeDown.png"
#define IMG_FLAMME_FIRE_GAUCHE       "sprite/LargeLeft.png"
#define IMG_FLAMME_FIRE_DROITE       "sprite/LargeRight.png"
#define IMG_FLAMME_FIRE_HRZ          "sprite/LargeHrz.png"
#define IMG_FLAMME_FIRE_VERT         "sprite/LargeVert.png"
#define IMG_FLAMME_FIRE_CENTER       "sprite/LargeCenter.png"
#define IMG_FLAMME_FIRE_MONSTER_BAS       "sprite/monster_down_LV.png"
#define IMG_FLAMME_FIRE_MONSTER_HAUT       "sprite/monster_up_LV.png"
#define IMG_FLAMME_FIRE_MONSTER_GAUCHE       "sprite/monster_left_LV.png"
#define IMG_FLAMME_FIRE_MONSTER_DROITE       "sprite/monster_right_LV.png"
#define IMG_FLAMME_LARGE_HAUT        "sprite/XLUP.png"
#define IMG_FLAMME_LARGE_BAS         "sprite/XLDOWN.png"
#define IMG_FLAMME_LARGE_GAUCHE      "sprite/XLLEFT.png"
#define IMG_FLAMME_LARGE_DROITE      "sprite/XLRIGHT.png"
#define IMG_FLAMME_LARGE_HRZ         "sprite/XLHRZ.png"
#define IMG_FLAMME_LARGE_VERT        "sprite/XLVERT.png"
#define IMG_FLAMME_LARGE_CENTER      "sprite/XLCENTER.png"
#define IMG_FLAMME_LARGE_MONSTER_BAS       "sprite/monster_down_XLV.png"
#define IMG_FLAMME_LARGE_MONSTER_HAUT       "sprite/monster_up_XLV.png"
#define IMG_FLAMME_LARGE_MONSTER_GAUCHE       "sprite/monster_left_XLV.png"
#define IMG_FLAMME_LARGE_MONSTER_DROITE       "sprite/monster_right_XLV.png"

// Maximum lives for a player
#define MAXIMUM_LIVES     4

//Nombre de décès avant game over
#define NB_DECES         3

// Maximum power of a bomb
#define BOMB_RANGE_MAX      4

// Maximum number of bombs available
#define BOMB_NUMBER_MAX     5

// Bonus
// Probability that a box contains a bonus
#define PROBA_BONUS 60

#define PROBA_BONUS_BOMB_RANGE 30

// Increase the power of a bomb +1
#define PROBA_BONUS_BOMB_RANGE_INC 50

// Decrease the power of a bomb -1
#define PROBA_BONUS_BOMB_RANGE_DEC 50

#define PROBA_BONUS_BOMB_NB    30

// Increase the number of bombs
#define PROBA_BONUS_BOMB_NB_INC  30
// Decrease the number of bombs
#define PROBA_BONUS_BOMB_NB_DEC  70

#define PROBA_BONUS_LIFE_INC 10

#define PROBA_BONUS_MONSTER      30

// Name of the file that contains the map
#ifdef FUNKEY
	#define SRC_LEVELS_1_PLAYER		"data/levels_1p_FunKey.lvl"
#else
	#define SRC_LEVELS_1_PLAYER		"data/levels_1p.lvl"
#endif
#define SRC_LEVELS_2_PLAYERS	"data/levels_2p.lvl"
#ifdef FUNKEY
	#define SRC_LEVELS_EDITOR		"data/levels_editor_FunKey.lvl"
#else
	#define SRC_LEVELS_EDITOR		"data/levels_editor.lvl"
#endif




#endif /* CONSTANT */
