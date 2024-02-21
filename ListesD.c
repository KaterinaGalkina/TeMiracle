#include "ListesD.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ListeD *creeListeD(void) {
  ListeD *racine = malloc(sizeof *racine);
  if (racine != NULL) {
    strcpy(racine->val, "");
    racine->prec = racine;
    racine->suiv = racine;
  }
  return racine;
}

// decalage vers la gauche:
void decalerListeD(ListeD *liste, int taille) { 
  if (taille < 2) {
    return;
  }
  ListeD *it = liste->suiv;
  liste->suiv = liste->suiv->suiv;
  it->suiv->prec = liste->prec;
  liste->prec->suiv = it;
  liste->prec = it;
  liste->suiv->prec = liste;
  it->suiv = liste;
}

void ajouterEnQueueD(ListeD *racine, char val[7]) {
  ListeD *nouvel_element = malloc(sizeof *nouvel_element);
  if (nouvel_element != NULL) {
    strcpy(nouvel_element->val, val);

    nouvel_element->prec = racine->prec;
    nouvel_element->suiv = racine;       

    racine->prec->suiv = nouvel_element;
    racine->prec = nouvel_element;
  }
}

void ajouterEnTeteD(ListeD *racine, char val[7]) {
  ListeD *nouvel_element = malloc(sizeof *nouvel_element);
  if (nouvel_element != NULL) {
    strcpy(nouvel_element->val, val);

    nouvel_element->prec = racine;
    nouvel_element->suiv = racine->suiv;

    racine->suiv->prec = nouvel_element;
    racine->suiv = nouvel_element;
  }
}

void supprimerEnTeteD(ListeD *racine) {
  if (racine->suiv == racine) {
    return;
  }
  ListeD *it = racine->suiv;
  racine->suiv = racine->suiv->suiv;
  racine->suiv->prec = racine;
  free(it);
}

void supprimerEnQueueD(ListeD *racine) {
  if (racine->prec == racine) {
    printf("bsjcdshcsbc\n");
    return;
  }
  ListeD *it = racine->prec;
  racine->prec->prec->suiv = racine;
  racine->prec = racine->prec->prec;
  free(it);
}

void afficheD(ListeD *l) {
  ListeD *it;
  for (it = l->suiv; it != l; it = it->suiv)
    printf("-> %s ", it->val);
}
