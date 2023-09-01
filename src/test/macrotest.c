#include <stdio.h>

#define RESET    "\x1b[0m"
#define PRINT(c1) printf(c1);
#define TEST2(c1, c2) printf("\x1b[%d;%dm", c1, c2);


int main() {
    TEST2(42, 1)
    printf( "HI");
    PRINT(RESET)
}
