#include "ListesS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ListeS *creeListeS(void) {
  ListeS *racine = malloc(sizeof *racine);
  if (racine != NULL) {
    racine->suiv = racine;
    strcpy(racine->val, "");
  }
  return racine;
}

void supprimerEnTeteS(ListeS *l) {
  if (l->suiv == l) {
    return;
  }
  ListeS *prem = l->suiv;
  l->suiv = l->suiv->suiv;
  free(prem);
}

// cette fonction renvoie 1 si il y a des pieces a supprimer et les supprime, 0
// sinon, et stocke dans typePieceSupprimee le type (forme ou couleur) des
// pieces supprimees
int supprimerOccrurencesMemeCouleurFormeEnQueueS(ListeS *l, int *typePieceSupprimee, int taille) {
  if (taille < 3) {
    *typePieceSupprimee = 0;
    return 0;
  }
  if ((taille == 3 && l->suiv->val[3] == l->suiv->suiv->val[3] &&
       l->suiv->suiv->suiv->val[3] == l->suiv->suiv->val[3]) ||
      (taille == 3 && l->suiv->val[5] == l->suiv->suiv->val[5] &&
       l->suiv->suiv->suiv->val[5] == l->suiv->suiv->val[5])) {
    int couleurInd = (l->suiv->suiv->suiv->val[3] == '2')   ? 0
                   : (l->suiv->suiv->suiv->val[3] == '3') ? 1
                   : (l->suiv->suiv->suiv->val[3] == '4') ? 2
                                                          : 3;

    int figureInd = (l->suiv->suiv->suiv->val[5] == 'R')   ? 0
                    : (l->suiv->suiv->suiv->val[5] == 'T') ? 1
                    : (l->suiv->suiv->suiv->val[5] == 'C') ? 2
                                                           : 3;

    if (l->suiv->val[5] == l->suiv->suiv->val[5] &&
        l->suiv->suiv->suiv->val[5] == l->suiv->suiv->val[5]) {
      *typePieceSupprimee = figureInd;
    } else {
      *typePieceSupprimee = 4 + couleurInd;
    }
    for (int k = 0; k < 3; k++) {
      supprimerEnTeteS(l);
    }
    return 1;
  }
  if (taille == 3) {
    *typePieceSupprimee = 0;
    return 0;
  }

  ListeS *it;
  for (it = l->suiv; it->suiv->suiv->suiv->suiv != l; it = it->suiv) {
  }
  if ((it->suiv->val[3] == it->suiv->suiv->val[3] &&
       it->suiv->suiv->val[3] == it->suiv->suiv->suiv->val[3]) ||
      (it->suiv->val[5] == it->suiv->suiv->val[5] &&
       it->suiv->suiv->val[5] == it->suiv->suiv->suiv->val[5])) {

    int couleurInd = (it->suiv->suiv->suiv->val[3] == '2')   ? 0
                   : (it->suiv->suiv->suiv->val[3] == '3') ? 1
                   : (it->suiv->suiv->suiv->val[3] == '4') ? 2
                                                           : 3;

    int figureInd = (it->suiv->suiv->suiv->val[5] == 'R')   ? 0
                    : (it->suiv->suiv->suiv->val[5] == 'T') ? 1
                    : (it->suiv->suiv->suiv->val[5] == 'C') ? 2
                                                            : 3;

    if ((it->suiv->val[3] == it->suiv->suiv->val[3] &&
         it->suiv->suiv->val[3] == it->suiv->suiv->suiv->val[3])) {
      *typePieceSupprimee = 4 + couleurInd;
    } else {
      *typePieceSupprimee = figureInd;
    }

    ListeS *ind = it->suiv;
    it->suiv = l;

    ListeS *next;
    while (ind != l) {
      next = ind->suiv;
      free(ind);
      ind = next;
    }
    return 1;
  }
  *typePieceSupprimee = 0;
  return 0;
}

int supprimerOccrurencesMemeCouleurFormeEnTeteS(ListeS *l,
                                                int *formeCouleurSupprime,
                                                int taille) {
  if (taille < 3) {
    return 0;
  }
  if ((l->suiv->val[3] == l->suiv->suiv->val[3] &&
       l->suiv->suiv->suiv->val[3] == l->suiv->suiv->val[3]) ||
      (l->suiv->val[5] == l->suiv->suiv->val[5] &&
       l->suiv->suiv->suiv->val[5] == l->suiv->suiv->val[5])) {

    int couleurInd = (l->suiv->val[3] == '2')   ? 0
                   : (l->suiv->val[3] == '3') ? 1
                   : (l->suiv->val[3] == '4') ? 2
                                              : 3;

    int figureInd = (l->suiv->val[5] == 'R')   ? 0
                    : (l->suiv->val[5] == 'T') ? 1
                    : (l->suiv->val[5] == 'C') ? 2
                                               : 3;

    if ((l->suiv->val[3] == l->suiv->suiv->val[3] &&
         l->suiv->suiv->val[3] == l->suiv->suiv->suiv->val[3])) {
      *formeCouleurSupprime = 4 + couleurInd;
    } else {
      *formeCouleurSupprime = figureInd;
    }

    supprimerEnTeteS(l);
    supprimerEnTeteS(l);
    supprimerEnTeteS(l);
    return 1;
  }
  return 0;
}

void ajouterEnQueueS(ListeS *racine, char val[7]) {
  ListeS *nouvel_element = malloc(sizeof *nouvel_element);
  if (nouvel_element != NULL) {
    strcpy(nouvel_element->val, val);

    ListeS *it;
    for (it = racine->suiv; it->suiv != racine; it = it->suiv) {
    }
    nouvel_element->suiv = racine;
    it->suiv = nouvel_element;
  }
}

void ajouterEnTeteS(ListeS *racine, char val[7]) {
  ListeS *nouvel_element = malloc(sizeof *nouvel_element);
  if (nouvel_element != NULL) {
    strcpy(nouvel_element->val, val);

    nouvel_element->suiv = racine->suiv;
    racine->suiv = nouvel_element;
  }
}

void afficheS(ListeS *l) {
  ListeS *it;
  for (it = l->suiv; it != l; it = it->suiv) {
    printf("-> %s ", it->val);
  }
}

int tailleS(ListeS *l) {
  int taille = 1;
  for (ListeS *it = l->suiv; it != l; it = it->suiv) {
    taille++;
  }
  return taille;
}