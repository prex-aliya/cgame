#include "render.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RESET    "\x1b[0;0m"

#define H 26    // Heigth
#define W 80    // Width
#define S H*W   // Size for Total Pixels


// https://stackoverflow.com/questions/9571738/picking-random-number-between-two-points-in-c
int rang_rad(int min, int max) {
  int diff = max-min;
  return (int) (((double)(diff+1)/RAND_MAX) * rand() + min);
}
static void reset(void) {
  printf(RESET "\x1b[1;1H");
}

static void render(const unsigned char *buf) { // Printing out buffer
                                               // TODO: choose methods of rendering.
  reset();
  int num = 0;
  for (int y=0; y<H; y++) {
    for (int x=0; x<W; num++, x++) {
      printf("\x1b[48;5;%dm ", buf[num]);
    }
    printf(RESET);
    for (int x=1; x<23; x++) {
            printf("%c", buf[S+(y*22)+x]);
    }
    printf(RESET "\n");
  }
  printf(RESET);
}


void border(unsigned char *buf) {
    memset(buf+S, 126, 22);             // Sets top border
    memset(buf+S+(22*(H-1)), 126, 22);  // bottom border
    buf[S+22] = 92;     // top right corner
    buf[S+(22*H)] = 47; // bottom right corner
    for (int x=S+(22*2); x<S+(22*H); x+=22)
      buf[x] = 124;     // right side border
}
static void td_mount(void) {
  printf("Mountian");
}
int td_ren(void) { // (t)op (d)own (ren)der
  static unsigned char buf[S+(22*H)];
  memset(buf, 33, S);

  

  render(buf);
  reset();
  return 0;
}
int dm_ren(void) { // (d)oo(m) (ren)der
  static unsigned char buf[S+(22*H)];
  memset(buf, 33, sizeof(buf));



  render(buf);
  reset();
  return 0;
}
int ld_ren(int length) { // (l)oa(d)ing screen (ren)derer
  static unsigned char buf[S+(22*H)];
  int shift = W*2; // Ammount status line shifts to move two line down

  length = length*10;
  for (int i=0; i < length; i++) {
    int seed = time(NULL)+i;
    srand(seed);

    for (int x=0; x<S; x++)
      buf[x] = rang_rad(1,256);
    for (int x=S+22; x<S+(22*H)+1; x++)
      buf[x] = rang_rad(32,123);

    unsigned char color;
    if (rang_rad(1,2) == 1)
      color = 255;
    else
      color = 0;
    memset(buf+shift, color, i/4);

    border(buf);

    render(buf);
    usleep(100000);
  }
}

/* vim: shiftwidth=2 tabstop=2 foldmethod=syntax
 */
