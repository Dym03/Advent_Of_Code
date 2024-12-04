#include <regex>

#include "../../Utils/utils.cpp"

int main() {
    std::vector<std::string> lines = read_lines();
    std::regex re(R"(mul\([0-9]{1,3},[0-9]{1,3}\)|do\(\)|don't\(\))");
    std::regex do_regex(R"(do\(\))");
    std::regex dont_regex(R"(don't\(\))");
    long long total = 0;
    bool adding = true;

    for (std::string line : lines) {
        std::smatch match;
        while (std::regex_search(line, match, re)) {
            std::string mul = match[0];
            std::cout << mul << '\n';
            line = match.suffix().str();
            if (std::regex_search(mul, do_regex)) {
                adding = true;
            } else if (std::regex_search(mul, dont_regex)) {
                adding = false;
            } else {
                std::vector<std::string> nums = split(',', mul.substr(4, mul.size() - 5));
                if (adding) {
                    int num1 = std::stoi(nums[0]);
                    int num2 = std::stoi(nums[1]);
                    total += num1 * num2;
                }
            }
        }
    }

    std::cout << "Total: " << total << std::endl;
    return 0;
}
