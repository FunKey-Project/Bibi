/*
* game.c
*
*  Created on: 15 mars 2010
*      Author: reveille
*/

#include <assert.h>
#include <time.h>

#include "../include/game.h"
#include "../include/game_time.h"
#include "../include/misc.h"
#include "../include/player.h"
#include "../include/bomb.h"
#include "../include/monsters.h"
#include "../include/flamme.h"

static struct s_game the_game;

#ifdef SOUND_FMOD_ACTIVATED
	static FSOUND_SAMPLE *bomb_explose;
#elif defined(SOUND_SDL_ACTIVATED)
	static Mix_Chunk *bomb_explose;
#endif //SOUND_FMOD_ACTIVATED

t_game game_new(int nb_joueur, int niveau, int mode, int kill_bomb) {
	game_time_init();

	FILE *level;
	const char *file_path;
	if (mode==1){
		if (nb_joueur==1){
			file_path = SRC_LEVELS_1_PLAYER;
		}
		else{
			file_path = SRC_LEVELS_2_PLAYERS;
		}
	}
	else{
		file_path = SRC_LEVELS_EDITOR;
	}

	level=fopen(file_path,"r");
	if(level==NULL){
		printf("ERROR: cannot open file: %s\n", file_path);
	}
	the_game.map = map_load_dynamic(level,niveau,nb_joueur);
	the_game.player1 = player_init(2,1,1,1);
	player_from_map(the_game.player1, the_game.map);
	the_game.player2 = NULL;
	if (nb_joueur==2){
		the_game.player2 = player_init(2,1,1,2);
		player2_from_map(the_game.player2, the_game.map);
	}
	the_game.kill_bomb=kill_bomb;
	the_game.list_bombs=NULL;
	the_game.nb_joueur=nb_joueur;
	the_game.list_flammes=NULL;
	the_game.list_monsters=NULL;
	the_game.viewport_mode=false;
	the_game.must_clear_screen=false;

#ifdef SOUND_FMOD_ACTIVATED
	bomb_explose = FSOUND_Sample_Load(FSOUND_FREE, "audio/bomb.wav", 0, 0, 0);
#elif defined(SOUND_SDL_ACTIVATED)
	bomb_explose = Mix_LoadWAV("audio/bomb.wav");
	//Mix_VolumeChunk(bomb_explose, MIX_MAX_VOLUME/2);
	if(bomb_explose != NULL) Mix_VolumeChunk(bomb_explose, 20);
#endif //SOUND_FMOD_ACTIVATED

	return &the_game;
}


void game_free(t_game game) {
	assert(game);
	t_bomb temp_bomb= game->list_bombs;
	t_monster temp_monster= game->list_monsters;
	t_flamme temp_flamme= game->list_flammes;
	player_free(game->player1);
	if (game->nb_joueur == 2)
	player_free(game->player2);
	map_free(game->map);
	while(temp_bomb != NULL)
	{
		t_bomb tmp=temp_bomb;
		temp_bomb = get_next_bomb(temp_bomb);
		bomb_free(tmp);
	}

	while(temp_monster != NULL)
	{
		t_monster tmp=temp_monster;
		temp_monster = get_next_monster(temp_monster);
		monster_free(tmp);
	}
	while(temp_flamme != NULL)
	{
		t_flamme tmp=temp_flamme;
		temp_flamme = get_next_flamme(temp_flamme);
		free(tmp);
	}

#ifdef SOUND_FMOD_ACTIVATED
	FSOUND_Sample_Free(bomb_explose);
#elif defined(SOUND_SDL_ACTIVATED)
	Mix_FreeChunk(bomb_explose);
#endif //SOUND_FMOD_ACTIVATED
}

t_player game_get_player1(t_game game) {
	assert(game);
	return game->player1;
}

t_player game_get_player2(t_game game) {
	assert(game);
	return game->player2;
}

void game_init_bomb(t_game game, int x, int y, int portee, int joueur) {
	assert(game);
	game->list_bombs=bomb_init(game->list_bombs, x, y, portee, joueur);
}


t_map game_the_map(t_game game) {
	assert(game);
	return game->map;
}

//
//void generate_bonus(t_game game, int x, int y){
//  t_map map = game_the_map(game);
//  int random1=rand() % 100;
//  int random2=rand() % 100;
//  int probacumulee_inf=0;
//  int probacumulee_sup=PROBA_BONUS_BOMB_RANGE * PROBA_BONUS_BOMB_RANGE_INC / 100;
//  if (random1 < PROBA_BONUS){
//    if (probacumulee_inf <= random2 && random2 <= probacumulee_sup)
//      map_set_cell_type(map, x, y, CELL_BONUS | (BONUS_BOMB_RANGE_INC << 4));
//
//    else{
//      probacumulee_inf=probacumulee_sup;
//      probacumulee_sup += PROBA_BONUS_BOMB_RANGE * PROBA_BONUS_BOMB_RANGE_DEC / 100;
//      if (probacumulee_inf <= random2 && random2 <= probacumulee_sup)
//		map_set_cell_type(map, x, y, CELL_BONUS | (BONUS_BOMB_RANGE_DEC << 4));
//      else{
//			probacumulee_inf=probacumulee_sup;
//			probacumulee_sup += PROBA_BONUS_BOMB_NB * PROBA_BONUS_BOMB_NB_INC / 100;
//			if (probacumulee_inf <= random2 && random2 <= probacumulee_sup)
//			map_set_cell_type(map, x, y, CELL_BONUS | (BONUS_BOMB_NB_INC << 4));
//			else{
//				probacumulee_inf=probacumulee_sup;
//				probacumulee_sup += PROBA_BONUS_BOMB_NB * PROBA_BONUS_BOMB_NB_DEC / 100;
//				if (probacumulee_inf <= random2 && random2 <= probacumulee_sup)
//				map_set_cell_type(map, x, y, CELL_BONUS | (BONUS_BOMB_NB_DEC << 4));
//				else {
//				    probacumulee_inf=probacumulee_sup;
//                    probacumulee_sup += PROBA_BONUS_LIFE_INC;
//                    if (probacumulee_inf <= random2 && random2 <= probacumulee_sup)
//                    map_set_cell_type(map, x, y, CELL_BONUS | (BONUS_LIFE_INC << 4));
//					else {
//					    map_set_cell_type(map,x,y,CELL_MONSTER);
//                        t_monster temp=game->list_monsters;
//                        game->list_monsters=monster_init(temp,x,y,1);
//					}
//				}
//			}
//        }
//    }
//  }
//  else map_set_cell_type(map,x,y,CELL_EMPTY);
//}

