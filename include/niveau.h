#ifndef NIVEAU_H_
#define NIVEAU_H_



extern int niveau_1_joueur(SDL_Surface *screen, int niveau);
extern int niveau_2_joueur(SDL_Surface *screen, int choix_niveau);
extern int editeur_choix_niveau(SDL_Surface *screen);
extern int are_you_sure(SDL_Surface *screen);
int choix_nb_joueurs(SDL_Surface *screen);
int options(SDL_Surface *screen);


#endif /* NIVEAU_H_ */
