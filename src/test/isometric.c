#include <stdio.h>


int mapsize = 50;

int main() {
  //int screenx = cartx * (tilew/2) - carty * (tilew/2);
  //int screeny = carty * (tileh/2) - cartx * (tileh/2);

  for (int x=0; x < mapsize; x++) {
    for (int y=mapsize; y >= 0; y--) { /* COUNTS BACKWARDS */

      int isox = x + y;
      int isoy = (x - y)/2;

      if (isox > 10 && isoy > 0 && isox < 20 && isoy < 10) {
        putchar('a');
      } else {
        putchar(' ');
      }

    }
    putchar('\n');
  }
}