t_bonus_type return_bonus(){
	int random1=rand() % 100;
	int random2=rand() % 100;
	int probacumulee_inf=0;
	int probacumulee_sup=PROBA_BONUS_BOMB_RANGE * PROBA_BONUS_BOMB_RANGE_INC / 100;
	if (random1 < PROBA_BONUS){
		if (probacumulee_inf <= random2 && random2 <= probacumulee_sup)
		return BONUS_BOMB_RANGE_INC;

		else{
			probacumulee_inf=probacumulee_sup;
			probacumulee_sup += PROBA_BONUS_BOMB_RANGE * PROBA_BONUS_BOMB_RANGE_DEC / 100;
			if (probacumulee_inf <= random2 && random2 <= probacumulee_sup)
			return BONUS_BOMB_RANGE_DEC;
			else{
				probacumulee_inf=probacumulee_sup;
				probacumulee_sup += PROBA_BONUS_BOMB_NB * PROBA_BONUS_BOMB_NB_INC / 100;
				if (probacumulee_inf <= random2 && random2 <= probacumulee_sup)
				return BONUS_BOMB_NB_INC;
				else{
					probacumulee_inf=probacumulee_sup;
					probacumulee_sup += PROBA_BONUS_BOMB_NB * PROBA_BONUS_BOMB_NB_DEC / 100;
					if (probacumulee_inf <= random2 && random2 <= probacumulee_sup)
					return BONUS_BOMB_NB_DEC;
					else {
						probacumulee_inf=probacumulee_sup;
						probacumulee_sup += PROBA_BONUS_LIFE_INC;
						if (probacumulee_inf <= random2 && random2 <= probacumulee_sup)
						return BONUS_LIFE_INC;
						else {
							return BONUS_MONSTER;
						}
					}
				}
			}
		}
	}
	else{
		return NO_BONUS;
	}
}


//Bombs management
void bombs_management (t_game game, int all_bombs){
	t_bomb aux=game->list_bombs;
	t_player player1 = game->player1;
	t_player player2 = game->player2;
	if (aux!=NULL){
		t_bomb aux2=get_next_bomb(aux);
		while(aux2 != NULL){
			int current_time = bomb_get_current_time(aux2);
			int next_time = -- current_time;

			if (next_time>=0){
				if (all_bombs==1){
					bomb_set_current_time(aux2, next_time);
				}
				aux=get_next_bomb(aux);
				aux2=get_next_bomb(aux);
			}
			else{

#ifdef SOUND_FMOD_ACTIVATED
				FSOUND_PlaySound(FSOUND_FREE, bomb_explose);
#elif defined(SOUND_SDL_ACTIVATED)
				Mix_PlayChannel(-1,bomb_explose,0);
#endif //SOUND_FMOD_ACTIVATED

				int xb=bomb_get_x(aux2);
				int yb=bomb_get_y(aux2);
				int xp1=player_get_x(player1);
				int yp1=player_get_y(player1);
				int xp2;
				int yp2;
				int portee=bomb_get_portee(aux2);

				if ( xb==xp1 && yb==yp1){
					player_decrease_lives(player1);
				}
				if ( game->nb_joueur==2){
					xp2=player_get_x(player2);
					yp2=player_get_y(player2);
					if (xb==xp2 && yb==yp2){
						player_decrease_lives(player2);
					}
				}
				set_next_bomb(aux,(get_next_bomb(aux2)));
				int joueur=bomb_get_joueur(aux2);
				free(aux2);
				aux2=get_next_bomb(aux);
				map_set_cell_type(game->map, xb, yb, CELL_FLAMME);
				game->list_flammes = flamme_init(game->list_flammes, xb ,yb, portee, CENTER);
				t_flamme temp = game->list_flammes;
				while (get_next_flamme(temp)!=NULL){
					temp=get_next_flamme(temp);
				}
				flamme_continue(temp,game);
				int bm,b;
				switch (joueur) {
				case 1:
					bm= player_get_nb_bomb_max(player1);
					b =player_get_nb_bomb(player1);
					if (b <bm)
					player_increase_nb_bomb(player1);
					break;
				case 2:
					bm= player_get_nb_bomb_max(player2);
					b =player_get_nb_bomb(player2);
					if (b <bm)
					player_increase_nb_bomb(player2);
					break;
				}
			}
		}
		if (game->list_bombs!=NULL){
			aux=game->list_bombs;
			int current_time = bomb_get_current_time(aux);
			int next_time = -- current_time;
			int joueur=bomb_get_joueur(aux);
			if (next_time>=0){
				if (all_bombs==1){
					bomb_set_current_time(aux, next_time);
				}
			}
			else {

#ifdef SOUND_FMOD_ACTIVATED
				FSOUND_PlaySound(FSOUND_FREE, bomb_explose);
#elif defined(SOUND_SDL_ACTIVATED)
				Mix_PlayChannel(-1,bomb_explose,0);
#endif //SOUND_FMOD_ACTIVATED
				int xb=bomb_get_x(aux);
				int yb=bomb_get_y(aux);
				int xp1=player_get_x(player1);
				int yp1=player_get_y(player1);
				int xp2;
				int yp2;
				int portee=bomb_get_portee(aux);

				if ( xb==xp1 && yb==yp1){

					player_decrease_lives(player1);
				}
				if ( game->nb_joueur==2){
					xp2=player_get_x(player2);
					yp2=player_get_y(player2);
					if (xb==xp2 && yb==yp2){
						player_decrease_lives(player2);
					}
				}

				game->list_bombs=get_next_bomb(aux);
				free(aux);
				map_set_cell_type(game->map, xb, yb, CELL_FLAMME);
				game->list_flammes = flamme_init(game->list_flammes, xb ,yb, portee, CENTER);
				t_flamme temp = game->list_flammes;
				while (get_next_flamme(temp)!=NULL){
					temp=get_next_flamme(temp);
				}
				flamme_continue(temp,game);
				int bm,b;
				switch (joueur) {
				case 1:
					bm= player_get_nb_bomb_max(player1);
					b =player_get_nb_bomb(player1);
					if (b <bm)
					player_increase_nb_bomb(player1);
					break;
				case 2:
					bm= player_get_nb_bomb_max(player2);
					b =player_get_nb_bomb(player2);
					if (b <bm)
					player_increase_nb_bomb(player2);
					break;
				}
			}
		}

	}
}

