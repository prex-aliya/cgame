#include <stdio.h>

#define XEND_ROW (4*width)-1
#define YEND_COLUMN (4*width)-1

#define BWHITE   "\x1b[37;47m"
#define RESET    "\x1b[0m"
#define BOARDER_CHAR "                                                                                                                          "


unsigned int height=7;
unsigned int width=15;

int main() {
  register unsigned int x,y;

  printf(BWHITE);
  printf(BOARDER_CHAR);
  printf(RESET);
  fputs("\n", stdout);

  for (y=0; y<4*height; y++) {
    printf(BWHITE "  " RESET);
    for (x=0; x<width; x++) {
      register unsigned short int z;
      for (z=0; z<4; z++) {
        printf("##");
      }
    }
    fputs("\n", stdout);
  }
}
