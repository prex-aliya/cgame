
#include "config.h"



/* NOTE: recursave functions are not part of the standard c language */
struct termios old_tio, new_tio;


/* TOP LEVEL FUNCTIONS: used to exit, need to be available everywhere. */
int sig_caught=0;
void signal_handler(int sig) {if (sig == SIGINT) {sig_caught=1;}}
void exit_game() {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio); /* restore former settings */
    printf("\x1b[0m");
    exit(0);
}
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
