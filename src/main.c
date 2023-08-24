/** @file paex_sine.c
    @brief A game to play around with rendering types in c.
    @author prex-aliya
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "config.h"

/* NOTE: recursave functions are not part of the standard c language */
struct termios old_tio, new_tio;


int sig_caught=0;
void signal_handler(int sig) {if (sig == SIGINT) {sig_caught=1;}}
void finish() {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio); /* restore former settings */
    exit(0);
}

int getinput() {
    /* NOTE:
     * 1 = up
     * 2 = down
     * 3 = right
     * 4 = left
       */
    unsigned short int value;
    char input = getchar();
    if (input == 27) {
        getchar(); /* Skip the ^[ for arrow keys */
         return getchar()-64;
    } else if (input == 119 || input == 107) {
         return 1;
    } else if (input == 114 || input == 106) {
         return 2;
    } else if (input == 115 || input == 108 ) {
         return 3;
    } else if (input == 97 || input == 104 ) {
         return 4;
    } else if (input == 113) {
         return 0;
    } else {
         return 5;
    }

    return 0;
}
FUNCTION_DEBUG

void td_lvl_ren(int x, int y) {

#define TD_LVL_VAR short int outy = (y/4)+(playery-7); \
    short int outx = (x/4)+(playerx-7);
#define TOFOUR(c) c c c c

    if (level == 0) {
        TD_LVL_VAR
        if (outy == 0 && outx <= 0) {
            printf(RESET YELLOW BYELLOW "        " RESET);
        } else if (outx > 16 && outy >= -1) {
            printf(RESET BGREEN YELLOW TOFOUR(".*") RESET);
        } else {
            printf(RESET GREEN TOFOUR("~~") RESET);
        }
    } else if (level == 1) {
        TD_LVL_VAR
        if (outy <= -1 && outy >= -3) {
            printf(RESET "\x1b[38;5;28m" TOFOUR("~~") RESET);
        } else if ( outy == 1) {
            printf(BYELLOW TOFOUR("  "));
        } else if ( outx == 19 && !(outy <= 0)) {
            printf(BYELLOW TOFOUR("  ") RESET);
        } else {
            printf(BGREEN YELLOW TOFOUR("/*") RESET);
        }
    } else {
            printf(RESET "\x1b[38;5;28m" TOFOUR("~~") RESET);
    }
}
void td_ren(unsigned int x, unsigned int y, unsigned short int map[mapy][mapx]) {
    if (x == 7 && y/4 == 3 && playerview == true) { /* render player */
        if (y-((y/4)*4) < 2) { printf(BBLACK "        " RESET);
        } else { printf(BRED "        " RESET); }
    } else if (x+(playerx-7) > mapx-1 || (y/4)+(playery-4) > mapy-1) {
        /* Rendrers Out Mountians Per Level */
        // TODO: split to seperate functions
        td_lvl_ren(x, y);
    } else {

        register int z;
        for (z=0; z<4; z++) {
#define ERROR_TD_REN_TILES 7-1
#define TOP_TD_REN_TILES ERROR_TD_REN_TILES-1
            // For constant look up time; sacrificing storage for speed
            char tiles[7][12] = {
                { BGREEN "  " },
                { BYELLOW "  " },
                { GREEN "//" },
                { BBLUE "  " },
                { BBLACK "||" },
                { BRED "!!" },
                { BBLACK "XX" }
            };
            /*
             * This is for computing the value of map at the current rendering
             * place, then used in a macro twice -in a macro to avoid over
             * complicating things when reading-, first to see if its withing
             * the correct range of values, if not error, if, then equal that
             * value.
             */
#define TILE_LOC (map_value <= TOP_TD_REN_TILES) ? map_value : \
        ERROR_TD_REN_TILES

            unsigned short int map_value = map[(y/4)+(playery-4)][(x+(z/4))+(playerx-7)];
            printf(RESET "%s" RESET, tiles[TILE_LOC]);
        }
    }
}
void render(unsigned short int map[mapy][mapx]) {
    register unsigned int x,y;

    PTOP

    for (y=0; y<4*height; y++) {
        printf(BWHITE "  " RESET); /* PRINT white boarder */
        for (x=0; x<width; x++) {

            td_ren(x,y,map);

        }
        printf(BWHITE "  \n" RESET);
    }


    PTOP
    printf(RESET "\n");

    if (level == 0) { printf(RESET "GO TO CAR"); /* Level 1 Objective */
    } else if (level == 1) {printf(RESET "GO TO THE TOWER"); } /* Level 2 Objective */

    HOUSE_DEBUG

    fputs("\n", stdout);
}

