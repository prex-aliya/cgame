/** @file main.c
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


/* TOP LEVEL FUNCTIONS: used to exit, need to be available everywhere. */
int sig_caught=0;
void signal_handler(int sig) {if (sig == SIGINT) {sig_caught=1;}}
void finish() {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio); /* restore former settings */
    printf("\x1b[0m");
    exit(0);
}

/* MISC/INPUT */
int getinput() {
    /* NOTE:
     * 1 = up
     * 2 = down
     * 3 = right
     * 4 = left
     */

    unsigned int value;
    char input = fgetc(stdin);
    if (input == 27) {
        fgetc(stdin); /* Skip the ^[ for arrow keys */
        /* The return values are equivilent to subtracting 64 from the input
         * keys numbers, so much smaller than if-else or switch */
        return fgetc(stdin)-64;
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

/* RENDERING */
void td_lvl_mtn(int x, int y) {
#define TD_MTN_OUT_VAR int outy = (y/4)+(playery-7);  \
    int outx = x+(playerx-7);
#define TO4(c) c c c c
#define PRINT_COLOR(color, print) printf(color TO4(print));


    if (level == 0) {
        TD_MTN_OUT_VAR
            if (outy == 0)
                PRINT_COLOR(BYELLOW YELLOW, "  ")
                else if (outx > 16 && outy >= -1)
                    PRINT_COLOR(BGREEN YELLOW, ".*")
                    else
                        PRINT_COLOR(GREEN, "~~") /* Trees */

                            } else if (level == 1) {
        TD_MTN_OUT_VAR
            if (outy <= -1 && outy >= -3) /* Trees */
                PRINT_COLOR(GREEN, "~~")
                else if ( outy == 1)
                    PRINT_COLOR(BYELLOW, "  ")
                    else if ( outx == 19 && !(outy <= 0))
                        PRINT_COLOR(BYELLOW, "  ")
                        else
                            PRINT_COLOR(BGREEN YELLOW, "/*")

                                } else {
        PRINT_COLOR(RED, "XX");
    }
}
void td_ren(unsigned int x, unsigned int y, int map[mapy][mapx]) {
    if (x == 7 && y/4 == 3 && playerview == true) { /* render player */
        if (y-((y/4)*4) < 2)
            printf(BBLACK TO4("  ") RESET);
        else
            printf(BRED TO4("  ") RESET);

    } else if (x+(playerx-7) > mapx-1 || (y/4)+(playery-4) > mapy-1) {
        td_lvl_mtn(x, y);
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
            #define TILE_LOC (map_value <= TOP_TD_REN_TILES) ? map_value :  \
            ERROR_TD_REN_TILES

            unsigned int map_value =
                map[(y/4)+(playery-4)][(x+(z/4))+(playerx-7)];
            printf(RESET "%s" RESET, tiles[TILE_LOC]);
        }
    }
}
void render(int map[mapy][mapx]) {
    register unsigned int x,y;
    // Never need negative in arrays, since all tiles will be in arrays.

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

/* MENU */
void printmenu(int select) {
    fputs("\033c\n\n", stdout); /* Clear Screen + Shift Down */

    char print_item[4][20] = {
    { "START" },
    { "SAVE" },
    { "SETTINGS" },
    { "QUIT" }
};

    /*
     * 19, and 4, is less than because arrays start a 0.
     * This shifts the array over by five, so that we can prepend the
     * "\x1b[1m>", this is faster than if select == on each, and printing the
     * prepend first. This counts down from 19 to 4, so we dont over write the
     * entire array with the first array point.
     */
    for (int j = 19; j > 4; j--)
        print_item[select][j] = print_item[select][j-5];

    /* TODO this is not very elagent */
    print_item[select][0] = '\x1b';
    print_item[select][1] = '[';
    print_item[select][2] = '1';
    print_item[select][3] = 'm';
    print_item[select][4] = '>';

    for (int i = 0; i < 4; i++)
        printf( RESET "\t\t%s\n", print_item[i] );
    fflush(stdout);
}
void menu() {
    int sel=0, input;
    int inc_sel[3] = {0, -1, 1};

    do {
        printmenu(sel);

        while (!kbhit()) { }
        input = getinput();

        if (input == 0) { finish();
        } else if (input == 5 || input == 3) {
            BEEP_SELECT

                if (sel == 0) break;
                else if (sel == 4) finish();
        } else sel == 1;

        /* Dont know why but putting these two together makes it seg-fault,
         * unknown reason for it happening.
         */
        sel += inc_sel[(input)];
        sel = sel % 4;

        BEEP
            usleep(MENU_UPDATE_SPEED);
    } while (true);
}

/* More Front End, more code, for the astetics. */
void level2() {}
void level1() {
    int map[19][19]={
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
        for (register int i=-1; i!=7; i++) {
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
void level05() {} // TODO: increase the cutscene to include going down the road
void level0() {
    int map[10][20]={
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
    } else if (playerx >= 1000) {
        /* If follow road long enough, start next level */
        BEEPL(2);
        playermove = false;
        playerview = false;
        playerx = 8; playery = 3;
        level1();
    } else if (playermove == false) {
        /* TODO: fix screen tearing */
        map[2][7]  = 0;
        map[2][10] = 3;

        fputs("\033c", stdout);
        render(map);

        sleep(1);
        map[2][8]  = 0;
        map[2][9]  = 0;
        map[2][10] = 0;

        for (int i=9; i<17; i++) {
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
/* Drivers The Game */
void runlevel() {
    fputs("\033c", stdout);
    /* Run the Level */
    /* TODO must be constant speed solution? */
    if (level == 0) level0();
    else if (level == 1) level1();
    else if (level == 2) level2();
    else finish();
}
void gameplay() {
    /*
     * This function is the main gameplay loop, we have an array that has if the
     * player should increase location by -1, or 1, or none, for a constant look
     * up time. Then if we should do x, or y. We do esc checking first so we
     * dont have to expend extra cycles. And while no keyboard inputs, we do
     * nothing, except render the screen. If we do not render the screen, it
     * will become glitchy, try yourself, may be fixed, and I didn't know.
     */
    const unsigned int inc_sel[6] = {0, -1, 1, 1, -1, 0};
    unsigned int resistances[2] = {0, 3};
    int input;
    fputs("\033c", stdout); // Clear screen
    level0();

    int input_num = 0;
    do {
        /* Resistance, this is how many time it is need to typed in a row for it
         * to register as an input. We consume a spacific ammount of inputs. */
        if (playerx < 0 || playerx >= mapx ||
            playery < 1 || playery >= mapy)
            player_resistance = 2; // Little resistance
        else
            player_resistance = 0; // No resistance

        // TODO remove need for counter.
        /* if input_num is >= to resistance then, get the input, else, 0 */
        input = (input_num >= player_resistance) ? getinput() : 0;

        if (input == 0) {
            fgetc(stdin); // Remove input from stdin
            input_num++;
        } else {
            input_num = 0; // Set counter to zero

            if (input == 5) menu();
            else if (input > 2) playerx+=inc_sel[input];
            else if (input <= 2) playery+=inc_sel[input];
        }
        
        while (!kbhit()) { /* While no new inputs */
            runlevel();
            FRAMES_INCREMENT // Some optional debug code.
            usleep(GAME_UPDATE_SPEED); /* Sleep in microseconds */
        }

    } while (1);
}

int main() {

    /* TODO: include ammount of times for beep */
    BEEP BEEP BEEP;

    /* Change Termios Settings/store settings. Mainly Magic */
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
 * - GLITCH
 *  ` if moving rapidly in (unconfirmed) y, then stop then press going left or
 *    right then will jump a space.
 */
