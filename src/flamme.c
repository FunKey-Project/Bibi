#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "SDL.h"
#include "SDL_image.h"
#include "../include/game.h"

struct s_flamme {
	enum e_type_flamme type; // Dans l'ordre HAUT BAS GAUCHE DROITE HRZLEFT HRZRIGHT VERTUP VERTDOWN CENTER
	int portee, x, y;
	SDL_Surface * type_img[4];
	int current_time; //contient 1 2 3 ou 4 selon l'etat d'avancement de la flamme
	t_flamme nxt;
};

static void flamme_load_img(t_flamme flamme,int place_tableau, const char *filename) {
	flamme->type_img[place_tableau] = IMG_Load(filename);
	if (!flamme->type_img[place_tableau])
	error("IMG_Load: %s\n", IMG_GetError());
}

t_flamme flamme_init(t_flamme list_flammes, int x, int y, int portee, enum e_type_flamme type) {
	t_flamme flamme = malloc(sizeof(*flamme));
	if (!flamme)
	error("Memory error");

	switch (type) {
	case HAUT :
		flamme_load_img(flamme,3 , IMG_FLAMME_THIN_HAUT);
		flamme_load_img(flamme,2 , IMG_FLAMME_MEDIUM_HAUT);
		flamme_load_img(flamme,1 , IMG_FLAMME_FIRE_HAUT);
		flamme_load_img(flamme,0 , IMG_FLAMME_LARGE_HAUT);
		break;
	case BAS :
		flamme_load_img(flamme,3 , IMG_FLAMME_THIN_BAS);
		flamme_load_img(flamme,2 , IMG_FLAMME_MEDIUM_BAS);
		flamme_load_img(flamme,1 , IMG_FLAMME_FIRE_BAS);
		flamme_load_img(flamme,0 , IMG_FLAMME_LARGE_BAS);
		break;
	case GAUCHE :
		flamme_load_img(flamme,3 , IMG_FLAMME_THIN_GAUCHE);
		flamme_load_img(flamme,2 , IMG_FLAMME_MEDIUM_GAUCHE);
		flamme_load_img(flamme,1 , IMG_FLAMME_FIRE_GAUCHE);
		flamme_load_img(flamme,0 , IMG_FLAMME_LARGE_GAUCHE);
		break;
	case DROITE :
		flamme_load_img(flamme,3 , IMG_FLAMME_THIN_DROITE);
		flamme_load_img(flamme,2 , IMG_FLAMME_MEDIUM_DROITE);
		flamme_load_img(flamme,1 , IMG_FLAMME_FIRE_DROITE);
		flamme_load_img(flamme,0 , IMG_FLAMME_LARGE_DROITE);
		break;
	case HRZLEFT :
		flamme_load_img(flamme,3, IMG_FLAMME_THIN_HRZ);
		flamme_load_img(flamme,2 , IMG_FLAMME_MEDIUM_HRZ);
		flamme_load_img(flamme,1 , IMG_FLAMME_FIRE_HRZ);
		flamme_load_img(flamme,0 , IMG_FLAMME_LARGE_HRZ);
		break;
	case HRZRIGHT :
		flamme_load_img(flamme,3 , IMG_FLAMME_THIN_HRZ);
		flamme_load_img(flamme,2 , IMG_FLAMME_MEDIUM_HRZ);
		flamme_load_img(flamme,1 , IMG_FLAMME_FIRE_HRZ);
		flamme_load_img(flamme,0 , IMG_FLAMME_LARGE_HRZ);
		break;
	case VERTUP :
		flamme_load_img(flamme,3 , IMG_FLAMME_THIN_VERT);
		flamme_load_img(flamme,2 , IMG_FLAMME_MEDIUM_VERT);
		flamme_load_img(flamme,1 , IMG_FLAMME_FIRE_VERT);
		flamme_load_img(flamme,0 , IMG_FLAMME_LARGE_VERT);
		break;
	case VERTDOWN :
		flamme_load_img(flamme,3 , IMG_FLAMME_THIN_VERT);
		flamme_load_img(flamme,2 , IMG_FLAMME_MEDIUM_VERT);
		flamme_load_img(flamme,1 , IMG_FLAMME_FIRE_VERT);
		flamme_load_img(flamme,0 , IMG_FLAMME_LARGE_VERT);
		break;
	case CENTER :
		flamme_load_img(flamme,3 , IMG_FLAMME_THIN_CENTER);
		flamme_load_img(flamme,2 , IMG_FLAMME_MEDIUM_CENTER);
		flamme_load_img(flamme,1 , IMG_FLAMME_FIRE_CENTER);
		flamme_load_img(flamme,0 , IMG_FLAMME_LARGE_CENTER);
		break;
	case M_BAS :
		flamme_load_img(flamme,3 , IMG_FLAMME_THIN_MONSTER_BAS);
		flamme_load_img(flamme,2 , IMG_FLAMME_MEDIUM_MONSTER_BAS);
		flamme_load_img(flamme,1 , IMG_FLAMME_FIRE_MONSTER_BAS);
		flamme_load_img(flamme,0 , IMG_FLAMME_LARGE_MONSTER_BAS);
		break;
		
	case M_HAUT :
		flamme_load_img(flamme,3 , IMG_FLAMME_THIN_MONSTER_HAUT);
		flamme_load_img(flamme,2 , IMG_FLAMME_MEDIUM_MONSTER_HAUT);
		flamme_load_img(flamme,1 , IMG_FLAMME_FIRE_MONSTER_HAUT);
		flamme_load_img(flamme,0 , IMG_FLAMME_LARGE_MONSTER_HAUT);
		break;
		
	case M_DROITE :
		flamme_load_img(flamme,3 , IMG_FLAMME_THIN_MONSTER_DROITE);
		flamme_load_img(flamme,2 , IMG_FLAMME_MEDIUM_MONSTER_DROITE);
		flamme_load_img(flamme,1 , IMG_FLAMME_FIRE_MONSTER_DROITE);
		flamme_load_img(flamme,0 , IMG_FLAMME_LARGE_MONSTER_DROITE);
		break;
		
	case M_GAUCHE :
		flamme_load_img(flamme,3 , IMG_FLAMME_THIN_MONSTER_GAUCHE);
		flamme_load_img(flamme,2 , IMG_FLAMME_MEDIUM_MONSTER_GAUCHE);
		flamme_load_img(flamme,1 , IMG_FLAMME_FIRE_MONSTER_GAUCHE);
		flamme_load_img(flamme,0 , IMG_FLAMME_LARGE_MONSTER_GAUCHE);
		break;

	}
	flamme->current_time = 3;
	flamme->type = type;
	flamme->x=x;
	flamme->y=y;
	flamme->portee=portee;
	flamme->nxt=NULL;
	if( list_flammes== NULL)
	{
		return flamme;
	}
	else
	{
		t_flamme temp=list_flammes;
		while(temp->nxt != NULL)
		{
			temp = temp->nxt;
		}
		temp->nxt = flamme;
		return list_flammes;
	}
}

