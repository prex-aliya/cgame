#include "main.h"

struct termios old_tio, new_tio;
int sig_caught=0;
void signal_handler(int sig) {
  if (sig == SIGINT) {
    sig_caught=1;
  }
}

void gameplay(void) {
  printf("da\n\nda\n");
}

int main() {

  str_ren(); // Start renderer
    ld_ren(1);
    td_ren(&gameplay);
  end_ren(); // Stop Renderer

  return 0;
}

// vim: shiftwidth=2 tabstop=2
