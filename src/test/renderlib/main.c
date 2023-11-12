#include "render.c"

int main(void) {

  static unsigned char buf[H*W];
  memset(buf, 255, H*W); // to white
  checker_ren(buf);
  
  return 0;
}