void flamme_management(t_game game) {
	t_flamme aux=game->list_flammes;
	t_map map=game->map;
	if(aux != NULL){
		t_flamme aux2=get_next_flamme(aux);
		while(aux2 != NULL){

			if (flamme_get_current_time(aux2) > 0){
				flamme_decrease_current_time(aux2);
				aux=get_next_flamme(aux);
				aux2=get_next_flamme(aux);
			}

			else{
				if (map_get_cell_type(game->map, flamme_get_x(aux2), flamme_get_y(aux2))!=CELL_MONSTER){
					switch ((map_get_cell_type(game->map, flamme_get_x(aux2), flamme_get_y(aux2)) & 0xf0) >> 4) {
					case BONUS_BOMB_RANGE_INC:
						map_set_cell_type(map, flamme_get_x(aux2), flamme_get_y(aux2), CELL_BONUS | (BONUS_BOMB_RANGE_INC << 4));
						break;
					case BONUS_BOMB_RANGE_DEC:
						map_set_cell_type(map, flamme_get_x(aux2), flamme_get_y(aux2), CELL_BONUS | (BONUS_BOMB_RANGE_DEC << 4));
						break;
					case BONUS_BOMB_NB_INC:
						map_set_cell_type(map, flamme_get_x(aux2), flamme_get_y(aux2), CELL_BONUS | (BONUS_BOMB_NB_INC << 4));
						break;
					case BONUS_BOMB_NB_DEC:
						map_set_cell_type(map, flamme_get_x(aux2), flamme_get_y(aux2), CELL_BONUS | (BONUS_BOMB_NB_DEC << 4));
						break;
					case BONUS_MONSTER:
						map_set_cell_type(map,flamme_get_x(aux2), flamme_get_y(aux2), CELL_MONSTER);
						t_monster temp=game->list_monsters;
						game->list_monsters=monster_init(temp,flamme_get_x(aux2), flamme_get_y(aux2),1);
						break;
					case BONUS_LIFE_INC:
						map_set_cell_type(map, flamme_get_x(aux2), flamme_get_y(aux2), CELL_BONUS | (BONUS_LIFE_INC << 4));
						break;
					case NO_BONUS:
						map_set_cell_type(map, flamme_get_x(aux2), flamme_get_y(aux2), CELL_EMPTY);
						break;
					default:
						map_set_cell_type(map, flamme_get_x(aux2), flamme_get_y(aux2), CELL_EMPTY);
						break;
					}
				}

				set_next_flamme(aux,(get_next_flamme(aux2)));
				free(aux2);
				aux2=get_next_flamme(aux);
			}
		}
		aux=game->list_flammes;
		if (flamme_get_current_time(aux) > 0){
			flamme_decrease_current_time(aux);
		}
		else{
			if (map_get_cell_type(game->map, flamme_get_x(aux), flamme_get_y(aux))!=CELL_MONSTER){
				switch ((map_get_cell_type(game->map, flamme_get_x(aux), flamme_get_y(aux)) & 0xf0) >> 4) {
				case BONUS_BOMB_RANGE_INC:
					map_set_cell_type(map, flamme_get_x(aux), flamme_get_y(aux), CELL_BONUS | (BONUS_BOMB_RANGE_INC << 4));
					break;
				case BONUS_BOMB_RANGE_DEC:
					map_set_cell_type(map, flamme_get_x(aux), flamme_get_y(aux), CELL_BONUS | (BONUS_BOMB_RANGE_DEC << 4));
					break;
				case BONUS_BOMB_NB_INC:
					map_set_cell_type(map, flamme_get_x(aux), flamme_get_y(aux), CELL_BONUS | (BONUS_BOMB_NB_INC << 4));
					break;
				case BONUS_BOMB_NB_DEC:
					map_set_cell_type(map, flamme_get_x(aux), flamme_get_y(aux), CELL_BONUS | (BONUS_BOMB_NB_DEC << 4));
					break;
				case BONUS_MONSTER:
					map_set_cell_type(map,flamme_get_x(aux), flamme_get_y(aux), CELL_MONSTER);
					t_monster temp=game->list_monsters;
					game->list_monsters=monster_init(temp,flamme_get_x(aux), flamme_get_y(aux),1);
					break;
				case BONUS_LIFE_INC:
					map_set_cell_type(map, flamme_get_x(aux), flamme_get_y(aux), CELL_BONUS | (BONUS_LIFE_INC << 4));
					break;
				case NO_BONUS:
					map_set_cell_type(map, flamme_get_x(aux), flamme_get_y(aux), CELL_EMPTY);
					break;
				default:
					map_set_cell_type(map, flamme_get_x(aux), flamme_get_y(aux), CELL_EMPTY);
					break;
				}
			}
			game->list_flammes=get_next_flamme(aux);
			free(aux);
		}
	}
}

