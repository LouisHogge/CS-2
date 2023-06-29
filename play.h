#ifndef play_h
#define play_h

#include "board.h"

Swap humanPlayer(Board *board, Swap hint);
Swap onlyHintPlayer(Board *board, Swap hint);

int  game(int width,
	  int height,
	  int explosive,
	  int nMaxSwaps,
	  Swap (*computeHint)(Board *),
	  Swap (*chooseSwap)(Board *, Swap), 
	  int show);

#endif
