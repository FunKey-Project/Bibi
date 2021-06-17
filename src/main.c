#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>  // rand(), srand()
#include <time.h>    // time()
#include <SDL.h>
#include <SDL_image.h>

#include "../include/constant.h"
#include "../include/game.h"
#include "../include/game_time.h"
#include "../include/editeur.h"
#include "../include/niveau.h"
#ifdef FUNKEY_MENU
#include "../include/menu.h"
#endif //FUNKEY_MENU


//printf("File: %s, func: %s, l.%d \n",__FILE__, __func__, __LINE__);

#ifdef SOUND_FMOD_ACTIVATED
	#include <FMOD/fmod.h>
#elif defined(SOUND_SDL_ACTIVATED)
	#include <SDL/SDL_mixer.h>
#endif //SOUND_SDL_ACTIVATED
bool audio_init_ok = false;

#ifdef HW_SCREEN_RESIZE
	SDL_Surface *hw_screen = NULL;
#endif //HW_SCREEN_RESIZE
SDL_Surface *screen=NULL;
bool exit_game = false;

//##################################################### input_update  ####################################################################
//Gere les entrees clavier lors de la phase de jeu.
int input_update(t_game game, int nb_joueur) {
	SDL_Event event;
	t_player player1 = game_get_player1(game);
	t_player player2 = NULL;

	if (nb_joueur == 2)
	player2 = game_get_player2(game);

	t_map map = game_the_map(game);
	int x, y;

	// Loop through all pending events and process only the latest one
	while (SDL_PollEvent(&event)){
		switch (event.type) {
		case SDL_QUIT:
			exit_game = true;
			return 2;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
#ifdef FUNKEY
			case SDLK_q:
#endif //FUNKEY
#ifdef FUNKEY_MENU
			  	run_menu_loop();

			  	/** Exit game ?*/
			  	if(exit_game) return 2;
#else //FUNKEY_MENU
				return 2;
#endif //FUNKEY_MENU

			case SDLK_UP:
#ifdef FUNKEY
			case SDLK_u:
#endif //FUNKEY
				player_set_current_way(player1, UP);
				player_move(player1, map);
				if (player_win(player1)==1){
					return 1;
				}
				break;

			case SDLK_DOWN:
#ifdef FUNKEY
			case SDLK_d:
#endif //FUNKEY
				player_set_current_way(player1, DOWN);
				player_move(player1, map);
				if (player_win(player1)==1){
					return 1;
				}
				break;

			case SDLK_RIGHT:
#ifdef FUNKEY
			case SDLK_r:
#endif //FUNKEY
				player_set_current_way(player1, RIGHT);
				player_move(player1, map);
				if (player_win(player1)==1){
					return 1;
				}
				break;

			case SDLK_LEFT:
#ifdef FUNKEY
			case SDLK_l:
#endif //FUNKEY
				player_set_current_way(player1, LEFT);
				player_move(player1, map);
				if (player_win(player1)==1){
					return 1;
				}
				break;

			case SDLK_v:
#ifdef FUNKEY
			case SDLK_x:
			case SDLK_y:
#endif //FUNKEY
				game->viewport_mode = !game->viewport_mode;
				game->must_clear_screen = true;
				//printf("Changed viewport mode to: %d\n", game->viewport_mode);
			break;

			//sert à poser une bombe pour le joueur 1
			case SDLK_END:	//cette touche sert pour les ordinateurs portables qui n'ont pas forcément la touce 0 à côté des flèches directionnelles)
			case SDLK_KP0:
#ifdef FUNKEY
			case SDLK_a:
			case SDLK_b:
#endif //FUNKEY
				x= player_get_x(player1);
				y= player_get_y(player1);

				if (player_get_nb_bomb(player1)>0 && map_get_cell_type(map,x,y)!=CELL_BOMB){
					int portee=player_portee_bomb(player1);
					game_init_bomb(game,x,y,portee,1);
					map_set_cell_type(map, x, y, CELL_BOMB);
					player_decrease_nb_bomb(player1);
				}
				break;

#ifndef FUNKEY
			// touches du joueur 2:
			case SDLK_e:
				if (nb_joueur == 2){
					player_set_current_way(player2, UP);
					player_move(player2, map);
					if (player_win(player2)==1){
						return 1;
					}}
				break;

			case SDLK_d:
				if (nb_joueur == 2){
					player_set_current_way(player2, DOWN);
					player_move(player2, map);
					if (player_win(player2)==1){
						return 1;
					}}
				break;

			case SDLK_f:
				if (nb_joueur == 2){
					player_set_current_way(player2, RIGHT);
					player_move(player2, map);
					if (player_win(player2)==1){
						return 1;
					}}
				break;

			case SDLK_s:
				if (nb_joueur == 2){
					player_set_current_way(player2, LEFT);
					player_move(player2, map);
					if (player_win(player2)==1){
						return 1;
					}
				}
				break;

			case SDLK_SPACE:    //sert à poser une bombe pour le joueur 2
				if (nb_joueur == 2){
					x= player_get_x(player2);
					y= player_get_y(player2);

					if (player_get_nb_bomb(player2)>0 && map_get_cell_type(map,x,y)!=CELL_BOMB){
						int portee=player_portee_bomb(player2);
						game_init_bomb(game,x,y,portee,2);
						map_set_cell_type(map, x, y, CELL_BOMB);
						player_decrease_nb_bomb(player2);
					}
				}
				break;
#endif // not defined (FUNKEY)

			default: break;
			}

			break;
		}
	}
	return 0;
}


