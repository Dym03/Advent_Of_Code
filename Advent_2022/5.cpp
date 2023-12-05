#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <sstream>

char stack_1[100] = "NCRTMZP";
char stack_2[100] = "DNTSBZ";
char stack_3[100] = "MHQRFCTG";
char stack_4[100] = "GRZ";
char stack_5[100] = "ZNRH";
char stack_6[100] = "FHSWPZLD";
char stack_7[100] = "WDZRCGM";
char stack_8[100] = "SJFLHWZQ";
char stack_9[100] = "SQPWN";


int main(){
    std::vector<std::stack<char>> stacks;
    std::vector<std::string> stack_strings;
    stack_strings.push_back(stack_1);
    stack_strings.push_back(stack_2);
    stack_strings.push_back(stack_3);
    stack_strings.push_back(stack_4);
    stack_strings.push_back(stack_5);
    stack_strings.push_back(stack_6);
    stack_strings.push_back(stack_7);
    stack_strings.push_back(stack_8);
    stack_strings.push_back(stack_9);

    for(int i = 0; i < 9; i++){
        std::stack<char> stack;
        for(int j = 0; j < stack_strings[i].length(); j++){
            stack.push(stack_strings[i][j]);
        }
        stacks.push_back(stack);
    }
    std::string input;
    while (getline(std::cin, input)){
        std::stringstream ss(input);
        std::string token;
        std::vector<std::string> tokens;
        while (getline(ss, token, ' ')){
            tokens.push_back(token);
        }
        int amounth = std::stoi(tokens[1]);
        int from = std::stoi(tokens[3]);
        int to = std::stoi(tokens[5]);
        //Part 1 
        // for (int i = 0; i < amounth; i++){
        //     stacks[to - 1].push(stacks[from - 1].top());
        //     stacks[from - 1].pop();
        // }
        //Part 2
        std::stack<char> temp;
        for (int i = 0; i < amounth; i++){
            temp.push(stacks[from - 1].top());
            stacks[from - 1].pop();
        }
        for (int i = 0; i < amounth; i++){
            stacks[to - 1].push(temp.top());
            temp.pop();
        }
    
    }

    for(int i = 0; i < stacks.size(); i++){
        
        std::cout << stacks[i].top() << std::endl;
    }

    return 0;
}

