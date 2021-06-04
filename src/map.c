#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL.h>
#include "SDL_image.h"

#include <time.h>
#include "../include/constant.h"
#include "../include/map.h"
#include "../include/misc.h"
#include "../include/game_time.h"
#include "../include/game.h"




struct t_map {
	int width;
	int height;

	t_cell_type *grid;

	SDL_Surface *img_wall;
	SDL_Surface *img_box;
	SDL_Surface *img_goal;
	SDL_Surface *img_bonus_bomb_range_inc;
	SDL_Surface *img_bonus_bomb_range_dec;
	SDL_Surface *img_bonus_bomb_nb_inc;
	SDL_Surface *img_bonus_bomb_nb_dec;
	SDL_Surface *img_bonus_life_inc;
	SDL_Surface *img_menu_lives_0;
	SDL_Surface *img_menu_lives_1;
	SDL_Surface *img_menu_lives_2;
	SDL_Surface *img_menu_lives_3;
	SDL_Surface *img_menu_lives_4;
	SDL_Surface *img_menu_bombs_0;
	SDL_Surface *img_menu_bombs_1;
	SDL_Surface *img_menu_bombs_2;
	SDL_Surface *img_menu_bombs_3;
	SDL_Surface *img_menu_bombs_4;
	SDL_Surface *img_menu_bombs_5;
	SDL_Surface *img_menu_range_0;
	SDL_Surface *img_menu_range_1;
	SDL_Surface *img_menu_range_2;
	SDL_Surface *img_menu_range_3;
	SDL_Surface *img_menu_range_4;
	SDL_Surface *img_menu_range_5;
	SDL_Surface *img_menu_empty;
	SDL_Surface *img_menu_player_1;
	SDL_Surface *img_menu_player_2;

	int nb_monsters;
};

#define CELL(i,j) (i +  map->width * j)

t_map map_new(int width, int height) {
	assert(width > 0 && height > 0);

	t_map map = malloc(sizeof(struct t_map));
	if (map == NULL)
	exit(EXIT_FAILURE);

	map->width = width;
	map->height = height;
	map->nb_monsters = 0;

	map->grid = (t_cell_type *) malloc(height * width * sizeof(t_cell_type));
	if (map->grid == NULL)
	exit(EXIT_FAILURE);

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++) {
			map->grid[CELL(i,j)] = CELL_EMPTY;
		}
	}

	// Sprite loading
	map->img_wall = load_image(IMG_MAP_WALL);
	map->img_box = load_image(IMG_MAP_BOX);
	map->img_goal = load_image(IMG_MAP_GOAL);
	map->img_bonus_bomb_range_inc = load_image(IMG_BONUS_BOMB_RANGE_INC);
	map->img_bonus_bomb_range_dec = load_image(IMG_BONUS_BOMB_RANGE_DEC);
	map->img_bonus_bomb_nb_inc = load_image(IMG_BONUS_BOMB_NB_INC);
	map->img_bonus_bomb_nb_dec = load_image(IMG_BONUS_BOMB_NB_DEC);
	map->img_bonus_life_inc = load_image(IMG_BONUS_LIFE_INC);
	map->img_menu_empty = load_image(IMG_MENU_EMPTY);
	map->img_menu_lives_0 = load_image(IMG_MENU_LIVES_0);
	map->img_menu_lives_1 = load_image(IMG_MENU_LIVES_1);
	map->img_menu_lives_2 = load_image(IMG_MENU_LIVES_2);
	map->img_menu_lives_3 = load_image(IMG_MENU_LIVES_3);
	map->img_menu_lives_4 = load_image(IMG_MENU_LIVES_4);
	map->img_menu_bombs_0 = load_image(IMG_MENU_BOMBS_0);
	map->img_menu_bombs_1 = load_image(IMG_MENU_BOMBS_1);
	map->img_menu_bombs_2 = load_image(IMG_MENU_BOMBS_2);
	map->img_menu_bombs_3 = load_image(IMG_MENU_BOMBS_3);
	map->img_menu_bombs_4 = load_image(IMG_MENU_BOMBS_4);
	map->img_menu_bombs_5 = load_image(IMG_MENU_BOMBS_5);
	map->img_menu_range_0 = load_image(IMG_MENU_RANGE_0);
	map->img_menu_range_1 = load_image(IMG_MENU_RANGE_1);
	map->img_menu_range_2 = load_image(IMG_MENU_RANGE_2);
	map->img_menu_range_3 = load_image(IMG_MENU_RANGE_3);
	map->img_menu_range_4 = load_image(IMG_MENU_RANGE_4);
	map->img_menu_range_5 = load_image(IMG_MENU_RANGE_5);
	map->img_menu_player_1 = load_image(IMG_MENU_PLAYER_1);
	map->img_menu_player_2 = load_image(IMG_MENU_PLAYER_2);

	return map;
}

