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
        if (getchar() == 91) {
            switch(getchar()) {
                case 65: /* up */
                    value = 1;
                    break;
                case 66: /* down */
                    value = 2;
                    break;
                case 67: /* right */
                    value = 3;
                    break;
                case 68: /* left */
                    value = 4;
                    break;
            }
        } else {
            value = 6; /* esc */
        }
    } else if (input == 119 || input == 107) {
        value = 1;
    } else if (input == 114 || input == 106) {
        value = 2;
    } else if (input == 115 || input == 108 ) {
        value = 3;
    } else if (input == 97 || input == 104 ) {
        value = 4;
    }
    else if (input == 113) {
        value = 0;
    } else {
        value = 5;
    }
    return value;
}

void debuginfo() {
    printf("\n(%d, %d)",playerx, playery);
    printf("\npmove: %d, pview: %d",playermove, playerview);
    printf("\nxmap: %d, ymap: %d",mapx, mapy);
    printf("\nframes: %d, level: %d",frames, level+1);
}
void td_lvl_ren(int x, int y) {
    #define BIT(c) c c c c
    if (level == 0) {
        short int outy = (y/4)+(playery-7);
        short int outx = (x/4)+(playerx-7);
        if (outy == 0 && outx >= -1) {
            printf(RESET BYELLOW BIT("  ") RESET);
        } else if (outx > 16 && outy >= -1) {
            printf(RESET BGREEN YELLOW BIT(".*") RESET);
        } else {
            printf(RESET "\x1b[38;5;28m" BIT("~~") RESET);
        }
    } else if (level == 1) {
        short int outy = (y/4)+(playery-7);
        short int outx = (x/4)+(playerx-7);
        if (outy <= -1 && outy >= -3) {
            printf(RESET "\x1b[38;5;28m" BIT("~~") RESET);
        } else if ( outy == 1) {
            printf(BYELLOW BIT("  "));
        } else if ( outx == 19 && !(outy <= 0)) {
            printf(BYELLOW BIT("  ") RESET);
        } else {
            printf(BGREEN YELLOW BIT("/*") RESET);
        }
    } else {
            printf(RESET "\x1b[38;5;28m" BIT("~~") RESET);
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

        register int z, zx = x*4;  /* zx for z, precomputed before loop */
        for (z=0; z<4; z++) {
            short int realx = zx+z;
            //short int sx = realx-((realx/4)*4);
            //short int sy = y-((y/4)*4);
            switch (map[(y/4)+(playery-4)][(realx/4)+(playerx-7)]) {
            case 0: /* Print Green **/
                //printf(BGREEN "%c%c", dirt[sy][sx], dirt[sy][sx]);
                printf(BGREEN "  ");
                break;
            case 1: /* Print Yellow */
                printf(BYELLOW "  ");
                break;
            case 2: /* Print Trees */
                printf("\x1b[38;5;28m^^");
                break;
            case 3: /* Print Blue */
                printf(RESET BBLUE "  ");
                break;
            case 4: /* Print Black */
                printf(BBLACK "||");
                break;
            case 5: /* Print Red */
                printf(BRED "!!");
                break;
            default: printf("XX"); /* Somethings Wrong */
            }
        }
    }
}
void render(unsigned short int map[mapy][mapx]) {
    /* if tile like is needed */
    //unsigned char dirt[4][4]={
    //    {"    "},
    //    {"    "},
    //    {"    "},
    //    {"    "}
    //};

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
    /* DEBUG */
    if (debug == true) { debuginfo(); }
    fputs("\n", stdout);
}

void printmenu(unsigned short int select) {
    register unsigned int i,e;
    fputs("\033c", stdout); /* Clear Screen */
    printf("\n\n");
    for (e=1; e<=4; e++) {
    /* https://www.theurbanpenguin.com/4184-2/
     * reset color */
        printf("\033[0;0m");
        for (i=0; i<width/4; i++) {
            printf("    ");
        }
        if (select == e) {
            printf("\033[1;37m>");
        }
        if (e == 1) {
            printf("START\033[0;0m\n");
        } else if (e == 2) {
            printf("SAVE\033[0;0m\n");
        } else if (e == 3) {
            printf("SETTINGS\033[0;0m\n");
        } else if (e == 4) {
            printf("QUIT\033[0;0m\n");
        } else {}
    }
}
void menu() {
    unsigned short int sel = 1;
    unsigned short int seln = 1;
    int input;

    do {
        /* TODO: if no change in select then donot print again */

        printmenu(sel);
        fflush(stdout);
        input = getinput();

        if (input == 1) {
            if (sel <= 1) {
            } else {
                sel--;
                BEEP
            }
        } else if (input == 2) {
            if (sel >= 4) {
            } else {
                sel++;
                BEEP
            }
        } else if (input == 0) {
            break;
        } else if (input == 5 || input == 3) {
            BEEP_SELECT
            if (sel == 1) {
                break;
            } else if (sel == 4) {
                finish();
            }
        } else { }
        fputs("\033c", stdout); /* Clear Screen */
        usleep(MENU_UPDATE_SPEED);


    } while (select != 0);
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
    if (level == 0) {
        level0();
    } else if (level == 1) {
        level1();
    } else {
        level2();
    }
}
void gameplay() {
    fputs("\033c", stdout);
    level0();

    do {
        int input = getinput();
        //printf("%d", input);


        if (input == 1) {
            if (playery <= 1) {
            } else {
                playery--;
                //BEEP
            }
        } else if (input == 2) {
            if (playery >= mapy-1+1) {
            } else {
                playery++;
                //BEEP
            }
        } else if (input == 3) {
            if (playerx >= mapx-1) {
            } else {
                playerx++;
                //BEEP
            }
        } else if (input == 4) {
            if (playerx <= 0) {
            } else {
                playerx--;
                //BEEP
            }
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
            frames+=1;
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

/*
 * TODO:
 * - MULTITREADING
 *  ` First with Audio, to do in background for fast gameplay
 */
