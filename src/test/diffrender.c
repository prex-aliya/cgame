#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define XEND_ROW (4*width)-1
#define YEND_COLUMN (4*width)-1

#define BWHITE   "\x1b[37;47m"
#define RESET    "\x1b[0m"
#define BOARDER_CHAR "                                                                                                                            "
#define PTOP printf(BWHITE BOARDER_CHAR RESET); \
  fputs("\n", stdout);

unsigned int height=7;
unsigned int width=15;
//double incline=-1;
double incline=-1;
double d = 1;

bool line(register int x, register int y){

  int x0 = 25;
  int y0 = 0;
  int x1 = 10;
  int y1 = 28;

  /* https://zingl.github.io/bresenham.html */
  // TODO HOW ? WORKS
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
  int err = dx+dy, e2;

  for(;;) {

    if (y0==y) {
      if (x0<x && x0%6 == d && x0< (x*-1)+60) return true;
      if (((x0==(x*-1)+65 || x0==(x*-1)+64) || (x0==x+4 || x0==x+5) || x0==(x*-1)+60 || x0==x)) return true;
    }
    if (x0==x1 && y0==y1) break;

    e2 = 2*err;
    if (e2 >= dy) {err += dy; x0 += sx; }
    if (e2 <= dx) {err += dx; y0 += sy; }
  }
 return false;
}

void render() {
  register int x,y;

  PTOP

  for (y=0; y<4*height; y++) {
    printf(BWHITE "  " RESET);
    for (x=0; x<width; x++) {
      register unsigned short int z;
      for (z=0; z<4; z++) {
        if (line((x*4)+z, y) == true) {
          printf("  ");
        } else {
          printf("##");
        }
      }
    }
    printf(BWHITE "  " RESET);
    fputs("\n", stdout);
  }
  PTOP
}

int main() {
  do {
    render();
    incline-=0.1;
    if (d<=1) d=5;
    else d-=1;

    sleep(1);
    if (d>10000) {
      return 0;
    }

  } while (1);
}