void flamme_continue(t_flamme flamme, t_game game) {

	int x1=flamme_get_x(flamme);
	int y1=flamme_get_y(flamme);
	int portee=flamme_get_portee(flamme);
	if (portee > 0){
		switch (flamme_get_type(flamme)){
		case CENTER:
			flamme_check_right(x1,y1,portee,game);
			flamme_check_left(x1,y1,portee,game);
			flamme_check_up(x1,y1,portee,game);
			flamme_check_down(x1,y1,portee,game);
			break;

		case HRZRIGHT:
			flamme_check_right(x1,y1,portee,game);
			break;

		case HRZLEFT:
			flamme_check_left(x1,y1,portee,game);
			break;

		case VERTUP:
			flamme_check_up(x1,y1,portee,game);
			break;

		case VERTDOWN:
			flamme_check_down(x1,y1,portee,game);
			break;

		case M_BAS:
			flamme_check_down(x1,y1,portee,game);
			break;

		case M_HAUT:
			flamme_check_up(x1,y1,portee,game);
			break;

		case M_GAUCHE:
			flamme_check_left(x1,y1,portee,game);
			break;

		case M_DROITE:
			flamme_check_right(x1,y1,portee,game);
			break;

		default:
			break;
		}
	}
}


void flamme_check_right(int x, int y, int portee, t_game game){
	t_map map=game->map;
	int xp1=player_get_x(game->player1);
	int yp1=player_get_y(game->player1);
	int xp2=-1;
	int yp2=-1;
	if ( game->nb_joueur==2){
		xp2=player_get_x(game->player2);
		yp2=player_get_y(game->player2);
	}
	if (map_is_inside(map, x + 1, y)){
		switch ((map_get_cell_type(map, x + 1, y))  & 0x0f ) {
		case CELL_BOX:
			switch ((map_get_cell_type(map, x + 1, y) & 0xf0) >> 4) {
			case BONUS_BOMB_RANGE_INC:
				map_set_cell_type(map, x + 1, y, CELL_FLAMME | (BONUS_BOMB_RANGE_INC << 4));
				break;
			case BONUS_BOMB_RANGE_DEC:
				map_set_cell_type(map, x + 1, y, CELL_FLAMME | (BONUS_BOMB_RANGE_DEC << 4));
				break;
			case BONUS_BOMB_NB_INC:
				map_set_cell_type(map, x + 1, y, CELL_FLAMME | (BONUS_BOMB_NB_INC << 4));
				break;
			case BONUS_BOMB_NB_DEC:
				map_set_cell_type(map, x + 1, y, CELL_FLAMME | (BONUS_BOMB_NB_DEC << 4));
				break;
			case BONUS_MONSTER:
				map_set_cell_type(map, x + 1, y, CELL_FLAMME | (BONUS_MONSTER << 4));
				break;
			case BONUS_LIFE_INC:
				map_set_cell_type(map, x + 1, y, CELL_FLAMME | (BONUS_LIFE_INC << 4));
				break;
			case NO_BONUS:
				map_set_cell_type(map, x + 1, y, CELL_FLAMME | (NO_BONUS<< 4));
				break;
			default:
				break;
			}
			game->list_flammes=flamme_init(game->list_flammes, x + 1, y, portee - 1, DROITE);
			break;

		case CELL_EMPTY:
			if (portee == 1){
				game->list_flammes= flamme_init(game->list_flammes, x + 1, y, portee - 1, DROITE);
				map_set_cell_type(map, x + 1, y, CELL_FLAMME);
			}
			else{
				map_set_cell_type(map, x + 1, y, CELL_FLAMME);
				game->list_flammes = flamme_init(game->list_flammes, x + 1, y, portee - 1, HRZRIGHT);
				t_flamme temp = game->list_flammes;
				while (get_next_flamme(temp)!=NULL){
					temp=get_next_flamme(temp);}
				flamme_continue(temp,game);
			}
			break;

		case CELL_FLAMME:
			if (portee == 1){
				game->list_flammes= flamme_init(game->list_flammes, x + 1, y, portee - 1, DROITE);
				map_set_cell_type(map, x + 1, y, CELL_FLAMME);
			}
			else{
				map_set_cell_type(map, x + 1, y, CELL_FLAMME);
				game->list_flammes = flamme_init(game->list_flammes, x + 1, y, portee - 1, HRZRIGHT);
				t_flamme temp = game->list_flammes;
				while (get_next_flamme(temp)!=NULL){
					temp=get_next_flamme(temp);}
				flamme_continue(temp,game);
			}
			break;


		case CELL_BONUS:
			game->list_flammes= flamme_init(game->list_flammes, x + 1, y, portee - 1, DROITE);
			map_set_cell_type(map, x + 1, y, CELL_FLAMME);
			break;


		case CELL_MONSTER:
			game->list_monsters=kill_monster(game->list_monsters,x+1,y);
			game->list_flammes= flamme_init(game->list_flammes, x + 1, y, portee - 1, DROITE);
			map_set_cell_type(map, x + 1, y, CELL_FLAMME);
			break;

		case CELL_PLAYER:
			game->list_flammes= flamme_init(game->list_flammes, x + 1, y, portee - 1, DROITE);
			if ( game->nb_joueur==1){
				player_decrease_lives(game->player1);
			}
			else{
				if (xp1==x+1 && yp1==y) {
					player_decrease_lives(game->player1);
				}
				if (xp2==x+1 && yp2==y) {
					player_decrease_lives(game->player2);
				}
			}
			break;

		case CELL_PLAYER_2:
			player_decrease_lives(game->player2);
			break;

		case CELL_BOMB:
			if (game->kill_bomb==0){
				if(player_get_x(game->player1)==x+1 && player_get_y(game->player1)==y)
				player_decrease_lives(game->player1);

				if(game->nb_joueur==2){
					if(player_get_x(game->player2)==x+1 && player_get_y(game->player2)==y)
					player_decrease_lives(game->player2);
				}
			}
			else
			kill_bomb(game, x+1, y);

			break;

		default:
			break;
		}
	}
}


