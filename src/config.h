#include <stdbool.h>
#include <stdlib.h> /* For NULL */
#include "../lib/paex_sine.c"

/* TODO: test if their is a way to have theis macros be right next to each other
 * without a space, so "START BBLACK GREEN END" would be, \x1b[40;32m", I wonder.
 */

/* COLORS */
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

/* BOARDER PRINT */
#define BOARDER_CHAR "                                                                                                                            "
#define PTOP printf(BWHITE BOARDER_CHAR RESET); \
  fputs("\n", stdout);

/* UPDATE SPEEDS */
#define GAME_UPDATE_SPEED 50000
#define MENU_UPDATE_SPEED 70000

/* AUDIO */
#define SAMPLE_LENGTH 44500
#define BEEP sine(60, SAMPLE_LENGTH);
#define BEEP_SELECT sine(100, 46000);
#define BEEPL(len) sine((len)*100, SAMPLE_LENGTH);



/* DEBUG */
#define DEBUG true
#if DEBUG
#define HOUSE_DEBUG debug();
#define FUNCTION_DEBUG void debug() {                               \
        printf("\n(%d, %d)",playerx, playery);                      \
        printf("\npmove: %d, pview: %d",playermove, playerview);    \
        printf("\nxmap: %d, ymap: %d",mapx, mapy);                  \
        printf("\nframes: %d, level: %d",frames, level+1);          \
    }
#define FRAMES_ASSIGN unsigned int frames=1;
#define FRAMES_INCREMENT frames+=1;

#else

#define HOUSE_DEBUG
#define FUNCTION_DEBUG
#define FRAMES_ASSIGN
#define FRAMES_INCREMENT
#endif




unsigned short int level=0;
FRAMES_ASSIGN

const unsigned int height=7;
const unsigned int width=15;
unsigned short int mapx=20;
unsigned short int mapy=10;

unsigned short int playerx=8;
unsigned short int playery=4;
bool playermove=true;
bool playerview=true;

unsigned short int slept=0; /* How Many Deaths */


/* Magic */
int kbhit() {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}

/* https://stackoverflow.com/questions/5488377/converting-an-integer-to-binary-in-c
   More Magic */
