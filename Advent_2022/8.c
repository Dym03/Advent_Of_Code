#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void counter_visible_on_line();
void counter_visible_on_collum();

int main() {
  int counter = 0;
  int line = 0;
  char buff[500];
  char buff_long[100000]= {0};
  char buff_int[500];
  int lenght = 0;
  while (fgets(buff, sizeof(buff), stdin) != NULL) {
    buff[strcspn(buff, "\n")] = 0;
    lenght = strlen(buff);
    for (int i = 0; i < lenght; i++) {
      buff_int[i] = buff[i] - '0';
    }
    if (line == 0) {
      counter += 2 * lenght;
    } else if (line != 99) {
      counter_visible_on_line(&counter, buff_int, lenght);
    }
    strcat(buff_long, buff);
    line++;
  }
  counter_visible_on_collum(&counter,buff_long,lenght);
  printf("%d", counter);
  return 0;
}

void counter_visible_on_line(int *counter, int *buff, int lenght) {
  int max_l = 0;
  int max_r = 0;
  for (int i = 0; i < lenght; i++) {
    if (max_l < buff[i]) {
      *counter += 1;
      max_l = buff[i];
      buff[i] = 0;
    }
    if (max_r < buff[(lenght - 1) - i]) {
      *counter += 1;
      max_r = buff[(lenght - 1) - i];
      buff[(lenght - 1) - i] = 0;
    }
  }
}
void counter_visible_on_collum(int *counter, int *buff, int lenght) {
  int colum_max_down = 0;
  int colum_max_up = 0;
  for (int j = 0; j < lenght; j++) {
    for (int i = 0; i < lenght; i++) {
      if (i != 0 || i != lenght - 1 && j != 0 || j != lenght - 1) {
        if (buff[i * 98 + j] > colum_max_up) {
          *counter += 1;
          colum_max_up = buff[i * 98 + j];
          buff[i * 98 + j] = 0;
        }
        if (buff[i * 98 + j] > colum_max_down) {
          *counter += 1;
          colum_max_down = buff[i * 98 + j];
          buff[i * 98 + j] = 0;
        }
      }
    }
  }
}