void flamme_check_left(int x, int y, int portee, t_game game){
	t_map map=game->map;
	int xp1=player_get_x(game->player1);
	int yp1=player_get_y(game->player1);
	int xp2=-1;
	int yp2=-1;
	if ( game->nb_joueur==2){
		xp2=player_get_x(game->player2);
		yp2=player_get_y(game->player2);
	}
	if (map_is_inside(map, x - 1, y)){
		switch ((map_get_cell_type(map, x - 1, y))  & 0x0f) { //Gere le cas vers la droite
			//             peut etre erreur avec le map is inside
		case CELL_BOX:
			switch ((map_get_cell_type(map, x - 1, y) & 0xf0) >> 4) {
			case BONUS_BOMB_RANGE_INC:
				map_set_cell_type(map, x - 1, y, CELL_FLAMME | (BONUS_BOMB_RANGE_INC << 4));
				break;
			case BONUS_BOMB_RANGE_DEC:
				map_set_cell_type(map, x - 1, y, CELL_FLAMME | (BONUS_BOMB_RANGE_DEC << 4));
				break;
			case BONUS_BOMB_NB_INC:
				map_set_cell_type(map, x - 1, y, CELL_FLAMME | (BONUS_BOMB_NB_INC << 4));
				break;
			case BONUS_BOMB_NB_DEC:
				map_set_cell_type(map, x - 1, y, CELL_FLAMME | (BONUS_BOMB_NB_DEC << 4));
				break;
			case BONUS_MONSTER:
				map_set_cell_type(map, x - 1, y, CELL_FLAMME | (BONUS_MONSTER << 4));
				break;
			case BONUS_LIFE_INC:
				map_set_cell_type(map, x - 1, y, CELL_FLAMME | (BONUS_LIFE_INC << 4));
				break;
			case NO_BONUS:
				map_set_cell_type(map, x - 1, y, CELL_FLAMME | (NO_BONUS<< 4));
				break;
			default:
				break;
			}
			game->list_flammes=flamme_init(game->list_flammes, x - 1, y, portee - 1, GAUCHE);
			break;

		case CELL_EMPTY:
			if (portee == 1){
				game->list_flammes= flamme_init(game->list_flammes, x - 1, y, portee - 1, GAUCHE);
				map_set_cell_type(map, x - 1, y, CELL_FLAMME);
			}
			else {
				map_set_cell_type(map, x - 1, y, CELL_FLAMME);
				game->list_flammes = flamme_init(game->list_flammes, x - 1, y, portee - 1, HRZLEFT);
				t_flamme temp = game->list_flammes;
				while (get_next_flamme(temp)!=NULL){
					temp=get_next_flamme(temp);}
				flamme_continue(temp,game);
			}
			break;


		case CELL_FLAMME:
			if (portee == 1){
				game->list_flammes= flamme_init(game->list_flammes, x - 1, y, portee - 1, GAUCHE);
				map_set_cell_type(map, x - 1, y, CELL_FLAMME);
			}
			else {
				map_set_cell_type(map, x - 1, y, CELL_FLAMME);
				game->list_flammes = flamme_init(game->list_flammes, x - 1, y, portee - 1, HRZLEFT);
				t_flamme temp = game->list_flammes;
				while (get_next_flamme(temp)!=NULL){
					temp=get_next_flamme(temp);}
				flamme_continue(temp,game);
			}
			break;


		case CELL_BONUS:
			game->list_flammes= flamme_init(game->list_flammes, x - 1, y, portee - 1, GAUCHE);
			map_set_cell_type(map, x - 1, y, CELL_FLAMME);
			break;


		case CELL_MONSTER:
			game->list_monsters=kill_monster(game->list_monsters,x-1,y);
			game->list_flammes= flamme_init(game->list_flammes, x - 1, y, portee - 1, GAUCHE);
			map_set_cell_type(map, x - 1, y, CELL_FLAMME);
			break;

		case CELL_PLAYER:
			game->list_flammes= flamme_init(game->list_flammes, x - 1, y, portee - 1, GAUCHE);
			if ( game->nb_joueur==1){
				player_decrease_lives(game->player1);
			}
			else{
				if (xp1==x-1 && yp1==y) {
					player_decrease_lives(game->player1);
				}
				if (xp2==x-1 && yp2==y) {
					player_decrease_lives(game->player2);
				}
			}
			break;

		case CELL_PLAYER_2:
			player_decrease_lives(game->player2);
			break;

		case CELL_BOMB:
			if (game->kill_bomb==0){
				if(player_get_x(game->player1)==x-1 && player_get_y(game->player1)==y)
				player_decrease_lives(game->player1);

				if(game->nb_joueur==2){
					if(player_get_x(game->player2)==x-1 && player_get_y(game->player2)==y)
					player_decrease_lives(game->player2);
				}
			}
			else
			kill_bomb(game, x-1, y);
			break;

		default:
			break;
		}
	}
}

