#include <stdio.h>

#define BBLACK   "\x1b[40m"
#define BWHITE   "\x1b[47m"
#define RESET    "\x1b[0m"

unsigned int_to_binary(unsigned k) {
  printf("%d", k);
    if (k == 0) {
      return 0;
    }
    if (k == 1) {
      return 1;                       /* optional */
      }
    return (k % 2) + 10 * int_to_binary(k / 2);
}

int main() {
  printf("%d", int_to_binary(12));
}
