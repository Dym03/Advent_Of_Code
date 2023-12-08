#include <iostream>
#include <map>

#include "../../Utils/utils.cpp"

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
  while (getline(std::cin, input)) {
    tokens = split('=', input);
    for (auto token : tokens) {
      trim(token);
    }
    map_name = tokens[0].substr(0, 3);
    left_right_tokens = split(',', tokens[1]);
    left_right.first = left_right_tokens[0].substr(2);
    left_right.second = left_right_tokens[1].substr(1, 3);
    map[map_name] = left_right;
  }

  std::string actual_position = "AAA";
  long counter = 1;
  for (int i = 0; i <= instructions.length(); i++, counter++) {
    if (i == instructions.length()) {
      i = 0;
    }
    left_right = map[actual_position];
    if ((instructions[i]) == 'L') {
      actual_position = left_right.first;
    } else if (instructions[i] == 'R') {
      actual_position = left_right.second;
    }
    if (actual_position == "ZZZ") {
      std::cout << counter << "\n";
      break;
    }
  }
}
