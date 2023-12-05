#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>

int main()
{   
    int elf_calories = 0;
    int best_elf_calories[3] = {};
    char line[101];

    while (fgets(line, 101, stdin) != NULL)
    {
        elf_calories += atoi(line);
        if (line[0] == '\n')
        {
            if (best_elf_calories[0] < elf_calories)
            {   
                best_elf_calories[2] = best_elf_calories[1];
                best_elf_calories[1] = best_elf_calories[0];
                best_elf_calories[0] = elf_calories;
            }else if(best_elf_calories[1] <= elf_calories){
                best_elf_calories[2] = best_elf_calories[1];
                best_elf_calories[1] = elf_calories;
            }else if(best_elf_calories[2] <= elf_calories){
                best_elf_calories[2] = elf_calories;
            }
            elf_calories = 0;
       }
    }
    int total_calories = best_elf_calories[0] + best_elf_calories[1] + best_elf_calories[2];
    printf("%d %d %d %d",best_elf_calories[0],best_elf_calories[1], best_elf_calories[2], total_calories);

    return 0;
}
