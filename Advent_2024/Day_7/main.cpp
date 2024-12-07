#include <iostream>

#include "../../Utils/utils.cpp"

enum Op {
    ADD,
    MUL,
    CONCAT
};

int check_answer(size_t answer, size_t total, Op operation, std::vector<int> numbers) {
    if (numbers.size() == 0 && total == answer) {
        return 1;
    }
    if (numbers.size() == 0) {
        return 0;
    }
    if (total > answer) {
        return 0;
    }
    size_t num = numbers[0];
    numbers.erase(numbers.begin());
    if (operation == ADD) {
        total += num;
    } else if (operation == MUL) {
        total *= num;
    } else if (operation == CONCAT) {
        std::string str_total = std::to_string(total);
        std::string str_num = std::to_string(num);
        str_total += str_num;
        total = std::stol(str_total);
    }
    return check_answer(answer, total, ADD, numbers) + check_answer(answer, total, MUL, numbers) + check_answer(answer, total, CONCAT, numbers);
}

int main() {
    std::vector<std::string> lines = read_lines();
    size_t total = 0;
    for (auto line : lines) {
        std::vector<std::string> tokens = split(':', line);
        size_t answer = std::stol(tokens[0]);
        std::vector<std::string> numbers = split(' ', tokens[1]);
        std::vector<int> nums;
        for (auto number : numbers) {
            nums.push_back(std::stoi(number));
        }
        if (check_answer(answer, 0, ADD, nums) > 0) {
            total += answer;
        }
    }
    std::cout << "Total: " << total << std::endl;
    return 0;
}
