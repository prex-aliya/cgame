#include "main.h"

int sig_caught=0;
void signal_handler(int sig) {
  if (sig == SIGINT) {
    sig_caught=1;
  }
}

void gameplay(void) {
  //printf("da\n\nda\n");
}

int main() {

  srt_ren(); // Start Renderer
    //ld_ren(1);
    td_ren(&gameplay);
  end_ren(); // Stop Renderer

  return 0;
}

// vim: shiftwidth=2 tabstop=2
