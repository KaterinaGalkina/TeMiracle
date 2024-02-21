#include "ListesD.h"
#include "ListesS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "commandesJeu.h"

// Genere une nouvelle piece de facon aleatoire et la met dans la chaine ch
void nouvelleFigure(char couleurs[5][2], char formes[5][2], char ch[7]) {
  int couleursIndice = rand() % 4;
  int formeIndice = rand() % 4;
  strcpy(ch, "\033[3");
  strcat(ch, couleurs[couleursIndice]);
  strcat(ch, "m");
  strcat(ch, formes[formeIndice]);
}

// Affecte a couleurInd et a formeInd l'indice du type de la figure passe a
// l'aide de la chaine de caractere val
void figureType(char *val, int *couleurInd, int *formeInd) {
  if (strcmp(val, "")) {
    if (val[3] == '2') {
      *couleurInd = 0;
    } else if (val[3] == '3') {
      *couleurInd = 1;
    } else if (val[3] == '4') {
      *couleurInd = 2;
    } else {
      *couleurInd = 3;
    }
    if (val[5] == 'R') {
      *formeInd = 0;
    } else if (val[5] == 'T') {
      *formeInd = 1;
    } else if (val[5] == 'C') {
      *formeInd = 2;
    } else {
      *formeInd = 3;
    }
  }
}

// Elle n'est pas utilise dans le main.c (Cf le dev.pdf Les difficultes rencontres)
// Mise a jour des listes secondaires apres une suppression de 3 elements au debut
void suppressionOccurenceDebutListesSec(int formeCouleurSupprime, ListeD *tab[8], int taillesTabSec[8]) {
  int couleurInd = 0;
  int figureInd = 0;
  for (int k = 0; k < 3; k++) {
    figureType(tab[formeCouleurSupprime]->suiv->val, &couleurInd, &figureInd);
    supprimerEnTeteD(tab[4 + couleurInd]);
    taillesTabSec[4 + couleurInd]--;
    supprimerEnTeteD(tab[figureInd]);
    taillesTabSec[figureInd]--;
  }
}

// Elle n'est pas utilise dans le main.c (Cf le dev.pdf Les difficultes rencontres)
// mise a jour des listes secondaires apres une suppression a la fin
void suppressionOccurenceFinListesSec(int formeCouleurSupprime, ListeD *tab[8], int taillesTabSec[8]) {
  int couleurInd = 0;
  int figureInd = 0;
  for (int k = 1; k <= 3; k++) {
    figureType(tab[formeCouleurSupprime]->prec->val, &couleurInd, &figureInd); // on recupere les caracteristiques de la derniere
    // piece a supprimer dans la liste secondaire
    supprimerEnQueueD(tab[4 + couleurInd]); // on supprime dernier element de la liste secondaire de la couleur de la piece a supprimer
    taillesTabSec[4 + couleurInd]--;
    supprimerEnQueueD(tab[figureInd]);
    // on supprime dernier element de la liste secondaire de la forme de la piece a supprimer
    taillesTabSec[figureInd]--;
  }
}

