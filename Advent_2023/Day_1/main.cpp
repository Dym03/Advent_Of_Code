#include <iostream>

#define NUMS "123456789"
std::string NUMBERS[] {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

int solve(){
    std::string s;
    int sum = 0;
    while (std::cin >> s){
        int f = s.find_first_of(NUMS) != std::string::npos ? s.find_first_of(NUMS) : s.size();
        int n_f = 0;
        int l = s.find_last_of(NUMS) != std::string::npos ? s.find_last_of(NUMS) : 0;
        int n_l = 0;
        std::string sub_s; 
        for (int i = 0; i < 9;i++){
            sub_s = s;
            while (sub_s.find(NUMBERS[i]) != std::string::npos){
                int idx = sub_s.find(NUMBERS[i]) + 1 + (s.size() - sub_s.size());
                if (idx < f){
                    f = idx;
                    n_f = i + 1;
                }else if (idx > l){
                    l = idx;
                    n_l = i + 1;
                }
                if (idx + NUMBERS[i].size() >= s.size()){
                    break;
                }else {
                    sub_s = s.substr(idx + NUMBERS[i].size());
                }
                std::cout << sub_s << "\n";
            }
        }
        int output = isdigit(s[f]) ? (s[f] - '0') * 10 : n_f * 10;
        output += isdigit(s[l]) ? s[l] - '0' : n_l;
        std::cout << output << "\n";
        sum += output;
    }
    return sum;
}



int main(){
    std::cout << solve() << "\n";
    return 0;
}