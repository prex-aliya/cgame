#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>

struct termios old_tio, new_tio;
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

void slicemenu(int i, int menu, char print_item[20]) {
  if (i == menu)
    printf( "\x1b[1m>" "\t\t%s\n", print_item );
  else
    printf( "\x1b[0m" "\t\t%s\n", print_item );
}
void printmenu(int menu, int length, char print_item[length][20]) {
  for (int i=0; i<4; i++) {
    slicemenu(i, menu, print_item[i]);
  }

  fflush(stdout);
  return;
}

void second_menu(int sel, char print_item[4][20]) {
  int inc_sel[3] = {0, -1, 1};
  char print_settings[4][20] = {
    { "OPTION1\t" },
    { "OPTION2\t" },
    { "OPTION3\t" },
    { "OPTION4\t" }
  };
  int input, second=0;

  while(1) {
    fputs("\n\n", stdout);
    printmenu(second, 4, print_settings);
    usleep(50000);


    while (!kbhit()) {}
    input = getinput();

    if (input == 5 || input == 3) {
    } else if (input == 4) {
      break;
    } else { /* TODO: if 4? */
      short int tmp = second+inc_sel[input];
      if (tmp >= 0 && tmp <= 3) second += inc_sel[(input)];
    }

    fputs("\033c\n\n", stdout); /* Clear Screen + Shift Down */

    printmenu(sel, 4, print_item);
  }
  return;
}
void menu() {
  int inc_sel[3] = {0, -1, 1};
  int sel = 0, input;

  while (1) {
    fputs("\033c\n\n", stdout); /* Clear Screen + Shift Down */
    char print_item[4][20] = {
      { "START\t" },
      { "SAVE\t" },
      { "SETTINGS" },
      { "QUIT\t" }
    };
    printmenu(sel, 4, print_item);

    while (!kbhit()) {}
    input = getinput();

    if (input == 5 || input == 3) {
      if (sel == 0) break;
      else if (sel == 4) break;
      else
        second_menu(sel, print_item);
    } else {
      short int tmp = sel+inc_sel[input];
      if (tmp >= 0 && tmp <= 3) sel += inc_sel[(input)];
    }

    usleep(50000);
  }
  return;
}

int main() {
        tcgetattr(STDIN_FILENO, &old_tio);
        new_tio = old_tio;
        new_tio.c_lflag &= (~ICANON & ~ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

        menu();

        tcsetattr(STDIN_FILENO, TCSANOW, &old_tio); /* restore former settings */
}
