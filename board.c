#include "board.h"
#include "hint.h"
#include "play.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

//création de la structure board de type opaque
struct Board_t
{
    int width;
    int height;
    int mode;
    char ** couleur;
};

//prototypes de fonctions propres au fichier board.c
void boardMarquageGem(Board*board, int** marquage);
void boardDestructionGem (Board *board, int **marquage,int j);
void boardMarquageExplosif(Board *board, int**marquage, int i, int j);

//boardInit crée un plateau de jeu de hauteur "heigth", largeur "width", mode de jeu "explosive" possédant un tableau "couleur" prêt à recevoir les gemmes
Board *boardInit(int width, int height, int explosive)
{
    Board* plateau_jeu = (Board*) malloc(sizeof(Board));
    if (!plateau_jeu)
    {
        fprintf(stderr, "Unable to allocate memory\n");
        return NULL;
    }
    plateau_jeu->width = width;
    plateau_jeu->height = height;
    plateau_jeu->mode = explosive;
    plateau_jeu->couleur = malloc(height * sizeof(char*));
    if (!(plateau_jeu->couleur))
    {
        fprintf(stderr, "Unable to allocate memory\n");
        free(plateau_jeu);
        return NULL;
    }
    for(int k=0;k<height;k++)
    {
        plateau_jeu->couleur[k]=(char* )malloc(width*sizeof(char));
        if(!plateau_jeu->couleur[k])
        {
            fprintf(stderr, "Unable to allocate memory\n");
            free(plateau_jeu);
            return NULL;
        }
    }
    //on remplit le plateau de jeu de manière aléatoire
    boardFillRandom(plateau_jeu);
    return plateau_jeu;
}

