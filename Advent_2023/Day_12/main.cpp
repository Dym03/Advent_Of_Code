#include "../../Utils/utils.cpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

std::unordered_set<std::string> possible_comb;

template <typename T> struct hash_vector {
  size_t operator()(const std::vector<T> &v) const {
    size_t hash = 0;
    for (const T &elem : v) {
      // Combine the hash value with a constant value
      hash ^= std::hash<T>{}(elem) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
  }
};

struct PairHash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &p) const {
    auto h1 = std::hash<T1>{}(p.first);
    auto h2 = hash_vector<typename T2::value_type>{}(p.second);

    // A simple combination of the two hash values
    return h1 ^ h2;
  }
};

std::map<std::pair<std::string, std::vector<int>>, long> cache;

long count_combinations(std::string input, std::vector<int> groups) {
  if (input.empty()) {
    return groups.empty() ? 1 : 0;
  }

  if (groups.empty()) {
    return input.find('#') != std::string::npos ? 0 : 1;
  }

  long result = 0;
  // if (cache.find(std::make_pair(input, groups)) != cache.end()) {
  // return cache[std::make_pair(input, groups)];
  //}

  if (input[0] == '.' || input[0] == '?') {
    result += count_combinations(input.substr(1), groups);
  }
  if (input[0] == '#' || input[0] == '?') {
    if (groups[0] <= input.length() and
        input.substr(0, groups[0]).find('.') == std::string::npos and
        (groups[0] == input.length() or input[groups[0]] != '#')) {
      int tmp = groups[0];
      groups.erase(groups.cbegin());
      if (tmp + 1 > input.length()) {
        input = "";
        result += count_combinations(input, groups);
      } else {
        result += count_combinations(input.substr(tmp + 1), groups);
      }
    }
  }
  // cache[std::make_pair(input, groups)] = result;
  return result;
}

void combinations(std::string s, int i, int n) {

  if (i == n) {
    // to check whether a string is valid combination
    bool is_combination = true;
    for (int j = 0; j < n; j++) {
      if (s[j] == '?') {
        is_combination = false;
        break;
      }
    }

    if (is_combination && possible_comb.find(s) == possible_comb.end()) {
      possible_comb.insert(s);
    }
    return;
  }

  if (s[i] == '?') {
    s[i] = '#';
    combinations(s, i + 1, n);
    s[i] = '.';
    combinations(s, i + 1, n);
  } else {
    combinations(s, i + 1, n);
  }
}

bool check_groups(std::string input, std::vector<int> groups) {
  std::vector<std::string> unknown = split('.', input);
  if (unknown.size() != groups.size()) {
    return false;
  }
  for (int i = 0; i < unknown.size(); i++) {
    for (int j = i; j < groups.size(); j++) {
      if (i == j && unknown[i].length() == groups[i]) {
        break;
      } else {
        return false;
      }
    }
  }
  return true;
}

int generate_combinations(std::string input, std::vector<int> groups) {
  int counter = 0;
  combinations(input, 0, input.size());
  for (auto a : possible_comb) {
    if (check_groups(a, groups)) {
      counter += 1;
    }
  }
  return counter;
}

void generate_five_times_longer(std::string &input, std::vector<int> &groups) {
  std::string tmp = input;
  int initial_size = groups.size();
  for (int i = 0; i < 2; i++) {
    input += '?' + tmp;
    for (int j = 0; j < initial_size; j++) {
      groups.push_back(groups[j]);
    }
  }
}

int main() {
  std::string input;
  std::vector<std::string> tokens;
  std::string left_side;
  std::vector<int> groups;
  long total_sum = 0;
  while (getline(std::cin, input)) {
    tokens = split(' ', input);
    left_side = tokens[0];
    tokens = split(',', tokens[1]);
    for (int i = 0; i < tokens.size(); i++) {
      groups.push_back(std::stoi(tokens[i]));
    }
    generate_five_times_longer(left_side, groups);
    // std::cout << left_side << '\n';
    // total_sum += generate_combinations(left_side, groups);
    total_sum += count_combinations(left_side, groups);
    std::cout << total_sum << '\n';
    groups.clear();
    possible_comb.clear();
  }
  std::cout << total_sum << '\n';
  return 0;
}
