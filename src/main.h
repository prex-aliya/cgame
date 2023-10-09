/** \file main.h
 ** \author prex-aliya
 ** \breif storage for macros, and arguments for main.c
 **
 ** This is a blood mess of a header file, has all includes as well. Also where
 ** the lengthy debug macros live, designed to be removable from the code, to not
 ** impact performance at all. Usally this is where the ugly part of making
 ** things fast are, or functions I have no Idea how they work, and will learn
 ** some day, maybe, live.
 **/

#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> // For NULL
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include "config.h"
#include "sound.c"

/* Flags */
#define playermove_t
#define playermove_f

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


/* DEBUG */
#if DEBUG_ENABLED

unsigned int frames=1; // Init variable for frames
#define FRAMES_INCREMENT frames+=1

#else
#define FRAMES_INCREMENT
#endif


/* AUDIO */
#define BEEP default_sine();
#define BEEP_SELECT sine(100, HIGH_SAMPLE_LENGTH);
#define BEEPL(len) sine((len)*100, NORMAL_SAMPLE_LENGTH);

/* UPDATE SPEEDS */
#define GAME_UPDATE_SPEED 40000
#define MENU_UPDATE_SPEED 50000

bool playermove=true;
bool playerview=true;
bool drove=true;

unsigned int flags = 1;
//0b00000001;
//  ^     ^^
//  |     |`player move
//  |     `player view
//  `did player drive

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