t_flamme get_next_flamme(t_flamme flamme){
	return flamme->nxt;
}

void set_next_flamme(t_flamme flamme, t_flamme nxt_flamme){
	flamme->nxt=nxt_flamme;
}


int flamme_get_x(t_flamme flamme) {
	assert(flamme != NULL);
	return flamme->x;
}

int flamme_get_y(t_flamme flamme) {
	assert(flamme != NULL);
	return flamme->y;
}

int flamme_get_portee(t_flamme flamme) {
	assert(flamme != NULL);
	return flamme->portee;
}

enum e_type_flamme flamme_get_type(t_flamme flamme) {
	assert(flamme != NULL);
	return flamme->type;
}

void flamme_decrease_current_time(t_flamme flamme) {
	assert(flamme);
	if (flamme->current_time >= 0)
	flamme->current_time--;
}
int flamme_get_current_time(t_flamme flamme) {
	assert(flamme);
	return(flamme->current_time);
}

void flamme_free(t_flamme flamme) {
	if (flamme != NULL){
		assert(flamme);
		int i;
		for (i = 0; i < 36; i++)
		SDL_FreeSurface(flamme->type_img[i]);

		free(flamme);
	}
}

void flamme_display(t_flamme flamme, SDL_Surface *screen) {
	assert(screen);

	SDL_Rect place;
	place.x = flamme->x * SIZE_BLOC;
	place.y = flamme->y * SIZE_BLOC;

	SDL_BlitSurface(flamme->type_img[flamme->current_time], NULL, screen,&place);
}
