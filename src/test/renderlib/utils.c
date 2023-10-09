#include <stdbool.h>
#include <stdarg.h>
#include <signal.h>

#define LOG_FILE "log"


bool log_s() { /* Restart Log */
  FILE* file = fopen(LOG_FILE, "w");
  if (!file) {
    fprintf(stderr, "ERROR: could not open LOG_FILE log file %s for writting",
        LOG_FILE);
    return false;
  }
  time_t now = time(NULL);
  char* date = ctime(&now);
  fprintf(file, "LOG_FILE log. local time %s\n", date);
  fclose(file);
  return true;
}
bool log_a(const char* message, ...) { /* Append/Add to Log */
  va_list argptr;
  FILE* file = fopen(LOG_FILE, "a");
  if (!file) {
    fprintf(stderr, "ERROR: could not open LOG_FILE log file %s for writting",
            LOG_FILE);
    return false;
  }
  /*  */
  va_start(argptr, message);
  vfprintf(file, message, argptr);
  va_end(argptr);

  fclose(file);
  return true;
}
bool log_e(const char* message, ...) { /* Error Log */
  va_list argptr;
  if (log_a(message)) {
    va_start(argptr, message);
    vfprintf(stderr, message, argptr);
    va_end(argptr);
    return true;
  }
  return false;
}

unsigned char getinput(void) { /* Get Input and Output */
    /* NOTE:
     * 0 = <esc/q>  Quit
     * 1 = <k/w>    Up
     * 2 = <j/r>    Down
     * 3 = <l/s>    Right
     * 4 = <h/a>    Left
     * 5 = other
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
    } else if (input == 113 || input == 1027) {
        return 0;
    } else {
        return 5;
    }

    return 0;
}
