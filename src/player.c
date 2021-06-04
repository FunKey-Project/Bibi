#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "SDL.h"
#include "SDL_image.h"
#include "../include/game.h"

struct s_player {
	int x, y, portee_bomb, win;
	SDL_Surface * directed_img[4];
	enum e_way current_way;
	int lives;
	int nb_bomb;
	int nb_bomb_max;//cette variable est utile pour les bonus. Elle sert à ce que, par exemple, si le joueur a posé toutes ses bombes (disons 2) et qu'avant qu'elles explosent le joueur prend un bonus decrease_nb_bombs, il ne sera autorisé qu'à poser une seule bombe après l'explosion des deux sur la map.
	int dead;
};

static void player_load_img(t_player player, enum e_way way,
const char *filename) {
	player->directed_img[way] = IMG_Load(filename);
	if (!player->directed_img[way])
	error("IMG_Load: %s\n", IMG_GetError());
}

t_player player_init( int nb_bomb, int portee_bomb, int lives, int no_joueur) {
	t_player player = malloc(sizeof(*player));
	if (!player)
	error("Memory error");
	switch (no_joueur) {
	case 1:
		player_load_img(player, LEFT, IMG_PLAYER_LEFT);
		player_load_img(player, RIGHT, IMG_PLAYER_RIGHT);
		player_load_img(player, UP, IMG_PLAYER_UP);
		player_load_img(player, DOWN, IMG_PLAYER_DOWN);
		break;
	case 2:
		player_load_img(player, LEFT, IMG_PLAYER_2_LEFT);
		player_load_img(player, RIGHT, IMG_PLAYER_2_RIGHT);
		player_load_img(player, UP, IMG_PLAYER_2_UP);
		player_load_img(player, DOWN, IMG_PLAYER_2_DOWN);
		break;
	default: break;
	}

	player->current_way = DOWN;
	player->win= 0;
	player->portee_bomb=portee_bomb;
	player->nb_bomb_max=nb_bomb;
	player->nb_bomb=nb_bomb;
	player->lives=lives;
	player->dead=6;
	return player;
}

void player_free(t_player player) {
	assert(player);
	int i;
	for (i = 0; i < 4; i++)
	SDL_FreeSurface(player->directed_img[i]);

	free(player);
}

