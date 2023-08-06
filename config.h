#include <stdbool.h>
#include <termios.h>
#include <sys/select.h>
#include <stdlib.h> /* For NULL */

/* TODO: test if their is a way to have theis macros be right next to each other
 * without a space, so "START BBLACK GREEN END" would be, \x1b[40;32m", wonder.
  */
/* ISO C99 Requires space after micro name */
#define BLACK    "\x1b[30m"
#define RED      "\x1b[31m"
#define GREEN    "\x1b[32m"
#define YELLOW   "\x1b[33m"
#define BLUE     "\x1b[34m"
#define MAGENTA  "\x1b[35m"
#define CYAN     "\x1b[36m"
#define WHITE    "\x1b[37m"

#define BBLACK   "\x1b[40m"
#define BRED     "\x1b[41m"
#define BGREEN   "\x1b[42m"
#define BYELLOW  "\x1b[43m"
#define BBLUE    "\x1b[44m"
#define BMAGENTA "\x1b[45m"
#define BCYAN    "\x1b[46m"
#define BWHITE   "\x1b[47m"

#define RESET    "\x1b[0m"


const bool debug=true;
unsigned short int level=0;
unsigned int frames=1;

unsigned int height=7;
unsigned int width=15;
unsigned short int mapx=20;
unsigned short int mapy=10;

unsigned int playerx=8;
unsigned int playery=4;
bool playermove=true;
bool playerview=true;


/* Magic !?! */
int kbhit() {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}
