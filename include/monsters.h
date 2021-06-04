#ifndef MONSTERS_H_
#define MONSTERS_H_

#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "../include/constant.h"

typedef struct s_monster * t_monster;

// Creates a new monster
extern t_monster monster_init(t_monster list_monsters, int x, int y, int portee_monstre);
extern void monster_free(t_monster monster);


// Returns the current position of the monster
extern int monster_get_x(t_monster monster);
extern int monster_get_y(t_monster monster);

//management of the following monster
extern t_monster get_next_monster(t_monster monster);
void set_next_monster(t_monster monster, t_monster nxt_monster);


extern int monster_portee(t_monster monster);

// Set the direction of the next move of the monster
enum e_way monster_get_current_way(t_monster monster);
extern void monster_set_current_way(t_monster monster, enum e_way way);

// Move the monster according to the current direction
extern int monster_move(t_monster monster, t_map  map, t_player player, t_player player2, int nb_joueur);
// Display the monster on the screen
extern void monster_display(t_monster monster, SDL_Surface *screen);

#endif /* MONSTERS_H_ */
