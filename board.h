#ifndef board_h
#define board_h

typedef struct Board_t Board;

typedef struct {int i1, j1, i2, j2;} Swap;

Board *boardInit(int width, int height, int explosive);
void   boardFillRandom(Board *board);
void   boardFree(Board *board);
void   boardPrint(Board *board);
int    boardSwap(Board *board, Swap swap, int execute);
void   boardAssignGem(Board  *board, int i, int j, char c);
char   boardGetGem(Board *board, int i, int j);
int    boardWidth(Board *board);
int    boardHeight(Board *board);

#endif