//boardFillRandom remplit le plateau de jeu "board" de manière aléatoire
void boardFillRandom(Board *board)
{
    //on crée tab_gemmes comprenant les différents types de gemmes
    char tab_gemmes[] = "RGBYPOW";
    for (int i=0;i<(board->height);i++)
    {
        for (int j=0;j<(board->width);j++)
        {
            if (i<2 && j<2)
            {
                //on attribue à la case du plateau de jeu une couleur aléatoire
                int gemme_aleatoire = rand() % (7);
                board->couleur[i][j] = tab_gemmes[gemme_aleatoire];
            }
            else if (i>1 && j<2)
            {
                //si 2 gemmes précédentes en hauteur sont identiques
                if (board->couleur[i-1][j] == board->couleur[i-2][j])
                {
                    int position_exclue = 0;
                    //on note la position dans le tableau de gemmes de la couleur de la gemme exclue
                    char gemme_exclue = board->couleur[i-1][j];
                    for (int s=0;s<7;s++)
                    {
                        if (tab_gemmes[s] == gemme_exclue)
                            position_exclue = s;
                    }
                    //on crée un tableau de gemmes d'1 case en moins ne contenant pas la couleur exclue
                    char *tab_reduit;
                    tab_reduit = malloc(7*sizeof(char));
                    for (int m=0;m<7;m++)
                    {
                        if (m==6)
                            tab_reduit[m] = '\0';
                        else if (m < position_exclue)
                            tab_reduit[m] = tab_gemmes[m];
                        else
                            tab_reduit[m] = tab_gemmes[m+1];
                    }
                    //on attribue à la case du plateau de jeu une couleur aléatoire parmis celles non-exclues présentent dans tab_reduit
                    int gemme_aleatoire = rand() % (6);
                    board->couleur[i][j] = tab_reduit[gemme_aleatoire];
                    free (tab_reduit);
                }
                //si aucune gemmes exclues
                else
                {
                    //on attribue à la case du plateau de jeu une couleur aléatoire
                    int gemme_aleatoire = rand() % (7);
                    board->couleur[i][j] = tab_gemmes[gemme_aleatoire];
                }
            }
            else if (j>1 && i<2)
            {
                //si 2 gemmes précédentes en largeur sont identiques
                if (board->couleur[i][j-1] == board->couleur[i][j-2])
                {
                    int position_exclue=0;
                    //on note la position dans le tableau de gemmes de la couleur de la gemme exclue
                    char gemme_exclue = board->couleur[i][j-1];
                    for (int t=0;t<7;t++)
                    {
                        if (tab_gemmes[t] == gemme_exclue)
                            position_exclue = t;
                    }
                    //on crée un tableau de gemmes d'1 case en moins ne contenant pas la couleur exclue
                    char *tab_reduit;
                    tab_reduit = malloc(7*sizeof(char));
                        for (int m=0;m<7;m++)
                        {
                            if (m==6)
                                tab_reduit[m] = '\0';
                            else if (m < position_exclue)
                                tab_reduit[m] = tab_gemmes[m];
                            else
                                tab_reduit[m] = tab_gemmes[m+1];
                        }
                    //on attribue à la case du plateau de jeu une couleur aléatoire parmis celles non-exclues présentent dans tab_reduit
                    int gemme_aleatoire = rand() % (6);
                    board->couleur[i][j] = tab_reduit[gemme_aleatoire];
                    free (tab_reduit);
                }
                //si aucune gemmes exclues
                else
                {
                    //on attribue à la case du plateau de jeu une couleur aléatoire
                    int gemme_aleatoire = rand() % (7);
                    board->couleur[i][j] = tab_gemmes[gemme_aleatoire];
                }
            }
            else
            {
                //Si les 2 gemmes précédentes, en largeur ou en hauteur, sont identiques
                if (board->couleur[i-1][j] == board->couleur[i-2][j] || board->couleur[i][j-1] == board->couleur[i][j-2])
                {
                    //reduction permet de savoir si 1 ou 2 couleurs doivent être exclues
                    //position_exclue1 et 2 sont initialiser à -1 pour faciliter les comparaisons entre eux et  éviter de rencontrer des problèmes vis à vis d'anciennes positions
                    int reduction=0, position_exclue1=-1, position_exclue2=-1;
                    //si 2 gemmes précédentes en hauteur sont identiques
                    if (board->couleur[i-1][j] == board->couleur[i-2][j])
                    {
                        //on note la position dans le tableau de gemmes de la couleur de la gemme exclue
                        char gemme_exclue1 = board->couleur[i-1][j];
                        for (int s=0;s<7;s++)
                        {
                            if (tab_gemmes[s] == gemme_exclue1)
                                position_exclue1 = s;
                        }
                        reduction++;
                    }
                    //idem que précédemment pour la largeur
                    if (board->couleur[i][j-1] == board->couleur[i][j-2])
                    {
                        char gemme_exclue2 = board->couleur[i][j-1];
                        for (int t=0;t<7;t++)
                        {
                            if (tab_gemmes[t] == gemme_exclue2)
                                position_exclue2 = t;
                        }
                        reduction++;
                    }
                    
                    //si 1 seul allignement de 2 gemmes
                    if (reduction==1)
                    {
                        //on crée un tableau de gemmes d'1 case en moins ne contenant pas la couleur exclue
                        char *tab_reduit;
                        tab_reduit = malloc(7*sizeof(char));
                        //on vérifie si l'allignement est en hauteur ou en largeur, puis on recopie les couleurs des gemmes excepté celle exclue dans le nouveau tableau de couleurs
                        if (position_exclue1 > position_exclue2)
                        {
                            for (int m=0;m<7;m++)
                            {
                                if (m==6)
                                    tab_reduit[m] = '\0';
                                else if (m < position_exclue1)
                                    tab_reduit[m] = tab_gemmes[m];
                                else
                                    tab_reduit[m] = tab_gemmes[m+1];
                            }
                        }
                        else
                        {
                            for (int m=0;m<7;m++)
                            {
                                if (m==6)
                                    tab_reduit[m] = '\0';
                                else if (m < position_exclue2)
                                    tab_reduit[m] = tab_gemmes[m];
                                else
                                    tab_reduit[m] = tab_gemmes[m+1];
                            }
                        }
                        //on attribue à la case du plateau de jeu une couleur aléatoire parmis celles non-exclues présentent dans tab_reduit
                        int gemme_aleatoire = rand() % (6);
                        board->couleur[i][j] = tab_reduit[gemme_aleatoire];
                        free (tab_reduit);
                    }
                    //si 2 allignements de 2 gemmes
                    else
                    {
                        //on crée un tableau de gemmes de 2 case en moins ne contenant pas les couleurs exclues
                        char *tab_reduit;
                        tab_reduit = malloc(6*sizeof(char));
                        //on recopie les couleurs des gemmes excepté celles exclues dans le nouveau tableau de couleurs
                        for (int m=0;m<6;m++)
                        {
                            if (m==5)
                                tab_reduit[m] = '\0';
                            if (m < position_exclue1 && m < position_exclue2)
                                tab_reduit[m] = tab_gemmes[m];
                            else if (m >= position_exclue1 || m >= position_exclue2)
                            {
                                if (position_exclue1 < position_exclue2)
                                {
                                    if (m >= (position_exclue2)-1)
                                        tab_reduit[m] = tab_gemmes[m+2];
                                    else
                                        tab_reduit[m] = tab_gemmes[m+1];
                                }
                                else
                                {
                                    if (m >= (position_exclue1)-1)
                                        tab_reduit[m] = tab_gemmes[m+2];
                                    else
                                        tab_reduit[m] = tab_gemmes[m+1];
                                }
                            }
                        }
                        //on attribue à la case du plateau de jeu une couleur aléatoire parmis celles non-exclues présentent dans tab_reduit
                        int gemme_aleatoire = rand() % (5);
                        board->couleur[i][j] = tab_reduit[gemme_aleatoire];
                        free (tab_reduit);
                    }
                }
                //si aucune gemmes exclues
                else
                {
                    //on attribue à la case du plateau de jeu une couleur aléatoire
                    int gemme_aleatoire = rand() % (7);
                    board->couleur[i][j] = tab_gemmes[gemme_aleatoire];
                }
            }
        }
    }
}