//##################################################### Main_game  ####################################################################
//Fonction principale une fois qu'une partie est lancee, gere l'actualisation du jeu.
//Cette fonction retourne un entier qui est le niveau à jouer ensuite (sert pour le mode 1 joueur)

int main_game(int nb_joueur, int niveau, int mode, int kill_bomb, int game_over) {
	//nb_joueur indique le nombre de joueurs
	//niveau indique le niveau à charger
	//mode indique si on est en mode jeu principal (dans ce cas mode=1) ou en mode "jouer aux niveaux édités" (mode=2)
	//kill_bomb indique si les bombes peuvent ou non s'exploser entre elles (non=0, oui(par défault)=1)
	//game_over indique combien de fois le joueur peut mourrir avant de faire game over. (3,2,1 ou 0)

	srand(time(NULL));
	SDL_Surface *menu = NULL;
	SDL_Event event;
	SDL_Rect positionMenu;
	positionMenu.x = 0;
	positionMenu.y = 0;
	t_game game = game_new(nb_joueur,niveau,mode, kill_bomb);			//on lance le jeu

#ifdef HW_SCREEN_RESIZE
	SDL_FillRect(hw_screen, NULL, 0x000000);
	if(screen != NULL) SDL_FreeSurface(screen);
	screen = SDL_CreateRGBSurface(SDL_SWSURFACE, SIZE_BLOC * map_get_width(game_the_map(game)),
		SIZE_BLOC * map_get_height(game_the_map(game)), WINDOW_BPP, 
		0, 0, 0, 0);
#else 
	screen = SDL_SetVideoMode(SIZE_BLOC * map_get_width(game_the_map(game)),
		SIZE_BLOC * map_get_height(game_the_map(game)), WINDOW_BPP,
		SDL_HWSURFACE);
	if (screen == NULL) {
		error("Can't set video mode: %s\n", SDL_GetError());
		exit(1);
	}
#endif //HW_SCREEN_RESIZE

	game_display(game, screen);

	int done = 0;		//variable qui indique quand doit s'arréter la boucle du jeu, elle peut prendre les valeurs 2,1, ou 0.
	// La boucle s'arrête pour les valeurs 2 et 1.
	int k=1, l=1, m=1, n=1;
	t_player player1 = game_get_player1(game);
	t_player player2=NULL;
	if (nb_joueur == 2)
	player2 = game_get_player2(game);

	//SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_INTERVAL, SDL_DEFAULT_REPEAT_INTERVAL);

	if (nb_joueur==1){			
		// boucle principale d'un jeu à 1 joueur:
		while (done==0 && player_get_dead(player1)!=0) {

			game_time_update();

			if(player_get_lives(player1)!=0){
				done = input_update(game, nb_joueur);  
				
				bombs_management(game,0);

				if (k==BOMBS_SPEED){
					k=1;
					bombs_management(game,1);    //Le management des bombes en jeu se fait tous les "BOMBS_SPEED"
					// Le pamètre 1 de cette fonction indique qu'elle doit gérer toutes les bombes en jeu
				}
				else k++;


				if (l==MONSTER_SPEED){
					l=1;
					monsters_management(game,1);			//Le management des monstres en jeu se fait tous les "MONSTER_SPEED"
					// Le pamètre 1 de cette fonction indique le nombre de joueurs
				}
				else l++;
			}
			else if (player_get_lives(player1)==0){
				if (m==ANIMATION_SPEED){
					m=1;
					player_die(player1);				// Quand le joueur meurt, le jeu s'arrête et une animation se lance.
				}
				else m++;
			}

			flamme_management(game);			// le management des flammes se fait toutes les actualisatons du jeu.

			game_display(game, screen);

		}
	}
	else if (nb_joueur==2){		
		// boucle principale d'un jeu à 2 joueurs:
		while (done==0 && player_get_dead(player1)!=0 && player_get_dead(player2)!=0) {

			game_time_update();

			if( player_get_lives(player1)!=0 && player_get_lives(player2)!=0){
				done = input_update(game,nb_joueur);
				
				bombs_management(game,0);

				if (k==BOMBS_SPEED){
					k=1;
					bombs_management(game,1);
				}
				else k++;

				if (l==MONSTER_SPEED){
					l=1;
					monsters_management(game, 2);
				}
				else l++;
			}
			else{
				if (player_get_lives(player1)==0){
					if (m==ANIMATION_SPEED){
						m=1;
						player_die(player1);
					}
					else m++;
				}

				if (player_get_lives(player2)==0){
					if (n==ANIMATION_SPEED){
						n=1;
						player_die(player2);
					}
					else n++;
				}
			}

			flamme_management(game);
			game_display(game, screen);

		}
	}

	/** Free game assets */
	game_free(game);

	int boucle=0;   //cette variable autorise ou non l'affichage de messages comme "vous avez gagné", "game over", ...
	
	if (nb_joueur==1 && player_win(player1)==1){
		menu = IMG_Load("sprite/you_win.png");
		boucle=1;
	}
	if (nb_joueur==1 && player_get_dead(player1)==0 && mode!=2){
		switch (game_over){
		case 3:
			menu = IMG_Load("sprite/you_die_3.png");
			break;
		case 2:
			menu = IMG_Load("sprite/you_die_2.png");
			break;
		case 1:
			menu = IMG_Load("sprite/you_die_1.png");
			break;
		case 0:
			menu = IMG_Load("sprite/game_over.png");
			break;
		}
		boucle=1;
	}
	else if (nb_joueur==2){
		if(player_get_dead(player1)==0 && player_get_dead(player2)==0){
			menu = IMG_Load("sprite/egalite.png");
			boucle=1;}

		else if (player_get_dead(player1)==0){
			menu = IMG_Load("sprite/p_2_win.png");
			boucle=1;
		}

		else if (player_get_dead(player2)==0){
			menu = IMG_Load("sprite/p_1_win.png");
			boucle=1;
		}
		else if(player_win(player1)){
			menu = IMG_Load("sprite/p_1_win.png");
			boucle=1;
		}
		else if(player_win(player2)){
			menu = IMG_Load("sprite/p_2_win.png");
			boucle=1;
		}
	}

	bool continu = false;
	if (boucle==1 && menu != NULL){
		
		/** Display Menu */
#ifdef HW_SCREEN_RESIZE
		SDL_FillRect(hw_screen, NULL, 0x000000);
		if(screen != NULL) SDL_FreeSurface(screen);
		screen = SDL_CreateRGBSurface(SDL_SWSURFACE, 480,480, WINDOW_BPP, 0, 0, 0, 0);
#else //HW_SCREEN_RESIZE
		screen = SDL_SetVideoMode(480,480, WINDOW_BPP,SDL_HWSURFACE);
		if (screen == NULL) {
			error("Can't set video mode: %s\n", SDL_GetError());
			exit(1);
		}
#endif //HW_SCREEN_RESIZE

		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		SDL_BlitSurface(menu, NULL, screen, &positionMenu);
		SDL_FreeSurface(menu);

#ifdef HW_SCREEN_RESIZE
		flip_NNOptimized_AllowOutOfScreen(screen, hw_screen,
	        HW_SCREEN_WIDTH,
	        MIN(screen->h*HW_SCREEN_WIDTH/screen->w, HW_SCREEN_HEIGHT));
		SDL_Flip(hw_screen);
#else //HW_SCREEN_RESIZE
		SDL_Flip(screen);
#endif //HW_SCREEN_RESIZE

		/** Wait for key press */
		while (!continu && !exit_game)
		{
			SDL_Delay(150); // For debounce
			SDL_WaitEvent(&event);
			switch(event.type)
			{
			case SDL_QUIT:
				continu = true;
				exit_game = true;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
				
				case SDLK_q:
	#ifdef FUNKEY_MENU
				  	run_menu_loop();

				  	/** Exit game ?*/
				  	if(exit_game) continu = true;
	#else //FUNKEY_MENU
					continu = true;
	#endif //FUNKEY_MENU
					break;

				case SDLK_ESCAPE:
				case SDLK_KP_ENTER:
				case SDLK_RETURN:
				case SDLK_a:
				case SDLK_b:
					continu = true;
					break;

				default: break;
				}
				break;
			}
		}
	}

	if (nb_joueur==1 && player_win(player1)==1){
		return niveau;			//permet de passer au niveau suivant
	}
	else if(game_over==0){
		game_over=NB_DECES;
		return 0;				// s'il y a game over la valeur retournée est 0 afin de reprendre le jeu depuis le niveau 1 
	}
	else if(done==2){
		return -1;				// la valeur -1 sert à indiquer que l'utilisateur a appuyé sur échap ou quitter
		// elle informe qu'il faudra rejouer le niveau sans décrémenter la variable game_over
		// (donc le nombre de décès autorisés avant d'avoir un game over)
	}
	else {
		return niveau-1;		// lorsque le joueur meurt, informe qu'il faut 
		//rejouer le niveau précédent et décrémenter la variable game_over
	}
}


