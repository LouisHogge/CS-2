#include "board.h"
#include "hint.h"
#include "play.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
  if (argc != 8) {
    printf("%s: Wrong number of arguments, expected %d\n", argv[0], 8);
    printf("Expected arguments: height width mode maxswaps player hint show\n");
    exit(-1);
  }

  srand(time(0));
  
  // srand(time(0)); uncomment is you want a different seed at each execution
  int height = (int) strtol(argv[1], NULL, 0);
  int width = (int) strtol(argv[2], NULL, 0);
  int mode = (int) strtol(argv[3], NULL, 0);			
  int max = (int) strtol(argv[4], NULL, 0);
  int player = (int) strtol(argv[5], NULL, 0);
  int hint = (int) strtol(argv[6], NULL, 0);
  int show = (int) strtol(argv[7], NULL, 0);
  Swap (*computeHint)(Board *) = hintBestExplosion;
  Swap (*chooseSwap)(Board *, Swap) = onlyHintPlayer;

  if (hint == 0)
    computeHint = hintRandom;

  if (player == 0)
    chooseSwap = humanPlayer;

  game(width,
       height,
       mode,
       max,
       computeHint,
       chooseSwap, 
       show);

  return 0;
}
