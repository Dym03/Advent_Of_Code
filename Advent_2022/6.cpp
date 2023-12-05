#include <iostream>
//first part
// #define LENGHT 4
#define LENGHT 14

int main(){
    std::string input;
    std::string code;
    while (getline(std::cin, input)){
        code += input;
    }
    std::string quadruple;
    int counter = 0;
    bool check = true;
    int start = 0;
    int end = LENGHT;
    while(end <= code.length()){
        check = true;
        quadruple = code.substr(start,LENGHT);
        for(int i = 0; i < LENGHT; i++){
            for(int j = i+1; j < LENGHT; j++){
                if (quadruple[i] == quadruple[j]){
                    check = false;
                    break;
                }
            }
            if (!check){
                break;
            }
        }
        if(check){
            break;
        }
        start += 1;
        end += 1;
    }
    std::cout << end << std::endl;


    return 0;
}











// #include <assert.h>
// #include <ctype.h>
// #include <stdbool.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// bool check_four();
// int i = 0;
// int counter = 0;
// bool brambor = true; 
// int main(){
//     char buff[10000];
//     char quadruple[14];
//     while(fgets(buff,10000,stdin)!= NULL){
//         i = 14;
//         buff[strcspn(buff,"\n")] = 0;
//         strncpy(quadruple,buff,4);
//         brambor = check_four(quadruple);
//         while(!brambor){
//             for(int l = 0; l < strlen(buff) - 1; l++){
//                 buff[l] = buff[l+1];
//             }
//             strncpy(quadruple,buff,14);
//             i +=1;
//             brambor = check_four(quadruple);
//         }
        
//         counter += i;
//     }

//     printf("%d",counter);
//     return 0;
// }

// bool check_four(char list[]){
//     bool check = true;
//     for(int i = 0; i < 14; i++){
//         for(int j = i+1; j < 14; j++){
//             if(list[i] == list[j]){
//                 check = false;
//             }
//         }
//     }
//     return check;
// }