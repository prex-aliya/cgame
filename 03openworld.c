#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "config.h"

/* NOTE: recursave functions are not part of the standard c language */
struct termios old_tio, new_tio;



int sig_caught=0;
void signal_handler(int sig) {
    if (sig == SIGINT) {
        sig_caught=1;
    }
}

int getinput() {
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
void render(unsigned short int map[mapy][mapx]) {
    register unsigned int x,y;

    unsigned char dirt[4][4]={
        {"    "},
        {"    "},
        {"    "},
        {"    "}
    };

    for (y=0; y<4*height; y++) {
        for (x=0; x<4*width; x++) {
            if (y == 0 || y == 4*height-1 ) { /* render the boarder first */
                printf("\e[1;47m        \e[0;37m");
                x = x + 3;
            } else if (x == 0 || x == 4*width-1) { /* rendering first means on top */
                printf("\e[1;47m  \e[0;0m");
            }
            else if (x/4 == 7 && y/4 == 3 && playerview == true) { /* render player */
                int sy = y-((y/4)*4);
                if (sy == 3) {
                    printf("\e[40m        \e[0m");
                } else if (sy == 2) {
                    printf("\e[40m        \e[0m");
                } else if (sy == 2) {
                    printf("\e[43m        \e[0m");
                } else {
                    printf("\e[1;41m        \e[0m");
                }
                x = x+3;
            }
            else {
                short int sx = x-((x/4)*4);
                short int sy = y-((y/4)*4);
                if ((x/4)+(playerx-7) > mapx-1 || (y/4)+(playery-4) > mapy-1) {
                    /* Rendrers Out Mountians Per Level */
                    // TODO: split to seperate functions
                    if (level == 0) {
                        printf("\e[0m\x1b[38;5;28m~~\e[0m");
                    } else if (level == 1) {
                        short int outy = (y/4)+(playery-7);
                        short int outx = (x/4)+(playerx-7);
                        if (outy <= -1 && outy >= -3) {
                            printf("\e[0m\x1b[38;5;28m~~\e[0m");
                        } else if ( outy == 1) {
                            printf("\e[43m%c%c", dirt[sy][sx], dirt[sy][sx]);
                        } else if ( outx == 19 && !(outy <= 0)) {
                            printf("\e[43m%c%c", dirt[sy][sx], dirt[sy][sx]);
                        } else {
                            printf("\e[42;33m.*\e[0m");
                        }
                    }
                } else if (map[(y/4)+(playery-4)][(x/4)+(playerx-7)] == 1) { /* renders green */
                    printf("\e[43m%c%c", dirt[sy][sx], dirt[sy][sx]);
                } else if (map[(y/4)+(playery-4)][(x/4)+(playerx-7)] == 0) { /* renders yellow */
                    printf("\e[42m%c%c", dirt[sy][sx], dirt[sy][sx]);
                } else if (map[(y/4)+(playery-4)][(x/4)+(playerx-7)] == 2) { /* renders trees */
                    printf("\x1b[38;5;28m^^");
                } else if (map[(y/4)+(playery-4)][(x/4)+(playerx-7)] == 3) { /* renders pure blue */
                    printf("\e[44m  ");
                } else if (map[(y/4)+(playery-4)][(x/4)+(playerx-7)] == 4) { /* renders pure grey */
                    printf("\e[40m!!");
                } else {
                    printf("  ");
                }
            }
        }
        printf("\e[0m\n");
    }
    if (level == 0) { printf("\e[0mGO TO CAR"); /* Level 1 Objective */
    } else if (level == 1) {printf("\e[0mGO TO THE TOWER"); } /* Level 2 Objective */
    /* DEBUG */
    if (debug == true) { debuginfo(); }
    fputs("\n", stdout);
}

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
        {0,0,0,0,0,4,0,4,4,4,4,4,0,4,0,0,0,0,0},
        {0,0,0,0,0,4,0,0,4,3,4,0,0,4,0,0,0,0,0},
        {0,0,0,0,0,4,0,0,4,4,4,0,0,4,0,0,0,0,0},
        {0,0,0,0,0,4,0,0,0,4,0,0,0,4,0,0,0,0,0},
        {0,0,0,0,0,4,0,0,0,0,0,0,0,4,0,0,0,0,0},
        {0,0,0,0,0,4,4,4,4,4,4,4,4,4,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1}
    };

    mapx = 19;
    mapy = 19;

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

            usleep(2000); /* sleep in microseconds */
            fputs("\033c", stdout);
            render(map);

            sleep(1);
        }

        playerx = 7;
        playery = 6;
        playermove = true;
        playerview = true;
    }
}
void level0(){
    unsigned short int map[10][20]={
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,3,3,3,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    if (playermove == false) {
        /* TODO: fix screen tearing */
        map[4][7]  = 0;
        map[4][10] = 3;

        fputs("\033c", stdout);
        render(map);

        sleep(1);
        map[4][8]  = 0;
        map[4][9]  = 0;
        map[4][10] = 0;

        for (short int i=9; i<17; i++) {
            map[3][i-1] = 1;
            map[3][i]   = 3;
            map[3][i+1] = 3;
            map[3][i+2] = 3;

            usleep(2000); /* sleep in microseconds */
            fputs("\033c", stdout);
            render(map);

            sleep(1);
        }

        level1();
    } else {
        render(map);
    }
}
void runlevel() {
    /* Run the Level */
    if (level == 0) {
    } else {
        level1();
    }
}
void gameplay() {
    fputs("\033c", stdout);
    //render(firstmap);
    level0();

    do {
        int input = getinput();
        printf("%d", input);


        if (input == 1) {
            if (playery <= 1) {
            } else {
                playery--;}
        } else if (input == 2) {
            if (playery >= mapy-1+1) {
            } else {
                playery++;
            }
        } else if (input == 3) {
            if (playerx >= mapx-1) {
            } else {
                playerx++;
            }
        } else if (input == 4) {
            if (playerx <= 0) {
            } else {
                playerx--;
            }
        }

        /* First Level Spacific */
        if (playerx == 8 && playery == 5 && level == 0) {
            playermove = false;
            playerview = false;
        }

        if (playermove == false) { /* This is for the begining scene */
            if (level == 0) {
            }
        }

        fputs("\033c", stdout);

        runlevel();
        usleep(2000); /* Sleep in microseconds */
        frames = frames+1;
    } while (1);
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
    unsigned short int select = 1;

    do {
        printmenu(select);
        int input = getinput();
        if (input == 1) {
            if (select <= 1) {
            } else {
                select--;
            }
        } else if (input == 2) {
            if (select >= 4) {
            } else {
                select++;
            }
        } else if (input == 0) {
            break;
        } else if (input == 5) {
            if (select == 1) {
                gameplay();
                break;
            } else if (select == 4) {
                break;
            }
        } else { }
    fputs("\033c", stdout); /* Clear Screen */
    usleep(2000);

    } while (select != 0);
}

int main() {
    // https://stackoverflow.com/questions/448944/c-non-blocking-keyboard-input

    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    menu();

    /* EXIT */
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio); /* restore former settings */
    return 0;
}
