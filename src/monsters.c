#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "SDL.h"
#include "SDL_image.h"

#include "../include/game.h"


struct s_monster {
	int x,y,portee_monstre;
	SDL_Surface * directed_img[4];
	enum e_way current_way;
	t_monster nxt;
};

static void monster_load_img(t_monster monster, enum e_way way,
const char *filename) {
	monster->directed_img[way] = IMG_Load(filename);
	if (!monster->directed_img[way])
	error("IMG_Load: %s\n", IMG_GetError());
}

t_monster monster_init(t_monster list_monsters, int x, int y, int portee_monstre) {
	t_monster monster = malloc(sizeof(*monster));
	if (!monster)
	error("Memory error");

	monster_load_img(monster, LEFT, IMG_MONSTER_LEFT);
	monster_load_img(monster, RIGHT, IMG_MONSTER_RIGHT);
	monster_load_img(monster, UP, IMG_MONSTER_UP);
	monster_load_img(monster, DOWN, IMG_MONSTER_DOWN);

	monster->current_way = DOWN;
	monster->x=x;
	monster->y=y;
	monster->portee_monstre=portee_monstre;
	monster->nxt=NULL;

	if( list_monsters== NULL)
	{
		return monster;
	}
	else
	{
		t_monster temp=list_monsters;
		while(temp->nxt != NULL)
		{
			temp = temp->nxt;
		}
		temp->nxt = monster;
		return list_monsters;
	}

}


void monster_free(t_monster monster) {
	if (monster != NULL){
		assert(monster);
		int i;
		for (i = 0; i < 4; i++)
		SDL_FreeSurface(monster->directed_img[i]);
		free(monster);
	}
}

t_monster get_next_monster(t_monster monster){
	return monster->nxt;
}

void set_next_monster(t_monster monster, t_monster nxt_monster){
	monster->nxt=nxt_monster;
}

int monster_get_x(t_monster monster) {
	assert(monster != NULL);
	return monster->x;
}

int monster_get_y(t_monster monster) {
	assert(monster != NULL);
	return monster->y;
}

int monster_portee(t_monster monster) {
	assert(monster != NULL);
	return monster->portee_monstre;
}

enum e_way monster_get_current_way(t_monster monster) {
	assert(monster);
	return(monster->current_way);
}

void monster_set_current_way(t_monster monster, enum e_way way) {
	assert(monster);
	monster->current_way = way;
}

void monster_increase_portee(t_monster monster) {
	assert(monster);
	if (monster->portee_monstre <BOMB_RANGE_MAX)
	monster->portee_monstre++;
}

void monster_decrease_portee(t_monster monster) {
	assert(monster);
	if (monster->portee_monstre >1)
	monster->portee_monstre--;
}

