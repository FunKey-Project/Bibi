#ifndef BOMB_H_
#define BOMB_H_

#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "../include/constant.h"

typedef struct s_bomb * t_bomb;

// Creates a new bomb at a given place
extern t_bomb bomb_init(t_bomb list_bombs, int x, int y, int portee, int joueur);
extern void bomb_free(t_bomb bomb);

// Returns the current position of a bomb
extern int bomb_get_x(t_bomb bomb);
extern int bomb_get_y(t_bomb bomb);
extern int bomb_get_joueur(t_bomb bomb);
extern int bomb_get_portee(t_bomb bomb);

//management of the following bomb
extern t_bomb get_next_bomb(t_bomb bomb);
void set_next_bomb(t_bomb bomb, t_bomb nxt_bomb);

// Set the next image of a bomb
extern void bomb_set_current_time(t_bomb bomb, int time);
extern int bomb_get_current_time(t_bomb bomb);

// Display a bomb on the screen
extern void bomb_display(t_bomb bomb, SDL_Surface *screen);





#endif /* BOMB_H_ */