//##################################################### Main ####################################################################
//Fonction principale du jeu
int main_menu_and_game() {
	SDL_Surface *menu = NULL;
	SDL_Event event;
	SDL_Rect positionMenu;
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		error("Can't init SDL:  %s\n", SDL_GetError());
	}
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
#ifdef FUNKEY
	SDL_ShowCursor(SDL_DISABLE);
#endif //FUNKEY

	/** Init Video */
#ifdef HW_SCREEN_RESIZE
	hw_screen = SDL_SetVideoMode(HW_SCREEN_WIDTH,HW_SCREEN_HEIGHT, WINDOW_BPP, SDL_HWSURFACE|SDL_DOUBLEBUF);
	if (hw_screen == NULL) {
		error("Can't set video mode: %s\n", SDL_GetError());
	}
	if(screen != NULL) SDL_FreeSurface(screen);
	screen = SDL_CreateRGBSurface(SDL_SWSURFACE, 700,500, WINDOW_BPP, 0, 0, 0, 0);
#else //HW_SCREEN_RESIZE
	screen = SDL_SetVideoMode(700,500, WINDOW_BPP,SDL_HWSURFACE);
	if (screen == NULL) {
		error("Can't set video mode: %s\n", SDL_GetError());
	}
#endif //HW_SCREEN_RESIZE
	SDL_WM_SetIcon(IMG_Load(IMG_PLAYER_DOWN), NULL);
	SDL_WM_SetCaption("[PG110] Projet 2010", NULL);

printf("File: %s, func: %s, l.%d \n",__FILE__, __func__, __LINE__);
	/** Load audio */
#ifdef SOUND_FMOD_ACTIVATED
	FMUSIC_MODULE *musique_menu_p;
	FMUSIC_MODULE *musique_editeur;
	FMUSIC_MODULE *musique_2_p;
	FMUSIC_MODULE *musique_1_p;
	FMUSIC_MODULE *musique_p_e;

	if(!FSOUND_Init(11025, 32, 0)){
		printf("Cannot init audio with FSOUND_Init\n");
	}
	else{
		audio_init_ok = true;
		musique_menu_p = FMUSIC_LoadSong("audio/mm2titl2.mid");
		musique_editeur = FMUSIC_LoadSong("audio/mm2crash.mid");
		musique_2_p = FMUSIC_LoadSong("audio/mm2wy1.mid");
		musique_1_p = FMUSIC_LoadSong("audio/mix_1_p.mid");
		musique_p_e = FMUSIC_LoadSong("audio/mm2flash.mid");
		FMUSIC_SetMasterVolume(musique_menu_p, 130);
		FMUSIC_SetMasterVolume(musique_p_e, 130);
		FMUSIC_SetMasterVolume(musique_editeur, 130);
		FMUSIC_SetMasterVolume(musique_2_p, 130);
		FMUSIC_SetMasterVolume(musique_1_p, 130);	
	}