// Met a jour la liste principale et les 4 listes secondaires ou le changement
// est possible
void decalageMisAJourTOUT(ListeS *listePrincipale, int indiceDuTableauDecalage, ListeD *tab[8]) {
  // On recupere l'indice de la piece correspondant au critere de decalage
  int indicePremierParamatre = (indiceDuTableauDecalage >= 4)
                                ? 3
                                : 5; 

  // On recupere la liste secondaire correspondant a la piece
  // choisie pour le decalage
  ListeD *listePieceADecaler =tab[indiceDuTableauDecalage]->suiv; 

  // on parcourt la liste principale pour trouver les pieces qui doivent etre decalees
  for (ListeS *it = listePrincipale->suiv; it != listePrincipale;
       it = it->suiv) {
    if (strcmp(listePieceADecaler->val, "") &&
        it->val[indicePremierParamatre] ==
            listePieceADecaler
                ->val[indicePremierParamatre]) { 
      strcpy(it->val, listePieceADecaler->val); // on effectue le decalage
      listePieceADecaler = listePieceADecaler->suiv;
    }
  }

  // Mise a jour des listes secondaires selon le second parametre
  int indiceDeuxiemeParamtre =
      (indiceDuTableauDecalage < 4)
          ? 3
          : 5; // on recupere l'autre parametre de la piece selon laquelle on ne
               // fait pas le decalage
  int zone = (indiceDuTableauDecalage < 4)
                 ? 4
                 : 0; // on recupere la partie de la piece correspondant a
                      // indiceDeuxiemeParamtre
  // on recupere les premiers elements des listes secondaires qui doivent etre
  // modifiees:
  ListeD *secList1 = tab[zone]->suiv;
  ListeD *secList2 = tab[zone + 1]->suiv;
  ListeD *secList3 = tab[zone + 2]->suiv;
  ListeD *secList4 = tab[zone + 3]->suiv;
  for (ListeS *it = listePrincipale->suiv; it != listePrincipale;
       it = it->suiv) { // on parcourt la liste principale pour trouver les
                        // pieces qui ont ete modifiees
    if (it->val[indiceDeuxiemeParamtre] == 'R' ||
        it->val[indiceDeuxiemeParamtre] == '2') {
      strcpy(secList1->val,
             it->val); // on re-remplit la liste secondaire avec l'element
                       // correspondant dans la liste principale
      secList1 = secList1->suiv;
    }
    if (it->val[indiceDeuxiemeParamtre] == 'T' ||
        it->val[indiceDeuxiemeParamtre] == '3') {
      strcpy(secList2->val, it->val);
      secList2 = secList2->suiv;
    }
    if (it->val[indiceDeuxiemeParamtre] == 'C' ||
        it->val[indiceDeuxiemeParamtre] == '4') {
      strcpy(secList3->val, it->val);
      secList3 = secList3->suiv;
    }
    if (it->val[indiceDeuxiemeParamtre] == 'L' ||
        it->val[indiceDeuxiemeParamtre] == '5') {
      strcpy(secList4->val, it->val);
      secList4 = secList4->suiv;
    }
  }
}

// Remplit le tableau numeroElementsASupprimer[15] des numeros des elements a supprimer de
// la liste principale apres les decalages
int StockeIndicesASupprimerApresDecalages(ListeS *listePrincipale,
                                          int numeroElementsASupprimer[15]) {
  int occColor = 0;
  int occFigure = 0;
  int tailleNumerosElementsSupprim = 0;
  int indiceActuel = 0;
  int couleurIndOccurence = 0;
  int figureIndOccurence = 0;
  int colorIndActuelle = 0;
  int figureIndActuelle = 0;

  ListeS *it = listePrincipale->suiv;
  while (it != listePrincipale) { // on parcourt la liste principale pour
                                  // chercher les elements a supprimer
    if (occFigure == 0 || occColor == 0) { //
      figureType(
          it->val, &couleurIndOccurence,
          &figureIndOccurence); // on recupere les caracteristiques de la piece
      occFigure = 0;
      occColor = 0;
    }
    figureType(it->val, &colorIndActuelle, &figureIndActuelle);

    if (colorIndActuelle ==
        couleurIndOccurence) { // on est encore au premier element
      occColor++;
    } else {
      if (occColor >=
          3) { // on a trouve une suite de 3 pieces minimun de la meme couleur
        for (int k = occColor - 1; k >= 0; k--) {
          numeroElementsASupprimer[tailleNumerosElementsSupprim++] =
              indiceActuel - k; // on repertorie les indices +1 dans la liste
                               // principale des elements a supprimer
        }
        figureType(it->val, &couleurIndOccurence, &figureIndOccurence);
        occFigure = 1;
        couleurIndOccurence = (it->val[3] == '2')   ? 0
                              : (it->val[3] == '3') ? 1
                              : (it->val[3] == '4') ? 2
                                                    : 3;
        occColor = 1;
        indiceActuel++;
        it = it->suiv;
        continue;
      } // il n'y a pas assez de pieces de la meme couleur
      couleurIndOccurence = (it->val[3] == '2')   ? 0
                            : (it->val[3] == '3') ? 1
                            : (it->val[3] == '4') ? 2
                                                  : 3;
      occColor = 1;
    }
    if (figureIndActuelle ==
        figureIndOccurence) { // on recommence mais pour trouver 3 pieces de
                              // meme forme
      occFigure++;
    } else {
      if (occFigure >= 3) {
        for (int k = occFigure - 1; k >= 0; k--) {
          numeroElementsASupprimer[tailleNumerosElementsSupprim++] = indiceActuel - k;
        }
        figureType(it->val, &couleurIndOccurence, &figureIndOccurence);
        occColor = 1;
      }
      figureIndOccurence = (it->val[5] == 'R')   ? 0
                           : (it->val[5] == 'T') ? 1
                           : (it->val[5] == 'C') ? 2
                                                 : 3;

      occFigure = 1;
    }
    indiceActuel++;
    it = it->suiv;
  }
  if (occColor >= 3) {
    for (int k = occColor - 1; k >= 0; k--) {
      numeroElementsASupprimer[tailleNumerosElementsSupprim++] = indiceActuel - k;
    }
    occFigure = 0;
  }
  if (occFigure >= 3) {
    for (int k = occFigure - 1; k >= 0; k--) {
      numeroElementsASupprimer[tailleNumerosElementsSupprim++] = indiceActuel - k;
    }
    occColor = 0;
  }
  return tailleNumerosElementsSupprim;
}

