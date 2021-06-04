/*
 * game.h
 *
 *  Created on: 15 mars 2010
 *      Author: reveillere
 */

#ifndef GAME_H_
#define GAME_H_

#include "../include/constant.h"
#include "../include/misc.h"

#include "../include/map.h"
#include "../include/player.h"
#include "../include/bomb.h"
#include "../include/monsters.h"
#include "../include/flamme.h"

#ifdef SOUND_FMOD_ACTIVATED
	#include <FMOD/fmod.h>
#endif //SOUND_FMOD_ACTIVATED

typedef struct s_game * t_game;

// Create a new game
extern t_game game_new(int nb_joueur, int niveau, int mode, int kill_bomb);
extern void game_free(t_game game);


// Return the player of the current game
extern t_player game_get_player1(t_game game);
extern t_player game_get_player2(t_game game);
// Return the map of the current game
extern t_map game_the_map(t_game game);

// Return a bomb of the current game
extern void game_init_bomb(t_game game, int x, int y, int portee, int joueur);
extern t_monster game_return_monster(t_game game, int monster_number);

// Generate a random bonus after an explosion
extern void generate_bonus(t_game game, int x, int y);
t_bonus_type return_bonus();

// Display the game on the screen
extern void game_display(t_game game, SDL_Surface *screen);

//manage the bombs
extern void bombs_management (t_game game, int all_bombs);

//manage the flammes
extern void flamme_management(t_game game);
extern void flamme_continue(t_flamme flamme, t_game game);
extern void flamme_check_right(int x, int y, int portee, t_game game);
extern void flamme_check_left(int x, int y, int portee, t_game game);
extern void flamme_check_up(int x, int y, int portee, t_game game);
extern void flamme_check_down(int x, int y, int portee, t_game game);

//manage the monsters
extern void monsters_management(t_game game, int nb_joueur);
extern t_monster kill_monster(t_monster list_monsters, int x, int y);

//explose a bomb
extern void kill_bomb(t_game game, int x, int y);

#endif /* GAME_H_ */
