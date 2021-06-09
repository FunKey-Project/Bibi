#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include "SDL_image.h"
#include "../include/constant.h"
#include "../include/editeur.h"
#include "../include/map.h"
#include "../include/misc.h"

void editeur(SDL_Surface *screen, int niveau)
{
	SDL_Surface *wall = NULL, *box = NULL, *goal = NULL, *player = NULL,
	*player_2 = NULL, *bomb_range_inc= NULL, *menu_empty= NULL, *menu_lives= NULL,
	*menu_bombs= NULL, *menu_range= NULL, *menu_player_1= NULL, *menu_player_2= NULL, *temp= NULL;
	SDL_Rect place;
	SDL_Rect place_joueur_1;
	SDL_Rect place_mouse;
	SDL_Event event;

	int continuer = 1, clicGaucheEnCours = 0, clicDroitEnCours = 0;
	int objetActuel = CELL_WALL;
	int i, j, xp1, yp1, player1_ok=1, xp2, yp2, player2_ok=1;

	FILE *level;
	const char *file_path = SRC_LEVELS_EDITOR;
	level=fopen(file_path,"r");
	if(level==NULL){
		printf("ERROR: cannot open file: %s\n", file_path);
	}

	t_map map = map_load_dynamic(level,niveau,2);

#ifdef HW_SCREEN_RESIZE
	uint16_t scaled_height, scaled_height_offset;
	//if(screen != NULL) SDL_FreeSurface(screen);
	screen = SDL_CreateRGBSurface(SDL_SWSURFACE, SIZE_BLOC * map_get_width(map),
				SIZE_BLOC * map_get_height(map), WINDOW_BPP, 0, 0, 0, 0);
#else //HW_SCREEN_RESIZE
	screen = SDL_SetVideoMode(SIZE_BLOC * map_get_width(map),
				SIZE_BLOC * map_get_height(map), WINDOW_BPP,
				SDL_HWSURFACE);
	if (screen == NULL) {
		error("Can't set video mode: %s\n", SDL_GetError());
		exit(1);
	}
#endif //HW_SCREEN_RESIZE


	// Chargement des objets et du niveau
	wall = load_image(IMG_MAP_WALL);
	box = load_image(IMG_MAP_BOX);
	goal = load_image(IMG_MAP_GOAL);
	player = load_image(IMG_PLAYER_DOWN);
	player_2 = load_image(IMG_PLAYER_2_DOWN);
	bomb_range_inc=load_image(IMG_BONUS_BOMB_RANGE_INC);
	menu_empty=load_image(IMG_MENU_EMPTY);
	menu_lives=load_image(IMG_MENU_LIVES_1);
	menu_bombs=load_image(IMG_MENU_BOMBS_1);
	menu_range=load_image(IMG_MENU_RANGE_1);
	menu_player_1=load_image(IMG_MENU_PLAYER_1);
	menu_player_2=load_image(IMG_MENU_PLAYER_2);
	temp=wall;

	// Boucle infinie de l'éditeur
	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
		case SDL_QUIT:
			continuer = 0;
			break;
		case SDL_MOUSEBUTTONDOWN:

#ifdef HW_SCREEN_RESIZE
			//printf("\nBefore: (%d, %d)\n", event.button.x, event.button.y);
			event.button.x = event.button.x*(SIZE_BLOC*map_get_width(map))/HW_SCREEN_WIDTH;
			scaled_height = MIN(SIZE_BLOC*map_get_height(map)*HW_SCREEN_WIDTH/(SIZE_BLOC*map_get_width(map)), HW_SCREEN_HEIGHT);
			scaled_height_offset = (HW_SCREEN_HEIGHT-scaled_height);
			event.button.y = MAX(event.button.y*(SIZE_BLOC*map_get_height(map))/scaled_height, scaled_height_offset)-scaled_height_offset;
			event.motion.x = event.button.x;
			event.motion.y = event.button.y;
			//printf("After: (%d, %d)\n", event.button.x, event.button.y);
#endif // HW_SCREEN_RESIZE

			if (event.button.button == SDL_BUTTON_LEFT)
			{
				// On met l'objet actuellement choisi (wall, box...) à l'endroit du clic
				if (objetActuel==CELL_PLAYER){
					if(player1_ok!=1){
						map_set_cell_type(map, xp1 , yp1, CELL_EMPTY);
					}
					map_set_cell_type(map, event.button.x / SIZE_BLOC, event.button.y / SIZE_BLOC, objetActuel);
					xp1=event.button.x / SIZE_BLOC; yp1=event.button.y / SIZE_BLOC;
					player1_ok--;
				}
				else if (objetActuel==CELL_PLAYER_2){
					if(player2_ok!=1){
						map_set_cell_type(map, xp2 , yp2, CELL_EMPTY);
					}
					map_set_cell_type(map, event.button.x / SIZE_BLOC, event.button.y / SIZE_BLOC, objetActuel);
					xp2=event.button.x / SIZE_BLOC; yp2=event.button.y / SIZE_BLOC;
					player2_ok--;
				}
				else {
					map_set_cell_type(map, event.button.x / SIZE_BLOC, event.button.y / SIZE_BLOC, objetActuel);
				}
				clicGaucheEnCours = 1; // On active un booléen pour retenir qu'un bouton est enfoncé
			}
			else if (event.button.button == SDL_BUTTON_RIGHT) // Le clic droit sert à effacer
			{
				map_set_cell_type(map, event.button.x / SIZE_BLOC, event.button.y / SIZE_BLOC, CELL_EMPTY);
				clicDroitEnCours = 1;
			}
			break;
		case SDL_MOUSEBUTTONUP: // On désactive le booléen qui disait qu'un bouton était enfoncé
			if (event.button.button == SDL_BUTTON_LEFT)
			clicGaucheEnCours = 0;
			else if (event.button.button == SDL_BUTTON_RIGHT)
			clicDroitEnCours = 0;
			break;
		case SDL_MOUSEMOTION:

#ifdef HW_SCREEN_RESIZE
			//printf("\nBefore: (%d, %d)\n", event.button.x, event.button.y);
			event.button.x = event.button.x*(SIZE_BLOC*map_get_width(map))/HW_SCREEN_WIDTH;
			scaled_height = MIN(SIZE_BLOC*map_get_height(map)*HW_SCREEN_WIDTH/(SIZE_BLOC*map_get_width(map)), HW_SCREEN_HEIGHT);
			scaled_height_offset = (HW_SCREEN_HEIGHT-scaled_height);
			event.button.y = MAX(event.button.y*(SIZE_BLOC*map_get_height(map))/scaled_height, scaled_height_offset)-scaled_height_offset;
			event.motion.x = event.button.x;
			event.motion.y = event.button.y;
			//printf("After: (%d, %d)\n", event.button.x, event.button.y);
#endif // HW_SCREEN_RESIZE

			if (clicGaucheEnCours) // Si on déplace la souris et que le bouton gauche de la souris est enfoncé
			{
				if (objetActuel==CELL_PLAYER){
					if(player1_ok!=1){
						map_set_cell_type(map, xp1 , yp1, CELL_EMPTY);
					}
					map_set_cell_type(map, event.button.x / SIZE_BLOC, event.button.y / SIZE_BLOC, objetActuel);
					xp1=event.button.x / SIZE_BLOC; yp1=event.button.y / SIZE_BLOC;
					player1_ok--;
				}
				else if (objetActuel==CELL_PLAYER_2){
					if(player2_ok!=1){
						map_set_cell_type(map, xp2 , yp2, CELL_EMPTY);
					}
					map_set_cell_type(map, event.button.x / SIZE_BLOC, event.button.y / SIZE_BLOC, objetActuel);
					xp2=event.button.x / SIZE_BLOC; yp2=event.button.y / SIZE_BLOC;
					player2_ok--;
				}
				else {
					map_set_cell_type(map, event.button.x / SIZE_BLOC, event.button.y / SIZE_BLOC, objetActuel);
				}
			}
			else if (clicDroitEnCours) // Pareil pour le bouton droit de la souris
			{
				map_set_cell_type(map, event.motion.x / SIZE_BLOC, event.motion.y / SIZE_BLOC, CELL_EMPTY);
			}
			break;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				continuer = 0;
				break;
			case SDLK_s:
				sauvegarderNiveau(map,niveau);
				break;
			case SDLK_KP1:
				objetActuel = CELL_WALL;
				temp=wall;
				break;
			case SDLK_KP2:
				objetActuel = CELL_BOX;
				temp=box;
				break;
			case SDLK_KP3:
				objetActuel = CELL_GOAL;
				temp=goal;
				break;
			case SDLK_KP4:
				objetActuel = CELL_PLAYER;
				temp=player;
				break;

			case SDLK_KP5:
				objetActuel = (CELL_BONUS | (BONUS_BOMB_RANGE_INC << 4));
				temp=bomb_range_inc;
				break;

			case SDLK_KP6:
				objetActuel = CELL_MENU_EMPTY;
				temp=menu_empty;
				break;

			case SDLK_KP7:
				objetActuel = CELL_MENU_LIVES;
				temp=menu_lives;
				break;

			case SDLK_KP8:
				objetActuel = CELL_MENU_BOMBS;
				temp=menu_bombs;
				break;

			case SDLK_KP9:
				objetActuel = CELL_MENU_RANGE;
				temp=menu_range;
				break;

			case SDLK_j:
				objetActuel = CELL_PLAYER_2;
				temp=player_2;
				break;

			case SDLK_k:
				objetActuel = CELL_MENU_PLAYER_1;
				temp=menu_player_1;
				break;

			case SDLK_l:
				objetActuel = CELL_MENU_PLAYER_2;
				temp=menu_player_2;
				break;

				default :
				break;
			}
			break;

		}

		// Effacement de l'écran
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));



		player1_ok=1;
		player2_ok=1;
		// Placement des objets à l'écran
		for (j = 0; j < map_get_height(map); j++) {
			for (i = 0; i < map_get_width(map); i++) {
				place.x = i * SIZE_BLOC;
				place.y = j * SIZE_BLOC;
				place_joueur_1.x=xp1* SIZE_BLOC;
				place_joueur_1.y=yp1* SIZE_BLOC;

				t_cell_type type = map_get_cell_type(map, i, j);
				switch (type & 0x0f) {
				case CELL_WALL:
					SDL_BlitSurface(wall, NULL, screen, &place);
					break;
				case CELL_BOX:
					SDL_BlitSurface(box, NULL, screen, &place);
					break;
				case CELL_GOAL:
					SDL_BlitSurface(goal, NULL, screen, &place);
					break;

				case CELL_PLAYER:
					if(player1_ok!=1){
						map_set_cell_type(map, xp1 , yp1, CELL_EMPTY);
						SDL_BlitSurface(player, NULL, screen, &place);
					}
					SDL_BlitSurface(player, NULL, screen, &place);
					xp1=i; yp1=j;
					player1_ok--;
					break;

				case CELL_PLAYER_2:
					if(player2_ok!=1){
						map_set_cell_type(map, xp2 , yp2, CELL_EMPTY);
						SDL_BlitSurface(player, NULL, screen, &place);
					}
					SDL_BlitSurface(player_2, NULL, screen, &place);
					xp2=i; yp2=j;
					player2_ok--;
					break;

				case CELL_MENU_EMPTY:
					SDL_BlitSurface(menu_empty, NULL, screen, &place);
					break;

				case CELL_MENU_PLAYER_1:
					SDL_BlitSurface(menu_player_1, NULL, screen, &place);
					break;

				case CELL_MENU_PLAYER_2:
					SDL_BlitSurface(menu_player_2, NULL, screen, &place);
					break;

				case CELL_MENU_LIVES:
					SDL_BlitSurface(menu_lives, NULL, screen, &place);
					break;

				case CELL_MENU_BOMBS:
					SDL_BlitSurface(menu_bombs, NULL, screen, &place);
					break;

				case CELL_MENU_RANGE:
					SDL_BlitSurface(menu_range, NULL, screen, &place);
					break;


				case CELL_BONUS:
					SDL_BlitSurface(bomb_range_inc, NULL, screen, &place);
					break;
				}
			}
		}

		if(!map_is_inside(map, event.motion.x, event.motion.y)){
			place_mouse.x = event.motion.x ;
			place_mouse.y = event.motion.y ;
			SDL_BlitSurface(temp, NULL, screen, &place_mouse);
		}

		// Mise à jour de l'écran
#ifdef HW_SCREEN_RESIZE
		SDL_FillRect(hw_screen, NULL, 0x000000);
		flip_NNOptimized_AllowOutOfScreen(screen, hw_screen,
	        HW_SCREEN_WIDTH,
	        MIN(screen->h*HW_SCREEN_WIDTH/screen->w, HW_SCREEN_HEIGHT));
		SDL_Flip(hw_screen);
#else //HW_SCREEN_RESIZE
		SDL_Flip(screen);
#endif //HW_SCREEN_RESIZE
	}

	SDL_FreeSurface(wall);
	SDL_FreeSurface(box);
	SDL_FreeSurface(goal);
	SDL_FreeSurface(player);
	SDL_FreeSurface(bomb_range_inc);
	SDL_FreeSurface(menu_empty);
	SDL_FreeSurface(menu_lives);
	SDL_FreeSurface(menu_bombs);
	SDL_FreeSurface(menu_range);
}
