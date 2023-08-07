#include <unistd.h>
#include <stdio.h>


int getch()
{
    int r;
    unsigned char c;

    /* What does this do? */
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}
int main() {
  unsigned char c;
  unsigned int cr;
  while (1) {
    c = getch();
    cr = c;
    printf("%c %d", c, cr);
  }
}
