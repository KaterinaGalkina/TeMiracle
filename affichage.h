#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include "ListesD.h"
#include "ListesS.h"
#include "commandesJeu.h"
#include "gestionFichiers.h"

void affichePiece(WINDOW *tableauDuJeu, int x, int y, char figures[5][2], int indFigure, int indColor);
void afficheListePieces(WINDOW *win, int x, int y, char figures[5][2], ListeS *listeAAfficher);
void animationPlacementFigure(WINDOW *tableauDuJeu, int milieu_x, int milieu_y, int taille, int pieceFigure, int pieceCouleur, int sens, char figuresAffichage[5][2]);
void animationSuppression(WINDOW *tableauDuJeu, int x, int y, int indexToDelete[15], int nombreElASupprimer, int listePlateauCopie[15][2], int taille, char figuresAffichage[5][2]);