#elif defined(SOUND_SDL_ACTIVATED)
	// Setup audio mode
	Mix_Music *musique_menu_p = NULL;
	Mix_Music *musique_editeur = NULL;
	Mix_Music *musique_2_p = NULL;
	Mix_Music *musique_1_p = NULL;
	Mix_Music *musique_p_e = NULL;

	//if(Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640)==-1){
	//if(Mix_OpenAudio(44100, AUDIO_S16, 1, 4096) < 0){
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1){
	    printf("Mix_OpenAudio: %s\n", Mix_GetError());
	    //exit(2);
	}
	else{
		audio_init_ok = true;
		//Mix_Volume(1,MIX_MAX_VOLUME);
		const char *music_file;
		music_file = "audio/mm2titl2.ogg";
		musique_menu_p = Mix_LoadMUS(music_file);
		if(musique_menu_p == NULL){
			printf("Mix_LoadMUS(\"%s\"): %s\n", Mix_GetError(), music_file);
		}
		music_file = "audio/mm2crash.ogg";
		musique_editeur = Mix_LoadMUS(music_file);
		if(musique_editeur == NULL){
			printf("Mix_LoadMUS(\"%s\"): %s\n", Mix_GetError(), music_file);
		}
		music_file = "audio/mm2wy1.ogg";
		musique_2_p = Mix_LoadMUS(music_file);
		if(musique_2_p == NULL){
			printf("Mix_LoadMUS(\"%s\"): %s\n", Mix_GetError(), music_file);
		}
		music_file = "audio/mix_1_p.ogg";
		musique_1_p = Mix_LoadMUS(music_file);
		if(musique_1_p == NULL){
			printf("Mix_LoadMUS(\"%s\"): %s\n", Mix_GetError(), music_file);
		}
		music_file = "audio/mm2flash.ogg";
		musique_p_e = Mix_LoadMUS(music_file);
		if(musique_p_e == NULL){
			printf("Mix_LoadMUS(\"%s\"): %s\n", Mix_GetError(), music_file);
		}
	}