int map_is_inside(t_map map, int x, int y) {
	assert(map);
	int width=map_get_width(map);
	int height=map_get_height(map);
	if (x<0 || x>=width)
	return 0;
	if (y<0 || y>=height)
	return 0;
	return 1;
}


void map_free(t_map map) {
	if (map == NULL)
	return;

	free(map->grid);
	SDL_FreeSurface(map->img_wall);
	SDL_FreeSurface(map->img_box);
	SDL_FreeSurface(map->img_goal);
	SDL_FreeSurface(map->img_bonus_bomb_range_inc);
	SDL_FreeSurface(map->img_bonus_bomb_range_dec);
	SDL_FreeSurface(map->img_bonus_bomb_nb_inc);
	SDL_FreeSurface(map->img_bonus_bomb_nb_dec);
	SDL_FreeSurface(map->img_bonus_life_inc);
	SDL_FreeSurface(map->img_menu_empty);
	SDL_FreeSurface(map->img_menu_lives_0);
	SDL_FreeSurface(map->img_menu_lives_1);
	SDL_FreeSurface(map->img_menu_lives_2);
	SDL_FreeSurface(map->img_menu_lives_3);
	SDL_FreeSurface(map->img_menu_lives_4);
	SDL_FreeSurface(map->img_menu_bombs_0);
	SDL_FreeSurface(map->img_menu_bombs_1);
	SDL_FreeSurface(map->img_menu_bombs_2);
	SDL_FreeSurface(map->img_menu_bombs_3);
	SDL_FreeSurface(map->img_menu_bombs_4);
	SDL_FreeSurface(map->img_menu_bombs_5);
	SDL_FreeSurface(map->img_menu_range_0);
	SDL_FreeSurface(map->img_menu_range_1);
	SDL_FreeSurface(map->img_menu_range_2);
	SDL_FreeSurface(map->img_menu_range_3);
	SDL_FreeSurface(map->img_menu_range_4);
	SDL_FreeSurface(map->img_menu_range_5);
	SDL_FreeSurface(map->img_menu_player_1);
	SDL_FreeSurface(map->img_menu_player_2);
	free(map);
}

int map_get_width(t_map map) {
	assert(map != NULL);
	return map->width;
}

int map_get_height(t_map map) {
	assert(map != NULL);
	return map->height;
}

t_cell_type map_get_cell_type(t_map map, int x, int y) {
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)];
}


void map_set_cell_type(t_map map, int x, int y, t_cell_type type) {
	assert(map && map_is_inside(map, x, y));

	map->grid[CELL(x,y)] = type;
}

t_map map_load_dynamic(FILE *fd, int niveau,int nb_joueur) {
	int width, height;
	char a;int k=1, i=0, continuer=1;
	int taille_tableau=0;
	t_map map =NULL;
	if (fd) {
		while (continuer==1){
			if (k==niveau){
				fscanf (fd, "%d:%d", &height, &width);
				taille_tableau= width*height;
				map = map_new(width, height);
				while (i<taille_tableau)
				{
					a=fgetc(fd);
					if (a != '\n' && a != ' '&& a != '\r')
					{
						switch (a){
						case '0':
							map->grid[i] = CELL_EMPTY;
							break;
						case '1':
							map->grid[i] = CELL_WALL;
							break;
						case '2':
							map->grid[i] = CELL_BOX | (return_bonus() << 4);
							break;
						case '3':
							map->grid[i] = CELL_GOAL;
							break;
						case '4':
							map->grid[i] = CELL_PLAYER;
							break;

						case 'j':
							if (nb_joueur==2)
							map->grid[i] = CELL_PLAYER_2;
							else
							map->grid[i] = CELL_EMPTY;
							break;

						case '5':
							map->grid[i] = (CELL_BONUS | (BONUS_BOMB_RANGE_INC << 4));
							break;

						case 'k':
							map->grid[i] = (CELL_MENU_PLAYER_1);
							break;

						case 'l':
							map->grid[i] = (CELL_MENU_PLAYER_2);
							break;

						case '6':
							map->grid[i] = (CELL_MENU_EMPTY);
							break;

						case '7':
							map->grid[i] = (CELL_MENU_LIVES);
							break;

						case '8':
							map->grid[i] = (CELL_MENU_BOMBS);
							break;

						case '9':
							map->grid[i] = (CELL_MENU_RANGE);
							break;

						default:	exit(EXIT_FAILURE);
						}
						i++;
					}
				}
				continuer=0;
			}
			a=fgetc(fd);
			if (a=='-') k++;
		}

		fclose(fd);
		return map;
	}
	exit(EXIT_FAILURE);
}

