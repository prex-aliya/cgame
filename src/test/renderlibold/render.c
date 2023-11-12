#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "render.h"


#define RESET     "\x1b[0;0m"
#define if_panel  false  // Want to show panel, WARNING will just disable panel,
                        // memory will still be used.
#define H 7*4        // (H)eigth
#define W 7*4        // (H)eigth
//#define W 15*8       // (W)idth Divisable by 4
#define S H*W       // (S)ize for Total Pixels
#define PL 22       // (P)anel (L)ength
#define SP S+(PL*H) // (S)ize for Total Pixels + (P)annel


struct termios old_tio, new_tio;


void srt_ren(void) {  // Start Renderer

  /* Change Termios Settings/store settings. */
  tcgetattr(STDIN_FILENO, &old_tio);
  new_tio = old_tio;
  new_tio.c_lflag &= (~ICANON & ~ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

  printf(RESET "\x1b[2J"); // clear screen
}
void end_ren(void) {  // End Renderer
  tcsetattr(STDIN_FILENO, TCSANOW, &old_tio); /* Restore Former Settings */
  printf("\x1b[0m"); /* Reset Formatting */
  return;
}

// https://stackoverflow.com/questions/9571738/picking-random-number-between-two-points-in-c
int rang_rad(int min, int max) {
  int diff = max-min;
  return (int) (((double)(diff+1)/RAND_MAX) * rand() + min);
}
static void reset(void) {
  printf(RESET "\x1b[1;1H"); // move cursor
}

void border(unsigned char *buf) {
  // PL is panel length
  memset(buf+S, 126, PL);             // Sets top border
  memset(buf+S+(PL*(H-1)), 126, PL);  // bottom border
  buf[S+PL] = 92;     // top right corner
  buf[S+(PL*H)] = 47; // bottom right corner
  for (int x=S+(PL*2); x<S+(PL*H); x+=PL)
    buf[x] = 124;     // right side border
}
static void render(const unsigned char *buf) { // Printing out buffer
                                // TODO: choose methods of rendering.
  fprintf(stdout, "P5\n%d %d\n255\n", H, H);
  fwrite(buf, H, H, stdout);
//  reset();
//  int num = 0;
//  for (int y=0; y<H; y++) {
//    for (int x=0; x<W; num++, x++) {
//      printf("\x1b[48;5;%dm ", buf[num]);
//    }
//
//#if (!if_panel) // if true add code
//    printf(RESET);
//    for (int x=1; x<PL+1; x++) {
//            printf("%c", buf[S+(y*PL)+x]);
//    }
//#endif
//
//    printf(RESET "\n");
//  }
//  printf(RESET);
}

int  td_ren(void (*gameplay)(void)) { // (t)op (d)own (ren)der
  static unsigned char buf[SP];

  memset(buf, 32, S);

  render(buf);
  //gameplay();
  return 0;
}
int  dm_ren(void) { // (d)oo(m) (ren)der
  static unsigned char buf[S];
  memset(buf, 33, sizeof(buf));

  render(buf);
  //reset();
  return 0;
}
int  ld_ren(int length) { // (l)oa(d)ing screen (ren)derer
  static unsigned char buf[SP]; // Size + Panel
  int shift = W*2; // Ammount status line shifts to move two line down

  length = length*10;
  for (int i=0; i < length; i++) {
    int seed = time(NULL)+i;
    srand(seed);

    // makes main part random color
    for (int x=0; x<S; x++)
      buf[x] = rang_rad(1,256);
    // makes side bar random, without including top boarder
    for (int x=S+PL; x<S+(PL*H)+1; x++) // PL panel length
      buf[x] = rang_rad(32,123);

    // Progress bar
    unsigned char color;
    if (rang_rad(1,2) == 1)
      color = 255;
    else
      color = 0;
    memset(buf+shift, color, i);

    border(buf);

    render(buf);
    usleep(100000);
  }

  (void)fgetc(stdin);
}

/* vim: set shiftwidth=2 tabstop=2 foldmethod=syntax */
