#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include "SDL_image.h"
#include "../include/editeur.h"
#include "../include/misc.h"
#include "../include/constant.h"
#include "../include/niveau.h"

#ifdef HW_SCREEN_RESIZE
	extern SDL_Surface *hw_screen;
#endif //HW_SCREEN_RESIZE

int niveau_1_joueur(SDL_Surface *screen, int niveau){
	int continuer = 1;
	SDL_Surface *menu = NULL;
	SDL_Event event;
	SDL_Rect positionMenu;

#ifdef HW_SCREEN_RESIZE
	SDL_FillRect(hw_screen, NULL, 0x000000);
	//if(screen != NULL) SDL_FreeSurface(screen);
	screen = SDL_CreateRGBSurface(SDL_SWSURFACE, 480,480, WINDOW_BPP, 0, 0, 0, 0);
#else //HW_SCREEN_RESIZE
	screen = SDL_SetVideoMode(480,480, WINDOW_BPP, SDL_HWSURFACE);
	if (screen == NULL) {
		error("Can't set video mode: %s\n", SDL_GetError());
		exit(1);
	}
#endif //HW_SCREEN_RESIZE

	switch (niveau){
    case -1:
		menu = IMG_Load("sprite/legende_editeur.png");
		break;
	case 0:
		menu = IMG_Load("sprite/win_the_game.png");
		break;
	case 1:
		menu = IMG_Load("sprite/niveau_1.png");
		break;
	case 2:
		menu = IMG_Load("sprite/niveau_2.png");
		break;
	case 3:
		menu = IMG_Load("sprite/niveau_3.png");
		break;
	case 4:
		menu = IMG_Load("sprite/niveau_4.png");
		break;
	case 5:
		menu = IMG_Load("sprite/niveau_5.png");
		break;
	case 6:
		menu = IMG_Load("sprite/niveau_6.png");
		break;
	case 7:
		menu = IMG_Load("sprite/niveau_7.png");
		break;
	case 8:
		menu = IMG_Load("sprite/niveau_8.png");
		break;
	case 9:
		menu = IMG_Load("sprite/niveau_9.png");
		break;
	case 10:
		menu = IMG_Load("sprite/niveau_10.png");
		break;
	}
	positionMenu.x = 0;
	positionMenu.y = 0;

	/** Wait for input */
	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
		case SDL_QUIT:
			if(menu != NULL) SDL_FreeSurface(menu);
			return 1;
			break;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
			case SDLK_q:
			case SDLK_b:
				if(menu != NULL) SDL_FreeSurface(menu);
				return 1;
				break;
			case SDLK_RETURN:
			case SDLK_KP_ENTER:
			case SDLK_a:
				continuer = 0;
				break;
			default: break;
			}
		}
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		SDL_BlitSurface(menu, NULL, screen, &positionMenu);
#ifdef HW_SCREEN_RESIZE
		flip_NNOptimized_AllowOutOfScreen(screen, hw_screen,
	        HW_SCREEN_WIDTH,
	        MIN(screen->h*HW_SCREEN_WIDTH/screen->w, HW_SCREEN_HEIGHT));
		SDL_Flip(hw_screen);
#else //HW_SCREEN_RESIZE
		SDL_Flip(screen);
#endif //HW_SCREEN_RESIZE
	}
	if(menu != NULL) SDL_FreeSurface(menu);
	return 0;
}