//boardFree libère le plateau de jeu "board"
void boardFree(Board *board)
{
    for (int i=0;i<board->height;i++)
    {
        free(board->couleur[i]);
    }
    free(board);
}

//boardPrint affiche de manière textuelle le plateau de jeu "board"
void boardPrint(Board *board)
{
    fprintf (stderr, "  ");
    for (int i=0,k=0;i<(board->width);i++,k++)
    {
        if (k>9)
            k=0;
        fprintf (stderr, "%d", k);
    }
    fprintf (stderr, "\n");
    for (int j=0,k=0;j<(board->height);j++,k++)
    {
        if (k>9)
            k=0;
        fprintf (stderr, "%d ", k);
        for (int u=0;u<(board->width);u++)
        {
            char c = board->couleur[j][u];
            fprintf (stderr, "%c", c);
        }
        fprintf (stderr, "\n");
    }
    fprintf (stderr, "\n");
}

//Dans boardSwap, le booléen execute indique si l’échange doit être exécuté ou non sur le plateau.
//Si execute=0, boardSwap renvoie le nombre total de gemmes marquées dans le plateau de jeu "board" lors de l’échange "swap" était effectué
//Si execute=1, boardSwap renvoie le nombre total de gemmes détruites dans le plateau de jeu "board" lors de l’échange "swap"
int boardSwap(Board *board, Swap swap, int execute)
{
    int nb_gemmes_marquees=0;
    //vérification validité coordonnées entrées par le joueur humain
    if (swap.i1<0 || swap.j1<0 || swap.i1>(board->height)-1 || swap.j1>(board->width)-1 ||swap.i2<0 || swap.j2<0 || swap.i2>(board->height)-1 || swap.j2>(board->width)-1 || (swap.i1 == swap.i2 && swap.j1 == swap.j2) || swap.i1 > swap.i2 +1 || swap.i1 < swap.i2 -1 || swap.j1 > swap.j2 +1 || swap.j1 < swap.j2 -1 || swap.i2 > swap.i1 +1 || swap.i2 < swap.i1 -1 || swap.j2 > swap.j1 +1 || swap.j2 < swap.j1 -1 || ((swap.i1 == swap.i2 +1 || swap.i1 == swap.i2 -1 || swap.i2 == swap.i1 +1 || swap.i2 == swap.i1 -1) && (swap.j1 != swap.j2)) || ((swap.j1 == swap.j2 +1 || swap.j1 == swap.j2 -1 || swap.j2 == swap.j1 +1 || swap.j2 == swap.j1 -1) && (swap.i1 != swap.i2)))
    {
        fprintf (stderr, "\nCoordonnées entrées invalides car identiques, non-adjacentes ou hors du plateau de jeu, réessayez !\n");
        return nb_gemmes_marquees;
    }
    else
    {
        //on crée un teableau "marquage" destiné à recevoir le marquage des gemmes
        int **marquage=malloc((board->height)*sizeof(int*));
        for (int f=0;f<board->height;f++)
            marquage[f]=malloc((board->width)*sizeof(int));
        //on crée une copie de notre plateau de jeu initial, dans laquelle on va tester l'échange
        Board* board_explo = boardInit(board->width,board->height, board->mode);
        for (int i=0;i<board->height;i++)
        {
            for (int j=0;j<board->width;j++)
                      board_explo->couleur[i][j]=board->couleur[i][j];
        }
        //on effectue l'échange dans la copie
        char aux = board_explo->couleur[swap.i1][swap.j1];
        board_explo->couleur[swap.i1][swap.j1] = board_explo->couleur[swap.i2][swap.j2];
        board_explo->couleur[swap.i2][swap.j2] = aux;
        //on marque les gemmes produisant un allignement
        boardMarquageGem(board_explo,marquage);
        //on compte le nombre de gemmes marquées
        for (int a=0;a<board->height;a++)
        {
               for(int b=0;b<board->width;b++)
            {
                if (marquage[a][b])
                    nb_gemmes_marquees++;
            }
        }
        boardFree(board_explo);
        //si execute=0 on renvoie le nombre de gemmes marquées
        if (!execute)
               return nb_gemmes_marquees;
        //sinon, on effectue l'échange dans le plareau de jeu initial
        char g = board->couleur[swap.i1][swap.j1];
        board->couleur[swap.i1][swap.j1] = board->couleur[swap.i2][swap.j2];
        board->couleur[swap.i2][swap.j2] = g;
        //boucle de type do while permettant de détruire les gemmes marquées et de vérifier si de nouveaux allignements se forment
        int nb_1;
        do
        {
            nb_1=0;
            for (int j=0;j<board->width;j++)
                boardDestructionGem(board, marquage,j);
            boardMarquageGem(board, marquage);
            for(int y=0;y<board->height;y++)
            {
                for(int z=0;z<board->width;z++)
                {
                    if(marquage[y][z])
                    {
                        nb_1++;
                        nb_gemmes_marquees++;
                    }
                }
            }
        }
        while(nb_1!=0);
        for (int p=0;p<board->height;p++)
        {
            free(marquage[p]);
        }
        free(marquage);
        return nb_gemmes_marquees;
    }
} 

