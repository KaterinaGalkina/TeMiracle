#include "ListesD.h"
#include "ListesS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gestionFichiers.h"

// Ajoute le score du joueur dans le fichier des meilleurs scores si il est dans
// le classement
int ajoutScoreClassement(FILE *fichierMeilleursScores, int score, char nomClassement[100]) {
  fichierMeilleursScores = fopen("donneesDuJeu/informationsGlobales/MeilleursScores.txt", "r");
  FILE *copieFichier = fopen("donneesDuJeu/informationsGlobales/copieMS.txt", "w");

  char nomUtilisateur[100] = "";
  int scoreIt = 0;
  int nbLigne = 0;


  if (nomClassement[strlen(nomClassement) - 1] == '\n') { // on retire le retour a la ligne final (present lorsqu'on enregistre une partie qui a deja ete enregistree)
    nomClassement[strlen(nomClassement) - 1] = '\0';
  }

  int cpt = 0;

  if (fichierMeilleursScores != NULL) {

    while (fscanf(fichierMeilleursScores, "%s : %d", nomUtilisateur,
                  &scoreIt) == 2) { // on parcourt le fichier ligne par ligne
      nbLigne++;
      if (nbLigne < 11) {
        if (score > scoreIt && cpt == 0) {
          fprintf(copieFichier, "%s : %d\n", nomClassement, score);
          cpt = 1;
        }
        fprintf(copieFichier, "%s : %d\n", nomUtilisateur, scoreIt);
      }
    }
    fclose(copieFichier);
    fclose(fichierMeilleursScores);
    remove("donneesDuJeu/informationsGlobales/MeilleursScores.txt");
    rename("donneesDuJeu/informationsGlobales/copieMS.txt",
           "donneesDuJeu/informationsGlobales/MeilleursScores.txt");
    return 0;
  } else {
    printf("Impossible d'ouvrir le fichier MeilleursScores.txt");
    return 1;
  }
}

int enregistrerPartie(ListeS *listePrincipale, ListeS *figuresAPlacer, int *score, int *taille, char nomClassement[100], char tousNomsSauvegardes[100][100], int tailleNomsSauvegardes) {
  char nomFichierSauvegarde[100] = "";
  strcpy(nomFichierSauvegarde, "donneesDuJeu/joueurs/");
  if (nomClassement[strlen(nomClassement) - 1] == '\n') { // on retire le retour a la ligne final (present lorsqu'on
              // enregistre une partie qui a deja ete enregistree)
    nomClassement[strlen(nomClassement) - 1] = '\0';
  }
  strcat(nomFichierSauvegarde, nomClassement);
  strcat(nomFichierSauvegarde, ".txt");

  FILE *f = fopen(nomFichierSauvegarde, "w");
  if (f != NULL) { // on enregistre la partie actuelle si le fichier n'existe
                   // pas encore
    fprintf(f, "%s\n", nomClassement);
    fprintf(f, "%d\n", *taille);
    for (ListeS *it = listePrincipale->suiv; it != listePrincipale;
         it = it->suiv) {
      fprintf(f, "%s\n", it->val);
    }
    for (ListeS *it = figuresAPlacer->suiv; it != figuresAPlacer;
         it = it->suiv) {
      fprintf(f, "%s\n", it->val);
    }
    fprintf(f, "%d\n", *score);
  } else {
    printf("Impossible d'ouvrir le fichier de la sauvegarde\n");
    fclose(f);
    return 0;
  }
  fclose(f);

  FILE *f2 =
      fopen("donneesDuJeu/informationsGlobales/nomsSauvegardes.txt", "a");
  if (f2 != NULL) { // on ajoute le noms au fichiers des noms seulement si le
                    // nom n'y est pas deja
    int cpt = 0;
    for (int k = 0; k < tailleNomsSauvegardes;
         k++) { // on cherche si le nom est deja dans nomSauvegardes pour le
                // rajouter si besoin
      if (!strcmp(tousNomsSauvegardes[k], nomClassement)) {
        cpt++;
      }
    }
    if (!cpt) {
      fprintf(f2, "%s\n", nomClassement);
    }
    fclose(f2);
    return 1;
  } else {
    printf("Impossible d'ajouter le nom au fichiers des noms\n");
    fclose(f2);
    return 0;
  }
}

