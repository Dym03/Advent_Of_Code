#include "../../Utils/utils.cpp"
#include "set"
#include "unordered_map"

int line_safe(std::vector<int> line, std::unordered_map<int, std::vector<int>> rules) {
    std::set<int> visited;
    for (auto i : line) {
        for (auto j : rules[i]) {
            if (visited.find(j) != visited.end()) {
                return -1;
            }
        }
        visited.insert(i);
    }

    return line[line.size() / 2];
}

int put_line_in_order(std::vector<int> line, std::unordered_map<int, std::vector<int>> rules) {
    std::vector<int> result;
    while (line_safe(line, rules) == -1) {
        std::set<int> visited = {};
        bool swapped = false;
        for (auto& i : line) {
            for (auto j : rules[i]) {
                if (visited.find(j) != visited.end()) {
                    for (auto& k : line) {
                        if (k == j) {
                            std::swap(i, k);
                            swapped = true;
                            break;
                        }
                    }
                    if (swapped) {
                        break;
                    }
                }
            }
            if (swapped) {
                break;
            }
            visited.insert(i);
        }
    }

    return line[line.size() / 2];
}

int main() {
    std::vector<std::string> lines = read_lines();
    std::unordered_map<int, std::vector<int>> rules;
    bool setting_rules = true;
    int i = 0;
    size_t sum = 0;
    size_t sum_p_2 = 0;
    for (std::string line : lines) {
        if (line.empty()) {
            setting_rules = false;
            continue;
        }
        if (setting_rules) {
            std::vector<std::string> tokens = split('|', line);
            int A = std::stoi(tokens[0]);
            int B = std::stoi(tokens[1]);
            rules[A].push_back(B);
        } else {
            std::vector<std::string> tokens = split(',', line);
            std::vector<int> int_tokens;
            for (std::string token : tokens) {
                int_tokens.push_back(std::stoi(token));
            }
            int result = line_safe(int_tokens, rules);
            if (result != -1) {
                sum += result;
            } else {
                result = put_line_in_order(int_tokens, rules);
                sum_p_2 += result;
            }
        }
        i++;
    }
    std::cout << sum << std::endl;
    std::cout << sum_p_2 << std::endl;
    return 0;
}
