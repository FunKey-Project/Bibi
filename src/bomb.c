#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "SDL.h"
#include "SDL_image.h"

#include "../include/game.h"

struct s_bomb {
	int x, y, portee;
	int joueur;
	SDL_Surface * time_image[4];
	int current_time; //contient 1 2 3 ou 4 selon l'etat de la bombe
	t_bomb nxt;
};


static void bomb_load_img(t_bomb bomb,int time, const char *filename) {
	bomb->time_image[time] = IMG_Load(filename);
	if (!bomb->time_image[time])
	error("IMG_Load: %s\n", IMG_GetError());

}

t_bomb bomb_init(t_bomb list_bombs, int x, int y, int portee, int joueur) {
	t_bomb bomb = malloc(sizeof(*bomb));
	if (!bomb)
	error("Memory error");

	bomb_load_img(bomb, 0, IMG_BOMB_TTL1);
	bomb_load_img(bomb, 1, IMG_BOMB_TTL2);
	bomb_load_img(bomb, 2, IMG_BOMB_TTL3);
	bomb_load_img(bomb, 3, IMG_BOMB_TTL4);

	bomb->current_time = 3;
	bomb->x=x;
	bomb->y=y;
	bomb->portee=portee;
	bomb->joueur=joueur;
	bomb->nxt=NULL;

	if( list_bombs== NULL)
	{
		return bomb;
	}
	else
	{
		t_bomb temp=list_bombs;
		while(temp->nxt != NULL)
		{
			temp = temp->nxt;
		}
		temp->nxt = bomb;
		return list_bombs;
	}
}

int bomb_get_x(t_bomb bomb) {
	assert(bomb != NULL);
	return bomb->x;
}

int bomb_get_y(t_bomb bomb) {
	assert(bomb != NULL);
	return bomb->y;
}
int bomb_get_joueur(t_bomb bomb) {
	assert(bomb != NULL);
	return bomb->joueur;
}

t_bomb get_next_bomb(t_bomb bomb){
	return bomb->nxt;
}

void set_next_bomb(t_bomb bomb, t_bomb nxt_bomb){
	bomb->nxt=nxt_bomb;
}

int bomb_get_portee(t_bomb bomb) {
	assert(bomb != NULL);
	return bomb->portee;
}

void bomb_set_current_time(t_bomb bomb, int time) {
	assert(bomb);
	bomb->current_time = time;
}

int bomb_get_current_time(t_bomb bomb) {
	assert(bomb);
	return(bomb->current_time);
}

void bomb_free(t_bomb bomb) {
	if (bomb != NULL){
		assert(bomb);
		int i;
		for (i = 0; i < 4; i++)
		SDL_FreeSurface(bomb->time_image[i]);

		free(bomb);
	}
}

void bomb_display(t_bomb bomb, SDL_Surface *screen) {
	assert(bomb->time_image[bomb->current_time]);
	assert(screen);

	SDL_Rect place;
	place.x = bomb->x * SIZE_BLOC;
	place.y = bomb->y * SIZE_BLOC;

	SDL_BlitSurface(bomb->time_image[bomb->current_time], NULL, screen,&place);
}


