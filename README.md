# TeMiracle

## Concernant le jeu

Temiracle est un jeu, dont le but est de gagner un nombre maximal de points et de battre les scores des autres joueurs. Les regles sont simples : il faut placer les figures sur une ligne pour composer des occurences d'au moins 3 elements de meme couleur ou de meme figure. 


Au debut du jeu, tapez espace pour commencer, et profitez en pour redimensionnez le terminal avant de commencer a jouer. Ensuite vous devez choisir entre continuer une partie sauvegarde (tapez 'e') ou d'en commencer une nouvelle ('n') et ensuite tapez enter. Ensuite, vous allez devoir entrez votre pseudo, apres quoi vois allez pouvoir jouer. Tous les commandes possibles sont ecrits dans un tableau "Commandes", apres avoir mis votre action souhaite, tapez enter pour valider.


## Les regles

A chaque etape, le jeu genere aleatoirement une nouvelle piece, que le joueur devra inserer sur le plateau de jeu. Il a deux possibilites : soit a gauche, soit a droite.

Si au moins 3 pieces consecutives ont la meme couleur ou la meme forme, alors les 3 pieces disparaissent du plateau de jeu.

En plus des insertions a gauche ou a droite, le joueur peut choisisr d'effectuer des decalages dans le but d'effectuer des suppressions non atteignables autrement. Ainsi on propose la possibilite de faire un decalage a gauche de toutes les pieces ayant une certaine forme, ou une certaine couleur.


## Compilation 

Pour pouvoir compiler et executer le projet avec l'inteface graphique vous avez besoin d'installer la bibliotheque Ncurses.

### Sur MacOs

```bash
brew install ncurses
```

ou 

```bash
sudo port install ncurses 
```

### Sur Linux

```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```

### Sur Windows 

Ceci est un peu plus complexe sur ce systeme d'exploitation, comme c'est une resource propre a Linux, mais vous pouvez le faire en suivant les etapes qui sont indiques dans [ce lien](https://www.youtube.com/watch?v=DtHO5DBZQHw) par exemple (C'est un tutoriel sur Youtube).


Ensuite, des que vous avez installe la bibliotheque, vous devez ouvrir le shell et vous placer dans le dossier qui contient tous les fichiers sources du projet. 

```bash
cd cheminVersDossier/TeMiracle
```

## Execution

Peu importe le systeme d'exploitation, compiler et executer en tappant les deux commandes suivantes dans le terminal :  

```bash
gcc -g -Wall main.c ListesS.c ListesD.c gestionFichiers.c commandesJeu.c affichage.c -o sortie -lncurses
```
```bash
./sortie
```

### Attention !

Si vous ne pouvez pas installer la bibliotheque necessaire et vous voulez executer le jeu sans interafece,
vous pouvez le faire avec le fichier mainSansInterface.c.

Pour le faire il faut  executer les deux commandes suivantes: 

```bash
gcc -g -Wall mainSansInteface.c ListesS.c ListesD.c gestionFichiers.c commandesJeu.c -o sortie2
```
```bash
./sortie2
```

Si vous executez le programme sans interface graphique et vous voulez voir le fichier avec les meilleurs joueurs
classes par ordre croissant, vous pouvez suivre ce chemin dans le code source DonneesDuJeu/informationsGlobales et
retrouvez la-dedans un fichier sous le nom de MeilleursScores.txt qui va contenir ces informations.
