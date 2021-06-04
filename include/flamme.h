#ifndef FLAMME_H_
#define FLAMME_H_

#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "../include/constant.h"

enum e_type_flamme {
	HAUT = 0, BAS, GAUCHE, DROITE, HRZLEFT, HRZRIGHT, VERTUP, VERTDOWN, CENTER, M_HAUT, M_BAS, M_GAUCHE, M_DROITE
};

typedef struct s_flamme * t_flamme;

extern t_flamme flamme_init(t_flamme list_flammes, int x, int y, int portee, enum e_type_flamme type);
extern void flamme_free(t_flamme flamme);

// Returns the current position of a flamme
extern int flamme_get_x(t_flamme flamme);
extern int flamme_get_y(t_flamme flamme);
extern int flamme_get_portee(t_flamme flamme);
extern enum e_type_flamme flamme_get_type(t_flamme flamme);
extern t_flamme get_next_flamme(t_flamme flamme);
void set_next_flamme(t_flamme flamme, t_flamme nxt_flamme);

// Set the next image of a flamme
extern void flamme_decrease_current_time(t_flamme flamme);
extern int flamme_get_current_time(t_flamme flamme);

// Display a flamme on the screen
extern void flamme_display(t_flamme flamme, SDL_Surface *screen);

#endif /* FLAMME_H_ */

