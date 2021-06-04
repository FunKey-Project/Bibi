/*
* misc.c
*
*  Created on: 15 mars 2010
*      Author: reveille
*/

#include "../include/misc.h"
#include "../include/constant.h"



void error(const char *s, ...) {
	va_list ap;
	va_start(ap, s);

	vfprintf(stderr, s, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}

SDL_Surface *load_image(const char *filename) {
	SDL_Surface *img = IMG_Load(filename);
	if (!img) {
		error("IMG_Load: %s\n", IMG_GetError());
	}
	return img;
}

#if defined(HW_SCREEN_RESIZE) && defined(HW_SCREEN_HEIGHT) && defined(HW_SCREEN_WIDTH)
  /// Nearest neighboor optimized with possible out of screen coordinates (for cropping)
  void flip_NNOptimized_AllowOutOfScreen(SDL_Surface *virtual_screen, SDL_Surface *hardware_screen, int new_w, int new_h){
    int w1=virtual_screen->w;
    //int h1=virtual_screen->h;
    int w2=new_w;
    int h2=new_h;
    int x_ratio = (int)((virtual_screen->w<<16)/w2);
    int y_ratio = (int)((virtual_screen->h<<16)/h2);
    int x2, y2 ;

    /// --- Compute padding for centering when out of bounds ---
    int y_padding = (HW_SCREEN_HEIGHT-new_h)/2;
    int x_padding = 0;
    if(w2>HW_SCREEN_WIDTH){
      x_padding = (w2-HW_SCREEN_WIDTH)/2 + 1;
    }
    int x_padding_ratio = x_padding*w1/w2;
    //printf("virtual_screen->h=%d, h2=%d\n", virtual_screen->h, h2);

    for (int i=0;i<h2;i++)
    {
      if(i>=HW_SCREEN_HEIGHT){
        continue;
      }

      uint16_t* t = (uint16_t*)(hardware_screen->pixels+((i+y_padding)* ((w2>HW_SCREEN_WIDTH)?HW_SCREEN_WIDTH:w2) )*sizeof(uint16_t));
      y2 = ((i*y_ratio)>>16);
      uint16_t* p = (uint16_t*)(virtual_screen->pixels + (y2*w1 + x_padding_ratio) *sizeof(uint16_t));
      int rat = 0;
      for (int j=0;j<w2;j++)
      {
        if(j>=HW_SCREEN_WIDTH){
          continue;
        }
        x2 = (rat>>16);
        *t++ = p[x2];
        rat += x_ratio;
        //printf("y=%d, x=%d, y2=%d, x2=%d, (y2*virtual_screen->w)+x2=%d\n", i, j, y2, x2, (y2*virtual_screen->w)+x2);
      }
    }
  }
#endif 