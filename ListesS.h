#ifndef LISTESS_H
#define LISTESS_H
#include <stdio.h>

typedef struct eS { 
  char val[7]; 
  struct eS *suiv;
} ListeS;

ListeS *creeListeS(void);

void supprimerEnTeteS(ListeS *racine);

int supprimerOccrurencesMemeCouleurFormeEnQueueS(ListeS *l, int *val, int taille);

int supprimerOccrurencesMemeCouleurFormeEnTeteS(ListeS *l, int *val, int taille);

void ajouterEnQueueS(ListeS *racine, char val[7]);

void ajouterEnTeteS(ListeS *racine, char val[7]);

void afficheS(ListeS *l);

int tailleS(ListeS *l);
#endif