#include <stdio.h>
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
double d;

bool line(register int x, register int y){
  
  int x1 = 0;
  int y1 = 0;
  int x2 = 27;
  int y2 = 15;

  int m = 2 * (y2-y1);
  int error = m-(x2-x1);
  for (int nx = x1, ny = y1; nx <= x2; nx++ ) {

    if (nx == x && ny == y) {
      return true;
    }

    error += m;

    if (error >= 0) {
      ny++;
      error -= 2*(x2-x1);
    }
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
  while (1) {
    render();
    incline-=0.1;
    printf("\t%f", d);
    d+=1;
    if (d>10000) {
      return 1;
    }
  }
}
