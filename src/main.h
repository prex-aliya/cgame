/** \file main.h
 ** \author prex-aliya
 ** \breif storage for macros, and arguments for main.c
 **
 ** This is a blood mess of a header file, has all includes as well. Also where
 ** the length debug macros live, designed to be removable from the code, to not
 ** impact performance at all. Usally this is where the ugly part of making
 ** things fast are, or functions I have no Idea how they work, and will learn
 ** some day, maybe.
 **/

#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h> // For NULL
#include <termios.h>
#include <unistd.h>
#include "sound.c"


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


/* LOG */
#define LOG_FILE "cgame.log"

/* BOARDER PRINT */
#define BOARDER_CHAR "                                                                                                                            "
#define PTOP printf(BWHITE BOARDER_CHAR RESET); \
    fputs("\n", stdout);

/* MENU */
#define MENU_LENGTH 4


/* DEBUG */
#define DEBUG_ENABLED true
#if DEBUG_ENABLED

unsigned int frames=1; // Init variable for frames
#define FRAMES_INCREMENT frames+=1

#else
#define FRAMES_INCREMENT
#endif



/* AUDIO */
#define NORMAL_SAMPLE_LENGTH 44500
#define HIGH_SAMPLE_LENGTH 46000
#define BEEP default_sine();
#define BEEP_SELECT sine(100, HIGH_SAMPLE_LENGTH);
#define BEEPL(len) sine((len)*100, NORMAL_SAMPLE_LENGTH);

/* UPDATE SPEEDS */
#define GAME_UPDATE_SPEED 40000
#define MENU_UPDATE_SPEED 50000

bool playermove=true;
bool playerview=true;

// TODO MOVE TO CONFIG.h
unsigned short int level=0; // No negative levels.

/* These cannot be negative, since they are in for loops. */
const unsigned int height=7; // TODO: make this be able to move
const unsigned int width=15;
unsigned short int mapx=20;
unsigned short int mapy=10;

int playerx=8;
int playery=4;


/* Magic: a replica of the kbhit function on windows */
int kbhit() {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}
