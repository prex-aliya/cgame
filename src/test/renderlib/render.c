#include "render.h"
#include <stdio.h>
#include <string.h>

#define S 21 // 7 * 3  tobe odd


static void render(const unsigned char *buf) {
  for (int i=0; i<S; i++) {
    for (int i=0; i<S; i++) {
      printf("%c", buf[i]);
      printf("%c", buf[i]);
      printf("%c", buf[i]);
      printf("%c", buf[i]);
    }
    printf("\n");
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

int dm_ren() { // (d)oo(m) (ren)der
  static unsigned char buf[S*S];
  memset(buf, 65, sizeof(buf));

  for (int i=0; i<S*S; i+=2)
    buf[i] = 67;


  render(buf);
  return 0;
}
