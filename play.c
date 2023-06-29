#include "board.h"
#include "hint.h"
#include "play.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

//humanPlayer renvoie l'échange encodé par le joueur humain
Swap humanPlayer(Board *board, Swap hint)
{
    //on récupère l'échange encodé par le joueur humain
    Swap echange;
    fprintf (stderr, "\n\nEncodez l'échange :\n\n");
    fprintf (stderr, "Position 1 :\n");
    scanf ("%d", &echange.i1);
    scanf ("%d", &echange.j1);
    fprintf (stderr, "Position 2 :\n");
    scanf ("%d", &echange.i2);
    scanf ("%d", &echange.j2);
    //signal de demande de l'indice par le joueur
    if (echange.i1==-1 && echange.j1==-1 && echange.i2==-1 && echange.j2==-1)
    {
        fprintf (stderr, "\nIndice :\nPosition 1 : (%d,%d)\nPosition 2 : (%d,%d)", hint.i1, hint.j1, hint.i2, hint.j2);
        return humanPlayer(board, hint);
    }
    //vérification de la validité de l'échange
    else if (!boardSwap(board,echange,0))
    {
        fprintf(stderr, "\nEchange ne produisant aucune explosion, réesayez!\n");
        return humanPlayer(board, hint);
    }
    return echange;
}
//humanPlayer renvoie l'échange encodé par le joueur ordinateur
Swap onlyHintPlayer(Board *board, Swap hint)
{
    //on retourne l'indice
    return hint;
}

//game gère une partie complète de bejeweled
int game(int width, int height, int explosive, int nMaxSwaps,
             Swap (*computeHint)(Board *), Swap (*chooseSwap)(Board *, Swap),
             int show)
{
    //initialisation du plateau de jeu
    Board *plateau_jeu = boardInit(width, height, explosive);
    int nb_gemmes_detruites=0;
    int nb_echange=0;
    //début de la partie
    for (nb_echange=0;nb_echange<=nMaxSwaps;nb_echange++)
    {
        //si show=1, on affiche le tableau à chaque échange
         if (show)
            boardPrint(plateau_jeu);
        //on vérifie s'il reste un échange possible
        Swap indice = computeHint(plateau_jeu);
        if (indice.i1==-1 && indice.j1==-1 && indice.i2==-1 && indice.j2==-1)
        {
            boardFree(plateau_jeu);
            fprintf (stderr, "\n\nFin de la partie : plus aucun échange n'est possible.\nVous avez détruit %d gemmes en %d échanges ! Bien joué :p\n\n", nb_gemmes_detruites, nb_echange);
            return nb_gemmes_detruites;
        }
        //s'il reste un échange possible
        else
        {
            //on obtient l'échange du joueur (humain ou ordi)
            Swap echange = chooseSwap(plateau_jeu, indice);
            //on applique l'échange au plateau et calcule le nombre de gemmes détruites par le joueur
            nb_gemmes_detruites+=boardSwap(plateau_jeu,echange,1);
        }
    }
    //si nMaxSwaps est atteind, fin de la partie
    boardFree(plateau_jeu);
    nb_echange--;
    fprintf(stderr, "\n\nFin de la partie : nombre d'échanges maximal atteint.\nVous avez détruit %d gemmes en %d échanges ! Bien joué :p\n\n", nb_gemmes_detruites, nb_echange);
    return nb_gemmes_detruites;
}