void sauvegarderNiveau(t_map map, int niveau){
	FILE* fichier = NULL;
	int i = 0, j=0, continuer=1,k=1;
	char a;
	fichier = fopen("data/niveaux.lvl", "r+");
	while (continuer==1){
		if (k==niveau) continuer=0;
		a=fgetc(fichier);
		if (a=='-') k++;
	}
	if (niveau==1) fseek(fichier, -1, SEEK_CUR);
	else fseek(fichier, 0, SEEK_CUR);
	fprintf(fichier, "%d:%d", map->height,map->width);
	for (j = 0; j < map->height; j++) {
		fprintf(fichier, "%s", "\n");
		for (i = 0; i < map->width; i++) {
			t_cell_type type = map->grid[CELL(i,j)];
			switch (type & 0x0f) {
			case CELL_EMPTY:
				fprintf(fichier, "%d", 0);
				break;

			case CELL_WALL:
				fprintf(fichier, "%d", 1);
				break;
			case CELL_BOX:
				fprintf(fichier, "%d", 2);
				break;
			case CELL_GOAL:
				fprintf(fichier, "%d", 3);
				break;
			case CELL_PLAYER:
				fprintf(fichier, "%d", 4);
				break;
			case CELL_PLAYER_2:
				fputc('j', fichier);
				break;
			case CELL_BONUS:
				switch ((type & 0xf0) >> 4) {
				case BONUS_BOMB_RANGE_INC:
					fprintf(fichier, "%d", 5);
					break;
				default:	fprintf(fichier, "%d", 0);
				}
				break;

			case CELL_MENU_EMPTY:
				fprintf(fichier, "%d", 6);
				break;

			case CELL_MENU_PLAYER_1:
				fputc('k', fichier);
				break;

			case CELL_MENU_PLAYER_2:
				fputc('l', fichier);
				break;

			case CELL_MENU_LIVES:
				fprintf(fichier, "%d", 7);
				break;

			case CELL_MENU_BOMBS:
				fprintf(fichier, "%d", 8);
				break;

			case CELL_MENU_RANGE:
				fprintf(fichier, "%d", 9);
				break;

			default:	fprintf(fichier, "%d", 0);
			}
		}
	}

	fprintf(fichier, "\n-\n");
	fclose(fichier);
}

// t_map map_load_static(void) {

