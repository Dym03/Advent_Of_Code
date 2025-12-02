#include <algorithm>
#include <climits>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>

#include "../../Utils/utils.cpp"

std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> adj_list_directional = {
    {"A", {{">", "v"}, {"^", "<"}}},
    {"^", {{"v", "v"}, {"A", ">"}}},
    {"v", {{">", ">"}, {"<", "<"}, {"^", "^"}}},
    {">", {{"A", "^"}, {"v", "<"}}},
    {"<", {{"v", ">"}}}};

std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> adj_list_numeric = {
    {"A", {{"0", "<"}, {"3", "^"}}},
    {"0", {{"A", ">"}, {"2", "^"}}},
    {"1", {{"4", "^"}, {"2", ">"}}},
    {"2", {{"0", "v"}, {"3", ">"}, {"1", "<"}, {"5", "^"}}},
    {"3", {{"A", "v"}, {"2", "<"}, {"6", "^"}}},
    {"4", {{"1", "v"}, {"5", ">"}, {"7", "^"}}},
    {"5", {{"2", "v"}, {"4", "<"}, {"6", ">"}, {"8", "^"}}},
    {"6", {{"3", "v"}, {"5", "<"}, {"9", "^"}}},
    {"7", {{"4", "v"}, {"8", ">"}}},
    {"8", {{"5", "v"}, {"7", "<"}, {"9", ">"}}},
    {"9", {{"6", "v"}, {"8", "<"}}}};

std::unordered_map<std::string, std::set<std::string>> get_numeric_mappings(std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> adj_list, std::string start_node) {
    std::set<std::string> visited;
    std::unordered_map<std::string, std::set<std::string>> distance;
    std::queue<std::string> q;
    q.push(start_node);
    distance[start_node].insert("");
    while (!q.empty()) {
        std::string current_node = q.front();
        q.pop();
        visited.insert(current_node);
        for (auto edge : adj_list[current_node]) {
            if (visited.find(edge.first) == visited.end()) {
                for (auto dir : edge.second) {
                    for (auto dist : distance[current_node]) {
                        distance[edge.first].insert(dist + dir);
                    }
                }
                q.push(edge.first);
            }
        }
    }
    return distance;
}

void generatePaths(
    const std::string& test_string,
    std::unordered_map<std::string, std::unordered_map<std::string, std::set<std::string>>>& numeric_mapping,
    std::string start_key,
    std::string current_result,
    std::set<std::string>& numeric_results) {
    if (test_string.empty()) {
        numeric_results.insert(current_result);
        return;
    }

    char next_char = test_string[0];
    std::string end_key(1, next_char);

    const auto& possible_values = numeric_mapping.at(start_key).at(end_key);

    for (const auto& value : possible_values) {
        generatePaths(
            test_string.substr(1),
            numeric_mapping,
            end_key,
            current_result + value + "A",
            numeric_results);
    }

    return;
}

std::unordered_map<std::pair<std::string, int>, std::set<std::string>, hash_pair> cache;

std::set<std::string> generatePaths_2(
    const std::string& test_string,
    std::unordered_map<std::string, std::unordered_map<std::string, std::set<std::string>>>& numeric_mapping,
    std::string start_key,
    std::string current_result, int n) {
    std::set<std::string> results;
    if (test_string.empty()) {
        results.insert(current_result);
        return results;
    }

    if (cache.count({test_string, n}) > 0) {
        // std::cout << test_string << " found in cache" << std::endl;
        // for (auto r : cache[{test_string, n}]) {
        //     std::cout << r << std::endl;
        // }
        return cache[{test_string, n}];
    }

    char next_char = test_string[0];
    std::string end_key(1, next_char);

    const auto& possible_values = numeric_mapping.at(start_key).at(end_key);
    for (const auto& value : possible_values) {
        auto res = generatePaths_2(
            test_string.substr(1),
            numeric_mapping,
            end_key,
            current_result + value + "A", n);
        for (auto r : res) {
            results.insert(r);
        }
    }

    cache[{test_string, n}] = results;

    return results;
}

long go(int n, std::unordered_map<std::string, std::unordered_map<std::string, std::set<std::string>>>& numeric_mapping, std::unordered_map<std::string, std::unordered_map<std::string, std::set<std::string>>>& directional_mapping, std::set<std::string> results) {
    if (n == 0) {
        long min_length = LONG_MAX;
        for (auto result : results) {
            min_length = std::min(min_length, (long)result.size());
        }
        return min_length;
    }
    std::set<std::string> new_results;
    std::set<std::string> filtered_results;
    for (auto result : results) {
        auto res = generatePaths_2(result, directional_mapping, "A", "", n);
        for (auto r : res) {
            new_results.insert(r);
        }
        size_t min_size = LONG_MAX;
        for (auto new_result : new_results) {
            if (min_size > new_result.size()) {
                min_size = new_result.size();
                filtered_results.clear();
                filtered_results.insert(new_result);
            } else if (min_size == new_result.size()) {
                filtered_results.insert(new_result);
            }
        }
    }

    return go(n - 1, numeric_mapping, directional_mapping, filtered_results);
}

int main() {
    std::vector<std::string> lines = read_lines();
    std::vector<std::string> keys = {"A", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    std::vector<std::string> directions = {"<", ">", "^", "v", "A"};
    std::unordered_map<std::string, std::unordered_map<std::string, std::set<std::string>>> numeric_mapping;
    std::unordered_map<std::string, std::unordered_map<std::string, std::set<std::string>>> directional_mapping;
    for (auto key : keys) {
        numeric_mapping[key] = get_numeric_mappings(adj_list_numeric, key);
    }
    for (auto key : directions) {
        directional_mapping[key] = get_numeric_mappings(adj_list_directional, key);
    }

    long total = 0;
    for (auto line : lines) {
        std::string test_string = line;
        int number = std::stoi(test_string.substr(0, 3));
        std::string start_key = "A";
        std::set<std::string> numeric_results;
        generatePaths(test_string, numeric_mapping, start_key, "", numeric_results);  // Generate all possible numeric paths
        long min_result = go(3, numeric_mapping, directional_mapping, numeric_results);
        // std::set<std::string> directional_results;
        // for (auto result : numeric_results) {
        //     generatePaths(result, directional_mapping, start_key, "", directional_results);  // Generate all possible directional paths
        // }
        // std::set<std::string> directional_results_2;
        // for (auto result : directional_results) {
        //     generatePaths(result, directional_mapping, start_key, "", directional_results_2);  // Generate all possible directional paths for the directional paths
        // }
        // int min_length = INT_MAX;
        // for (auto result : directional_results_2) {
        //     min_length = std::min(min_length, (int)result.size());
        // }
        // std::cout << "Min length: " << min_length << std::endl;
        // std::cout << "Result for this line = " << number * min_length << std::endl;
        total += number * min_result;
    }
    std::cout << "Total: " << total << std::endl;

    return 0;
}
