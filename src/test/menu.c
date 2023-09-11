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

//void menu() {
//  int inc_sel[3] = {0, -1, 1};
//  int sel = 0, input;
//
//  while (1) {
//    fputs("\033c\n\n", stdout); /* Clear Screen + Shift Down */
//    char print_item[4][20] = {
//      { "START\t" },
//      { "SAVE\t" },
//      { "SETTINGS" },
//      { "QUIT\t" }
//    };
//    printmenu(sel, 4, print_item);
//
//    while (!kbhit()) {}
//    input = getinput();
//
//    if (input == 5 || input == 3) {
//      if (sel == 0) break;
//      else if (sel == 4) break;
//      else
//        second_menu(sel, print_item);
//    } else {
//      short int tmp = sel+inc_sel[input];
//      if (tmp >= 0 && tmp <= 3) sel += inc_sel[(input)];
//    }
//
//    usleep(50000);
//  }
//  return;
//}
void columnpp() {}
void column() {}
void data_input(int input, int sel) {
  int inc_sel[3] = {0, -1, 1};

  if (input == 5 || input == 3) {
    return;
  } else if (input == 1 || input == 2) {
    short int tmp = sel+inc_sel[input];
    if (tmp >= 0 && tmp <= 3) sel += inc_sel[(input)];
  }
}
void menu() {
  int inc_sel[3] = {0, -1, 1};
  int sel = 0, input, second = 0;

  char print_item[4][20] = {
    { "START\t" },
    { "SAVE\t" },
    { "SETTINGS" },
    { "QUIT\t" }
  };
  char print_yn[2][20] = {
    { "NO" },
    { "YES" }
  };

  while(1) {
    fputs("\033c\n\n", stdout); /* Clear Screen + Shift Down */

    for (int i=0; i!=7; i++) {
      // COLUMN 1
      if (i < 4)
        if (sel == i)
          printf(">" "\t\t%s", print_item[i]);
        else
          printf("\t\t%s", print_item[i]);
      else printf("\t\t\t");

      // COLUMN 2
      if (sel == 3 && i < 2+second)
          printf("\t%s", print_yn[i-second]);
      else if ((sel == 1 || sel == 2) && i < 5+second)
          printf("\t%s", print_item[i-second]);
      else printf("\t\t");

      fputs("\n", stdout);
    }

    while (!kbhit()) {}
    input = getinput();

    if (input == 5 || input == 3) {
      return;
    } else if (input == 1 || input == 2) {
      short int tmp = sel+inc_sel[input];
      if (tmp >= 0 && tmp <= 3) sel += inc_sel[(input)];
    }

    fflush(stdout);
    usleep(50000);
  }
}

int main() {
        tcgetattr(STDIN_FILENO, &old_tio);
        new_tio = old_tio;
        new_tio.c_lflag &= (~ICANON & ~ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

        menu();

        tcsetattr(STDIN_FILENO, TCSANOW, &old_tio); /* restore former settings */
}