#endif //SOUND_SDL_ACTIVATED

	/** Init vars for graphics and game */
	menu = IMG_Load("sprite/menu_1_p.png");
	positionMenu.x = 0;
	positionMenu.y = 0;

	bool done = false;			//variable de la boucle principale du bomberman (false: le jeu continu,  true: le jeu s'arrête)
	int niveau_reussi=0, ancien_niveau=1, choix_entrer_dans_niveau, choix_entrer_niveau_2p=1, choix_niveau=1,
	choix_ancien_niveau, choix_option, nb_joueurs, sure, kill_bomb=1;

	//niveau_reussi sert pour le mode 1 joueur à indiquer à quel niveau se trouve le joueur
	//ancien_niveau informe à quel niveau se trouvait le joueur avant de lancer main_game
	//choix_entrer_dans_niveau=0 si le joueur accepte de rentrer dans le niveau indiqué, 1 sinon.
	//choix_entrer_niveau_2p permet de choisir lequel des 6 niveaux disponibles en 2 joueurs l'utilisateur choisi.
	//choix_niveau sert à la même chose que choix_entrer_niveau_2p mais pour l'éditeur et le mode "jouer aux jeux édités"
	//idem pour choix_option sauf que c'est pour le mode option (1:jouer son, 2: arrêter son, 3: les bombes s'explosent
	//entres elles, 4: les bombes ne s'explosent pas entre elles)
	//choix_ancien_niveau sert (en deux joueurs et en mode "jouer aux jeux édités) à replacer automatiquement 
	//le curseur sur le niveau qui vien d'être joué.
	//sure=1 si l'utilisateur est sûr de quitter, 0 sinon.
	//kill_bomb=0 si les bombes ne s'explosent pas entre elles, 1 sinon.
	//audio sert pour le son (voir version Windows)

	int choix_actuel=1;  //indique la position du curseur dans le menu principal (1: jeu 1 joueur, 2: jeu 2 joueurs,
	//3:editeur, 4:jouer aux niveaux edités, 5: options, 6: quitter)
	int game_over=NB_DECES;
	int  resize=0; //Si resize=1 on remet l'écran à la taille 700*500
	bool menu_change = true;
	int play_music=1;
	int audio=1;
	int prev_time_ms = 0;

	while (!done)
	{
		if (play_music==1){
			if(audio_init_ok && audio==1){
#ifdef SOUND_FMOD_ACTIVATED
		 		FMUSIC_PlaySong(musique_menu_p);
#elif defined(SOUND_SDL_ACTIVATED)
		 		if(Mix_PlayMusic(musique_menu_p, -1) < 0){
					printf("Error in File: %s, func: %s, l.%d - Cannot play music!\n", 
						__FILE__, __func__, __LINE__);
				}
#endif //SOUND_SDL_ACTIVATED
		 	}
			play_music=0;
		}

		//SDL_WaitEvent(&event);
		event.type = 0; // somehow we need to reset here the last event, or it keep being "a"
		while (SDL_PollEvent(&event));
		switch(event.type)
		{

		case SDL_QUIT:
#ifdef SOUND_FMOD_ACTIVATED
			//FMUSIC_StopSong(musique_menu_p);
#elif defined(SOUND_SDL_ACTIVATED)
		 	Mix_HaltMusic();
#endif //SOUND_SDL_ACTIVATED
			done = 1;
			break;

		case SDL_KEYDOWN:
			switch(event.key.keysym.sym){
			
			case SDLK_ESCAPE: // Veut arrêter le jeu
			case SDLK_q: // Veut arrêter le jeu
#ifdef SOUND_FMOD_ACTIVATED
				//FMUSIC_StopSong(musique_menu_p);
#elif defined(SOUND_SDL_ACTIVATED)
				Mix_HaltMusic();
#endif //SOUND_SDL_ACTIVATED
				done = 1;
				break;
			
			case SDLK_UP:
			case SDLK_u:
				switch(choix_actuel){
				case 1:
					menu = IMG_Load("sprite/menu_q.png");
					choix_actuel=6;
					break;
				case 2:
					menu = IMG_Load("sprite/menu_1_p.png");
					choix_actuel=1;
					break;
				case 3:
					menu = IMG_Load("sprite/menu_2_p.png");
					choix_actuel=2;
					break;
				case 4:
					menu = IMG_Load("sprite/menu_e.png");
					choix_actuel=3;
					break;
				case 5:
					menu = IMG_Load("sprite/menu_j_e.png");
					choix_actuel=4;
					break;
				case 6:
					menu = IMG_Load("sprite/menu_o.png");
					choix_actuel=5;
					break;
				}
				menu_change = true;
				break;
			
			case SDLK_DOWN:
			case SDLK_d:
				switch(choix_actuel){
				case 1:
					menu = IMG_Load("sprite/menu_2_p.png");
					choix_actuel=2;
					break;
				case 2:
					menu = IMG_Load("sprite/menu_e.png");
					choix_actuel=3;
					break;
				case 3:
					menu = IMG_Load("sprite/menu_j_e.png");
					choix_actuel=4;
					break;
				case 4:
					menu = IMG_Load("sprite/menu_o.png");
					choix_actuel=5;
					break;
				case 5:
					menu = IMG_Load("sprite/menu_q.png");
					choix_actuel=6;
					break;
				case 6:
					menu = IMG_Load("sprite/menu_1_p.png");
					choix_actuel=1;
					break;
				}
				menu_change = true;
				break;

			case SDLK_RETURN:
			case SDLK_KP_ENTER:
			case SDLK_a:
				switch(choix_actuel){
					
				case 1:						//on rentre dans le mode 1 joueur.
				 	if(audio_init_ok && audio==1){
#ifdef SOUND_FMOD_ACTIVATED
						FMUSIC_StopSong(musique_menu_p);
				 		FMUSIC_PlaySong(musique_1_p);
#elif defined(SOUND_SDL_ACTIVATED)
				 		Mix_HaltMusic();
				 		if(Mix_PlayMusic(musique_1_p, -1) < 0){
							printf("Error in File: %s, func: %s, l.%d - Cannot play music!\n", 
								__FILE__, __func__, __LINE__);
						}
#endif //SOUND_SDL_ACTIVATED
					}
					while (niveau_reussi<10){  //En effet il n'y a que 10 niveaux dans ce jeu

						if(game_over<0){
							niveau_reussi=0;	//après game over le joueur repart du niveau 1;
							game_over=NB_DECES;
						}
						choix_entrer_dans_niveau = niveau_1_joueur(screen,niveau_reussi+1);
						if (choix_entrer_dans_niveau==0){
							ancien_niveau=niveau_reussi;
							niveau_reussi=main_game(1,niveau_reussi+1,1, kill_bomb, game_over); // le jeu se lance, 
							//le résultat retourné est stocké dans la variable niveau_reussi.

							if(ancien_niveau==niveau_reussi && ancien_niveau!=0){
								game_over--;		//on regarde si le joueur est mort, dans ce cas on décrémente game_over.
							}
							else if(niveau_reussi==-1){		//si l'utilisateur a fait échap ou quitter durant la partie.
								niveau_reussi=ancien_niveau;
							}

							if (niveau_reussi>=10){ //si l'utilisateur à gagné le mode 1 joueur
								choix_entrer_dans_niveau=niveau_1_joueur(screen,0);
								niveau_reussi=0;
								game_over=NB_DECES;
								// FMUSIC_StopSong(musique_1_p);
								// play_music=1;
								break;
							}

						}
						else{
							if(are_you_sure(screen)==1){  //si l'utilisateur veut quitter le mode 1 joueur
								play_music=1;
				 				if(audio_init_ok && audio==1){
#ifdef SOUND_FMOD_ACTIVATED
									FMUSIC_StopSong(musique_1_p);
#elif defined(SOUND_SDL_ACTIVATED)
							 		Mix_HaltMusic();
#endif //SOUND_SDL_ACTIVATED
								}
								break;
							}
						}
					}
					resize=1;
					break;

				case 2:
				 	if(audio_init_ok && audio==1){
#ifdef SOUND_FMOD_ACTIVATED
						FMUSIC_StopSong(musique_menu_p);
						FMUSIC_PlaySong(musique_2_p);
#elif defined(SOUND_SDL_ACTIVATED)
				 		Mix_HaltMusic();
				 		if(Mix_PlayMusic(musique_2_p, -1) < 0){
							printf("Error in File: %s, func: %s, l.%d - Cannot play music!\n", 
								__FILE__, __func__, __LINE__);
						}
#endif //SOUND_SDL_ACTIVATED
					}
					sure=2;
					choix_entrer_niveau_2p=1;
					choix_ancien_niveau=choix_entrer_niveau_2p;
					while (sure!=1){
						if (choix_entrer_niveau_2p!=0)
						choix_ancien_niveau=choix_entrer_niveau_2p;

						choix_entrer_niveau_2p=niveau_2_joueur(screen,choix_ancien_niveau);

						if(choix_entrer_niveau_2p==0)
						sure=are_you_sure(screen);
						else
						main_game(2,choix_entrer_niveau_2p,1, kill_bomb, game_over);


					}

					play_music=1;
	 				if(audio_init_ok && audio==1){
#ifdef SOUND_FMOD_ACTIVATED
						FMUSIC_StopSong(musique_2_p);
#elif defined(SOUND_SDL_ACTIVATED)
				 		Mix_HaltMusic();
#endif //SOUND_SDL_ACTIVATED
				 	}
					resize=1;
					break;

				case 3:
				 	if(audio_init_ok && audio==1){
#ifdef SOUND_FMOD_ACTIVATED
						FMUSIC_StopSong(musique_menu_p);
						FMUSIC_PlaySong(musique_editeur);
#elif defined(SOUND_SDL_ACTIVATED)
				 		Mix_HaltMusic();
				 		if(Mix_PlayMusic(musique_editeur, -1) < 0){
							printf("Error in File: %s, func: %s, l.%d - Cannot play music!\n", 
								__FILE__, __func__, __LINE__);
						}
#endif //SOUND_SDL_ACTIVATED
					}
					sure=2;
					choix_niveau=1;
					choix_ancien_niveau=choix_niveau;
					while (sure!=1){
						if (choix_niveau!=0)
						choix_ancien_niveau=choix_niveau;

						choix_niveau=niveau_2_joueur(screen,choix_ancien_niveau);

						if(choix_niveau==0)
							sure=are_you_sure(screen);
						else if (niveau_1_joueur(screen,-1)==0)
							editeur(screen,choix_niveau);

					}

					play_music=1;
	 				if(audio_init_ok && audio==1){
#ifdef SOUND_FMOD_ACTIVATED
						FMUSIC_StopSong(musique_editeur);
#elif defined(SOUND_SDL_ACTIVATED)
				 		Mix_HaltMusic();
#endif //SOUND_SDL_ACTIVATED
				 	}

					resize=1;
					break;

				case 4:
				 	if(audio_init_ok && audio==1){
#ifdef SOUND_FMOD_ACTIVATED
						FMUSIC_StopSong(musique_menu_p);
						FMUSIC_PlaySong(musique_p_e);
#elif defined(SOUND_SDL_ACTIVATED)
				 		Mix_HaltMusic();
				 		if(Mix_PlayMusic(musique_p_e, -1) < 0){
							printf("Error in File: %s, func: %s, l.%d - Cannot play music!\n", 
								__FILE__, __func__, __LINE__);
						}
#endif //SOUND_SDL_ACTIVATED
					}
					sure=2;
					choix_niveau=1;
					choix_ancien_niveau=choix_niveau;
					while (sure!=1){
						if (choix_niveau!=0){
							choix_ancien_niveau=choix_niveau;
						}
						choix_niveau=niveau_2_joueur(screen,choix_ancien_niveau);


						if(choix_niveau==0){
							sure=are_you_sure(screen);
						}
						else{

							nb_joueurs=choix_nb_joueurs(screen);

							if(nb_joueurs!=0 ){
								main_game(nb_joueurs,choix_niveau,2, kill_bomb, game_over);
							}
						}

					}

					play_music=1;
	 				if(audio_init_ok && audio==1){
#ifdef SOUND_FMOD_ACTIVATED
						FMUSIC_StopSong(musique_p_e);
#elif defined(SOUND_SDL_ACTIVATED)
				 		Mix_HaltMusic();
#endif //SOUND_SDL_ACTIVATED
				 	}

					resize=1;
					break;

				case 5:
					choix_option=options(screen);

					switch(choix_option){
					case 0:
						break;
					case 1:
					 	if(audio_init_ok){
#ifdef SOUND_FMOD_ACTIVATED
							FMUSIC_PlaySong(musique_menu_p);
#elif defined(SOUND_SDL_ACTIVATED)
					 		if(Mix_PlayMusic(musique_menu_p, -1) < 0){
								printf("Error in File: %s, func: %s, l.%d - Cannot play music!\n", 
									__FILE__, __func__, __LINE__);
							}
#endif //SOUND_SDL_ACTIVATED
						}
						audio=1;
						break;
					case 2:
					 	if(audio_init_ok){
#ifdef SOUND_FMOD_ACTIVATED
							FMUSIC_StopSong(musique_menu_p);
#elif defined(SOUND_SDL_ACTIVATED)
							Mix_HaltMusic();
#endif //SOUND_SDL_ACTIVATED
						}
						audio=0;
						break;
					case 3:
						kill_bomb=1;
						break;
					case 4:
						kill_bomb=0;
						break;
					default: break;
					}
					resize=1;
					break;


				case 6:
					if(are_you_sure(screen)==1){
						if(audio_init_ok && audio==1){
#ifdef SOUND_FMOD_ACTIVATED
							FMUSIC_StopSong(musique_menu_p);
#elif defined(SOUND_SDL_ACTIVATED)
							Mix_HaltMusic();
#endif //SOUND_FMOD_ACTIVATED
						}
						done=1;
						resize=1;
						break;
					}
					else{
						resize=1;
					}
					break;


				default: 
					break;

				}
				break;
			
			default: 
				break;
			
			}
			break; 

			default: 
				break;
		}
		
		/** Reset main menu screen */
		if (resize==1){
#ifdef HW_SCREEN_RESIZE
			SDL_FillRect(hw_screen, NULL, 0x000000);
			if(screen != NULL) SDL_FreeSurface(screen);
			screen = SDL_CreateRGBSurface(SDL_SWSURFACE, 700,500, WINDOW_BPP, 0, 0, 0, 0);
#else //HW_SCREEN_RESIZE
			screen = SDL_SetVideoMode(700,500, WINDOW_BPP,SDL_HWSURFACE);
			if (screen == NULL) {
				error("Can't set video mode: %s\n", SDL_GetError());
				exit(1);
			}
#endif //HW_SCREEN_RESIZE
		}

		// Effacement de l'écran
		if(menu_change || resize){
			//SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
			SDL_BlitSurface(menu, NULL, screen, &positionMenu);
			menu_change = false;
		}
		resize=0;

#ifdef HW_SCREEN_RESIZE
		flip_NNOptimized_AllowOutOfScreen(screen, hw_screen,
	        HW_SCREEN_WIDTH,
	        MIN(screen->h*HW_SCREEN_WIDTH/screen->w, HW_SCREEN_HEIGHT));
		SDL_Flip(hw_screen);
#else //HW_SCREEN_RESIZE
		SDL_Flip(screen);
#endif //HW_SCREEN_RESIZE

		/** FPS handling */
#define FPS_MENU	30		
		int cur_time_ms = SDL_GetTicks();
		int wait_time_ms = 1000/FPS_MENU - (cur_time_ms-prev_time_ms);
		if(wait_time_ms > 0){
			SDL_Delay(wait_time_ms);
		}
		prev_time_ms = SDL_GetTicks();
	}

	if(audio_init_ok){
#ifdef SOUND_FMOD_ACTIVATED
		FMUSIC_FreeSong(musique_menu_p);
		FMUSIC_FreeSong(musique_2_p);
		FMUSIC_FreeSong(musique_1_p);
		FMUSIC_FreeSong(musique_p_e);
		FMUSIC_FreeSong(musique_editeur);
#elif defined(SOUND_SDL_ACTIVATED)
		Mix_FreeMusic(musique_menu_p);
		Mix_FreeMusic(musique_2_p);
		Mix_FreeMusic(musique_1_p);
		Mix_FreeMusic(musique_p_e);
		Mix_FreeMusic(musique_editeur);
		Mix_CloseAudio();
#endif 	//SOUND_SDL_ACTIVATED
	}

	if(menu != NULL) SDL_FreeSurface(menu);
	SDL_Quit();

	return EXIT_SUCCESS;
}