void flamme_check_up(int x, int y, int portee, t_game game){
	t_map map=game->map;
	int xp1=player_get_x(game->player1);
	int yp1=player_get_y(game->player1);
	int xp2=-1;
	int yp2=-1;
	if ( game->nb_joueur==2){
		xp2=player_get_x(game->player2);
		yp2=player_get_y(game->player2);
	}
	if (map_is_inside(map, x, y - 1)){
		switch ((map_get_cell_type(map, x, y - 1)) & 0x0f) {
		case CELL_BOX:
			switch ((map_get_cell_type(map, x, y-1) & 0xf0) >> 4) {
			case BONUS_BOMB_RANGE_INC:
				map_set_cell_type(map, x, y-1, CELL_FLAMME | (BONUS_BOMB_RANGE_INC << 4));
				break;
			case BONUS_BOMB_RANGE_DEC:
				map_set_cell_type(map, x, y-1, CELL_FLAMME | (BONUS_BOMB_RANGE_DEC << 4));
				break;
			case BONUS_BOMB_NB_INC:
				map_set_cell_type(map, x, y-1, CELL_FLAMME | (BONUS_BOMB_NB_INC << 4));
				break;
			case BONUS_BOMB_NB_DEC:
				map_set_cell_type(map, x, y-1, CELL_FLAMME | (BONUS_BOMB_NB_DEC << 4));
				break;
			case BONUS_MONSTER:
				map_set_cell_type(map, x, y-1, CELL_FLAMME | (BONUS_MONSTER << 4));
				break;
			case BONUS_LIFE_INC:
				map_set_cell_type(map, x, y-1, CELL_FLAMME | (BONUS_LIFE_INC << 4));
				break;
			case NO_BONUS:
				map_set_cell_type(map, x, y-1, CELL_FLAMME | (NO_BONUS<< 4));
				break;
			default:
				break;
			}
			game->list_flammes=flamme_init(game->list_flammes, x, y - 1, portee - 1, HAUT);
			break;

		case CELL_EMPTY:
			if (portee == 1){
				map_set_cell_type(map, x, y - 1, CELL_FLAMME);
				game->list_flammes= flamme_init(game->list_flammes, x, y - 1, portee - 1, HAUT);
			}
			else{
				map_set_cell_type(map, x, y - 1, CELL_FLAMME);
				game->list_flammes = flamme_init(game->list_flammes, x, y - 1, portee - 1, VERTUP);
				t_flamme temp = game->list_flammes;
				while (get_next_flamme(temp)!=NULL){
					temp=get_next_flamme(temp);}
				flamme_continue(temp,game);
			}
			break;

		case CELL_FLAMME:
			if (portee == 1){
				map_set_cell_type(map, x, y - 1, CELL_FLAMME);
				game->list_flammes= flamme_init(game->list_flammes, x, y - 1, portee - 1, HAUT);
			}
			else{
				map_set_cell_type(map, x, y - 1, CELL_FLAMME);
				game->list_flammes = flamme_init(game->list_flammes, x, y - 1, portee - 1, VERTUP);
				t_flamme temp = game->list_flammes;
				while (get_next_flamme(temp)!=NULL){
					temp=get_next_flamme(temp);}
				flamme_continue(temp,game);
			}
			break;


		case CELL_BONUS:
			map_set_cell_type(map, x, y - 1, CELL_FLAMME);
			game->list_flammes= flamme_init(game->list_flammes, x, y - 1, portee - 1, HAUT);
			break;


		case CELL_MONSTER:
			game->list_monsters=kill_monster(game->list_monsters,x,y-1);
			map_set_cell_type(map, x, y - 1, CELL_FLAMME);
			game->list_flammes= flamme_init(game->list_flammes, x, y - 1, portee - 1, HAUT);
			break;

		case CELL_PLAYER:
			game->list_flammes= flamme_init(game->list_flammes, x, y - 1, portee - 1, HAUT);
			if ( game->nb_joueur==1){
				player_decrease_lives(game->player1);
			}
			else{
				if (xp1==x && yp1==y-1) {
					player_decrease_lives(game->player1);
				}
				if (xp2==x && yp2==y-1) {
					player_decrease_lives(game->player2);
				}
			}
			break;

		case CELL_PLAYER_2:
			player_decrease_lives(game->player2);
			break;

		case CELL_BOMB:
			if (game->kill_bomb==0){
				if(player_get_x(game->player1)==x && player_get_y(game->player1)==y-1)
				player_decrease_lives(game->player1);

				if(game->nb_joueur==2){
					if(player_get_x(game->player2)==x && player_get_y(game->player2)==y-1)
					player_decrease_lives(game->player2);
				}
			}
			else
			kill_bomb(game, x, y-1);

			break;

		default:
			break;
		}
	}
}

void flamme_check_down (int x, int y, int portee, t_game game){
	t_map map=game->map;
	int xp1=player_get_x(game->player1);
	int yp1=player_get_y(game->player1);
	int xp2=-1;
	int yp2=-1;
	if ( game->nb_joueur==2){
		xp2=player_get_x(game->player2);
		yp2=player_get_y(game->player2);
	}
	if (map_is_inside(map, x, y + 1)){
		switch ((map_get_cell_type(map, x, y + 1)) & 0x0f) {
		case CELL_BOX:
			switch ((map_get_cell_type(map, x, y+1) & 0xf0) >> 4) {
			case BONUS_BOMB_RANGE_INC:
				map_set_cell_type(map, x, y+1, CELL_FLAMME | (BONUS_BOMB_RANGE_INC << 4));
				break;
			case BONUS_BOMB_RANGE_DEC:
				map_set_cell_type(map, x, y+1, CELL_FLAMME | (BONUS_BOMB_RANGE_DEC << 4));
				break;
			case BONUS_BOMB_NB_INC:
				map_set_cell_type(map, x, y+1, CELL_FLAMME | (BONUS_BOMB_NB_INC << 4));
				break;
			case BONUS_BOMB_NB_DEC:
				map_set_cell_type(map, x, y+1, CELL_FLAMME | (BONUS_BOMB_NB_DEC << 4));
				break;
			case BONUS_MONSTER:
				map_set_cell_type(map, x, y+1, CELL_FLAMME | (BONUS_MONSTER << 4));
				break;
			case BONUS_LIFE_INC:
				map_set_cell_type(map, x, y+1, CELL_FLAMME | (BONUS_LIFE_INC << 4));
				break;
			case NO_BONUS:
				map_set_cell_type(map, x, y+1, CELL_FLAMME | (NO_BONUS<< 4));
				break;
			default:
				break;
			}
			game->list_flammes=flamme_init(game->list_flammes, x, y + 1, portee - 1, BAS);
			break;

		case CELL_EMPTY:
			if (portee == 1){
				map_set_cell_type(map, x, y + 1, CELL_FLAMME);
				game->list_flammes= flamme_init(game->list_flammes, x, y + 1, portee - 1, BAS);
			}
			else{
				map_set_cell_type(map, x, y + 1, CELL_FLAMME);
				game->list_flammes = flamme_init(game->list_flammes, x, y + 1, portee - 1, VERTDOWN);
				t_flamme temp = game->list_flammes;
				while (get_next_flamme(temp)!=NULL){
					temp=get_next_flamme(temp);}
				flamme_continue(temp,game);
			}
			break;

		case CELL_FLAMME:
			if (portee == 1){
				map_set_cell_type(map, x, y + 1, CELL_FLAMME);
				game->list_flammes= flamme_init(game->list_flammes, x, y + 1, portee - 1, BAS);
			}
			else{
				map_set_cell_type(map, x, y + 1, CELL_FLAMME);
				game->list_flammes = flamme_init(game->list_flammes, x, y + 1, portee - 1, VERTDOWN);
				t_flamme temp = game->list_flammes;
				while (get_next_flamme(temp)!=NULL){
					temp=get_next_flamme(temp);}
				flamme_continue(temp,game);
			}
			break;


		case CELL_BONUS:
			map_set_cell_type(map, x, y + 1, CELL_FLAMME);
			game->list_flammes=flamme_init(game->list_flammes, x, y + 1, portee - 1, BAS);
			break;


		case CELL_MONSTER:
			game->list_monsters=kill_monster(game->list_monsters,x,y+1);
			map_set_cell_type(map, x, y + 1, CELL_FLAMME);
			game->list_flammes=flamme_init(game->list_flammes, x, y + 1, portee - 1, BAS);

			break;

		case CELL_PLAYER:
			game->list_flammes=flamme_init(game->list_flammes, x, y + 1, portee - 1, BAS);
			if ( game->nb_joueur==1){
				player_decrease_lives(game->player1);
			}
			else{
				if (xp1==x && yp1==y+1) {
					player_decrease_lives(game->player1);
				}
				if (xp2==x && yp2==y+1) {
					player_decrease_lives(game->player2);
				}
			}
			break;

		case CELL_PLAYER_2:
			player_decrease_lives(game->player2);
			break;

		case CELL_BOMB:
			if (game->kill_bomb==0){
				if(player_get_x(game->player1)==x && player_get_y(game->player1)==y+1)
				player_decrease_lives(game->player1);

				if(game->nb_joueur==2){
					if(player_get_x(game->player2)==x && player_get_y(game->player2)==y+1)
					player_decrease_lives(game->player2);
				}
			}
			else
			kill_bomb(game, x, y+1);

			break;

		default:
			break;
		}
	}
}

