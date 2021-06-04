/*
 * map.h
 *
 *  Created on: 16 mars 2010
 *      Author: BUSO, GROSSE
 */

#ifndef MAP_H_
#define MAP_H_

#include <stdlib.h>
#include <stdio.h>
#include "SDL.h"
#include "SDL_image.h"


typedef enum {
  CELL_EMPTY = 0, CELL_GOAL, CELL_WALL, CELL_PLAYER, CELL_PLAYER_2, CELL_BOX,
  CELL_BONUS, CELL_MONSTER, CELL_BOMB, CELL_FLAMME, CELL_MENU_EMPTY,CELL_MENU_PLAYER_1,
  CELL_MENU_PLAYER_2,CELL_MENU_LIVES, CELL_MENU_BOMBS, CELL_MENU_RANGE} t_cell_type;

typedef enum {
	BONUS_BOMB_RANGE_INC = 1, BONUS_BOMB_RANGE_DEC, BONUS_BOMB_NB_INC, BONUS_BOMB_NB_DEC, BONUS_MONSTER, BONUS_LIFE_INC, NO_BONUS
} t_bonus_type;

typedef struct t_map * t_map;

// Create a new empty map
extern t_map map_new(int width, int height);
extern void map_free(t_map map);

// Return the height and width of a map
extern int map_get_width(t_map map);
extern int map_get_height(t_map map);

// Return the type of a cell
extern t_cell_type map_get_cell_type(t_map map, int x, int y);
// Set the type of a cell
extern void  map_set_cell_type(t_map map, int x, int y, t_cell_type type);

// Test if (x,y) is within the map'
extern int map_is_inside(t_map map, int x, int y);

// Load a static pre-calculated map
//extern t_map map_load_static(void);

// Load a dynamic map
extern t_map map_load_dynamic(FILE* fd, int niveau, int nb_joueur);
// Load a map
extern void sauvegarderNiveau(t_map map, int niveau);

// Display the map on the screen
extern void map_display(t_map map, int lives_player, int bombs_player, int range_player, int lives_player2, int bombs_player2, int range_player2, SDL_Surface *screen);

#endif /* MAP_H_ */
