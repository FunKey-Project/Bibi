#ifndef PLAYER_H_
#define PLAYER_H_

#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "../include/constant.h"

enum e_way {
	UP = 0, DOWN, LEFT, RIGHT
};

enum e_player {
	PLAYER_STD, PLAYER_MONSTER
};

typedef struct s_player * t_player;

// Creates a new player with a given number of available bombs
extern t_player player_init(int nb_bomb, int portee_bomb, int lives, int no_joueur);
extern void player_free(t_player player);

// Returns the current position of the player
extern int player_get_x(t_player player);
extern int player_get_y(t_player player);

// Confirm if the player has won the game
extern int player_win(t_player player);

// Return the caracteristics of the player
extern int player_portee_bomb(t_player player);

//manage the lives of the player
extern int player_get_lives(t_player player);
extern void player_increase_lives(t_player player);
extern void player_decrease_lives(t_player player);
extern void player_die(t_player player);
extern int player_get_dead(t_player player);
// Set the direction of the next move of the player
extern void player_set_current_way(t_player player, enum e_way way);

// Load the player position from the map
extern int player_from_map(t_player player, t_map map);
extern void player2_from_map(t_player player, t_map map);

// Move the player according to the current direction
extern int player_move(t_player player, t_map  map);

//Manage the number of bombs available
extern int player_get_nb_bomb(t_player player);
extern int player_get_nb_bomb_max(t_player player);
extern void player_set_nb_bomb(t_player player, int bomb_number);
extern void player_increase_nb_bomb(t_player player);
extern void player_decrease_nb_bomb(t_player player);
extern void player_increase_nb_bomb_max(t_player player);
extern void player_decrease_nb_bomb_max(t_player player);


// Display the player on the screen
extern void player_display(t_player player, SDL_Surface *screen);

#endif /* PLAYER_H_ */