// charge les donnees de la partie sauvegardee
void chargerDonnees(ListeS *listePrincipale, ListeS *figuresAPlacer, int *score,
                    int *taille, char nomClassement[100],
                    char nomPartieAContinuer[100]) {
  char nomFichier[200] = "";
  strcat(nomFichier, "donneesDuJeu/joueurs/");
  strcat(nomFichier, nomPartieAContinuer);
  strcat(nomFichier, ".txt");
  FILE *f = fopen(nomFichier, "r");
  if (f != NULL) {
    char ligne[1000] = "";
    if (!fgets(ligne, 1000, f)) {
      return;
    }
    strcpy(nomClassement, ligne);

    if (!fgets(ligne, 1000, f)) {
      return;
    }
    *taille = atoi(ligne);

    for (int i = 0; i < *taille; i++) {
      if (!fgets(ligne, 1000, f)) {
        return;
      }
      char *a = strchr(ligne, '\n');
      *a = '\0'; // on a enleve le saut a la ligne a la fin de la chaine
      ajouterEnQueueS(listePrincipale, ligne);
    }

    for (int j = 0; j < 5; j++) {
      if (!fgets(ligne, 1000, f)) {
        return;
      }
      char *b = strchr(ligne, '\n');
      *b = '\0';
      ajouterEnQueueS(figuresAPlacer, ligne);
    }

    if (!fgets(ligne, 1000, f)) {
      return;
    }
    *score = atoi(ligne);

    fclose(f);
  } else {
    printf("impossible de lire le fichier 'Sauvegarde.txt'\n");
  }
}

// recupere la liste des parties pouvant etre continuees et renvoie le nombre de
// parties possibles
int recupererNomsSauvegardes( char tousNomsSauvegardes[100][100], int *tailleNomsSauvegardes) { // ATTENTION BESOIN DUN PUTAIN DE RETOUR A LA
                                  // LIGNE
  // DE MERDE FINAL SUR LE FICHIER nomsSauvegardes.txt

  FILE *f = fopen("donneesDuJeu/informationsGlobales/nomsSauvegardes.txt", "r");
  int cpt = 0;
  if (f != NULL) {
    char ligne[100] = "";
    while (fgets(ligne, 100, f)) {
      char *a = strchr(ligne, '\n');
      *a = '\0';
      strcpy(tousNomsSauvegardes[(*tailleNomsSauvegardes)++], ligne);
      cpt++;
    }
  } else {
    printf("Impossible de lire le fichier 'nomsSauvegardes.txt'\n");
  }
  fclose(f);
  return cpt;
}

// met a jour les sauvegardes apres avoir termine une partie sauvegardee
void mettreAJourSauvegardes(char *nomClassement) {
  FILE *f = fopen("donneesDuJeu/informationsGlobales/nomsSauvegardes.txt", "r");
  FILE *f2 = fopen("donneesDuJeu/informationsGlobales/copie.txt", "w");
  char tmp[100] = ""; // cree une chaine de caracteres temporaires pour le nom
                      // du fichier a supprimer
  char ch[100]="donneesDuJeu/joueurs/";
  strcpy(tmp, nomClassement);
  strcat(ch, tmp);
  //tmp[strlen(tmp) - 1] = '\0'; // on enleve le retour a la ligne final
  strcat(ch, ".txt");
  if (f != NULL) {
    char ligne[1000] = "";
    while (fgets(ligne, 1000, f) != NULL) {
      ligne[strlen(ligne)-1] = '\0'; //on enleve le retour a la ligne final
      if (strcmp(ligne, nomClassement)) { // si ligne et nomClassement sont differents, ligne est recopie dans le fichier
        fprintf(f2, "%s\n", ligne);
      }
    }
    fclose(f);
    fclose(f2);
    remove(ch); // on supprime le fichier de la sauvegarde
    remove("donneesDuJeu/informationsGlobales/nomsSauvegardes.txt");
    rename("donneesDuJeu/informationsGlobales/copie.txt",
           "donneesDuJeu/informationsGlobales/nomsSauvegardes.txt");
  } else {
    printf("Impossible d'ouvrir le fichier 'nomsSauvegardes.txt'\n");
  }
}