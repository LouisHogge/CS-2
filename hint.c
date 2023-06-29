#include "board.h"
#include "hint.h"
#include "play.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

//hintRandom cherche un allignement de manière aléatoire
Swap hintRandom(Board *board)
{
    int width = boardWidth (board);
    int height = boardHeight (board);
    //on initialise longueur à 1
    int longueur = 1;
    //on crée un tableau tab1 de taille longueur
    Swap *tab;
    tab = malloc(longueur*sizeof(Swap));
    for (int i=0;i<height;i++)
    {
        for (int j=0;j<width;j++)
        {
            if (i<height-1)
            {
                //on crée un échange et vérifie s'il produit des explosions
                Swap echange_V;
                echange_V.i1=i;
                echange_V.j1=j;
                echange_V.i2=i+1;
                echange_V.j2=j;
                int nb_explo_echange_V = boardSwap(board, echange_V, 0);
                //si l'échange produit des explosions
                if (nb_explo_echange_V)
                {
                    //si tab1 est de longueur 1
                    if (longueur == 1)
                    {
                        //tab1 = malloc(longueur*sizeof(Swap));
                        tab[0].i1 = i;
                        tab[0].j1 = j;
                        tab[0].i2 = i+1;
                        tab[0].j2 = j;
                        longueur ++;
                    }
                    else
                    {
                        //on ajoute une case au tableau tab
                        tab = (Swap*) realloc(tab,longueur*sizeof(Swap));
                        //on ajoute le nouvel échange à la dernière case de tab
                        tab[longueur-1].i1 = i;
                        tab[longueur-1].j1 = j;
                        tab[longueur-1].i2 = i+1;
                        tab[longueur-1].j2 = j;
                        //on incrémente longueur
                        longueur ++;
                    }
                }
            }
            if (j<width-1)
            {
                //on crée un échange et vérifie s'il produit des explosions
                Swap echange_H;
                echange_H.i1=i;
                echange_H.j1=j;
                echange_H.i2=i;
                echange_H.j2=j+1;
                int nb_explo_echange_H = boardSwap(board, echange_H, 0);
                //si l'échange produit des explosions
                if (nb_explo_echange_H)
                {
                    //si tab1 est de longueur 1
                    if (longueur == 1)
                    {
                        //tab1 = malloc(longueur*sizeof(Swap));
                        tab[0].i1 = i;
                        tab[0].j1 = j;
                        tab[0].i2 = i;
                        tab[0].j2 = j+1;
                        longueur ++;
                    }
                    else
                    {
                        //on ajoute une case au tableau tab
                        tab = (Swap*) realloc(tab,longueur*sizeof(Swap));
                        //on ajoute le nouvel échange à la dernière case de tab
                        tab[longueur-1].i1 = i;
                        tab[longueur-1].j1 = j;
                        tab[longueur-1].i2 = i;
                        tab[longueur-1].j2 = j+1;
                        //on incrémente longueur
                        longueur ++;
                    }
                }
            }
        }
    }
    Swap random_explosion;
    //si plus d'échange produisant un allignement
    if (longueur == 1)
    {
        random_explosion.i1=-1;
        random_explosion.j1=-1;
        random_explosion.i2=-1;
        random_explosion.j2=-1;
    }
    //sinon on renvoie un échange choisi de manière aléatoire parmis ceux produisant un allignement
    else
    {
        int nb_aleatoire = rand() % (longueur-1);
        random_explosion = tab[nb_aleatoire];
    }
    free (tab);
    return random_explosion;
}

//hintBestExplosion cherche l'allignement produisant le plus de destructions
Swap hintBestExplosion(Board *board)
{
    int width = boardWidth (board);
    int height = boardHeight (board);
    Swap best_explosion;
    int nb_explo_best_explosion=0;
    for (int i=0;i<height;i++)
    {
        for (int j=0;j<width;j++)
        {
            //test des échanges verticaux
            if (i<height-1)
            {
                Swap echange_V;
                echange_V.i1=i;
                echange_V.j1=j;
                echange_V.i2=i+1;
                echange_V.j2=j;
                int nb_explo_echange_V = boardSwap(board, echange_V, 0);
                if (nb_explo_echange_V > nb_explo_best_explosion)
                {
                    best_explosion = echange_V;
                    nb_explo_best_explosion = nb_explo_echange_V;
                }
            }
            //test des échanges horizontaux
            if (j<width-1)
            {
                Swap echange_H;
                echange_H.i1=i;
                echange_H.j1=j;
                echange_H.i2=i;
                echange_H.j2=j+1;
                int nb_explo_echange_H = boardSwap(board, echange_H, 0);
                if (nb_explo_echange_H > nb_explo_best_explosion)
                {
                    best_explosion = echange_H;
                    nb_explo_best_explosion = nb_explo_echange_H;
                }
            }
        }
    }
    //si plus d'échange produisant un allignement
    if (nb_explo_best_explosion == 0)
    {
        best_explosion.i1=-1;
        best_explosion.j1=-1;
        best_explosion.i2=-1;
        best_explosion.j2=-1;
    }
    return best_explosion;
}
