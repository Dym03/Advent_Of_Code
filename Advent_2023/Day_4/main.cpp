#include <iostream>
#include <map>
#include <set>
#include <vector>
int main() {
    std::string input;
    std::string winning_nums;
    std::string my_nums;
    std::set<int> winning_set;
    std::vector<int> my_nums_vec;
    std::map<int, int> my_nums_map;
    int actual_card = 0;
    int count = 0;
    int total_sum = 0;
    int total_cards = 0;
    int sum = 0;
    while (getline(std::cin, input)) {
        count = 0;
        winning_set.clear();
        my_nums_vec.clear();
        actual_card = std::stoi(input.substr(input.find_first_of(" ") + 1, input.find_first_of(':') - 1));
        my_nums_map[actual_card] += 1;
        input = input.substr(input.find_first_of(':') + 2, input.length());
        winning_nums = input.substr(0, input.find_first_of('|'));
        while (winning_nums.find_first_of(' ') != std::string::npos) {
            if (winning_nums.find_first_of(' ') == 0) {
                winning_nums = winning_nums.substr(1, winning_nums.length());
            }
            winning_set.insert(std::stoi(winning_nums.substr(0, winning_nums.find_first_of(' '))));
            winning_nums = winning_nums.substr(winning_nums.find_first_of(' ') + 1, winning_nums.length());
        }
        my_nums = input.substr(input.find_first_of('|') + 2, input.length());
        my_nums += " ";

        while (my_nums.find_first_of(' ') != std::string::npos) {
            if (my_nums.find_first_of(' ') == 0) {
                my_nums = my_nums.substr(1, my_nums.length());
            }
            int temp = std::stoi(my_nums.substr(0, my_nums.find_first_of(' ')));
            my_nums_vec.push_back(temp);
            if (winning_set.count(temp) != 0) {
                count++;
            }
            my_nums = my_nums.substr(my_nums.find_first_of(' ') + 1, my_nums.length());
        }
        // 
        // Part 1
        //  if(count >= 1){
        //          sum = 1;
        //          for(int i = 1; i < count; i++){
        //              sum *= 2;
        //          }
        //  }
        //  total_sum += sum;
        //  sum = 0;
        //} 
        //Part 2
        for (int i = 1; i < count + 1; i++) {
            my_nums_map[actual_card + (i)] += 1 * my_nums_map[actual_card];
        }

    }
    for (auto i : my_nums_map) {
        total_cards += i.second;
    }
    std::cout << total_sum << std::endl;
    std::cout << total_cards << std::endl;
    return 0;
}
