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
#include "affichage.h" 

// Permet d'afficher une piece d'une figure et d'une couleur particuliere
void affichePiece(WINDOW *tableauDuJeu, int x, int y, char figures[5][2], int indFigure, int indCouleur) {
  attron(COLOR_PAIR(indCouleur + 1));
  mvprintw(y, x, "%s ", figures[indFigure]);
  attroff(COLOR_PAIR(indCouleur + 1)); 
}

//affiche une liste simplement chainee de pieces en ligne 
void afficheListePieces(WINDOW *win, int x, int y, char figures[5][2], ListeS *listeAAfficher) {
  ListeS *it;
  int c;
  int f;
  int cpt = 0;
  for(it = listeAAfficher->suiv; it != listeAAfficher; it = it->suiv) {
    figureType(it->val, &c, &f);
    affichePiece(win, x + cpt, y, figures, f, c);
    cpt = cpt + 2;
  }
}

// Fait une animation du placement d'une figure a droite ou a gauche (a droite si sens = 0 )
void animationPlacementFigure(WINDOW *tableauDuJeu, int milieu_x, int milieu_y, int taille, int pieceFigure, int pieceCouleur, int sens, char figuresAffichage[5][2]) {
  if (sens == 0) {
    for (int k=0; k <= (int)taille/2 + 1 ; k++) {
      affichePiece(tableauDuJeu, milieu_x - k, milieu_y - 2, figuresAffichage, pieceFigure, pieceCouleur);
      refresh();
      usleep(100000);
      mvprintw(milieu_y - 2, milieu_x - k, " ");
      refresh();
    }
    for (int k=1; k >=0; k--) {
      affichePiece(tableauDuJeu, milieu_x - (int)taille/2 - 1, milieu_y - k, figuresAffichage, pieceFigure, pieceCouleur);
      refresh();
      usleep(100000);
      mvprintw(milieu_y - k, milieu_x - (int)taille/2 - 1, " ");
      refresh();
    }
  } else {
    for (int k=0; k <= (int)taille/2 + 1 ; k++) {
      affichePiece(tableauDuJeu, milieu_x + k + 1, milieu_y - 2, figuresAffichage, pieceFigure, pieceCouleur);
      refresh();
      usleep(100000);
      mvprintw(milieu_y - 2, milieu_x + k + 1, " ");
      refresh();
    }
    for (int k=1; k >=0; k--) {
      affichePiece(tableauDuJeu, milieu_x + (int)taille/2 + 2, milieu_y - k, figuresAffichage, pieceFigure, pieceCouleur);
      refresh();
      usleep(100000);
      mvprintw(milieu_y - k, milieu_x + (int)taille/2 + 2, " ");
      refresh();
    }
  }
}

// Annimation d'une suppression qui fait clignoter les pieces du tableau qui vont se supprimer
void animationSuppression(WINDOW *tableauDuJeu, int x, int y, int numeroElementsASupprimer[15], int nombreElASupprimer, int listePlateauCopie[15][2], int taille, char figuresAffichage[5][2]) {
  int ind = 0;
  int placement = 0;
  for(int g=0; g<3; g++) {
    ind = 0;
    placement = 0;
    // parcours de la liste
    for (int k=0; k<taille; k++) {
      if ((nombreElASupprimer > ind) && (k+1 == numeroElementsASupprimer[ind])) {
        mvprintw(y, x + placement, " ");
        // affichePiece(tableauDuJeu, x - (int)(taille/2) + k, y, figures, listePrincipale[k][0], 5);
        ind++;
      } else {
        affichePiece(tableauDuJeu, x + placement, y, figuresAffichage, listePlateauCopie[k][0], listePlateauCopie[k][1]);
      }
      placement+=2;
    }
    refresh();
    usleep(200000);
    placement = 0;
    // parcours de la liste
    for (int k=0; k<taille; k++) {
      affichePiece(tableauDuJeu, x + placement, y, figuresAffichage, listePlateauCopie[k][0], listePlateauCopie[k][1]);
      placement +=2;
    }
    refresh();
    usleep(200000);
  }
}