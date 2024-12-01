#include <algorithm>

#include "../../Utils/utils.cpp"

int main() {
    std::vector<std::string> lines = read_lines();
    std::vector<int> left_nums = {};
    std::vector<int> right_nums = {};
    for (auto line : lines) {
        std::vector<std::string> words = split(' ', line);
        left_nums.push_back(std::stoi(words[0]));
        right_nums.push_back(std::stoi(words[1]));
    }
    std::sort(left_nums.begin(), left_nums.end());
    std::sort(right_nums.begin(), right_nums.end());
    // part_1
    size_t sum_dist = 0;
    for (int i = 0; i < left_nums.size(); i++) {
        sum_dist += std::abs(left_nums[i] - right_nums[i]);
    }
    std::cout << *right_nums.begin() << " " << *(right_nums.end() - 1) << std::endl;
    std::cout << sum_dist << std::endl;
    // part 2
    size_t sum_similarities = 0;
    for (auto num : left_nums) {
        int count = 0;

        for (auto i = std::find(right_nums.begin(), right_nums.end(), num); i < std::find(right_nums.begin(), right_nums.end(), num + 1); i++) {
            if (*i > num) {
                break;
            }
            count += 1;
        }
        sum_similarities += num * count;
    }
    std::cout << sum_similarities << std::endl;

    return 0;
}
