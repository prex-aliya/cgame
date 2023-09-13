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


int menu_print(int *sel, int len, char print_item[len][20]) {
  //printf("\033[0;0HHello\n");
  fputs("\n\n", stdout); /* Shift Down */
  for (int i=0; i!=len; i++) {

    if (i < len)
      if (*sel == i) {
        printf("\x1b[1m>" "\t\t%s\n", print_item[i]);
        fputs("\x1b[0m", stdout);
      } else
        printf(" \t\t%s\n", print_item[i]);
  }
}
void menu_data(int sel[2], int *i, int *menu_length) {
  char print_o[5][20] = {
    { "option1" },
    { "option2" },
    { "option3" },
    { "option4" },
    { "option5" }
  };
  char print_yn[2][20] = {
    { "YES" },
    { "NO" }
  };
  char print_item[4][20] = {
    { "START" },
    { "SAVE" },
    { "SETTINGS" },
    { "QUIT" }
  };

  if (*i == 0) {
    menu_print(&sel[*i], 4, print_item);
    *menu_length = 4;
  } else if (*&sel[0] == 1 && *i == 1) {
    menu_print(&sel[*i], 5, print_o);
    *menu_length = 5;
  } else {
    menu_print(&sel[*i], 2, print_yn);
    *menu_length = 2;
  }
}
int menu_yn() {
  unsigned int ret_val[2] = {0, 1};
  unsigned int sel = 0;
  unsigned int input;
  char print_yn[2][20] = {
    { "NO" },
    { "YES" }
  };


  while (1) {
    menu_print(&sel, 2, print_yn);
    printf("\x1b[4A");

    while (kbhit()) fgetc(stdin);
    input = getinput();

    if (input == 3 || input == 5) {
      printf("\x1b[4B");
      return ret_val[sel];
    } else {
      sel = (sel == 1) ? 0 : 1;
    }

    usleep(100000);
  }

  printf("\x1b[A2");

}
int menu_input(int *input, int *sel, int *cur_sel, int *m_len) {
  int inc_sel[3] = {0, -1, 1};
  while (!kbhit()) {}
  *input = getinput();
  while (kbhit()) fgetc(stdin);

  if (*input == 5 || *input == 3) {
    //*cur_sel += 1;

    if (*cur_sel == 0) {
      switch (*sel) {
        case 0: *cur_sel+=1; break;
        case 1: *cur_sel+=1; break;
        case 2: break;
        case 3: if (*cur_sel==0) if (menu_yn()) return 1;
          else *cur_sel-=1;
          break;
      }
    } else if (1) {
      switch (*sel) {
        case 0: *cur_sel+=1; break;
        case 1: if (*cur_sel==0) return 1;
                  else *cur_sel-=1;
                  break;
      }
    } else return 1;

  } else if (*input == 4) {
      *cur_sel-=1;
  } else if (*input == 1 || *input == 2) {
    short int tmp = *sel+inc_sel[*input];
    if (tmp >= 0 && tmp < *m_len) *sel += inc_sel[*input];
  }

  return 0;
}

void menu() {
  int input,cur_sel = 0;
  int menu_length = 4;
  int sel[2] = {0,0};

  while (1) { // TODO not finite
    // PRINT
    fputs("\033c", stdout); /* Clear Screen + Shift Down */
    for (int i=0; i<=cur_sel; i++)
      menu_data(sel, &i, &menu_length);
      //menu_print(&sel[i]);

    usleep(100000);

    // INPUT
    if (menu_input(&input, &sel[cur_sel], &cur_sel, &menu_length))
      return;
    if (cur_sel > 1)
      cur_sel = 1;
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
