#include "../../Utils/utils.cpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

std::unordered_set<std::string> possible_comb;

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
  for (int i = 0; i < 4; i++) {
    input += '?' + input;
    for (int j = 0; j < initial_size; j++) {
      groups.push_back(groups[j]);
    }
  }
  std::cout << input << '\n';
  for (auto grp : groups) {
    std::cout << grp << ' ';
  }
  std::cout << '\n';
}

int main() {
  std::string input;
  std::vector<std::string> tokens;
  std::string left_side;
  std::vector<int> groups;
  int total_sum = 0;
  while (getline(std::cin, input)) {
    tokens = split(' ', input);
    left_side = tokens[0];
    tokens = split(',', tokens[1]);
    for (int i = 0; i < tokens.size(); i++) {
      groups.push_back(std::stoi(tokens[i]));
    }
    generate_five_times_longer(left_side, groups);
    total_sum += generate_combinations(left_side, groups);
    groups.clear();
    possible_comb.clear();
  }
  std::cout << total_sum << '\n';
  return 0;
}
