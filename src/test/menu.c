#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>



struct termios old_tio, new_tio;
/* Magic: a replica of the kbhit function on windows */
short int getinput() {
        /* NOTE:
         * 1 = up
         * 2 = down
         * 3 = right
         * 4 = left
         */

        char input = fgetc(stdin);
        if (input == 27) {
                getchar(); /* Skip the ^[ for arrow keys */
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
int kbhit() {
        struct timeval tv = { 0L, 0L };
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(0, &fds);
        return select(1, &fds, NULL, NULL, &tv) > 0;
}

void printmenu(int select, int select2, bool second_level) {
        fputs("\033c\n\n", stdout); /* Clear Screen + Shift Down */

        char print_item[4][20] = {
                { "START\t" },
                { "SAVE\t" },
                { "SETTINGS" },
                { "QUIT\t" }
        };

        char print_settings[4][20] = {
                { "START\t" },
                { "SAVE\t" },
                { "SETTINGS" },
                { "QUIT\t" },
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



        for (int i = 0; i < 7; i++) {
                if (i < 4)
                        printf( "\x1b[0m" "\t\t%s", print_item[i] );
                else printf( "\t\t\t" );

                if (second_level) {
                        printf("\t");
                        if (select2 == i-select)
                                printf("\x1b[1m>");
                        for (int x = 0; x < 4; x++)
                                if (i == select+x) printf( "\x1b[0m" "%s", print_settings[x]);
                }

                printf("\n");
        }

        //printf("%d", select);
        fflush(stdout);
}
void menu() {
        // Must not be negative, or arrays will segfault.
        bool second_level = false;
        int sel=0, input, sel2=0;
        int inc_sel[3] = {0, -1, 1};


        do {
                printmenu(sel, sel2, second_level);

                while (!kbhit()) { }
                input = getinput();

                if (!second_level) {
                        if (input == 5 || input == 3) {
                                if (sel == 2) {
                                        second_level = true;
                                        sel2=0;
                                }
                        } else {
                                short int tmp = sel+inc_sel[(input)];
                                if (tmp >= 0 && tmp <= 3) sel += inc_sel[(input)];
                        }
                } else {
                        if (input == 4) {
                                second_level = false;
                        } else {
                                short int tmp = sel2+inc_sel[(input)];
                                if (tmp >= 0 && tmp <= 3) sel2 += inc_sel[(input)];
                        }
                }

                usleep(50000);
        } while (1);
}

int main() {
        tcgetattr(STDIN_FILENO, &old_tio);
        new_tio = old_tio;
        new_tio.c_lflag &= (~ICANON & ~ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

        menu();

        tcsetattr(STDIN_FILENO, TCSANOW, &old_tio); /* restore former settings */
}
