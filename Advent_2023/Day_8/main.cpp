#include <algorithm>
#include <array>
#include <climits>
#include <iostream>
#include <list>
#include <map>

#include "../../Utils/utils.cpp"

bool check_all_locations(std::vector<std::string> locations, long counter,
                         std::array<long, 6> &needed_steps) {
  bool all = true;
  for (int i = 0; i < locations.size(); i++) {
    if (locations[i][2] != 'Z') {
      all = false;
    } else {
      needed_steps[i] = counter;
    }
  }
  return all;
}

bool check_all_needed_steps(std::array<long, 6> needed_steps) {
  for (int i = 0; i < needed_steps.size(); i++) {
    if (needed_steps[i] == 0) {
      return false;
    }
  }
  return true;
}

long gcd(long a, long b) {
  if (b == 0)
    return a;
  return gcd(b, a % b);
}

long calculate_lcm(std::array<long, 6> steps) {
  long ans = steps[0];
  for (int i = 1; i < steps.size(); i++) {
    ans = (steps[i] * ans) / std::__gcd(steps[i], ans);
  }
  return ans;
}

int main() {
  std::map<std::string, std::pair<std::string, std::string>> map;
  std::string instructions =
      "LRRLRRRLRLRLLRRLRLRLLRLRLRLLLLRRRLLRRRLRRRLRRRLLRLLRLRRLRLRLRRRLLLLRRLRL"
      "RRLRRLLRRRLRRLRLRRLRRLRRLRRLRLLRRLRRLLLLRLRLRRLLRRLLRRLRLLRLRRLRRLRRLRRR"
      "LRRLLLRRLRRRLRLRRRLLRLRRLRRRLRRLLRRRLRRLRLLRRLLRRLRRLRRRLRRLLRRLRRRLRLRL"
      "RLRLRLRRLRRLLRRRLRLRRLRRRLRLRLRLRLRLRRRLRRLRRRLLRRLRLLRRRLRRLRLLLLRRRLRR"
      "LRRRR";
  std::string input;
  std::vector<std::string> tokens;
  std::vector<std::string> left_right_tokens;
  std::string map_name;
  std::pair<std::string, std::string> left_right;
  std::vector<std::string> all_actual_locations;
  while (getline(std::cin, input)) {
    tokens = split('=', input);
    for (auto &token : tokens) {
      trim(token);
    }
    map_name = tokens[0];
    if (map_name[2] == 'A') {
      all_actual_locations.push_back(map_name);
    }
    left_right_tokens = split(',', tokens[1]);
    left_right.first = left_right_tokens[0].substr(1);
    left_right.second = left_right_tokens[1].substr(1, 3);
    map[map_name] = left_right;
  }
  std::array<long, 6> steps{0};
  long counter = 1;
  // Part 1
  // std::string actual_position = "AAA";
  //  for (int i = 0; i <= instructions.length(); i++, counter++) {
  //    if (i == instructions.length()) {
  //      i = 0;
  //    }
  //    left_right = map[actual_position];
  //    if ((instructions[i]) == 'L') {
  //      actual_position = left_right.first;
  //    } else if (instructions[i] == 'R') {
  //      actual_position = left_right.second;
  //    }
  //    if (actual_position == "ZZZ") {
  //      std::cout << counter << "\n";
  //      break;
  //    }
  //  }
  for (int i = 0; i <= instructions.length(); i++, counter++) {
    if (i == instructions.length()) {
      i = 0;
    }
    if ((instructions[i]) == 'L') {
      for (auto &actual_position : all_actual_locations) {
        left_right = map[actual_position];
        actual_position = left_right.first;
      }
    } else if (instructions[i] == 'R') {
      for (auto &actual_position : all_actual_locations) {
        left_right = map[actual_position];
        actual_position = left_right.second;
      }
    }
    if (check_all_locations(all_actual_locations, counter, steps) ||
        check_all_needed_steps(steps)) {
      std::cout << counter << "\n";
      break;
    }
  }
  for (auto a : steps) {
    std::cout << a << " ";
  }
  std::cout << "\n";

  std::cout << calculate_lcm(steps);
}
