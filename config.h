#include <stdbool.h>
#include <termios.h>
#include <sys/select.h>
#include <stdlib.h> /* For NULL */

const bool debug=true;
unsigned int level=0;
unsigned short int frames=1;

unsigned int height=7;
unsigned int width=15;
unsigned short int mapx=20;
unsigned short int mapy=10;

unsigned int playerx=8;
unsigned int playery=4;
bool playermove=true;
bool playerview=true;


int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}
