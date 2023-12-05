#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char line[101];
  char line1[101];
  char line2[101];
  int sum = 0;
  int found1 = 0;
  int lenght = 0, lenght_1 = 0, lenght_2 = 0;
  bool found = false;
  while (fgets(line, 101, stdin) != NULL) {
    fgets(line1, 101, stdin);
    fgets(line2, 101, stdin);
    line[strcspn(line, "\n")] = 0;
    line1[strcspn(line1, "\n")] = 0;
    line2[strcspn(line2, "\n")] = 0;
    lenght = strlen(line);
    lenght_1 = strlen(line1);
    lenght_2 = strlen(line2);
    found = false;
    for (int i = 0; i < lenght; i++) {
      for (int j = 0; j < lenght_1; j++) {
        if (line[i] == line1[j]) {
          for (int k = 0; k < lenght_2; k++) {
            if (line[i] == line2[k] && !found) {
              if (isupper(line[i])) {
                sum += line[i] - 38;
                found = true;
                found1 ++;
                printf("%d %c\n",found1,line[i]);
              } else if (islower(line[i])) {
                sum += line[i] - 'a' + 1;
                found = true;
                found1 ++;
                printf("%d %c\n",found1,line[i]);
              }
            }
          }
        }
      }
    }
    //Part 1
    // for(int i = 0; i < lenght/2; i++){
    //      for(int j = 0; j < lenght/2; j++){
    //          if(line[i] ==line[lenght/2 + j]){
    //              if(islower(line[i]) && !found){
    //                  sum += line[i] - 'a' + 1;
    //                  found = true;
    //              }else if(isupper(line[i]) &&  !found){
    //                  sum += line[i] - 38;
    //                  found = true;
    //              }
    //          }
    //      }
    //  }
  }
  printf("%d", sum);
}