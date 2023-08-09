#include <stdio.h>

int main() {

  int x1 = 3;
  int y1 = 2;
  int x2 = 15;
  int y2 = 5;

  int m_new = 2 * (y2-y1);
  int slope_error_new = m_new - (x2 - x1);
  for (int nx = x1, ny = y1; nx <= x2; nx++ ) {
    printf("%d, %d\n", nx, ny);

    slope_error_new += m_new;

    if (slope_error_new >= 0) {
      ny++;
      slope_error_new -= 2*(x2-x1);
    }
  }

}
