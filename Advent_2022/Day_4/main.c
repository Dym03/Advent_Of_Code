#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char line[101];
  int rozsah_1 = 0, rozsah_2 = 0, rozsah_3 = 0, rozsah_4 = 0;
  int sum = 0;
  while (fgets(line,101,stdin) != NULL) {
    line[strcspn(line,"\n")]= 0;
    rozsah_1 = atoi(strtok(line,"- , "));
    rozsah_2 = atoi(strtok(NULL,"- ,"));
    rozsah_3 = atoi(strtok(NULL,", -"));
    rozsah_4 = atoi(strtok(NULL,"- ,"));
    if((rozsah_1 <= rozsah_3 && rozsah_2 >= rozsah_4)||(rozsah_3 <= rozsah_1 && rozsah_4 >= rozsah_2)){
        sum += 1;
    }else if((rozsah_2 > rozsah_3 && rozsah_1 > rozsah_4 )||(rozsah_3 > rozsah_2 && rozsah_4 > rozsah_1)){

    }else{
        sum +=1;
    }
 printf("%d",sum);
  return 0;
}