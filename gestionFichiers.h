#include "ListesD.h"
#include "ListesS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int ajoutScoreClassement(FILE *fichierMeilleursScores, int score, char nomClassement[100]);

int enregistrerPartie(ListeS *listePrincipale, ListeS *figuresAPlacer, int *score, int *taille, char nomClassement[100], char tousNomsSauvegardes[100][100], int tailleNomsSauvegardes);

void chargerDonnees(ListeS *listePrincipale, ListeS *figuresAPlacer, int *score, int *taille, char nomClassement[100], char nomPartieAContinuer[100]);

int recupererNomsSauvegardes(char tousNomsSauvegardes[100][100], int *tailleNomsSauvegardes);

void mettreAJourSauvegardes(char *nomClassement);