int main_game_no_menu() {
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		error("Can't init SDL:  %s\n", SDL_GetError());
	}
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
#ifdef FUNKEY
	SDL_ShowCursor(SDL_DISABLE);
#endif //FUNKEY

	/** Init Video */
#ifdef HW_SCREEN_RESIZE
	hw_screen = SDL_SetVideoMode(HW_SCREEN_WIDTH,HW_SCREEN_HEIGHT, WINDOW_BPP, SDL_HWSURFACE|SDL_DOUBLEBUF);
	if (hw_screen == NULL) {
		error("Can't set video mode: %s\n", SDL_GetError());
	}
	if(screen != NULL) SDL_FreeSurface(screen);
	screen = SDL_CreateRGBSurface(SDL_SWSURFACE, 700,500, WINDOW_BPP, 0, 0, 0, 0);
#else //HW_SCREEN_RESIZE
	screen = SDL_SetVideoMode(700,500, WINDOW_BPP,SDL_HWSURFACE);
	if (screen == NULL) {
		error("Can't set video mode: %s\n", SDL_GetError());
	}
#endif //HW_SCREEN_RESIZE
	SDL_WM_SetIcon(IMG_Load(IMG_PLAYER_DOWN), NULL);
	SDL_WM_SetCaption("[PG110] Projet 2010", NULL);

