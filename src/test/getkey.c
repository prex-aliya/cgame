#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // For NULL
#include <termios.h>
#include <unistd.h>

struct termios old_tio, new_tio;
int sig_caught=0;
void signal_handler(int sig) {if (sig == SIGINT) {sig_caught=1;}}
void exit_game() {
    printf("\x1b[0m");
    exit(0);
}
int getinput() {
    /* NOTE:
     * 1 = up
     * 2 = down
     * 3 = right
     * 4 = left
     */

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
int kbhit() {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}
int main() {
    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    // Must not be negative, or arrays will make a segfault.
    unsigned int sel=0, input;

    do {
        fflush(stdout);
        if (kbhit()) {
            input = getinput();
            fgetc(stdout);
        } else {
        }

        usleep(10000);
        sel++;
        printf("%d", input);
        input = 1;
    } while (1);

    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio); /* restore former settings */
}
