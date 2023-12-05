#include <iostream>
#include <set>
#include <sstream>
#include <vector>
#include <ranges>
#include <string_view>

struct Number {
    int value;
    int start_idx;
    int end_idx;
    bool added = false;
};

int main() {
    std::set set_numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    std::string numbers = "123456789";
    std::string line;
    std::string line_copy;
    int line_lenght = 0;

    int sum = 0;
    int sum2 = 0;
    std::set<char> special_char = {'*', '/', '=', '+', '-', '@', '#', '$', '%', '^', '&', '(', ')', '_', '{', '}', '[', ']', '|', ':', ';', '\\', '<', '>', '?', '/', '~', '!'};
    std::string special_char_str = "*=/=+-@#$%^&()_{}[]|:;\\<>?/~!";
    bool can_be_added = false;
    std::vector<std::vector<Number>> numbers_in_lines;
    std::vector<std::vector<std::pair<char, int>>> idx_of_special_chars;
    
    int adj_line[] = {-1, 0, 1};
    while (std::getline(std::cin, line)) {
        line_lenght = line.size();
        numbers_in_lines.push_back(std::vector<Number>());
        idx_of_special_chars.push_back(std::vector<std::pair<char, int>>());
        line_copy = line;
        // Read the numbers in the line
        while (line_copy.find_first_of(numbers) != std::string::npos) {
            can_be_added = false;
            int idx = line_copy.find_first_of(numbers) + (line.size() - line_copy.size());
            std::cout << idx << std::endl;
            Number number;
            number.start_idx = idx;

            while (set_numbers.count(line[idx] - '0') != 0) {
                idx++;
            }

            number.end_idx = idx;
            if (idx != line.size()) {
                if (special_char.count(line[idx])) {
                    can_be_added = true;
                }
            }

            number.value = std::stoi(line.substr(number.start_idx, number.end_idx - number.start_idx));
            
            numbers_in_lines.back().push_back(number);
            
            std::cout << number.value << " " << number.start_idx << " " << number.end_idx << std::endl;
            if (line.size() > number.end_idx + 1) {
                line_copy = line.substr(number.end_idx + 1);
            } else {
                break;
            }
        }
        line_copy = line;
        // Read the special characters in the line
        while (line_copy.find_first_of(special_char_str) != std::string::npos) {
            int idx = line_copy.find_first_of(special_char_str) + (line.size() - line_copy.size());
            idx_of_special_chars.back().push_back(std::make_pair(line[idx], idx));
            line_copy = line.substr(idx + 1);
        }
    }

    // first part
    for (int i = 0; i < idx_of_special_chars.size(); i++) {
        for (int j = 0; j < idx_of_special_chars[i].size(); j++) {
            int idx = idx_of_special_chars[i][j].second;
            for (int l : adj_line) {
                int line_idx = i;
                if (i + l >= 0 && i + l < numbers_in_lines.size()) {
                    line_idx = i + l;
                } else {
                    continue;
                }
                for (int k = 0; k < numbers_in_lines[line_idx].size(); k++) {
                    int lower_bound = numbers_in_lines[line_idx][k].start_idx > 0 ? numbers_in_lines[line_idx][k].start_idx - 1 : 0;
                    int upper_bound = numbers_in_lines[line_idx][k].end_idx < line_lenght ? numbers_in_lines[line_idx][k].end_idx : line_lenght - 1;
                    if (lower_bound <= idx && upper_bound >= idx) {
                        if (numbers_in_lines[line_idx][k].added == false) {
                            sum += numbers_in_lines[line_idx][k].value;
                            numbers_in_lines[line_idx][k].added = true;
                        }
                    }
                }
            }
        }
    }
    std::vector<int> nums_of_neighbours;
    for (int i = 0; i < idx_of_special_chars.size(); i++) {
        for (int j = 0; j < idx_of_special_chars[i].size(); j++) {
            int neighbours = 0;
            nums_of_neighbours.clear();
            if (idx_of_special_chars[i][j].first == '*') {
                int idx = idx_of_special_chars[i][j].second;
                for (int l : adj_line) {
                    int line_idx = i;
                    if (i + l >= 0 && i + l < numbers_in_lines.size()) {
                        line_idx = i + l;
                    } else {
                        continue;
                    }
                    for (int k = 0; k < numbers_in_lines[line_idx].size(); k++) {
                        int lower_bound = numbers_in_lines[line_idx][k].start_idx > 0 ? numbers_in_lines[line_idx][k].start_idx - 1 : 0;
                        int upper_bound = numbers_in_lines[line_idx][k].end_idx < line_lenght ? numbers_in_lines[line_idx][k].end_idx : line_lenght - 1;
                        if (lower_bound <= idx && upper_bound >= idx) {
                            neighbours++;
                            nums_of_neighbours.push_back(numbers_in_lines[line_idx][k].value);
                        }
                    }
                }
            }
            if (neighbours == 2) {
                sum2 += nums_of_neighbours[0] * nums_of_neighbours[1];
            }
        }
    }

    std::cout << sum << std::endl;
    std::cout << sum2 << std::endl;

    return 0;
}