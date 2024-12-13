#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// A = rock ,B = Paper, C = Scissors
// X = rocl, Y = Paper, Z = scissors

int main() {
  char* elf;
  char* me;
  char line[5] = {};
  int score = 0;
  while (fgets(line, 5, stdin)) {
    line[strcspn(line, "\n")] = 0;
    elf = strtok(line, " ");
    me = strtok(NULL, " ");
    if (me[0] == 'Y') {
      me[0] = elf[0] + 23;
    } else if (me[0] == 'X') {
      if (elf[0] == 'A') {
        me[0] = elf[0] + 23 + 2;
      } else {
        me[0] = elf[0] + 23 - 1;
      }
    } else if (me[0] == 'Z') {
      if (elf[0] == 'C') {
        me[0] = elf[0] + 23 - 2;
      } else {
        me[0] = elf[0] + 23 + 1;
      }
    }
    me[0] -= 23;
    if (strcmp(elf, me) == 0) {
      score += 3;
      score += me[0] - 64;
    } else if (elf[0] == 'A' && me[0] == 'B') {
      score += 8;
    } else if (elf[0] == 'B' && me[0] == 'C') {
      score += 9;
    } else if (elf[0] == 'C' && me[0] == 'A') {
      score += 7;
    } else if (elf[0] == 'A' && me[0] == 'C') {
      score += 3;
    } else if (elf[0] == 'B' && me[0] == 'A') {
      score += 1;
    } else if (elf[0] == 'C' && me[0] == 'B') {
      score += 2;
    }
  }
  printf("%d", score);
  return 0;
}