// t_map map = map_new(12, 12);
// t_cell_type
// themap[144] = { CELL_PLAYER, CELL_BOX, CELL_EMPTY, CELL_EMPTY,
// CELL_EMPTY, CELL_WALL, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
// CELL_BOX, CELL_BOX, CELL_EMPTY, CELL_EMPTY, CELL_WALL,
// CELL_WALL, CELL_EMPTY, CELL_EMPTY, CELL_BOX, CELL_EMPTY,
// CELL_WALL, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
// CELL_EMPTY, CELL_BOX, CELL_EMPTY, CELL_EMPTY, CELL_WALL,
// CELL_WALL, CELL_WALL, CELL_WALL, CELL_EMPTY, CELL_BOX,
// CELL_BOX, CELL_EMPTY, CELL_BOX, CELL_BOX, CELL_WALL,
// CELL_WALL, CELL_WALL, CELL_EMPTY, CELL_BOX, CELL_EMPTY,
// CELL_EMPTY, CELL_BOX, CELL_EMPTY, CELL_EMPTY, CELL_BOX,
// CELL_WALL, CELL_BOX, CELL_EMPTY, CELL_EMPTY, CELL_BOX,
// CELL_BOX, CELL_BOX, CELL_WALL, CELL_WALL, CELL_EMPTY,
// CELL_EMPTY, CELL_WALL, CELL_BOX, CELL_BOX, CELL_EMPTY,
// CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_EMPTY,
// CELL_EMPTY, CELL_WALL, CELL_WALL, CELL_EMPTY, CELL_EMPTY,
// CELL_EMPTY, CELL_WALL, CELL_BOX, CELL_BOX, CELL_BOX,
// CELL_WALL, CELL_WALL, CELL_BOX, CELL_EMPTY, CELL_EMPTY,
// CELL_EMPTY, CELL_BOX, CELL_BOX, CELL_WALL, CELL_BOX,
// CELL_BOX, CELL_WALL, CELL_WALL, CELL_EMPTY, CELL_BOX,
// CELL_BOX, CELL_EMPTY, CELL_BOX, CELL_EMPTY, CELL_EMPTY,
// CELL_WALL, CELL_WALL, CELL_BOX, CELL_EMPTY, CELL_EMPTY,
// CELL_BOX, CELL_BOX, CELL_BOX, CELL_WALL, CELL_EMPTY,
// CELL_WALL, CELL_WALL, CELL_WALL, CELL_BOX, CELL_EMPTY,
// CELL_WALL, CELL_WALL, CELL_EMPTY, CELL_EMPTY, CELL_BOX,
// CELL_BOX, CELL_EMPTY, CELL_WALL, CELL_EMPTY, CELL_BOX,
// CELL_EMPTY, CELL_EMPTY, CELL_WALL, CELL_EMPTY, CELL_BOX,
// CELL_BOX, CELL_WALL, CELL_BOX, CELL_EMPTY, CELL_EMPTY,
// CELL_EMPTY, CELL_WALL, CELL_EMPTY, CELL_EMPTY, CELL_WALL,
// CELL_WALL, CELL_EMPTY, CELL_EMPTY, CELL_WALL, CELL_GOAL };
// for (int i = 0; i < 144; i++)
// map->grid[i] = themap[i];
// return map;
// }


