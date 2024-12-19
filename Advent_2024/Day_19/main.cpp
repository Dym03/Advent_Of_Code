#include <algorithm>  // std::count
#include <functional>
#include <iostream>  // std::cout, std::endl
#include <iostream>
#include <numeric>
#include <set>
#include <unordered_map>
#include <vector>  // std::vector

#include "../../Utils/utils.cpp"

std::unordered_map<std::string, long long> cache;

long find_solutions_p2(std::string pattern, std::set<std::string>& unique_patterns, size_t max_pattern_length) {
    if (pattern.size() == 0) {
        return 1;
    }
    if (cache.count(pattern) > 0) {
        return cache[pattern];
    }

    std::string act_substr = "";
    std::vector<long> solutions = {};
    for (int i = 0; i < max_pattern_length; i++) {
        act_substr += pattern[i];
        if (unique_patterns.count(act_substr) > 0) {
            solutions.push_back(find_solutions_p2(pattern.substr(i + 1), unique_patterns, max_pattern_length));
            cache[pattern] += solutions.back();
        }
    }

    return std::accumulate(solutions.begin(), solutions.end(), 0L);
}

int main() {
    std::vector<std::string> lines = read_lines();
    bool first_line = true;
    size_t count = 0;
    size_t count_p2 = 0;
    size_t max_pattern_length = 0;
    std::string a = "a";
    std::set<std::string> unique_patterns;
    for (int i = 0; i < lines.size(); i++) {
        if (first_line) {
            std::vector<std::string> patterns = split(',', lines[i]);
            for (int j = 0; j < patterns.size(); j++) {
                trim(patterns[j]);
                max_pattern_length = std::max(max_pattern_length, patterns[j].size());
                unique_patterns.insert(patterns[j]);
            }
            first_line = false;
            i += 1;
            continue;
        }

        long solutions = find_solutions_p2(lines[i], unique_patterns, max_pattern_length);
        if (solutions > 0) {
            count += 1;
        }
        count_p2 += solutions;
    }
    std::cout << "Part 1 = " << count << std::endl;
    std::cout << "Part 2 = " << count_p2 << std::endl;

    return 0;
}