void monsters_management(t_game game, int nb_joueur) {
	t_monster temp=game->list_monsters;
	t_map map = game_the_map(game);
	if (game->list_monsters!=NULL){
		while(temp != NULL){
			int random1=rand() % 5;
			int x= monster_get_x(temp);
			int y= monster_get_y(temp);
			int portee=monster_portee(temp);
			switch (random1) {
			case 0:
				monster_set_current_way(temp, UP);
				monster_move(temp,map,game->player1,game->player2, nb_joueur);
				break;
			case 1:
				monster_set_current_way(temp, RIGHT);
				monster_move(temp,map,game->player1,game->player2, nb_joueur);
				break;
			case 2:
				monster_set_current_way(temp, DOWN);
				monster_move(temp,map,game->player1,game->player2, nb_joueur);
				break;
			case 3:
				monster_set_current_way(temp, LEFT);
				monster_move(temp,map,game->player1,game->player2, nb_joueur);
				break;
			case 4:
				switch (monster_get_current_way(temp)) {
				case UP:
					game->list_flammes = flamme_init(game->list_flammes, x, y,portee, M_HAUT);
					t_flamme aux = game->list_flammes;
					while (get_next_flamme(aux)!=NULL){
						aux=get_next_flamme(aux);}
					flamme_continue(aux,game);
					map_set_cell_type(map, x, y, CELL_MONSTER);
					break;
				case DOWN:
					game->list_flammes = flamme_init(game->list_flammes, x, y,portee, M_BAS);
					t_flamme aux2 = game->list_flammes;
					while (get_next_flamme(aux2)!=NULL){
						aux2=get_next_flamme(aux2);}
					flamme_continue(aux2,game);
					map_set_cell_type(map, x, y, CELL_MONSTER);
					break;
				case RIGHT:
					game->list_flammes = flamme_init(game->list_flammes, x, y,portee, M_DROITE);
					t_flamme aux3 = game->list_flammes;
					while (get_next_flamme(aux3)!=NULL){
						aux3=get_next_flamme(aux3);}
					flamme_continue(aux3,game);
					map_set_cell_type(map, x, y, CELL_MONSTER);
					break;
				case LEFT:
					game->list_flammes = flamme_init(game->list_flammes, x, y,portee, M_GAUCHE);
					t_flamme aux4 = game->list_flammes;
					while (get_next_flamme(aux4)!=NULL){
						aux4=get_next_flamme(aux4);}
					flamme_continue(aux4,game);
					map_set_cell_type(map, x, y, CELL_MONSTER);
					break;
				}
			}
			temp=get_next_monster(temp);
		}
	}
}


t_monster kill_monster(t_monster list_monsters, int x, int y) {
	if(list_monsters == NULL)
	return NULL;

	else{

		if ( monster_get_x(list_monsters) == x && monster_get_y(list_monsters) == y){
			t_monster tmp = get_next_monster(list_monsters);
			free(list_monsters);
			return tmp;
		}
		else
		{
			t_monster temp=list_monsters;
			while ( (monster_get_x(get_next_monster(temp)) != x || monster_get_y(get_next_monster(temp)) != y) && get_next_monster(temp)!=NULL){
				temp = get_next_monster(temp);
			}
			if(get_next_monster(temp)!=NULL){
				t_monster tmp=get_next_monster(get_next_monster(temp));
				free(get_next_monster(temp));
				set_next_monster(temp,tmp);
			}
			return list_monsters;
		}
	}
}


void kill_bomb(t_game game, int x, int y) {

	t_bomb list_bombs=game->list_bombs;
	if(list_bombs != NULL){
		t_player player1 = game->player1;
		t_player player2 = game->player2;
		int xp1=player_get_x(player1);
		int yp1=player_get_y(player1);
		int xp2;
		int yp2;
		if ( x==xp1 && y==yp1){
			player_decrease_lives(player1);
		}
		if ( game->nb_joueur==2){
			xp2=player_get_x(player2);
			yp2=player_get_y(player2);
			if (x==xp2 && y==yp2){
				player_decrease_lives(player2);
			}
		}
		if ( bomb_get_x(list_bombs) == x && bomb_get_y(list_bombs) == y){
			bomb_set_current_time(list_bombs, 0);
//			bombs_management(game,0);
		}
		else
		{
			t_bomb temp=list_bombs;
			while ( bomb_get_x(get_next_bomb(temp)) != x || bomb_get_y(get_next_bomb(temp)) != y){
				temp = get_next_bomb(temp);
			}
			temp = get_next_bomb(temp);
			bomb_set_current_time(temp, 0);
//			bombs_management(game,0);
		}
	}
}


