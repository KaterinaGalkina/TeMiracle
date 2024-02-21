#include "ListesD.h"
#include "ListesS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void nouvelleFigure(char couleurs[5][2], char formes[5][2], char ch[7]);

void figureType(char *val, int *couleurInd, int *formesInd);

void suppressionOccurenceDebutListesSec(int formeCouleurSupprime, ListeD *tab[8], int taillesTabSec[8]);

void suppressionOccurenceFinListesSec(int formeCouleurSupprime, ListeD *tab[8], int taillesTabSec[8]);

void decalageMisAJourTOUT(ListeS *listePrincipale, int indiceDuTableauDecalage, ListeD *tab[8]);

int StockeIndicesASupprimerApresDecalages(ListeS *listePrincipale, int numeroElementsASupprimer[15]);

void SuppressionMiseAjourTousLesListesApresDecalages(ListeS *listePrincipale, ListeD *tab[8], int numeroElementsASupprimer[15], int tailleNumerosElementsSupprim, int *taille, int taillesTabSec[8]);

void reinitialiserListesSecondaires(ListeS *listePrincipale, int taillesTabSec[8], ListeD *tab[]);