#ifdef FUNKEY_MENU
    if(TTF_Init())
    {
        fprintf(stderr, "Error TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
	  /// --- Init menu ---
	  init_menu_SDL();
#endif //FUNKEY_MENU

	/** Load audio */
#ifdef SOUND_FMOD_ACTIVATED
	FMUSIC_MODULE *musique_menu_p;
	FMUSIC_MODULE *musique_editeur;
	FMUSIC_MODULE *musique_2_p;
	FMUSIC_MODULE *musique_1_p;
	FMUSIC_MODULE *musique_p_e;

	if(!FSOUND_Init(11025, 32, 0)){
		printf("Cannot init audio with FSOUND_Init\n");
	}
	else{
		audio_init_ok = true;
		musique_menu_p = FMUSIC_LoadSong("audio/mm2titl2.mid");
		musique_editeur = FMUSIC_LoadSong("audio/mm2crash.mid");
		musique_2_p = FMUSIC_LoadSong("audio/mm2wy1.mid");
		musique_1_p = FMUSIC_LoadSong("audio/mix_1_p.mid");
		musique_p_e = FMUSIC_LoadSong("audio/mm2flash.mid");
		FMUSIC_SetMasterVolume(musique_menu_p, 130);
		FMUSIC_SetMasterVolume(musique_p_e, 130);
		FMUSIC_SetMasterVolume(musique_editeur, 130);
		FMUSIC_SetMasterVolume(musique_2_p, 130);
		FMUSIC_SetMasterVolume(musique_1_p, 130);	
	}
#elif defined(SOUND_SDL_ACTIVATED)
	// Setup audio mode
	Mix_Music *musique_menu_p = NULL;
	Mix_Music *musique_editeur = NULL;
	Mix_Music *musique_2_p = NULL;
	Mix_Music *musique_1_p = NULL;
	Mix_Music *musique_p_e = NULL;

	//if(Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640)==-1){
	//if(Mix_OpenAudio(44100, AUDIO_S16, 1, 4096) < 0){
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1){
	    printf("Mix_OpenAudio: %s\n", Mix_GetError());
	    //exit(2);
	}
	else{
		audio_init_ok = true;
		//Mix_Volume(1,MIX_MAX_VOLUME);
		const char *music_file;
		music_file = "audio/mm2titl2.ogg";
		musique_menu_p = Mix_LoadMUS(music_file);
		if(musique_menu_p == NULL){
			printf("Mix_LoadMUS(\"%s\"): %s\n", Mix_GetError(), music_file);
		}
		music_file = "audio/mm2crash.ogg";
		musique_editeur = Mix_LoadMUS(music_file);
		if(musique_editeur == NULL){
			printf("Mix_LoadMUS(\"%s\"): %s\n", Mix_GetError(), music_file);
		}
		music_file = "audio/mm2wy1.ogg";
		musique_2_p = Mix_LoadMUS(music_file);
		if(musique_2_p == NULL){
			printf("Mix_LoadMUS(\"%s\"): %s\n", Mix_GetError(), music_file);
		}
		music_file = "audio/mix_1_p.ogg";
		musique_1_p = Mix_LoadMUS(music_file);
		if(musique_1_p == NULL){
			printf("Mix_LoadMUS(\"%s\"): %s\n", Mix_GetError(), music_file);
		}
		music_file = "audio/mm2flash.ogg";
		musique_p_e = Mix_LoadMUS(music_file);
		if(musique_p_e == NULL){
			printf("Mix_LoadMUS(\"%s\"): %s\n", Mix_GetError(), music_file);
		}
	}
#endif //SOUND_SDL_ACTIVATED
		
 	if(audio_init_ok){
#ifdef SOUND_FMOD_ACTIVATED
		FMUSIC_StopSong(musique_menu_p);
 		FMUSIC_PlaySong(musique_1_p);
#elif defined(SOUND_SDL_ACTIVATED)
 		Mix_HaltMusic();
 		if(Mix_PlayMusic(musique_1_p, -1) < 0){
			printf("Error in File: %s, func: %s, l.%d - Cannot play music!\n", 
				__FILE__, __func__, __LINE__);
		}
#endif //SOUND_SDL_ACTIVATED
	}

	/** 1 player Game */
	int niveau_reussi=0, ancien_niveau=1, choix_entrer_dans_niveau, kill_bomb=1;
	int game_over=NB_DECES;
	while (niveau_reussi<10 && !exit_game){  //En effet il n'y a que 10 niveaux dans ce jeu

		if(game_over<0){
			niveau_reussi=0;	//après game over le joueur repart du niveau 1;
			game_over=NB_DECES;
		}
		choix_entrer_dans_niveau = niveau_1_joueur(screen,niveau_reussi+1);

		/** exit game ? */
		if(exit_game) break;

		if (choix_entrer_dans_niveau==0){
			ancien_niveau=niveau_reussi;
			niveau_reussi=main_game(1,niveau_reussi+1,1, kill_bomb, game_over); // le jeu se lance, 
			//le résultat retourné est stocké dans la variable niveau_reussi.

			/** exit game ? */
			if(exit_game) break;

			if(ancien_niveau==niveau_reussi && ancien_niveau!=0){
				game_over--;		//on regarde si le joueur est mort, dans ce cas on décrémente game_over.
			}
			else if(niveau_reussi==-1){		//si l'utilisateur a fait échap ou quitter durant la partie.
				niveau_reussi=ancien_niveau;
			}

			if (niveau_reussi>=10){ //si l'utilisateur à gagné le mode 1 joueur
				choix_entrer_dans_niveau=niveau_1_joueur(screen,0);

				/** exit game ? */
				if(exit_game) break;

				niveau_reussi=0;
				game_over=NB_DECES;
				// FMUSIC_StopSong(musique_1_p);
				// play_music=1;
				break;
			}

		}
		else{
			if(are_you_sure(screen)==1){  //si l'utilisateur veut quitter le mode 1 joueur
 				if(audio_init_ok){
#ifdef SOUND_FMOD_ACTIVATED
					FMUSIC_StopSong(musique_1_p);
#elif defined(SOUND_SDL_ACTIVATED)
			 		Mix_HaltMusic();
#endif //SOUND_SDL_ACTIVATED
				}
				break;
			}
		}
	}
					
	if(audio_init_ok){
#ifdef SOUND_FMOD_ACTIVATED
		FMUSIC_FreeSong(musique_menu_p);
		FMUSIC_FreeSong(musique_2_p);
		FMUSIC_FreeSong(musique_1_p);
		FMUSIC_FreeSong(musique_p_e);
		FMUSIC_FreeSong(musique_editeur);
#elif defined(SOUND_SDL_ACTIVATED)
		Mix_FreeMusic(musique_menu_p);
		Mix_FreeMusic(musique_2_p);
		Mix_FreeMusic(musique_1_p);
		Mix_FreeMusic(musique_p_e);
		Mix_FreeMusic(musique_editeur);
		Mix_CloseAudio();
#endif 	//SOUND_SDL_ACTIVATED
	}


#ifdef FUNKEY_MENU
  	deinit_menu_SDL();
    TTF_Quit();
#endif //FUNKEY_MENU
	SDL_Quit();

	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
#ifdef BYPASS_MENU
	return main_game_no_menu();
#else
	return main_menu_and_game();
#endif
}