void printmenu(unsigned short int select) {
    //register unsigned int i,e;
    fputs("\033c\n\n", stdout); /* Clear Screen */

    //for (e=1; e<=4; e++) {
        //printf( RESET );
        //for (i=0; i<width/4; i++) {
        //    printf("    ");
        //}
        //if (select == e) {
        //    printf( ">");
        //}


    char print_item[4][20] = {
        { "START" },
        { "SAVE" },
        { "SETTINGS" },
        { "QUIT" }
    };

    char print_item_select[20];
    strcpy(print_item_select, print_item[select]);
    sprintf(print_item[select], "\x1b[1m>%s" RESET, print_item_select);

    printf( "\t\t%s\n", print_item[0] );
    printf( "\t\t%s\n", print_item[1] );
    printf( "\t\t%s\n", print_item[2] );
    printf( "\t\t%s\n", print_item[3] );

        //if (e == 1) {
        //    printf("START\033[0;0m\n");
        //} else if (e == 2) {
        //    printf("SAVE\033[0;0m\n");
        //} else if (e == 3) {
        //    printf("SETTINGS\033[0;0m\n");
        //} else if (e == 4) {
        //    printf("QUIT\033[0;0m\n");
        //} else {}
    //}
}
void menu() {
    unsigned short int sel=0, seln=1;
    unsigned short int input;

    do {
        printmenu(sel);
        fflush(stdout);

        while (!kbhit()) {}

        input = getinput();

        if (input == 1 && sel > 0) {
            sel--;
            BEEP
                } else if (input == 2 && sel < 3) {
            sel++;
            BEEP
                } else if (input == 0) break;
        else if (input == 5 || input == 3) {
            BEEP_SELECT

                if (sel == 1) break;
                else if (sel == 4) finish();
        } else sel == 1;

        fputs("\033c", stdout); /* Clear Screen */
        usleep(MENU_UPDATE_SPEED);
        seln = sel;


    } while (true);
}


void level2(){}
void level1(){
    unsigned short int map[19][19]={
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,3,3,3,1,1,1,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,4,4,4,4,4,4,4,4,4,0,0,0,0,0},
        {0,0,0,0,0,4,0,0,0,0,0,0,0,4,0,0,0,0,0},
        {0,0,0,0,0,4,0,0,0,5,0,0,0,4,0,0,0,0,0},
        {0,0,0,0,0,4,0,0,5,5,5,0,0,4,0,0,0,0,0},
        {0,0,0,0,0,4,0,0,5,3,5,0,0,4,0,0,0,0,0},
        {0,0,0,0,0,4,0,5,5,5,5,5,0,4,0,0,0,0,0},
        {0,0,0,0,0,4,0,0,0,0,0,0,0,4,0,0,0,0,0},
        {0,0,0,0,0,4,0,0,0,0,0,0,0,4,0,0,0,0,0},
        {0,0,0,0,0,4,4,4,4,4,4,4,4,4,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1}
    };

    mapx = 19;
    mapy = 19;

    if (playerx == 9 && playery == 12) {
        BEEPL(2);
        playermove = false;
        playerview = false;
        level += 1;
        level2();
        return;
    }

    if (playermove == true) {
        render(map);
    } else {
        map[4][6] = 1;
        map[4][7] = 1;
        map[4][8] = 1;

        fputs("\033c", stdout);
        level = 1;
        render(map);
        for (short int i=-1; i!=7; i++) {
            map[4][i-1] = 1;
            map[4][i]   = 3;
            map[4][i+1] = 3;
            map[4][i+2] = 3;

            if (i>=5) {
                usleep(700000);
            } else if (i>=4) {
                usleep(500000);
            } else {
                usleep(400000); /* sleep in microseconds */
            }

            fputs("\033c", stdout);
            render(map);
        }

        sleep(1);
        mapx=16;
        mapy=16;
        playerx=7;
        playery=6;
        playermove=true;
        playerview=true;
    }
}
void level0(){
    unsigned short int map[10][20]={
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {0,0,0,0,0,0,0,3,3,3,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    /* First Level Spacific */
    if (playerx == 8 && playery == 3) {
        BEEPL(2);
        playermove = false;
        playerview = false;
    }

    if (playermove == false) {
        /* TODO: fix screen tearing */
        map[2][7]  = 0;
        map[2][10] = 3;

        fputs("\033c", stdout);
        render(map);

        sleep(1);
        map[2][8]  = 0;
        map[2][9]  = 0;
        map[2][10] = 0;

        for (short int i=9; i<17; i++) {
            map[3][i-1] = 1;
            map[3][i]   = 3;
            map[3][i+1] = 3;
            map[3][i+2] = 3;

            usleep(2000); /* sleep in microseconds */
            fputs("\033c", stdout);
            render(map);

            usleep(500000);
        }

        level1();
    } else {
        render(map);
    }
}
void runlevel() {
    /* Run the Level */
    if (level == 0) level0();
    else if (level == 1) level1();
    else if (level == 2) level2();
    else finish();
}
void gameplay() {
    fputs("\033c", stdout);
    level0();

    do {
        int input = getinput();


        if (input == 1) {
            if (playery <= 1) {
            } else playery--;

        } else if (input == 2) {
            if (playery >= mapy-1+1) {
            } else playery++;

        } else if (input == 3) {
            if (playerx >= mapx-1) {
            } else playerx++;

        } else if (input == 4) {
            if (playerx <= 0) {
            } else playerx--;

        } else if (input == 6) {
            /*
            * TODO: Change new function escape, deturmines which during what
            * level/stage
            */
            menu();
        }

        while (!kbhit()) {
            usleep(GAME_UPDATE_SPEED); /* Sleep in microseconds */
            fputs("\033c", stdout);
            runlevel();
            FRAMES_INCREMENT
        }

    } while (1);
}

int main() {
    // https://stackoverflow.com/questions/448944/c-non-blocking-keyboard-input

    BEEP

    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    menu();
    gameplay();

    /* EXIT */
    finish();
}

/* TODO:
 * - MULTITREADING
 *  ` First with Audio, to do in background for fast gameplay
 *   https://dev.to/quantumsheep/basics-of-multithreading-in-c-4pam
 */
