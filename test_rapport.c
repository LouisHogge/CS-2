#include "board.h"
#include "hint.h"
#include "play.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main()
{
    int height = 8;
    int width = 8;
    int mode = 0;    //0=mode classique       | 1=mode explosif
    int max = 1000;
    int player = 1;  //1=joueur ordi
    int hint = 0;    //0=indice aléatoire     | 1=best indice
    int show = 0;    //0=plateau non-affiché  | 1=plateau affiché à chaque échange

    /*int height = 8;
    int width = 8;
    int mode = 0;    //0=mode classique       | 1=mode explosif
    int max = 1000;
    int player = 1;  //1=joueur ordi
    int hint = 1;    //0=indice aléatoire     | 1=best indice
    int show = 0;*/    //0=plateau non-affiché  | 1=plateau affiché à chaque échange

    /*int height = 8;
    int width = 8;
    int mode = 1;    //0=mode classique       | 1=mode explosif
    int max = 1000;
    int player = 1;  //1=joueur ordi
    int hint = 0;    //0=indice aléatoire     | 1=best indice
    int show = 0;*/    //0=plateau non-affiché  | 1=plateau affiché à chaque échange

    /*int height = 8;
    int width = 8;
    int mode = 1;    //0=mode classique       | 1=mode explosif
    int max = 1000;
    int player = 1;  //1=joueur ordi
    int hint = 1;    //0=indice aléatoire     | 1=best indice
    int show = 0;*/    //0=plateau non-affiché  | 1=plateau affiché à chaque échange
    
    Swap (*computeHint)(Board *) = hintBestExplosion;
    Swap (*chooseSwap)(Board *, Swap) = onlyHintPlayer;
    if (hint == 0)
      computeHint = hintRandom;

    if (player == 0)
      chooseSwap = humanPlayer;

    //tab récupère dans la case 0 le nb gemmes détruites et dans la case 1 le nb d'échanges
    int* tab=malloc(2*sizeof(int));
    int* tab1=malloc(1000*sizeof(int)); //tab pour nb gemmes détruites
    int* tab2=malloc(1000*sizeof(int)); //tab pour nb échanges
    
    //boucle de 1000 parties
    for (int i=0;i<1000;i++)
    {
        tab = game(width, height, mode, max, computeHint, chooseSwap, show);
        tab1[i]=tab[0];
        tab2[i]=tab[1];
    }
    //on affiche les tableaux obtenus
    /*fprintf (stderr,"\nTableau nb gemmes détruites :\n");
    for (int s=0;s<1000;s++)
        {
            fprintf (stderr,"%d ", tab1[s]);
        }
    fprintf (stderr,"\nTableau nb échanges :\n");
    for (int t=0;t<1000;t++)
        {
            fprintf (stderr,"%d ", tab2[t]);
        }*/
    
    
    //calcul moyenne nb gemmes détruites
    int moyenne_nb_gemmes_detruites=0;
    int ecart_type_nb_gemmes_detruites=0;
    for(int j=0;j<1000;j++)
    {
        moyenne_nb_gemmes_detruites += tab1[j];
    }
    moyenne_nb_gemmes_detruites = moyenne_nb_gemmes_detruites/1000;
    
    //calcul écart-type nb gemmes détruites
    for(int m=0;m<1000;m++)
    {
        ecart_type_nb_gemmes_detruites += pow((tab1[m]-moyenne_nb_gemmes_detruites),2);
    }
    ecart_type_nb_gemmes_detruites = sqrt(ecart_type_nb_gemmes_detruites/1000);
    
    //on affiche les valeurs
    fprintf (stderr,"\nMoyenne nb gemmes détruites : %d", moyenne_nb_gemmes_detruites);
    fprintf (stderr,"\nEcart-type nb gemmes détruites : %d\n", ecart_type_nb_gemmes_detruites);
    
    //calcul moyenne nb échanges
    int moyenne_echange=0;
    int ecart_type_echange=0;
    for(int k=0;k<1000;k++)
    {
        moyenne_echange += tab2[k];
    }
    moyenne_echange = moyenne_echange/1000;
    
    //calcul écart-type nb échanges
    for(int n=0;n<1000;n++)
    {
        ecart_type_echange += pow((tab2[n]-moyenne_echange),2);
    }
    ecart_type_echange = sqrt(ecart_type_echange/1000);
    
    //on affiche les valeurs
    fprintf (stderr,"\nMoyenne nb échange : %d", moyenne_echange);
    fprintf (stderr,"\nEcart-type nb échange : %d\n", ecart_type_echange);
    
    return 0;
}