int niveau_2_joueur(SDL_Surface *screen, int choix_niveau){
	
#ifdef HW_SCREEN_RESIZE
	SDL_FillRect(hw_screen, NULL, 0x000000);
	//if(screen != NULL) SDL_FreeSurface(screen);
	screen = SDL_CreateRGBSurface(SDL_SWSURFACE, 480,480, WINDOW_BPP, 0, 0, 0, 0);
#else //HW_SCREEN_RESIZE
	screen = SDL_SetVideoMode(480,480, WINDOW_BPP, SDL_HWSURFACE);
	if (screen == NULL) {
		error("Can't set video mode: %s\n", SDL_GetError());
		exit(1);
	}
#endif //HW_SCREEN_RESIZE
	int continuer = 1, choix_actuel=choix_niveau;
	SDL_Surface *menu = NULL;
	SDL_Event event;
	SDL_Rect positionMenu;
	positionMenu.x = 0;
	positionMenu.y = 0;
	switch (choix_actuel){
	case 1:
		menu = IMG_Load("sprite/niveau_2p_1.png");
		break;
	case 2:
		menu = IMG_Load("sprite/niveau_2p_2.png");
		break;
	case 3:
		menu = IMG_Load("sprite/niveau_2p_3.png");
		break;
	case 4:
		menu = IMG_Load("sprite/niveau_2p_4.png");
		break;
	case 5:
		menu = IMG_Load("sprite/niveau_2p_5.png");
		break;
	case 6:
		menu = IMG_Load("sprite/niveau_2p_6.png");
		break;
	default: break;
	}
	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
		case SDL_QUIT:
			continuer=0;
			break;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
			{
			case SDLK_ESCAPE: // Veut arrêter le jeu
			case SDLK_b: // Veut arrêter le jeu
			case SDLK_q:
				continuer=0;
				break;
			case SDLK_UP:
			case SDLK_u:
				switch(choix_actuel){
				case 1:
					menu = IMG_Load("sprite/niveau_2p_6.png");;
					choix_actuel=6;
					break;
				case 2:
					menu = IMG_Load("sprite/niveau_2p_1.png");
					choix_actuel=1;
					break;
				case 3:
					menu = IMG_Load("sprite/niveau_2p_2.png");
					choix_actuel=2;
					break;
				case 4:
					menu = IMG_Load("sprite/niveau_2p_3.png");;
					choix_actuel=3;
					break;
				case 5:
					menu = IMG_Load("sprite/niveau_2p_4.png");
					choix_actuel=4;
					break;
				case 6:
					menu = IMG_Load("sprite/niveau_2p_5.png");
					choix_actuel=5;
					break;
				}
				break;
			case SDLK_DOWN:
			case SDLK_d:
				switch(choix_actuel){
				case 1:
					menu = IMG_Load("sprite/niveau_2p_2.png");;
					choix_actuel=2;
					break;
				case 2:
					menu = IMG_Load("sprite/niveau_2p_3.png");
					choix_actuel=3;
					break;
				case 3:
					menu = IMG_Load("sprite/niveau_2p_4.png");
					choix_actuel=4;
					break;
				case 4:
					menu = IMG_Load("sprite/niveau_2p_5.png");;
					choix_actuel=5;
					break;
				case 5:
					menu = IMG_Load("sprite/niveau_2p_6.png");
					choix_actuel=6;
					break;
				case 6:
					menu = IMG_Load("sprite/niveau_2p_1.png");
					choix_actuel=1;
					break;
				}
				break;
			case SDLK_RETURN:
			case SDLK_KP_ENTER:
			case SDLK_a:
				if(menu != NULL) SDL_FreeSurface(menu);
				return choix_actuel;
				break;

				default : break;
			}
			break;
		}

		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		SDL_BlitSurface(menu, NULL, screen, &positionMenu);
#ifdef HW_SCREEN_RESIZE
		flip_NNOptimized_AllowOutOfScreen(screen, hw_screen,
	        HW_SCREEN_WIDTH,
	        MIN(screen->h*HW_SCREEN_WIDTH/screen->w, HW_SCREEN_HEIGHT));
		SDL_Flip(hw_screen);
#else //HW_SCREEN_RESIZE
		SDL_Flip(screen);
#endif //HW_SCREEN_RESIZE
	}
	if(menu != NULL) SDL_FreeSurface(menu);
	return 0;
}