// Effectue des suppression dans la liste principale et en meme temps dans les
// listes secondaires
void SuppressionMiseAjourTousLesListesApresDecalages(ListeS *listePrincipale, ListeD *tab[8], int numeroElementsASupprimer[15], int tailleNumerosElementsSupprim, int *taille, int taillesTabSec[8]) {
  int indice = 0;
  ListeS *it = listePrincipale;
  ListeD *it_sec[8] = {tab[0], tab[1], tab[2], tab[3],
                       tab[4], tab[5], tab[6], tab[7]};
  int couleurInd = 0;
  int figureInd = 0;
  
  *taille -= tailleNumerosElementsSupprim;

  // tous en meme temps pour parcourir qu'une seule fois la boucle
  for (int k = 0; k <= tailleNumerosElementsSupprim - 1; k++) {
    // On cherche selon l'indice l'element a supprimer
    while ((indice + 1) != numeroElementsASupprimer[k]) {
      figureType(it->suiv->val, &couleurInd, &figureInd);
      it_sec[4 + couleurInd] = it_sec[4 + couleurInd]->suiv;
      it_sec[figureInd] = it_sec[figureInd]->suiv;
      it = it->suiv;
      indice++;
    } // it->suiv pointe sur element a supprimer

    figureType(it->suiv->val, &couleurInd, &figureInd);
    supprimerEnTeteD(it_sec[4 + couleurInd]);
    taillesTabSec[4 + couleurInd]--;
    supprimerEnTeteD(it_sec[figureInd]);
    taillesTabSec[figureInd]--;
    supprimerEnTeteS(it);
    indice++;
  }
}

void reinitialiserListesSecondaires(ListeS *listePrincipale, int taillesTabSec[8], ListeD *tab[]) {
  for (int k = 0; k < 8; k++) { // on vide toutes les listes secondaires
    while (taillesTabSec[k] != 0) {
      supprimerEnTeteD(tab[k]);
      taillesTabSec[k]--;
    }
  }
  int indCouleurActuelle = 0;
  int indFormeActuelle = 0;
  for (ListeS *it = listePrincipale->suiv; it != listePrincipale;
       it = it->suiv) { // on les remplit de nouveau, a partir de la nouvelle
                        // liste principale, post suppressions
    figureType(it->val, &indCouleurActuelle, &indFormeActuelle);
    ajouterEnQueueD(tab[4 + indCouleurActuelle], it->val);
    ajouterEnQueueD(tab[indFormeActuelle], it->val);
    taillesTabSec[4 + indCouleurActuelle]++;
    taillesTabSec[indFormeActuelle]++;
  }
}