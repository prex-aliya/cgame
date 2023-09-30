#include "render.h"
#include <stdio.h>
#include <string.h>

#define H 7
#define W 15
#define S H*W


static void render(const unsigned char *buf) { // Printing out buffer
  // TODO: choose methods of rendering.
  for (int y=0; y<H; y++) {
    for (int n=0; n<3; n++) { // Replicating x 3 times down
      for (int x=0; x<W; x++) {
        printf("%c", buf[x+(W*y)]);
        printf("%c", buf[x+(W*y)]);
        printf("%c", buf[x+(W*y)]);
        printf("%c", buf[x+(W*y)]);
      }
      printf("\n");
    }
  }
}


static void td_mount() {
  printf("Mountian");
}
int td_ren() { // (t)op (d)own (ren)der
  printf("I am top down render");
  td_mount();

  return 0;
}

static void dm_building(unsigned int num, unsigned char *buf) {
  /* There are for loops that take a number and changes the buf[] to a specified
   * number, */
  if (num < 7) {
    for (int i=num+(num*W); i<S; i+=W)
      buf[i] = 35;

    /* This takes num and adds num multiplied by W, or width of screen, num is
     * used to shift over to desired column, and (num*W), is used to shift down
     * by how much we are from starting edge.
     */
  } else if (num == 7)
    for (int i=num; i<S; i+=W)
      buf[i] = 35;
  else {
    for (int i=num+(W*(14-num)); i<S; i+=W)
      buf[i] = 35;

    /* This is for the flip side of the buildings.
     * Takes the number, shift from 0 to column wanted, then (W*(14-num)) takes
     * num and finds the difference to find how far it is from the edge, then
     * multiply it by W, width of screen, to shift entire building down, by
     * specified number
     */
  }
}
int dm_ren() { // (d)oo(m) (ren)der
  static unsigned char buf[S];
  memset(buf, 32, sizeof(buf));

  //for (int i=0; i<S; i+=2)
  //  buf[i] = 67;

  //for (int i=0; i<S; i+=W) // First Closest Building
    //buf[i] = 35;

  dm_building(0, buf);
  dm_building(1, buf);
  dm_building(4, buf);
  dm_building(5, buf);
  dm_building(8, buf);
  dm_building(9, buf);
  dm_building(12, buf);
  dm_building(13, buf);
  dm_building(14, buf);

  render(buf);
  return 0;
}