int editeur_choix_niveau(SDL_Surface *screen){
	
#ifdef HW_SCREEN_RESIZE
	SDL_FillRect(hw_screen, NULL, 0x000000);
	//if(screen != NULL) SDL_FreeSurface(screen);
	screen = SDL_CreateRGBSurface(SDL_SWSURFACE, 480,480, WINDOW_BPP, 0, 0, 0, 0);
#else //HW_SCREEN_RESIZE
	screen = SDL_SetVideoMode(480,480, WINDOW_BPP, SDL_HWSURFACE);
	if (screen == NULL) {
		error("Can't set video mode: %s\n", SDL_GetError());
		exit(1);
	}
#endif //HW_SCREEN_RESIZE
	int continuer = 1, choix_actuel=1;
	SDL_Surface *menu = NULL;
	SDL_Event event;
	SDL_Rect positionMenu;
	positionMenu.x = 0;
	positionMenu.y = 0;
	menu = IMG_Load("sprite/niveau_2p_1.png");
	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
		case SDL_QUIT:
			continuer=0;
			break;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
			{
			case SDLK_ESCAPE: // Veut arrêter le jeu
			case SDLK_b: // Veut arrêter le jeu
			case SDLK_q:
				continuer=0;
				break;
			case SDLK_UP:
			case SDLK_u:
				switch(choix_actuel){
				case 1:
					menu = IMG_Load("sprite/niveau_2p_6.png");;
					choix_actuel=6;
					break;
				case 2:
					menu = IMG_Load("sprite/niveau_2p_1.png");
					choix_actuel=1;
					break;
				case 3:
					menu = IMG_Load("sprite/niveau_2p_2.png");
					choix_actuel=2;
					break;
				case 4:
					menu = IMG_Load("sprite/niveau_2p_3.png");;
					choix_actuel=3;
					break;
				case 5:
					menu = IMG_Load("sprite/niveau_2p_4.png");
					choix_actuel=4;
					break;
				case 6:
					menu = IMG_Load("sprite/niveau_2p_5.png");
					choix_actuel=5;
					break;
				}
				break;
			case SDLK_DOWN:
			case SDLK_d:
				switch(choix_actuel){
				case 1:
					menu = IMG_Load("sprite/niveau_2p_2.png");;
					choix_actuel=2;
					break;
				case 2:
					menu = IMG_Load("sprite/niveau_2p_3.png");
					choix_actuel=3;
					break;
				case 3:
					menu = IMG_Load("sprite/niveau_2p_4.png");
					choix_actuel=4;
					break;
				case 4:
					menu = IMG_Load("sprite/niveau_2p_5.png");;
					choix_actuel=5;
					break;
				case 5:
					menu = IMG_Load("sprite/niveau_2p_6.png");
					choix_actuel=6;
					break;
				case 6:
					menu = IMG_Load("sprite/niveau_2p_1.png");
					choix_actuel=1;
					break;
				}
				break;
			case SDLK_RETURN:
			case SDLK_KP_ENTER:
			case SDLK_a:
				if(menu != NULL) SDL_FreeSurface(menu);
				return choix_actuel;
				break;

				default : break;
			}
			break;
		}

		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		SDL_BlitSurface(menu, NULL, screen, &positionMenu);
#ifdef HW_SCREEN_RESIZE
		flip_NNOptimized_AllowOutOfScreen(screen, hw_screen,
	        HW_SCREEN_WIDTH,
	        MIN(screen->h*HW_SCREEN_WIDTH/screen->w, HW_SCREEN_HEIGHT));
		SDL_Flip(hw_screen);
#else //HW_SCREEN_RESIZE
		SDL_Flip(screen);
#endif //HW_SCREEN_RESIZE
	}
	if(menu != NULL) SDL_FreeSurface(menu);
	return 0;
}


int are_you_sure(SDL_Surface *screen){

#ifdef FUNKEY
	return 1;
#endif //FUNKEY

	
#ifdef HW_SCREEN_RESIZE
	SDL_FillRect(hw_screen, NULL, 0x000000);
	//if(screen != NULL) SDL_FreeSurface(screen);
	screen = SDL_CreateRGBSurface(SDL_SWSURFACE, 480,480, WINDOW_BPP, 0, 0, 0, 0);
#else //HW_SCREEN_RESIZE
	screen = SDL_SetVideoMode(480,480, WINDOW_BPP, SDL_HWSURFACE);
	if (screen == NULL) {
		error("Can't set video mode: %s\n", SDL_GetError());
		exit(1);
	}
#endif //HW_SCREEN_RESIZE

	int continuer = 1, choix_actuel=1;
	SDL_Surface *menu = NULL;
	SDL_Event event;
	SDL_Rect positionMenu;
	positionMenu.x = 0;
	positionMenu.y = 0;
	menu = IMG_Load("sprite/sure_yes.png");
	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
		case SDL_QUIT:
			continuer=0;
			break;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
			{
			case SDLK_ESCAPE: // Veut arrêter le jeu
			case SDLK_b: // Veut arrêter le jeu
			case SDLK_q:
				continuer=0;
				break;
			case SDLK_LEFT:
			case SDLK_l:
				switch(choix_actuel){
				case 1:
					if(menu != NULL) SDL_FreeSurface(menu);
					menu = IMG_Load("sprite/sure_no.png");;
					choix_actuel=2;
					break;
				case 2:
					if(menu != NULL) SDL_FreeSurface(menu);
					menu = IMG_Load("sprite/sure_yes.png");
					choix_actuel=1;
					break;
				}
				break;
			case SDLK_RIGHT:
			case SDLK_r:
				switch(choix_actuel){
				case 1:
					if(menu != NULL) SDL_FreeSurface(menu);
					menu = IMG_Load("sprite/sure_no.png");;
					choix_actuel=2;
					break;
				case 2:
					if(menu != NULL) SDL_FreeSurface(menu);
					menu = IMG_Load("sprite/sure_yes.png");
					choix_actuel=1;
					break;
				}
				break;
			case SDLK_RETURN:
			case SDLK_KP_ENTER:
			case SDLK_a:
				if(menu != NULL) SDL_FreeSurface(menu);
				return choix_actuel;
				break;

				default : break;
			}
			break;
		}

		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		SDL_BlitSurface(menu, NULL, screen, &positionMenu);
