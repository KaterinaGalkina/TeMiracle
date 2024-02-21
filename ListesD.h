#ifndef LISTESD_H
#define LISTESD_H
#include <stdio.h>

typedef struct eD {
  char val[7];
  struct eD *prec;
  struct eD *suiv;
} ListeD;

ListeD *creeListeD(void);

void decalerListeD(ListeD *liste, int taille); 

void ajouterEnQueueD(ListeD *racine, char val[7]);

void ajouterEnTeteD(ListeD *racine, char val[7]);

void supprimerEnTeteD(ListeD *racine);

void supprimerEnQueueD(ListeD *racine);

void afficheD(ListeD *l);
#endif