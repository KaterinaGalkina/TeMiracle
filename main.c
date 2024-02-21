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

int main(void) {
  // commandes pour compiler et executer:
  // gcc -g -Wall main.c ListesS.c ListesD.c gestionFichiers.c commandesJeu.c affichage.c -o sortie -lncurses
  // ./sortie

  srand(time(NULL));
  initscr();

  // Definition des variables 

  //on definit les variables pour l'interface graphique
  WINDOW *pageArrivee;
  WINDOW *pageIntermadiaire;
  WINDOW *tableauDuJeu;
  WINDOW *boiteProchainesFigures;
  WINDOW *boiteScore;
  WINDOW *tableauInfoCommandes;
  WINDOW *pageFinale;
  WINDOW *pageClassement;


  // Choix de propriete d'une piece (utilise par void figureType(char *val, int *couleurInd, int *formeInd))
  char couleurs[5][2] = {"2", "3", "4", "5"};
  char figures[5][2] = {"R", "T", "C", "L"};
  char figuresAffichage[5][2] = {"r", "t", "c", "l"};

  start_color();
  // Définir les paires de couleurs pour l'affichage
  init_pair(1, COLOR_GREEN, COLOR_BLACK); 
  init_pair(2, COLOR_YELLOW, COLOR_BLACK); 
  init_pair(3, COLOR_BLUE, COLOR_BLACK); 
  init_pair(4, COLOR_MAGENTA, COLOR_BLACK);


  // Joli affichage de choix pour les decalages
  char ChoixDecalages[8][20] = {"Rectangles (R)",       "Triangles (T)",
                                "Cercles (C)",          "Losanges (L)",
                                "Vert", "Jaune",
                                "Bleu", "Rose"};

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

  // Variable pour stocker la reponse de l'utilisateur, peut etre : 'a', 'd', 'w', 's', 'c', 'q' ....
  char n[2] = "";

  // Variables pour stocker les indices du couleur et d'une forme d'une piece
  int couleurInd = 0;
  int figureInd = 0;

  // indice du critere dans tab selon laquelle la piece a ete supprimee
  int formeCouleurSupprime = 0;
  // est egale a 0 si le decalage n'est pas possible et 1 sinon
  int decalagePossible = 0;

  // reponse de l'utilisateur a s'il veut commencer une nouvelle partie 
  // ou continuer la partie enregistree
  char nvpartie[10] = "";

  // Variables qui vont stocker le choix de l'utilisateur sur le type d'une
  // piece par rapport a la quelle le decalage sera effectue, et le nombre de
  // decalages vers la gauche
  int indicePermutationType = 0;
  int nbDecalages = 0;
  
  // tableau pour conserver les decalages possibles 
  // (Pour verifier que l'utilisateur n'a choisit pas un numero hors les numero possibles) 
  int decalagesPossibles[8] = {0};

  // Tableau et variable qui va stocker la taille de ce tableau pour stocker les
  // numeros des elements a supprimer via les decalages
  int numeroElemetsASupprimer[15] = {0};
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

  // Copie des criteres de la liste principale, sera utilisee pour les annimations de suppressions
  int listePlateauCopie[15][2];

  // variable qui stocke le nombre de noms sauvegardes
  int pasDeNomssauve = recupererNomsSauvegardes(tousNomsSauvegardes, &tailleNomsSauvegardes);

  pageArrivee = subwin(stdscr, LINES -1, COLS - 1, 0, 0);

  while(1) { // on affiche la page d'arrivee
    delwin(pageArrivee);

    clear();
    pageArrivee = subwin(stdscr, LINES -1, COLS - 1, 0, 0);
    box(pageArrivee, ACS_VLINE, ACS_HLINE);
    mvwprintw(pageArrivee, (int)LINES/3 + 0, (int)COLS/2 - 40, "######  ######      #   #      ##  #####       #       #####   ##      ###### ");
    mvwprintw(pageArrivee, (int)LINES/3 + 1, (int)COLS/2 - 40, "  ##    ##         ### ###     ##  ##  ##     ###     ##   ##  ##      ##     ");
    mvwprintw(pageArrivee, (int)LINES/3 + 2, (int)COLS/2 - 40, "  ##    ######    ## ### ##    ##  #####     ## ##   ##        ##      ###### ");
    mvwprintw(pageArrivee, (int)LINES/3 + 3, (int)COLS/2 - 40, "  ##    ##       ##   #   ##   ##  ## ##    #######   ##   ##  ##      ##     ");
    mvwprintw(pageArrivee, (int)LINES/3 + 4, (int)COLS/2 - 40, "  ##    ######  ##         ##  ##  ##  ##  ##     ##   #####   ######  ###### ");
    char *msg1 = "Tapez sur ESPACE pour commencer a jouer";
    mvprintw((LINES/2) + 6, (COLS/2) - (strlen(msg1)/2), "%s", msg1);
    wrefresh(pageArrivee);
    if(getch() == ' ' ) {
        break;
    }
  }

  while(1) { // on demande si l'utilisateur veut commencer ou continuer une partie 
    clear();
    pageIntermadiaire = subwin(stdscr, LINES - 1, COLS - 1, 0, 0);
    box(pageIntermadiaire, ACS_VLINE, ACS_HLINE);
    char *msg11 = "tapez 'n' si vous souhaitez commencer une nouvelle partie et tapez 'e' si vous souhaitez continuer une partie enregistree : ";
    char *msg111 = "Attention : ne redimentionnez pas l'ecran a cette etape !";
    mvprintw(LINES - 2, (COLS/2) - (strlen(msg111)/2), "%s", msg111);
    mvprintw((LINES/3), (COLS/2) - (strlen(msg11)/2), "%s", msg11);
  
    wrefresh(pageIntermadiaire);

    getstr(nvpartie);
    while((strcmp(nvpartie, "e") != 0) && (strcmp(nvpartie, "n") != 0)) { //on verifie que l'utilisateur a bien rentre un e ou un n
      char *msg13 = "Veuillez rentrer une reponse valide : ";
      mvprintw(LINES/2, (COLS/2) - (strlen(msg13)/2), "%s", msg13);
      getstr(nvpartie);
    }

    if(strcmp(nvpartie, "n") == 0) { //l'utilisateur veut commencer une nouvelle partie 
    mvprintw(LINES - 2, (COLS/2) - (strlen(msg111)/2), "%s", msg111);
    char *msg12 = "Entrez votre pseudo (sans espaces) pour le classement : ";
    mvprintw((LINES/2), (COLS/2) - (strlen(msg12)/2), "%s", msg12);
    getstr(nomClassement);

    for (int k = 0; k < 5; k++) { // on remplit la liste des 5 prochains elements a placer
      nouvelleFigure(couleurs, figures, figureGeneree);
      ajouterEnQueueS(figuresAPlacer, figureGeneree);
      } 
    break;
    }
    if(strcmp(nvpartie, "e") == 0) { //l'utilisateur veut continuer une partie enregistree
      if (!pasDeNomssauve) {
        char *msg14 = "Aucune partie n'est disponible, une nouvelle partie a ete lancee ";
        mvprintw(LINES/2, (COLS/2) - (strlen(msg14)/2), "%s", msg14);
        mvprintw(LINES - 2, (COLS/2) - (strlen(msg111)/2), "%s", msg111);
        char *msg12 = "Entrez votre pseudo (sans espaces) pour le classement : ";
        mvprintw((LINES/2), (COLS/2) - (strlen(msg12)/2), "%s", msg12);
        
        getstr(nomClassement);

        for (int k = 0; k < 5; k++) { // on remplit la liste des 5 prochains elements a placer
          nouvelleFigure(couleurs, figures, figureGeneree);
          ajouterEnQueueS(figuresAPlacer, figureGeneree);
        }
      } else { //on affiche les parties disponibles
          clear();
          char *msg17 = "Voici les parties disponibles : ";
          mvprintw((LINES/3), (COLS/2) - (strlen(msg17)/2), "%s", msg17);
          for (int k = 0; k < tailleNomsSauvegardes; k++) {
            mvprintw((LINES/3) + k+1, (COLS/2)-(strlen(tousNomsSauvegardes[k])/2), "%s", tousNomsSauvegardes[k]);
          }
          mvprintw(LINES - 2 , (COLS/2) - (strlen(msg111)/2), "%s", msg111);
          char *msg18 = "Tapez le pseudo de la partie que vous souhatiez continuer : ";
          mvprintw((LINES/3)+ tailleNomsSauvegardes + 1, (COLS/2) - (strlen(msg18)/2), "%s", msg18);
          getstr(nomClassement);
          wrefresh(pageIntermadiaire);

          //on verifie si le nom tape est bien disponible:
          int a = 0;
          for (int k = 0; k < tailleNomsSauvegardes; k++) {
            if(!strcmp(tousNomsSauvegardes[k],nomClassement)) {
              a++;
            }
          }
          if(!a) { //le nom tape ne correspond pas aux noms proposes
            clear();
            char *msg19 = "Ce nom n'est pas disponible, une nouvelle partie a ete lancee :";
            mvprintw(LINES/3, (COLS/2) - (strlen(msg19)/2), "%s", msg19);
            mvprintw(LINES - 2, (COLS/2) - (strlen(msg111)/2), "%s", msg111);
            char *msg12 = "Entrez votre pseudo (sans espaces) pour le classement : ";
            mvprintw((LINES/2), (COLS/2) - (strlen(msg12)/2), "%s", msg12);
            getstr(nomClassement);

            for (int k = 0; k < 5; k++) { // on remplit la liste des 5 prochains elements a placer
              nouvelleFigure(couleurs, figures, figureGeneree);
              ajouterEnQueueS(figuresAPlacer, figureGeneree);
              }
            wrefresh(pageIntermadiaire);
            break;
          } else { //on charge les donnees de la partie souhaitee
              char nomPartieAContinuer[100] = ""; // variable pour stocker le nom de la partie a continuer
              strcpy(nomPartieAContinuer, nomClassement);
              chargerDonnees(listePrincipale, figuresAPlacer, &score, &taille, nomClassement, nomPartieAContinuer);
              reinitialiserListesSecondaires(listePrincipale, taillesTabSec, tab);
              break;
            }
        }

    }
    if(getch()=='q') {
      break;
    }
  }

  int cpt1 = 1;
  while (taille < 15 && cpt1) {
    tableauDuJeu = subwin(stdscr, LINES/3, (COLS/3)+(2*COLS/3), 0, 0);
    boiteProchainesFigures = subwin(stdscr, LINES/3, COLS/3, LINES/3,0);
    boiteScore = subwin(stdscr, LINES/3, COLS/3, 2*LINES/3, 0);
    tableauInfoCommandes = subwin(stdscr, 2*LINES/3, 2*COLS/3, LINES/3, COLS/3);

    while(1) {

      clear();

      delwin(tableauDuJeu);
      delwin(boiteProchainesFigures);
      delwin(boiteScore);
      delwin(tableauInfoCommandes);


      tableauDuJeu = subwin(stdscr, LINES/3, (COLS/3)+(2*COLS/3), 0, 0);
      box(tableauDuJeu, ACS_VLINE, ACS_HLINE);
      char *msg2 = "Plateau du jeu actuel";  
      mvprintw(0, (COLS/2) - (strlen(msg2)/2), "%s", msg2);
      afficheListePieces(tableauDuJeu, (COLS/2)-taille, (int)LINES/6 + 1 , figuresAffichage, listePrincipale);


      boiteProchainesFigures = subwin(stdscr, LINES/3, COLS/3, LINES/3,0);
      box(boiteProchainesFigures, ACS_VLINE, ACS_HLINE);
      char *msg4 = "Prochaines pieces";
      mvprintw(LINES/3, (COLS/6) - (strlen(msg4)/2), "%s", msg4);
      afficheListePieces(boiteProchainesFigures, (COLS/6) - 5, (LINES/2)-1, figuresAffichage, figuresAPlacer);


      boiteScore = subwin(stdscr, LINES/3, COLS/3, 2*LINES/3, 0); 
      box(boiteScore, ACS_VLINE, ACS_HLINE);
      char *msg5 = "Score";
      char scoreTmp[20] = "";
      sprintf(scoreTmp, "%d", score); //on convertit le score en chaine de caracteres pour pouvoir calculer la taille pour le centrer dans la boite score 
      mvprintw(2*LINES/3, (COLS/6) - (strlen(msg5)/2), "%s", msg5);
      mvprintw(5*LINES/6, (COLS/6) - (strlen(scoreTmp)/2), "%d", score); //positionnement de 5*LINES/6 a revoir ?

      tableauInfoCommandes = subwin(stdscr, 2*LINES/3, 2*COLS/3, LINES/3, COLS/3);
      box(tableauInfoCommandes, ACS_VLINE, ACS_HLINE);
      char *msg3 = "commandes disponibles";
      char *msg6 = "tapez 'a' pour placer une figure a gauche";
      char *msg7 = "tapez 'd' pour placer une figure a droite";
      char *msg8 = "tapez 'w' pour les decalages, possibles seulement apres l'ajout d'une figure";
      char *msg9 = "tapez 's' pour sauvegarder votre partie";
      char *msg10 = "tapez 'c' pour voir le classement (vous allez pouvoir reprendre votre partie apres)";
      char *msg11 = "tapez 'q' pour quitter la partie (votre partie sera sauvegardee automatiquement)";
      mvprintw(LINES/3, (4*COLS/6) - (strlen(msg3)/2), "%s", msg3);
      move((LINES/3)+1, (COLS/3)+2);
      printw("%s", msg6);
      move((LINES/3)+2, (COLS/3)+2);
      printw( "%s", msg7);
      move((LINES/3)+3, (COLS/3)+2);
      printw("%s", msg8);
      move((LINES/3)+4, (COLS/3)+2);
      printw("%s", msg9);
      move((LINES/3)+5, (COLS/3)+2);
      printw("%s", msg10);
      move((LINES/3)+6, (COLS/3)+2);
      printw("%s", msg11);


      wrefresh(boiteProchainesFigures);
      wrefresh(boiteScore);
      wrefresh(tableauDuJeu); 
      wrefresh(tableauInfoCommandes);

      move((LINES/3)+ 8, (COLS/3)+2);
      getstr(n);

      if (!strcmp(n,"a")) {
        char figureActuelle[7] = "";
        strcpy(figureActuelle, figuresAPlacer->suiv->val);
        figureType(figureActuelle, &couleurInd, &figureInd);

        // on reinitialise la liste des 5 prochains elements a placer
        nouvelleFigure(couleurs, figures, figureGeneree);
        supprimerEnTeteS(figuresAPlacer);

        ajouterEnQueueS(figuresAPlacer, figureGeneree);
        wrefresh(boiteProchainesFigures);
        // on ajoute la figure a gauche dans la liste principale et les listes secondaires, et on met a jour les tailles
        animationPlacementFigure(tableauDuJeu, (COLS/2) - (int)taille/2, 3, taille, figureInd, couleurInd, 0, figuresAffichage);
        ajouterEnTeteS(listePrincipale, figureActuelle);
        ajouterEnTeteD(tab[4 + couleurInd], figureActuelle);
        ajouterEnTeteD(tab[figureInd], figureActuelle);
        taille++;
        taillesTabSec[4 + couleurInd]++;
        taillesTabSec[figureInd]++;
        afficheListePieces(tableauDuJeu, (COLS/2)-taille, (int)LINES/6 + 1, figuresAffichage, listePrincipale);

        int indice = 0;
        for (ListeS* it = listePrincipale->suiv; it!= listePrincipale; it=it->suiv) {
          figureType(it->val, &couleurInd, &figureInd);
          listePlateauCopie[indice][0] = figureInd;
          listePlateauCopie[indice++][1] = couleurInd;
        }

        // Ici c'est [1,2,3] car si on supprime nous allons supprimer les 3 premieres elements du plateau
        for (int k=1; k<=3; k++) {
          numeroElemetsASupprimer[k-1]=k;
        }
        // on regarde si on peut supprimer des pieces de meme couleur ou de meme forme:
        if (supprimerOccrurencesMemeCouleurFormeEnTeteS(listePrincipale, &formeCouleurSupprime, taille)) {

          animationSuppression(tableauDuJeu, (COLS/2)-taille, (int)LINES/6 + 1, numeroElemetsASupprimer, 3, listePlateauCopie, taille, figuresAffichage);
          score += 30;
          taille -= 3;
          reinitialiserListesSecondaires(listePrincipale, taillesTabSec, tab);
          // suppressionOccurenceDebutListesSec(formColorDeleted, tab, taillesTabSec);
        }
        if (taille >= 15) {
          break;
        }
      } else if(!strcmp(n,"d")) { // on ajoute la figure a droite dans la liste principale et dans les listes secondaires, et on met a jour les tailles
        char figureActuelle[7] = "";
        strcpy(figureActuelle, figuresAPlacer->suiv->val);
        figureType(figureActuelle, &couleurInd, &figureInd);

        // on reinitialise la liste des 5 prochains elements a placer
        nouvelleFigure(couleurs, figures, figureGeneree);
        supprimerEnTeteS(figuresAPlacer);
        ajouterEnQueueS(figuresAPlacer, figureGeneree);
        wrefresh(boiteProchainesFigures);
        animationPlacementFigure(tableauDuJeu, (COLS/2), 3, taille, figureInd, couleurInd, 1, figuresAffichage);

        ajouterEnQueueS(listePrincipale, figureActuelle);
        ajouterEnQueueD(tab[4 + couleurInd], figureActuelle);
        ajouterEnQueueD(tab[figureInd], figureActuelle);

        taille++;
        taillesTabSec[4 + couleurInd]++;
        taillesTabSec[figureInd]++;
        afficheListePieces(tableauDuJeu, (COLS/2)-taille, (int)LINES/6 + 1, figuresAffichage, listePrincipale);

        int indice = 0;
        for (ListeS* it = listePrincipale->suiv; it!= listePrincipale; it=it->suiv) {
          figureType(it->val, &couleurInd, &figureInd);
          listePlateauCopie[indice][0] = figureInd;
          listePlateauCopie[indice++][1] = couleurInd;
        }

        // Ici c'est [taille - 2, taille - 1, taille] car si on supprime nous allons supprimer les 3 premieres elements du plateau
        for (int k=2; k>=0; k--) {
          numeroElemetsASupprimer[2-k]=taille - k;
        }

        if (supprimerOccrurencesMemeCouleurFormeEnQueueS(listePrincipale, &formeCouleurSupprime, taille)) {
          animationSuppression(tableauDuJeu, (COLS/2)-taille, (int)LINES/6 + 1, numeroElemetsASupprimer, 3, listePlateauCopie, taille, figuresAffichage);
          score += 30;
          taille -= 3;
          reinitialiserListesSecondaires(listePrincipale, taillesTabSec, tab);
          // suppressionOccurenceFinListesSec(formColorDeleted, tab, taillesTabSec);
        }
        if (taille >= 15) {
          break;
        }
      } else if (!strcmp(n,"s")) { // on enregistre la partie et on arrete le programme
        if (enregistrerPartie(listePrincipale, figuresAPlacer, &score, &taille, nomClassement, tousNomsSauvegardes, tailleNomsSauvegardes)) {
          clear();
          pageFinale = subwin(stdscr, LINES -1, COLS - 1, 0, 0);
          box(pageArrivee, ACS_VLINE, ACS_HLINE);
          char *msg15 = "Partie enregistree";
          mvprintw(LINES/2, (COLS/2) - (strlen(msg15)/2), "%s", msg15);
          char *msgQuit = "Pour quitter tappez 'q'";
          mvprintw(LINES/2 + 2, (COLS/2) - (strlen(msgQuit)/2), "%s", msgQuit);
          if(getch() == 'q') {
            cpt1 = 0;
            break;
          }
        } else {
          clear();
          pageFinale = subwin(stdscr, LINES -1, COLS - 1, 0, 0);
          box(pageArrivee, ACS_VLINE, ACS_HLINE);
          char *msg16 = "Erreur dans la sauvegarde de la partie";
          mvprintw(2, (COLS/2) - (strlen(msg16)/2), "%s", msg16);
          char *msgQuit = "Pour quitter tappez 'q'";
          mvprintw(LINES/2 + 2, (COLS/2) - (strlen(msgQuit)/2), "%s", msgQuit);
          if(getch() == 'q') {
            cpt1 = 0;
            break;
          }
        }
      } else if(!strcmp(n,"c")) { //on affiche le classement
      // Sauvegarde automatique
        while(1) {
          clear();
          pageClassement = subwin(stdscr, LINES -1, COLS - 1, 0, 0);
          box(pageClassement, ACS_VLINE, ACS_HLINE);
          FILE *f = fopen("donneesDuJeu/informationsGlobales/MeilleursScores.txt", "r");

          if(f!=NULL) {
            char l[100] = "";
            int cpt3 = 1;
            char *msg43 = "Classement des 10 meilleurs joueurs :";
            mvprintw(2, (COLS/2)-(int)(strlen(msg43))/2, "%s", msg43);


            while(fgets(l, 100, f) != NULL && cpt3<=10) {
              mvprintw(3 + cpt3, (COLS/2)-strlen(l), "%d/ %s",cpt3, l);
              cpt3++;
            }
            curs_set(0);
            wrefresh(pageClassement);
            fclose(f);
            char *msg44 = "Pour reprendre votre partie, tapez 'q'";
            mvprintw(cpt3 + 5, (COLS/2)-(int)(strlen(msg44))/2 , "%s", msg44);
          } else {
            mvprintw(2, (COLS/2)-(int)(strlen("Erreur d'affichage du classement"))/2, "%s", "Erreur d'affichage du classement");
          }
          if(getch() == 'q') {
            break;
          }
        } 

      afficheListePieces(tableauDuJeu, (COLS/2)-taille, 4, figuresAffichage, listePrincipale);
      mvprintw(5*LINES/6, (COLS/6) - (strlen(scoreTmp)/2), "%d", score);
      wrefresh(tableauDuJeu);
      wrefresh(boiteScore);
      } 
      if(!strcmp(n,"w")) {
        // on regarde si au moins une des listes secondaires contient minimum 2 elements
        decalagePossible = 0;
        for (int k = 0; k < 8 && (!decalagePossible); k++) {
          if (taillesTabSec[k] > 1) {
            decalagePossible = 1;
          }
        }
        if(decalagePossible == 0) {
          char *msg20 = "Vous n'avez pas assez de figures sur le plateau";
          mvprintw((LINES/3)+8, (COLS/3)+2, "%s", msg20);
          char *msg21 = "pour faire un decalage !";
          mvprintw((LINES/3)+9, (COLS/3)+2, "%s", msg21);
          char *msg22 = "Veillez poser une piece !";
          mvprintw((LINES/3)+10, (COLS/3)+2, "%s", msg22);
          wrefresh(tableauInfoCommandes);
          sleep(4);
        } else {
          indicePermutationType = 0;
          nbDecalages = 0;
          int cpt2 = 0;
          for (int k = 0; k < 8; k++) {
            decalagesPossibles[k] = 0;
            if (taillesTabSec[k] >= 2) {
              decalagesPossibles[k]=1;
              move((LINES/3)+ 8 +cpt2, (COLS/3)+2);
              printw("Si c'est %s, tapez %d\n", ChoixDecalages[k], k);
              cpt2++;
            }
          }

          char tmp3[2] = "";
          move((LINES/3)+ 8 +cpt2, (COLS/3)+2);
          cpt2++;
          getstr(tmp3);
          indicePermutationType = atoi(tmp3);

          // On verifie si la valeur entre par l'utilisateur est bien dans la liste des choix
          int mauvaisValeur = 1;
          for (int k=0; k<8 && mauvaisValeur; k++) {
            if (decalagesPossibles[k]==1 && k==indicePermutationType) {
              mauvaisValeur = 0;
            }
          }
          if (!mauvaisValeur) { // Si il n'y a pas d'erreurs, on effectue le decalage
            move((LINES/3)+ 8 +cpt2 + 2, (COLS/3)+2);
            printw("De combien de fois vers la gauche ? ");
            char tmp4[2] = "";
            getstr(tmp4);
            nbDecalages = atoi(tmp4);

            // mise a jour de la liste secondaire en fonction du nombre de decalages demandes:
            for (int k = 0; k < nbDecalages; k++) {
              decalerListeD(tab[indicePermutationType], taillesTabSec[indicePermutationType]);
            }

            // Mise a jour des 4 listes de figures (si le decalage etait selon une des 4 couleurs), ou de couleurs (si le decalage etait selon une des 4 figures)
            decalageMisAJourTOUT(listePrincipale, indicePermutationType, tab);

            // Mise a jour de la liste principale si il y a des suppressions à faire (on doit verifier la liste entiere et non juste en queue ou en tete comme au debut):
            suppressionCascade = 1; // compteur pour les suppressions en cascade
            if (taille >= 4) {
              do {
                tailleAvantMiseAJour = taille;
                tailleNumerosElementsSupprim = StockeIndicesASupprimerApresDecalages(listePrincipale, numeroElemetsASupprimer);

                if (tailleNumerosElementsSupprim) { // si il y a des elements a supprimer on effctue un animation
                  int indice = 0;
                  for (ListeS* it = listePrincipale->suiv; it!= listePrincipale; it=it->suiv) {
                    figureType(it->val, &couleurInd, &figureInd);
                    listePlateauCopie[indice][0] = figureInd;
                    listePlateauCopie[indice++][1] = couleurInd;
                  }
                  animationSuppression(tableauDuJeu, (COLS/2)-taille, (int)LINES/6 + 1, numeroElemetsASupprimer, tailleNumerosElementsSupprim, listePlateauCopie, taille, figuresAffichage);
                }

                score += tailleNumerosElementsSupprim * 15 * suppressionCascade;
                suppressionCascade++;

                SuppressionMiseAjourTousLesListesApresDecalages(listePrincipale, tab, numeroElemetsASupprimer, tailleNumerosElementsSupprim, &taille, taillesTabSec);

              } while (tailleAvantMiseAJour != taille);
            }
          } else { // Si l'utilisateur s'est trompe on lui l'indique
            mvprintw((LINES/3)+8, (COLS/3)+2, "%s", "Le numero n'est pas dans la liste des decalages possibles !");
            mvprintw((LINES/3)+9, (COLS/3)+2, "%s", "Decalage est impossible");
            // On efface les choix propposes possibles en les ecrasant par les espaces 
            for (int k=2; k<=8; k++) {
              mvprintw((LINES/3)+8 + k, (COLS/3)+2, "%s", "                                              ");
            }
            wrefresh(tableauInfoCommandes);
            sleep(4);
          }
        }
      } 
      if (!strcmp(n,"q")) {
        if (!enregistrerPartie(listePrincipale, figuresAPlacer, &score, &taille, nomClassement, tousNomsSauvegardes, tailleNomsSauvegardes)) {
          clear();
          pageFinale = subwin(stdscr, LINES -1, COLS - 1, 0, 0);
          box(pageArrivee, ACS_VLINE, ACS_HLINE);
          char *msg16 = "Erreur dans la sauvegarde de la partie";
          mvprintw(2, (COLS/2) - (strlen(msg16)/2), "%s", msg16);
        } 
        cpt1 = 0;
        break;
      }
    }
  }

  char ch;
  // S'execute seulement si nous avons perdu, et pas lorsque nous avons sauvegarde notre partie
  if (taille == 15) {
    ajoutScoreClassement(fichierMeilleursScores, score, nomClassement);
    mettreAJourSauvegardes(nomClassement);
    while(1) {
      clear();
      pageFinale = subwin(stdscr, LINES -1, COLS - 1, 0, 0);
      box(pageArrivee, ACS_VLINE, ACS_HLINE);
      char *msg32 = "La partie est terminee, voici votre score final";
      mvprintw((LINES/3), (COLS/2) - (strlen(msg32)/2), "%s : %d", msg32, score);
      char *msg33 = "Pour quitter le programme, tapez 'q'";
      mvprintw((2*LINES/3), (COLS/2) - (strlen(msg33)/2), "%s", msg33);
      mvprintw((2*LINES/3) + 3, (COLS/2) - (strlen(msg33)/2), "%s", "Pour voir le calssement des meilleurs joueurs, tapez 'c'");
      wrefresh(pageFinale);
      ch = getch();
      if (ch == 'q') {
        break;
      } else if (ch == 'c') { //on affiche le classement
        while(1) {
          ch = getch();
          if (ch == 'q') {
            break;
          }
          clear();
          pageClassement = subwin(stdscr, LINES -1, COLS - 1, 0, 0);
          box(pageClassement, ACS_VLINE, ACS_HLINE);
          FILE *f = fopen("donneesDuJeu/informationsGlobales/MeilleursScores.txt", "r");
          if(f!=NULL) {
            char l[100] = "";
            int places = 1;
            char *msg43 = "Classement des meilleurs joueurs :";
            mvprintw(2, (COLS/2)-(int)(strlen(msg43))/2, "%s", msg43);

            while(fgets(l, 100, f) != NULL) {
              mvprintw(3 + places, (COLS/2)- (int)strlen(l)/2, "%d/ %s", places, l);
              places++;
            }
            curs_set(0);
            wrefresh(pageClassement);
            fclose(f);
            char *msg44 = "Pour sortir, tapez 'q'";
            mvprintw(places + 5, (COLS/2)-(int)(strlen(msg44))/2 , "%s", msg44);
          } else {
            printf("Erreur d'ouverture du classement");
            return 1;
          }
        }
      } 
      if (ch == 'q') {
            break;
      } 
    }
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

  delwin(pageArrivee);
  delwin(pageIntermadiaire);
  delwin(tableauDuJeu);
  delwin(boiteProchainesFigures);
  delwin(boiteScore);
  delwin(tableauInfoCommandes);
  delwin(pageFinale);
  delwin(pageClassement);

  endwin();
  return 0;

}