#ifdef HW_SCREEN_RESIZE
		flip_NNOptimized_AllowOutOfScreen(screen, hw_screen,
	        HW_SCREEN_WIDTH,
	        MIN(screen->h*HW_SCREEN_WIDTH/screen->w, HW_SCREEN_HEIGHT));
		SDL_Flip(hw_screen);
#else //HW_SCREEN_RESIZE
		SDL_Flip(screen);
#endif //HW_SCREEN_RESIZE
	}
	if(menu != NULL) SDL_FreeSurface(menu);
	return 0;
}

int choix_nb_joueurs(SDL_Surface *screen){
	
#ifdef HW_SCREEN_RESIZE
	SDL_FillRect(hw_screen, NULL, 0x000000);
	//if(screen != NULL) SDL_FreeSurface(screen);
	screen = SDL_CreateRGBSurface(SDL_SWSURFACE, 480,480, WINDOW_BPP, 0, 0, 0, 0);
#else //HW_SCREEN_RESIZE
	screen = SDL_SetVideoMode(480,480, WINDOW_BPP, SDL_HWSURFACE);
	if (screen == NULL) {
		error("Can't set video mode: %s\n", SDL_GetError());
		exit(1);
	}
#endif //HW_SCREEN_RESIZE
	int continuer = 1, choix_actuel=1;
	SDL_Surface *menu = NULL;
	SDL_Event event;
	SDL_Rect positionMenu;
	positionMenu.x = 0;
	positionMenu.y = 0;
	menu = IMG_Load("sprite/menu_nb_joueur_1.png");
	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
		case SDL_QUIT:
			continuer=0;
			break;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
			{
			case SDLK_ESCAPE: // Veut arrêter le jeu
			case SDLK_b: // Veut arrêter le jeu
			case SDLK_q:
				continuer=0;
				break;
			case SDLK_UP:
			case SDLK_u:
				switch(choix_actuel){
				case 1:
					menu = IMG_Load("sprite/menu_nb_joueur_2.png");;
					choix_actuel=2;
					break;
				case 2:
					menu = IMG_Load("sprite/menu_nb_joueur_1.png");
					choix_actuel=1;
					break;
				}
				break;
			case SDLK_DOWN:
			case SDLK_d:
				switch(choix_actuel){
				case 1:
					menu = IMG_Load("sprite/menu_nb_joueur_2.png");;
					choix_actuel=2;
					break;
				case 2:
					menu = IMG_Load("sprite/menu_nb_joueur_1.png");
					choix_actuel=1;
					break;
				}
				break;
			case SDLK_RETURN:
			case SDLK_KP_ENTER:
			case SDLK_a:
				if(menu != NULL) SDL_FreeSurface(menu);
				return choix_actuel;
				break;

				default : break;
			}
			break;
		}

		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		SDL_BlitSurface(menu, NULL, screen, &positionMenu);
#ifdef HW_SCREEN_RESIZE
		flip_NNOptimized_AllowOutOfScreen(screen, hw_screen,
	        HW_SCREEN_WIDTH,
	        MIN(screen->h*HW_SCREEN_WIDTH/screen->w, HW_SCREEN_HEIGHT));
		SDL_Flip(hw_screen);
#else //HW_SCREEN_RESIZE
		SDL_Flip(screen);
#endif //HW_SCREEN_RESIZE
	}
	if(menu != NULL) SDL_FreeSurface(menu);
	return 0;
}

