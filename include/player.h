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

struct s_player {
	int x, y, portee_bomb, win;
	SDL_Surface * directed_img[4];
	enum e_way current_way;
	int lives;
	int nb_bomb;
	int nb_bomb_max;//cette variable est utile pour les bonus. Elle sert à ce que, par exemple, si le joueur a posé toutes ses bombes (disons 2) et qu'avant qu'elles explosent le joueur prend un bonus decrease_nb_bombs, il ne sera autorisé qu'à poser une seule bombe après l'explosion des deux sur la map.
	int dead;
};
typedef struct s_player * t_player;


// Creates a new player with a given number of available bombs
t_player player_init(int nb_bomb, int portee_bomb, int lives, int no_joueur);
void player_free(t_player player);

// Returns the current position of the player
int player_get_x(t_player player);
int player_get_y(t_player player);

// Confirm if the player has won the game
int player_win(t_player player);

// Return the caracteristics of the player
int player_portee_bomb(t_player player);

//manage the lives of the player
int player_get_lives(t_player player);
void player_increase_lives(t_player player);
void player_decrease_lives(t_player player);
void player_die(t_player player);
int player_get_dead(t_player player);
// Set the direction of the next move of the player
void player_set_current_way(t_player player, enum e_way way);

// Load the player position from the map
int player_from_map(t_player player, t_map map);
void player2_from_map(t_player player, t_map map);

// Move the player according to the current direction
int player_move(t_player player, t_map  map);

//Manage the number of bombs available
int player_get_nb_bomb(t_player player);
int player_get_nb_bomb_max(t_player player);
void player_set_nb_bomb(t_player player, int bomb_number);
void player_increase_nb_bomb(t_player player);
void player_decrease_nb_bomb(t_player player);
void player_increase_nb_bomb_max(t_player player);
void player_decrease_nb_bomb_max(t_player player);


// Display the player on the screen
void player_display(t_player player, SDL_Surface *screen);

#endif /* PLAYER_H_ */