static int monster_move_aux(t_monster monster, t_player player, t_player player2, int nb_joueur, t_map map, int x1, int y1, int x2,
int y2) {
	t_cell_type type;
	if (!map_is_inside(map, x1, y1))
	return 0;

	type=map_get_cell_type(map, x1, y1);
	switch (type & 0x0f) {
	case CELL_WALL:
		return 0;
		break;


	case CELL_BOMB:
		return 0;
		break;

	case CELL_PLAYER:
		if ( nb_joueur==1){
			player_decrease_lives(player);
		}
		else{
			if (player_get_x(player)==x1 && player_get_y(player)==y1) {
				player_decrease_lives(player);
			}
			if (player_get_x(player2)==x1 && player_get_y(player2)==y1) {
				player_decrease_lives(player2);
			}
		}
		break;

	case CELL_FLAMME:
		return 0;;
		break;

	case CELL_MENU_EMPTY:
		return 0;
		break;

	case CELL_MENU_LIVES:
		return 0;
		break;

	case CELL_MENU_BOMBS:
		return 0;
		break;

	case CELL_MENU_RANGE:
		return 0;
		break;

	case CELL_BONUS:
		switch ((type & 0xf0) >> 4) {
		case BONUS_BOMB_RANGE_INC:
			monster_increase_portee(monster);
			break;
		case BONUS_BOMB_RANGE_DEC:
			monster_decrease_portee(monster);
			break;
		default:
			break;
		}
		break;

	case CELL_BOX:

		if (!map_is_inside(map, x2, y2))
		return 0;
		switch (map_get_cell_type(map,x2,y2)& 0x0f){
		case CELL_WALL:
			return 0;
			break;

		case CELL_BOX:
			return 0;
			break;

		case CELL_GOAL:
			return 0;
			break;

		case CELL_MONSTER:
			return 0;
			break;

		case CELL_PLAYER:
			return 0;
			break;

		case CELL_PLAYER_2:
			return 0;
			break;

		case CELL_BOMB:
			return 0;
			break;

		case CELL_MENU_EMPTY:
			return 0;
			break;

		case CELL_MENU_LIVES:
			return 0;
			break;

		case CELL_MENU_BOMBS:
			return 0;
			break;

		case CELL_MENU_RANGE:
			return 0;
			break;

		case CELL_MENU_PLAYER_1:
			return 0;
			break;

		case CELL_MENU_PLAYER_2:
			return 0;
			break;

		default:
			break;
		}

		map_set_cell_type(map, x1, y1, CELL_EMPTY);

		switch ((type & 0xf0) >> 4) {
		case BONUS_BOMB_RANGE_INC:
			map_set_cell_type(map, x2, y2, CELL_BOX | (BONUS_BOMB_RANGE_INC << 4));
			break;
		case BONUS_BOMB_RANGE_DEC:
			map_set_cell_type(map, x2, y2, CELL_BOX | (BONUS_BOMB_RANGE_DEC << 4));
			break;
		case BONUS_BOMB_NB_INC:
			map_set_cell_type(map, x2, y2, CELL_BOX | (BONUS_BOMB_NB_INC << 4));
			break;
		case BONUS_BOMB_NB_DEC:
			map_set_cell_type(map, x2, y2, CELL_BOX | (BONUS_BOMB_NB_DEC << 4));
			break;
		case BONUS_MONSTER:
			map_set_cell_type(map, x2, y2, CELL_BOX | (BONUS_MONSTER << 4));
			break;
		case BONUS_LIFE_INC:
			map_set_cell_type(map, x2, y2, CELL_BOX | (BONUS_LIFE_INC << 4));
			break;
		case NO_BONUS:
			map_set_cell_type(map, x2, y2, CELL_BOX | (NO_BONUS << 4));
			break;
		default:
			break;
		}
		return 1;

		break;


	case CELL_GOAL:
		return 0;
		break;

	case CELL_MONSTER:
		return 0;
		break;

	case CELL_MENU_PLAYER_1:
		return 0;
		break;

	case CELL_MENU_PLAYER_2:
		return 0;
		break;

	default:
		break;
	}

	// monster has moved
	return 1;
}



int monster_move(t_monster monster, t_map map, t_player player, t_player player2, int nb_joueur) {
	int x = monster->x;
	int y = monster->y;
	int move = 0;

	switch (monster->current_way) {
	case UP:
		if (monster_move_aux(monster, player, player2,nb_joueur, map, x, y - 1, x, y - 2)) {
			monster->y--;
			move = 1;
		}
		break;

	case DOWN:
		if (monster_move_aux(monster, player, player2, nb_joueur, map, x, y + 1, x, y + 2)) {
			monster->y++;
			move = 1;
		}
		break;

	case LEFT:
		if (monster_move_aux(monster, player, player2, nb_joueur, map, x - 1, y, x - 2, y)) {
			monster->x--;
			move = 1;
		}
		break;

	case RIGHT:
		if (monster_move_aux(monster, player, player2, nb_joueur, map, x + 1, y, x + 2, y)) {
			monster->x++;
			move = 1;
		}
		break;
	}

	if (move) {
		map_set_cell_type(map, x, y, CELL_EMPTY);
		map_set_cell_type(map, monster->x, monster->y, CELL_MONSTER);
	}
	return move;
}

void monster_display(t_monster monster, SDL_Surface *screen) {
	assert(monster);
	assert(monster->directed_img[monster->current_way]);
	assert(screen);

	SDL_Rect place;
	place.x = monster->x * SIZE_BLOC;
	place.y = monster->y * SIZE_BLOC;

	SDL_BlitSurface(monster->directed_img[monster->current_way], NULL, screen,
	&place);

}