//boardMarquageGem marque les gemmes produisant un allignement
void boardMarquageGem(Board* board, int** marquage)
{
	for (int t=0;t<board->height;t++)
    {
    	for (int v=0;v<board->width;v++)
    		marquage[t][v]=0;
    }
    for (int i=0; i<board->height;i++)
    {
    	for (int j=0; j<board->width;j++)
    	{
			if (i<(board->height)-2 && board->couleur[i][j]==board->couleur[i+1][j] && board->couleur[i][j]==board->couleur[i+2][j])
    		{
    			marquage[i][j]=1;
    			marquage[i+1][j]=1;
    			marquage[i+2][j]=1;
    			if(board->mode)
    			{
    				boardMarquageExplosif(board,marquage,i,j);
    				boardMarquageExplosif(board,marquage,i+1,j);
    				boardMarquageExplosif(board,marquage,i+2,j);
    			}
    		}
    		if (j<(board->width)-2 && board->couleur[i][j]==board->couleur[i][j+1] && board->couleur[i][j]==board->couleur[i][j+2])
    		{
    			marquage[i][j]=1;
    			marquage[i][j+1]=1;
    			marquage[i][j+2]=1;
    			if(board->mode)
                {
					boardMarquageExplosif(board,marquage,i,j);
    				boardMarquageExplosif(board,marquage,i,j+1);
    				boardMarquageExplosif(board,marquage,i,j+2);
    			}
    		}
    	}
    }
}

//boardDestructionGem détruit les gemmes marquées
void boardDestructionGem (Board *board, int **marquage,int j)
{
	//détruit les gemmes marquées de 1 dans une colonne de board dans board et les remplace
	for (int i=(board->height)-1;i>=0;i--)
    {
        if (marquage[i][j]==1)
        {
            for (int k=i;k>0;k--)
            {
                marquage[k][j]=marquage[k-1][j];
                board->couleur[k][j]=board->couleur[k-1][j];
            }
            marquage[0][j]=0;
            char tab_gemmes[] = "RGBYPOW";
            int nb_aleatoire=rand() %(7);
            board->couleur[0][j]=tab_gemmes[nb_aleatoire];
            boardDestructionGem(board, marquage,j);
        }
    }
}

//boardMarquageExplosif est une fonction complémentaire pour le mode explosif qui marque les gemmes adjacentes de même couleurs à celles produisant un allignement
void boardMarquageExplosif(Board *board, int**marquage, int i, int j)
{
	if (i>0 && board->couleur[i][j]==board->couleur[i-1][j] && marquage[i-1][j]==0)
	{
		marquage[i-1][j]=1;
		boardMarquageExplosif(board, marquage,i-1,j);
	}
	if (i<(board->height)-1 && board->couleur[i][j]==board->couleur[i+1][j] && marquage[i+1][j]==0)
	{
		marquage[i+1][j]=1;
		boardMarquageExplosif(board, marquage,i+1,j);
	}
	if (j>0 && board->couleur[i][j]==board->couleur[i][j-1] && marquage[i][j-1]==0)
	{
		marquage[i][j-1]=1;
		boardMarquageExplosif(board, marquage,i,j-1);
	}
	if (j<(board->width)-1 && board->couleur[i][j]==board->couleur[i][j+1] && marquage[i][j+1]==0)
	{
		marquage[i][j+1]=1;
		boardMarquageExplosif(board, marquage,i,j+1);
	}
}

//assigne la couleur c à la position (i,j) dans le plateau pointé par board
void boardAssignGem(Board *board, int i, int j, char c)
{
    board->couleur[i][j] = c;
}
//renvoie la couleur de la gemme située à la position (i, j) dans le plateau pointé par board
char boardGetGem(Board *board, int i, int j)
{
    return (board->couleur[i][j]);
}
//renvoie la largeur du plateau pointé par board
int boardWidth(Board *board)
{
    return board->width;
}
//renvoie la hauteur du plateau pointé par board
int boardHeight(Board *board)
{
    return board->height;
}
