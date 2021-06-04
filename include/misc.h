/*
 * misc.h
 *
 *  Created on: 15 mars 2010
 *      Author: reveille
 */

#ifndef MISC_H_
#define MISC_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include <assert.h>

#include "SDL.h"
#include "SDL_image.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#ifdef HW_SCREEN_RESIZE
	extern SDL_Surface *hw_screen;
#endif //HW_SCREEN_RESIZE

#ifdef SOUND_SDL_ACTIVATED
	extern bool audio_init_ok;
#endif //SOUND_SDL_ACTIVATED


// Print an error message on stderr and exit
extern void error(const char *s, ...);

// Load an image, raise an error in case of failure
extern SDL_Surface *load_image(const char *filename);

// Copy surface with Nearest neighboor scaling
void flip_NNOptimized_AllowOutOfScreen(SDL_Surface *virtual_screen, SDL_Surface *hardware_screen, int new_w, int new_h);

#endif /* MISC_H_ */
