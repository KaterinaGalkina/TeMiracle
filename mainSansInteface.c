#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ListesD.h"
#include "ListesS.h"
#include "commandesJeu.h"
#include "gestionFichiers.h"

int main(void) {
  // Commandes pour compiler et executer le meme jeu SANS INTERFACE GRAPHIQUE
  // gcc -g -Wall mainSansInteface.c ListesS.c ListesD.c gestionFichiers.c commandesJeu.c -o sortie2
  // ./sortie2

  srand(time(NULL));

  printf("\033[0;0H");
  printf("\033[2J");

  // Definition des variables 

  // Choix de propriete d'une piece (utilise par void figureType(char *val, int *couleurInd, int *formeInd))
  char couleurs[5][2] = {"2", "3", "4", "5"};
  char figures[5][2] = {"R", "T", "C", "L"};

  // Joli affichage de choix pour les decalages
  char ChoixDecalages[8][20] = {"Rectangles (R)",       "Triangles (T)",
                                "Cercles (C)",          "Losanges (L)",
                                "\033[32mVert\033[39m", "\033[33mJaune\033[39m",
                                "\033[34mBleu\033[39m", "\033[35mRose\033[39m"};

  // La nouvelle figure generee
  char figureGeneree[7] = "";

  // La liste des 5 prochains elements a placer
  ListeS *figuresAPlacer = creeListeS();

  // Plateau de jeu
  ListeS *listePrincipale = creeListeS();

  // taille du plateau de jeu
  int taille = 0;

  // Listes secondaires doublement chainees par forme et par couleur
  ListeD *Rect = creeListeD();
  ListeD *Triangle = creeListeD();
  ListeD *Cercle = creeListeD();
  ListeD *Losange = creeListeD();

  ListeD *Vert = creeListeD();
  ListeD *Jaune = creeListeD();
  ListeD *Bleu = creeListeD();
  ListeD *Rouge = creeListeD();

  // on repertorie les listes secondaires dans un tableau pour faciliter leur manipulation
  ListeD *tab[8] = {Rect, Triangle, Cercle, Losange, Vert, Jaune, Bleu, Rouge};

  // Tailles respectives des listes secondaires
  int taillesTabSec[8] = {0};

  // Variable pour stocker la reponse de l'utilisateur a, d, w...
  char n[3] = "";

  // Variables pour stocker les indices du couleur et d'une forme d'une piece
  int couleurInd = 0;
  int figureInd = 0;

  // indice du critere dans tab selon laquelle la piece a ete supprimee
  int formeCouleurSupprime = 0;

  // est egale a 0 si le decalage n'est pas possible et 1 sinon
  int decalagePossible = 0;

  // reponse de l'utilisateur a s'il veut commencer une nouvelle partie 
  // ou continuer la partie enregistree
  char num = 'n';

  // Variables qui vont stocker le choix de l'utilisateur sur le type d'une
  // piece par rapport a la quelle le decalage sera effectue, et le nombre de
  // decalages vers la gauche
  int indicePermutationType = 0;
  int nbDecalages = 0;

  // le tableau qui stoque les choix des decalages
  int numeroDecalagePossible[8] = {0};

  // Tableau et variable qui va stocker la taille de ce tableau pour stocker les
  // numeros des elements a supprimer via les decalages
  int numeroElementsASupprimer[15] = {0};
  int tailleNumerosElementsSupprim = 0;

  // Sera utilise pour comparer la taille du plateau avant les suppressions via
  // decalages et apres
  int tailleAvantMiseAJour = 0;

  // Compteur du nombre de suppression en cascade en une fois
  int suppressionCascade = 0;

  // Pour stocker le score actuel
  int score = 0;

  // nom du joueur qui va entrer son score dans le classement
  char nomClassement[100] = "";

  // ce fichier recensera les 10 meilleurs scores
  FILE *fichierMeilleursScores = NULL;

  // creation de la liste des noms des parties pouvant etre continuees
  // et sa taille 
  char tousNomsSauvegardes[100][100] = {""};
  int tailleNomsSauvegardes = 0;

  // variable qui stocke le nombre de noms sauvegardes
  int pasDeNomssauve = recupererNomsSauvegardes(tousNomsSauvegardes, &tailleNomsSauvegardes);


  printf("\033[39mTapez 'n' pour commencer une nouvelle partie et tapez 'e' pour "
         "continuer une partie sauvegardee :\n");
  if (scanf("%c", &num) != 1) {
    printf("erreur de lecture\n");
  }

  if (num == 'n') { // on commence une nouvelle partie
    printf("Rentrez votre pseudo (sans espaces) pour le classement : \n");
    if (scanf("%s", nomClassement) != 1) {
      printf("Erreur de lecture\n");
    }
    for (int k = 0; k < 5;
         k++) { // on remplit la liste des 5 prochains elements a placer
      nouvelleFigure(couleurs, figures, figureGeneree);
      ajouterEnQueueS(figuresAPlacer, figureGeneree);
    }
  } else { // on continue une partie sauvegardee
    if (!pasDeNomssauve) {
      printf("Aucune partie n'est disponible, une nouvelle partie a ete lancee "
             ":\n");
      printf("Rentrez votre pseudo (sans espaces) pour le classement : \n");
      if (scanf("%s", nomClassement) != 1) {
        printf("Erreur de lecture\n");
      }
      for (int k = 0; k < 5;
           k++) { // on remplit la liste des 5 prochains elements a placer
        nouvelleFigure(couleurs, figures, figureGeneree);
        ajouterEnQueueS(figuresAPlacer, figureGeneree);
      }
    } else {
      printf("Voici les parties enregistrees :\n");

      for (int k = 0; k < tailleNomsSauvegardes; k++) {
        printf("%s\n", tousNomsSauvegardes[k]);
      }
      printf("\nTapez le pseudo de la partie que vous souhaitez continuer : ");
      if (scanf("%s", nomClassement) != 1) {
        printf("Erreur de lecture\n");
      }
      //on verifie si le nom tape est bien disponible:
      int cpt = 0;
      for (int k = 0; k < tailleNomsSauvegardes; k++) {
        if(!strcmp(tousNomsSauvegardes[k],nomClassement)) {
          cpt++;
        }
      }
      if(!cpt) {
        printf("Ce nom n'est pas disponible, une nouvelle partie a ete lancee :\n");
        printf("Rentrez votre pseudo (sans espaces) pour le classement : \n");
        if (scanf("%s", nomClassement) != 1) {
          printf("Erreur de lecture\n");
        }
        for (int k = 0; k < 5;
             k++) { // on remplit la liste des 5 prochains elements a placer
          nouvelleFigure(couleurs, figures, figureGeneree);
          ajouterEnQueueS(figuresAPlacer, figureGeneree);
        }
      } else {
        char nomPartieAContinuer[100] = ""; // variable pour stocker le nom de la partie a continuer
        strcpy(nomPartieAContinuer, nomClassement);
        chargerDonnees(listePrincipale, figuresAPlacer, &score, &taille, nomClassement, nomPartieAContinuer);
        reinitialiserListesSecondaires(listePrincipale, taillesTabSec, tab);
        afficheS(listePrincipale);
        printf("\n\n");
      }
    }
  }
  int premierTourBoucle = 1;
  while (taille < 15) {
    if (premierTourBoucle) {
      printf("\033[39mVoici les commandes disponibles :\n");
      printf("Tapez 'a' pour placer la figure a gauche\n");
      printf("Tapez 'd' pour placer la figure a droite\n");
      printf("Tapez 'w' pour effectuer un decalage\n");
      printf("Tapez 's' pour sauvegarder votre partie\n\n");
      premierTourBoucle=0;
    }
    printf("\033[0;0H");
    printf("\033[2J");
    printf("Plateau de jeu actuel: \n");
    afficheS(listePrincipale);
    printf("\n\n");

    char figureActuelle[7] = "";
    strcpy(figureActuelle, figuresAPlacer->suiv->val);
    figureType(figureActuelle, &couleurInd, &figureInd);

    printf("\033[39mVotre score actuel : %d\n", score);
    printf("\033[39mLa prochaine figure a placer est : %s\n", figureActuelle);
    printf("\033[39mLes 4 figures a placer apres sont : ");
    for (ListeS *it = figuresAPlacer->suiv->suiv; it != figuresAPlacer;
         it = it->suiv) {
      printf("%s ", it->val);
    }
    printf("\n\033[39mSi vous voulez placer cette figure a gauche tapez 'a'\n");
    printf("Si a droite, tapez 'd'\n");

    if (scanf("%s", n) != 1) {
      printf("Erreur de lecture\n");
    }

    if (!strcmp(n, "s")) { // on enregistre la partie et on arrete le
                                     // programme
      if (enregistrerPartie(listePrincipale, figuresAPlacer, &score, &taille,
                            nomClassement, tousNomsSauvegardes,
                            tailleNomsSauvegardes)) {
        
        printf("\033[39mVotre partie a bien ete sauvegardee\n");
        printf("Pour voir le tableau des meilleurs joueurs allez dans\n");
        printf("donneesDuJeu/informationsGlobales et cliquer sur MeilleursScores\n");
        break;
      } else {
        printf("\033[39mErreur dans la sauvegarde de la partie\n");
        break;
      }
    }

    if (!strcmp(n, "a")) {
      // on reinitialise la liste des 5 prochains elements a placer
      nouvelleFigure(couleurs, figures, figureGeneree);
      supprimerEnTeteS(figuresAPlacer);
      ajouterEnQueueS(figuresAPlacer, figureGeneree);

      // on ajoute la figure a gauche dans la liste principale et les listes
      // secondaires, et on met a jour les tailles
      ajouterEnTeteS(listePrincipale, figureActuelle);
      ajouterEnTeteD(tab[4 + couleurInd], figureActuelle);
      ajouterEnTeteD(tab[figureInd], figureActuelle);
      taille++;
      taillesTabSec[4 + couleurInd]++;
      taillesTabSec[figureInd]++;

      // on regarde si on peut supprimer des pieces de meme couleur ou de meme
      // forme:
      if (supprimerOccrurencesMemeCouleurFormeEnTeteS(
              listePrincipale, &formeCouleurSupprime, taille)) {
        score += 30;
        taille -= 3;
        reinitialiserListesSecondaires(listePrincipale, taillesTabSec, tab);
        // suppressionOccurenceDebutListesSec(formeCouleurSupprime, tab,
        // taillesTabSec);
      }
    } else if (!strcmp(n, "d")) { // on ajoute la figure a droite dans la liste principale et dans
             // les listes secondaires, et on met a jour les tailles

      // on reinitialise la liste des 5 prochains elements a placer
      nouvelleFigure(couleurs, figures, figureGeneree);
      supprimerEnTeteS(figuresAPlacer);
      ajouterEnQueueS(figuresAPlacer, figureGeneree);

      ajouterEnQueueS(listePrincipale, figureActuelle);
      ajouterEnQueueD(tab[4 + couleurInd], figureActuelle);
      ajouterEnQueueD(tab[figureInd], figureActuelle);

      taille++;
      taillesTabSec[4 + couleurInd]++;
      taillesTabSec[figureInd]++;
      formeCouleurSupprime = 0;

      if (supprimerOccrurencesMemeCouleurFormeEnQueueS(listePrincipale, &formeCouleurSupprime, taille)) {
        score += 30;
        taille -= 3;
        reinitialiserListesSecondaires(listePrincipale, taillesTabSec, tab);
        // suppressionOccurenceFinListesSec(formeCouleurSupprime, tab,
        // taillesTabSec);
      }
    } else if (!strcmp(n, "w")) {
      // on regarde si au moins une des listes secondaires contient minimum 2 elements
      decalagePossible = 0;
      for (int k = 0; k < 8 && (!decalagePossible); k++) {
        if (taillesTabSec[k] > 1) {
          decalagePossible = 1;
        }
      }
      if (decalagePossible) {
        indicePermutationType = 0;
        nbDecalages = 0;
        for (int k = 0; k < 8; k++) {
          numeroDecalagePossible[k] = 0;
          if (taillesTabSec[k] >= 2) {
            numeroDecalagePossible[k] = 1;
            printf("Si c'est %s, tapez %d\n", ChoixDecalages[k], k);
          }
        }
        if (scanf("%d", &indicePermutationType) != 1) {
          printf("Erreur de lecture\n");
        }
        // on verifie si l'utilisateur ne s'est pas trompe dans la saisie du numero de decalage
        int erreurDeSaisie = 1; 
        for (int k=0; k<8; k++) {
          if (k == indicePermutationType && numeroDecalagePossible[k]) {
            erreurDeSaisie = 0;
          }
        } 
        if (!erreurDeSaisie) { // si il ne s'est pas trompe, on fait le decalage
          printf("De combien de fois vers la gauche ?\n");
          if (scanf("%d", &nbDecalages) != 1) {
            printf("Erreur de lecture\n");
          }
          // mise a jour de la liste secondaire en fonction du nombre de decalages
          // demandes:
          for (int k = 0; k < nbDecalages; k++) {
            decalerListeD(tab[indicePermutationType],
                          taillesTabSec[indicePermutationType]);
          }

          // Mise a jour des 4 listes de figures (si le decalage etait selon une
          // des 4 couleurs), ou de couleurs (si le decalage etait selon une des 4
          // figures)
          decalageMisAJourTOUT(listePrincipale, indicePermutationType, tab);

          // Mise a jour de la liste principale si il y a des suppressions à faire
          // (on doit verifier la liste entiere et non juste en queue ou en tete
          // comme au debut):

          suppressionCascade = 1; // compteur pour les suppressions en cascade
          if (taille >= 4) {
            do {
              tailleAvantMiseAJour = taille;
              tailleNumerosElementsSupprim = StockeIndicesASupprimerApresDecalages(
                  listePrincipale, numeroElementsASupprimer);

              // Si il aura des modifications nous allons afficher le tableau 
              if (tailleNumerosElementsSupprim) {
                afficheS(listePrincipale);
              }
              printf("\n\033[39m");
              score += tailleNumerosElementsSupprim * 15 * suppressionCascade;
              suppressionCascade++;

              SuppressionMiseAjourTousLesListesApresDecalages(
                  listePrincipale, tab, numeroElementsASupprimer, tailleNumerosElementsSupprim,
                  &taille, taillesTabSec);

            } while (tailleAvantMiseAJour != taille);
          }
        } else {
          printf("Le numero saisie n'est pas dans la liste des possibilites\n");
        }
      } else {
        printf("Impossible de faire le decalage\n");
        printf("Il n'y a pas assez de figures sur le plateau\n");
      }
    }
  }

  // S'execute seulement si nous avons perdu,
  // et pas lorsque nous avons sauvegarde notre partie
  if (taille == 15) {
    printf("Voici votre score final : %d\n", score);
    printf("Liste finale : \n");
    afficheS(listePrincipale);
    printf("\033[39m\n");
    ajoutScoreClassement(fichierMeilleursScores, score, nomClassement);
    mettreAJourSauvegardes(nomClassement);
    printf("Pour voir le tableau des meilleurs joueurs allez dans\n");
    printf("donneesDuJeu/informationsGlobales et cliquer sur MeilleursScores\n");
  }

  // Liberations de tous les listes pour lesquelles nous avons alloue la memoire
  ListeS *aux;
  ListeS *it = listePrincipale->suiv;
  while (it != listePrincipale) {
    aux = it->suiv;
    free(it);
    it = aux;
  }
  free(listePrincipale);

  it = figuresAPlacer->suiv;
  while (it != figuresAPlacer) {
    aux = it->suiv;
    free(it);
    it = aux;
  }
  free(figuresAPlacer);

  ListeD *it2;
  ListeD *aux2;
  for (int k = 0; k < 8; k++) {
    it2 = tab[k]->suiv;
    while (it2 != tab[k]) {
      aux2 = it2->suiv;
      free(it2);
      it2 = aux2;
    }
    free(tab[k]);
  }
  return 0;
}