int player_get_x(t_player player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(t_player player) {
	assert(player != NULL);
	return player->y;
}

int player_win(t_player player){
	assert(player != NULL);
	return player->win;
}

int player_portee_bomb(t_player player){
	assert(player != NULL);
	return player->portee_bomb;
}

void player_set_current_way(t_player player, enum e_way way) {
	assert(player);
	player->current_way = way;
}

void player_increase_lives(t_player player) {
	assert(player);
	if (player->lives <MAXIMUM_LIVES)
	player->lives++;
}

void player_decrease_lives(t_player player) {
	assert(player);
	if (player->lives >0)
	player->lives--;
}

int player_get_lives(t_player player) {
	assert(player);
	return player->lives;
}

int player_get_nb_bomb(t_player player) {
	assert(player);
	return player->nb_bomb;
}

int player_get_nb_bomb_max(t_player player) {
	assert(player);
	return player->nb_bomb_max;
}

void player_set_nb_bomb(t_player player, int bomb_number) {
	assert(player);
	player->nb_bomb= bomb_number;
}

void player_increase_nb_bomb_max(t_player player) {
	assert(player);
	if (player->nb_bomb <BOMB_NUMBER_MAX)
	player->nb_bomb_max++;
}

void player_decrease_nb_bomb_max(t_player player) {
	assert(player);
	if (player->nb_bomb_max >1)
	player->nb_bomb_max--;
}

void player_increase_nb_bomb(t_player player) {
	assert(player);
	if (player->nb_bomb <BOMB_NUMBER_MAX)
	player->nb_bomb++;
}

void player_decrease_nb_bomb(t_player player) {
	assert(player);
	if (player->nb_bomb >0)
	player->nb_bomb--;
}


void player_increase_portee(t_player player) {
	assert(player);
	if (player->portee_bomb <BOMB_RANGE_MAX)
	player->portee_bomb++;
}

void player_decrease_portee(t_player player) {
	assert(player);
	if (player->portee_bomb >1)
	player->portee_bomb--;
}


int player_from_map(t_player player, t_map map) {
	int i, j;
	for (i = 0; i < map_get_height(map); i++) {
		for (j = 0; j < map_get_width(map); j++) {
			if (map_get_cell_type(map, j, i) == CELL_PLAYER) {
				player->x = j;
				player->y = i;
				return 0;
			}
		}
	}
	return 0;
}

void player2_from_map(t_player player, t_map map) {
	int i, j;
	for (i = 0; i < map_get_height(map); i++) {
		for (j = 0; j < map_get_width(map); j++) {
			if (map_get_cell_type(map, j, i) == CELL_PLAYER_2) {
				player->x = j;
				player->y = i;
			}
		}
	}
}

void player_die(t_player player){
	if (player->dead>0){
		switch (player->current_way){
		case UP:
			player->current_way=RIGHT;
			break;
		case RIGHT:
			player->current_way=DOWN;
			break;
		case DOWN:
			player->current_way=LEFT;
			break;
		case LEFT:
			player->current_way=UP;
			break;
		}
		player->dead--;
	}
}

int player_get_dead(t_player player) {
	assert(player);
	return player->dead;
}

static int player_move_aux(t_player player, t_map map, int x1, int y1, int x2, int y2) {
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

	case CELL_FLAMME:
		player_decrease_lives(player);
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



	case CELL_BONUS:
		switch ((type & 0xf0) >> 4) {
		case BONUS_BOMB_RANGE_INC:
			player_increase_portee(player);
			break;
		case BONUS_BOMB_RANGE_DEC:
			player_decrease_portee(player);
			break;
		case BONUS_BOMB_NB_INC:
			player_increase_nb_bomb_max(player);
			player_increase_nb_bomb(player);
			break;
		case BONUS_BOMB_NB_DEC:
			player_decrease_nb_bomb_max(player);
			if (player->nb_bomb>1)
			player_decrease_nb_bomb(player);
			break;
		case BONUS_LIFE_INC:
			if (player->lives<MAXIMUM_LIVES)
			player->lives++;
			break;

		default:
			break;
		}
		break;

	case CELL_GOAL:
		player->win=1;
		break;

	case CELL_MONSTER:
		player->lives--;
		break;

	case CELL_PLAYER:
		return 0;
		break;

	case CELL_PLAYER_2:
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

	// Player has moved
	return 1;
}

int player_move(t_player player, t_map map) {
	int x = player->x;
	int y = player->y;
	int move = 0;

	switch (player->current_way) {
	case UP:
		if (player_move_aux(player, map, x, y - 1, x, y - 2)) {
			player->y--;
			move = 1;
		}
		break;

	case DOWN:
		if (player_move_aux(player, map, x, y + 1, x, y + 2)) {
			player->y++;
			move = 1;
		}
		break;

	case LEFT:
		if (player_move_aux(player, map, x - 1, y, x - 2, y)) {
			player->x--;
			move = 1;
		}
		break;

	case RIGHT:
		if (player_move_aux(player, map, x + 1, y, x + 2, y)) {
			player->x++;
			move = 1;
		}
		break;
	}

	if (move) {
		if  ( map_get_cell_type(map,x,y)==CELL_PLAYER || map_get_cell_type(map,x,y)==CELL_PLAYER_2){
			map_set_cell_type(map, x, y, CELL_EMPTY);
		}
		map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
	}
	return move;
}

void player_display(t_player player, SDL_Surface *screen) {
	assert(player);
	assert(player->directed_img[player->current_way]);
	assert(screen);

	SDL_Rect place;
	place.x = player->x * SIZE_BLOC;
	place.y = player->y * SIZE_BLOC;

	SDL_BlitSurface(player->directed_img[player->current_way], NULL, screen,
	&place);
}