void game_display(t_game game, SDL_Surface *screen) {
	assert(game);
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 
		GAME_BG_COLOR_R, GAME_BG_COLOR_G, GAME_BG_COLOR_B));
	t_bomb temp_bomb= game->list_bombs;
	t_monster temp_monster= game->list_monsters;
	t_flamme temp_flamme= game->list_flammes;
	int lives_player1 = player_get_lives(game->player1);
	int bombs_player1 = player_get_nb_bomb(game->player1);
	int range_player1 = player_portee_bomb(game->player1);
	int lives_player2 = 0;
	int bombs_player2 = 0;
	int range_player2 = 0;

	static uint16_t viewport_x = 0;
	static uint16_t viewport_y = 0;

	if (game->nb_joueur == 2){
		lives_player2 = player_get_lives(game->player2);
		bombs_player2 = player_get_nb_bomb(game->player2);
		range_player2 = player_portee_bomb(game->player2);
	}
	map_display(game->map, lives_player1, bombs_player1, range_player1, lives_player2, bombs_player2, range_player2, screen);

	while(temp_bomb != NULL)
	{
		t_bomb tmp=temp_bomb;
		temp_bomb = get_next_bomb(temp_bomb);
		bomb_display(tmp,screen);
	}

	while(temp_monster != NULL)
	{
		t_monster tmp=temp_monster;
		temp_monster = get_next_monster(temp_monster);
		monster_display(tmp,screen);
	}

	player_display(game->player1, screen);

	if (game->nb_joueur == 2)
	player_display(game->player2, screen);


	while(temp_flamme != NULL)
	{
		t_flamme tmp=temp_flamme;
		temp_flamme = get_next_flamme(temp_flamme);
		flamme_display(tmp,screen);
	}

	/** Flip screen */
#ifdef HW_SCREEN_RESIZE
		if(game->must_clear_screen){
			SDL_FillRect(hw_screen, NULL, 0x000000);
			game->must_clear_screen = false;
		}

		if(!game->viewport_mode){
			
			/******* Scaled Zoom */
			flip_NNOptimized_AllowOutOfScreen(screen, hw_screen,
		        HW_SCREEN_WIDTH,
		        MIN(screen->h*HW_SCREEN_WIDTH/screen->w, HW_SCREEN_HEIGHT));
		}
		else{

			/****** Viewport view */
#define MIN_BLOCKS_VISIBLE_IN_VIEWPORT_MODE 1
			/* Adapt on the right */
			if(SIZE_BLOC*player_get_x(game->player1) >= viewport_x+HW_SCREEN_WIDTH - MIN_BLOCKS_VISIBLE_IN_VIEWPORT_MODE*SIZE_BLOC){
				//printf("%d, %d\n", SIZE_BLOC*player_get_x(game->player1), viewport_x+HW_SCREEN_WIDTH);
				viewport_x = MIN(SIZE_BLOC*(player_get_x(game->player1)+1+MIN_BLOCKS_VISIBLE_IN_VIEWPORT_MODE) - HW_SCREEN_WIDTH, 
					SIZE_BLOC*map_get_width(game->map)-HW_SCREEN_WIDTH);
				//printf("viewport_x = %d\n", viewport_x);
			}
			/* Adapt on the left */
			if(SIZE_BLOC*player_get_x(game->player1) < viewport_x+MIN_BLOCKS_VISIBLE_IN_VIEWPORT_MODE*SIZE_BLOC){
				viewport_x = MIN(SIZE_BLOC*(MAX(player_get_x(game->player1), MIN_BLOCKS_VISIBLE_IN_VIEWPORT_MODE)-MIN_BLOCKS_VISIBLE_IN_VIEWPORT_MODE), 
					SIZE_BLOC*map_get_width(game->map)-HW_SCREEN_WIDTH);			
			}
			/* Adapt on the bottom */
			if(SIZE_BLOC*player_get_y(game->player1) >= viewport_y+HW_SCREEN_HEIGHT - MIN_BLOCKS_VISIBLE_IN_VIEWPORT_MODE*SIZE_BLOC){
				viewport_y = MIN(SIZE_BLOC*(player_get_y(game->player1)+1+MIN_BLOCKS_VISIBLE_IN_VIEWPORT_MODE)- HW_SCREEN_HEIGHT, 
					SIZE_BLOC*map_get_height(game->map)-HW_SCREEN_HEIGHT);
			}
			/* Adapt on the top */
			if(SIZE_BLOC*player_get_y(game->player1) < viewport_y+MIN_BLOCKS_VISIBLE_IN_VIEWPORT_MODE*SIZE_BLOC){
				viewport_y = MIN(SIZE_BLOC*(MAX(player_get_y(game->player1), MIN_BLOCKS_VISIBLE_IN_VIEWPORT_MODE)-MIN_BLOCKS_VISIBLE_IN_VIEWPORT_MODE), 
					SIZE_BLOC*map_get_height(game->map)-HW_SCREEN_HEIGHT);
			}

			/*uint16_t viewport_x = MIN(SIZE_BLOC*player_get_x(game->player1), SIZE_BLOC*map_get_width(game->map)-HW_SCREEN_WIDTH);
			uint16_t viewport_y = MIN(SIZE_BLOC*player_get_y(game->player1), SIZE_BLOC*map_get_height(game->map)-HW_SCREEN_HEIGHT);*/
			SDL_Rect viewport_rect = {viewport_x, viewport_y, HW_SCREEN_WIDTH, HW_SCREEN_HEIGHT};
			SDL_BlitSurface(screen, &viewport_rect, hw_screen, NULL);
		}

		/** Flip screen */
		SDL_Flip(hw_screen);
#else //HW_SCREEN_RESIZE
		SDL_Flip(screen);
#endif //HW_SCREEN_RESIZE
}