int options(SDL_Surface *screen){
	
#ifdef HW_SCREEN_RESIZE
	SDL_FillRect(hw_screen, NULL, 0x000000);
	//if(screen != NULL) SDL_FreeSurface(screen);
	screen = SDL_CreateRGBSurface(SDL_SWSURFACE, 480,480, WINDOW_BPP, 0, 0, 0, 0);
#else //HW_SCREEN_RESIZE
	screen = SDL_SetVideoMode(480,480, WINDOW_BPP, SDL_HWSURFACE);
	if (screen == NULL) {
		error("Can't set video mode: %s\n", SDL_GetError());
		exit(1);
	}
#endif //HW_SCREEN_RESIZE
	int continuer = 1, choix_actuel=1;
	SDL_Surface *menu = NULL;
	SDL_Event event;
	SDL_Rect positionMenu;
	positionMenu.x = 0;
	positionMenu.y = 0;
	menu = IMG_Load("sprite/options_s_o.png");
	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
		case SDL_QUIT:
			continuer=0;
			break;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
			{
			case SDLK_ESCAPE: // Veut arrêter le jeu
			case SDLK_b: // Veut arrêter le jeu
			case SDLK_q:
				continuer=0;
				break;
			case SDLK_LEFT:
			case SDLK_l:
				switch(choix_actuel){
				case 1:
					menu = IMG_Load("sprite/options_s_n.png");;
					choix_actuel=2;
					break;
				case 2:
					menu = IMG_Load("sprite/options_s_o.png");
					choix_actuel=1;
					break;
				case 3:
					menu = IMG_Load("sprite/options_b_n.png");
					choix_actuel=4;
					break;
				case 4:
					menu = IMG_Load("sprite/options_b_o.png");
					choix_actuel=3;
					break;
				}
				break;
			case SDLK_RIGHT:
			case SDLK_r:
				switch(choix_actuel){
				case 1:
					menu = IMG_Load("sprite/options_s_n.png");;
					choix_actuel=2;
					break;
				case 2:
					menu = IMG_Load("sprite/options_s_o.png");
					choix_actuel=1;
					break;
				case 3:
					menu = IMG_Load("sprite/options_b_n.png");
					choix_actuel=4;
					break;
				case 4:
					menu = IMG_Load("sprite/options_b_o.png");
					choix_actuel=3;
					break;
				}
				break;
			case SDLK_UP:
			case SDLK_u:
				switch(choix_actuel){
				case 1:
					menu = IMG_Load("sprite/options_b_o.png");;
					choix_actuel=3;
					break;
				case 2:
					menu = IMG_Load("sprite/options_b_o.png");
					choix_actuel=3;
					break;
				case 3:
					menu = IMG_Load("sprite/options_s_o.png");
					choix_actuel=1;
					break;
				case 4:
					menu = IMG_Load("sprite/options_s_o.png");
					choix_actuel=1;
					break;
				}
				break;
			case SDLK_DOWN:
			case SDLK_d:
				switch(choix_actuel){
				case 1:
					menu = IMG_Load("sprite/options_b_o.png");;
					choix_actuel=3;
					break;
				case 2:
					menu = IMG_Load("sprite/options_b_o.png");
					choix_actuel=3;
					break;
				case 3:
					menu = IMG_Load("sprite/options_s_o.png");
					choix_actuel=1;
					break;
				case 4:
					menu = IMG_Load("sprite/options_s_o.png");
					choix_actuel=1;
					break;
				}
				break;
			case SDLK_RETURN:
			case SDLK_KP_ENTER:
			case SDLK_a:
				if(menu != NULL) SDL_FreeSurface(menu);
				return choix_actuel;
				break;

				default : break;
			}
			break;
		}

		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		SDL_BlitSurface(menu, NULL, screen, &positionMenu);
#ifdef HW_SCREEN_RESIZE
		flip_NNOptimized_AllowOutOfScreen(screen, hw_screen,
	        HW_SCREEN_WIDTH,
	        MIN(screen->h*HW_SCREEN_WIDTH/screen->w, HW_SCREEN_HEIGHT));
		SDL_Flip(hw_screen);
#else //HW_SCREEN_RESIZE
		SDL_Flip(screen);
#endif //HW_SCREEN_RESIZE
	}
	if(menu != NULL) SDL_FreeSurface(menu);
	return 0;
}