void map_display(t_map map, int lives_player, int bombs_player, int range_player, int lives_player2, int bombs_player2, int range_player2, SDL_Surface *screen) {
	int i, j, k=0, l=0, m=0;

	assert(map != NULL);
	assert(screen != NULL);
	assert(map->height > 0 && map->width > 0);
	assert(map->img_wall != NULL);
	assert(map->img_box != NULL);
	assert(map->img_goal != NULL);

	for (j = 0; j < map->height; j++) {
		for (i = 0; i < map->width; i++) {
			SDL_Rect place;
			place.x = i * SIZE_BLOC;
			place.y = j * SIZE_BLOC;

			t_cell_type type = map->grid[CELL(i,j)];

			switch (type & 0x0f) {
			case CELL_WALL:
				SDL_BlitSurface(map->img_wall, NULL, screen, &place);
				break;
			case CELL_BOX:
				SDL_BlitSurface(map->img_box, NULL, screen, &place);
				break;
			case CELL_GOAL:
				SDL_BlitSurface(map->img_goal, NULL, screen, &place);
				break;

			case CELL_MENU_EMPTY:
				SDL_BlitSurface(map->img_menu_empty, NULL, screen, &place);
				break;

			case CELL_MENU_PLAYER_1:
				SDL_BlitSurface(map->img_menu_player_1, NULL, screen, &place);
				break;

			case CELL_MENU_PLAYER_2:
				SDL_BlitSurface(map->img_menu_player_2, NULL, screen, &place);
				break;

			case CELL_MENU_LIVES:
				if (k==0){
					switch (lives_player){
					case 0:
						SDL_BlitSurface(map->img_menu_lives_0, NULL, screen, &place);
						break;
					case 1:
						SDL_BlitSurface(map->img_menu_lives_1, NULL, screen, &place);
						break;
					case 2:
						SDL_BlitSurface(map->img_menu_lives_2, NULL, screen, &place);
						break;
					case 3:
						SDL_BlitSurface(map->img_menu_lives_3, NULL, screen, &place);
						break;
					case 4:
						SDL_BlitSurface(map->img_menu_lives_4, NULL, screen, &place);
						break;
					}
					k++;
				}
				else {
					switch (lives_player2){
					case 0:
						SDL_BlitSurface(map->img_menu_lives_0, NULL, screen, &place);
						break;
					case 1:
						SDL_BlitSurface(map->img_menu_lives_1, NULL, screen, &place);
						break;
					case 2:
						SDL_BlitSurface(map->img_menu_lives_2, NULL, screen, &place);
						break;
					case 3:
						SDL_BlitSurface(map->img_menu_lives_3, NULL, screen, &place);
						break;
					case 4:
						SDL_BlitSurface(map->img_menu_lives_4, NULL, screen, &place);
						break;
					}
				}
				break;

			case CELL_MENU_BOMBS:
				if (l==0){
					switch (bombs_player){
					case 0:
						SDL_BlitSurface(map->img_menu_bombs_0, NULL, screen, &place);
						break;
					case 1:
						SDL_BlitSurface(map->img_menu_bombs_1, NULL, screen, &place);
						break;
					case 2:
						SDL_BlitSurface(map->img_menu_bombs_2, NULL, screen, &place);
						break;
					case 3:
						SDL_BlitSurface(map->img_menu_bombs_3, NULL, screen, &place);
						break;
					case 4:
						SDL_BlitSurface(map->img_menu_bombs_4, NULL, screen, &place);
						break;
					case 5:
						SDL_BlitSurface(map->img_menu_bombs_5, NULL, screen, &place);
						break;
					}
					l++;
				}
				else {
					switch (bombs_player2){
					case 0:
						SDL_BlitSurface(map->img_menu_bombs_0, NULL, screen, &place);
						break;
					case 1:
						SDL_BlitSurface(map->img_menu_bombs_1, NULL, screen, &place);
						break;
					case 2:
						SDL_BlitSurface(map->img_menu_bombs_2, NULL, screen, &place);
						break;
					case 3:
						SDL_BlitSurface(map->img_menu_bombs_3, NULL, screen, &place);
						break;
					case 4:
						SDL_BlitSurface(map->img_menu_bombs_4, NULL, screen, &place);
						break;
					case 5:
						SDL_BlitSurface(map->img_menu_bombs_5, NULL, screen, &place);
						break;
					}
				}
				break;

			case CELL_MENU_RANGE:
				if (m==0){
					switch (range_player){
					case 0:
						SDL_BlitSurface(map->img_menu_range_0, NULL, screen, &place);
						break;
					case 1:
						SDL_BlitSurface(map->img_menu_range_1, NULL, screen, &place);
						break;
					case 2:
						SDL_BlitSurface(map->img_menu_range_2, NULL, screen, &place);
						break;
					case 3:
						SDL_BlitSurface(map->img_menu_range_3, NULL, screen, &place);
						break;
					case 4:
						SDL_BlitSurface(map->img_menu_range_4, NULL, screen, &place);
						break;
					case 5:
						SDL_BlitSurface(map->img_menu_range_5, NULL, screen, &place);
						break;
					}
					m++;
				}
				else {
					switch (range_player2){
					case 0:
						SDL_BlitSurface(map->img_menu_range_0, NULL, screen, &place);
						break;
					case 1:
						SDL_BlitSurface(map->img_menu_range_1, NULL, screen, &place);
						break;
					case 2:
						SDL_BlitSurface(map->img_menu_range_2, NULL, screen, &place);
						break;
					case 3:
						SDL_BlitSurface(map->img_menu_range_3, NULL, screen, &place);
						break;
					case 4:
						SDL_BlitSurface(map->img_menu_range_4, NULL, screen, &place);
						break;
					case 5:
						SDL_BlitSurface(map->img_menu_range_5, NULL, screen, &place);
						break;
					}
				}
				break;


			case CELL_BONUS:
				switch ((type & 0xf0) >> 4) {
				case BONUS_BOMB_RANGE_INC:
					SDL_BlitSurface(map->img_bonus_bomb_range_inc, NULL,
					screen, &place);
					break;

				case BONUS_BOMB_RANGE_DEC:
					SDL_BlitSurface(map->img_bonus_bomb_range_dec, NULL,
					screen, &place);
					break;

				case BONUS_BOMB_NB_INC:
					SDL_BlitSurface(map->img_bonus_bomb_nb_inc, NULL, screen,
					&place);
					break;

				case BONUS_BOMB_NB_DEC:
					SDL_BlitSurface(map->img_bonus_bomb_nb_dec, NULL, screen,
					&place);
					break;

				case BONUS_LIFE_INC:
					SDL_BlitSurface(map->img_bonus_life_inc, NULL,
					screen, &place);
					break;
				}
			}
		}
	}
}

