#include "render.h"

static void render(const unsigned char *buf, FILE *f) {
  fprintf(f, "P5\n%d %d\n255\n", W, H);
  fwrite(buf, W, H, f);
}

void checker_ren(unsigned char *buf){

  for(int x=1; x<H-1; x++) {
    for(int y=1; y<W-1; y++) {
      if ((x%8 < 4 && y%8 < 4))
        buf[(x*W)+y]=0;
      if (x/4 == 3 && y/4 == 7) // player
        buf[(x*W)+y]=0;
    }
  }


  render(buf, stdout);
}
