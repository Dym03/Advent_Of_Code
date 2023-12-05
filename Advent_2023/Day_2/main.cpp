#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <vector>

int main(){
    std::map<std::string, int> max_input;
    std::map<std::string, int> min_input;
    max_input["red"] = 12;
    max_input["green"] = 13;
    max_input["blue"] = 14;
    min_input["red"] = 0;
    min_input["green"] = 0;
    min_input["blue"] = 0;
    std::string colours [] = {"red", "green", "blue"};
    int sum_id = 0;
    int sum_min = 0;
    int mul_min = 1;
    std::string input;
    std::string line;
    std::vector <std::string> tokens;
    std::vector<std::string> parsed_tokens;
    bool valid = true;
    while (getline(std::cin, line)){
        int id = 0;
        mul_min = 1;
        valid = true;
        min_input.clear();
        tokens.clear();
        parsed_tokens.clear();
        id = std::stoi(line.substr(line.find_first_of(" "), line.find(':')));
        // std::cout << id << std::endl;
        line = line.substr(line.find(':') + 1);

        std::stringstream check1(line);


        while (getline(check1, input, ';')){
            // std::cout << input << std::endl;
            tokens.push_back(input);
        }
        for(int i = 0; i < tokens.size(); i++){
            std::stringstream check2(tokens[i]);
            while (getline(check2, input, ',')){
                // std::cout << input << std::endl;
                parsed_tokens.push_back(input.substr(input.find_first_of(" ") + 1));
            }
        }
        for(int i = 0; i < parsed_tokens.size(); i++){
            int num = std::stoi(parsed_tokens[i].substr(0, parsed_tokens[i].find_first_of(" ")));
            std::string color = parsed_tokens[i].substr(parsed_tokens[i].find_first_of(" ") + 1);
            std::cout << num << " " <<  color << std::endl;
            if(num > max_input[color]){
                valid = false;
            }
            min_input[color] = std::max(min_input[color], num);
        }
        for(int i = 0; i < colours->size(); i++){
            mul_min *= min_input[colours[i]];
        }
        sum_min += mul_min;
        //first part
        if (valid){
            sum_id += id;
        }
    }
    std::cout << sum_min << std::endl;

}