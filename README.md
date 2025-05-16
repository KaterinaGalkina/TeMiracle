# TeMiracle

**Please note: The game is in French.**

## About the game

TeMiracle is a game where the goal is to earn the highest possible score and beat other players’ records. The rules are simple: you place pieces in a line to form sequences of at least 3 elements of the same color or the same shape.

At the start of the game, press the space bar to begin. Take this opportunity to resize your terminal before playing. Then, choose whether to continue a saved game (press ‘e’) or start a new one (press ‘n’) and press Enter. Next, enter your nickname, and you can start playing. All possible commands are listed in the “Commands” table. After entering your desired action, press Enter to confirm.

This is an academic collaborative project made by two people in 2024.

## Rules

At each step, the game randomly generates a new piece that the player must insert onto the game board. You have two options: insert it on the left or on the right.

If at least 3 consecutive pieces share the same color or the same shape, those 3 pieces disappear from the board.

Besides inserting on the left or right, the player can perform shifts to enable removals that would otherwise be impossible. Specifically, you can shift all pieces of a certain shape or color to the left.

## Compilation

To compile and run the project with the graphical interface, you need to install the Ncurses library.

### On macOS

```bash
brew install ncurses
```

ou 

```bash
sudo port install ncurses 
```

### On Linux

```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```

### On Windows 

This is a bit more complex on Windows since Ncurses is a Linux-specific resource, but you can follow the steps shown in [this tutorial](https://www.youtube.com/watch?v=DtHO5DBZQHw) on YouTube.


Once the library is installed, open your shell and navigate to the folder containing all the project source files:

```bash
cd path/to/TeMiracle
```

## Execution

Regardless of your operating system, compile and run the program by typing these two commands in the terminal:

```bash
gcc -g -Wall main.c ListesS.c ListesD.c gestionFichiers.c commandesJeu.c affichage.c -o sortie -lncurses
```

```bash
./sortie
```

Warning!
If you cannot install the required library and want to run the game without the interface, you can use the mainSansInterface.c file.

To do so, execute these commands:

```bash
gcc -g -Wall mainSansInterface.c ListesS.c ListesD.c gestionFichiers.c commandesJeu.c -o sortie2
```

```bash
./sortie2
```

If you run the program without the graphical interface and want to see the file with the best players ranked in ascending order, navigate to the following path in the source code DonneesDuJeu/informationsGlobales. There you will find a file named MeilleursScores.txt containing this information.
