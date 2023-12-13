#include "../../Utils/utils.cpp"
#include <iostream>
#include <set>
#include <vector>

bool is_palindrome(std::string input, int &pos, int length, int &act_pos) {
  if (input.length() < 1) {
    return false;
  }
  int l = 0;
  int r = input.length() - 1;
  while (l < r) {
    if (input[l] != input[r]) {
      return false;
    }
    l += 1;
    r -= 1;
  }
  pos = l;
  act_pos = l + (length - input.length());
  return true;
}

bool left_side_equals_right_side(std::string input, int &pos, int length,
                                 int &act_pos) {
  if (is_palindrome(input, pos, length, act_pos)) {
    return true;
  } else {
    return left_side_equals_right_side(input.substr(1), pos, length, act_pos) ||
           left_side_equals_right_side(input.substr(0, input.length() - 1), pos,
                                       length, act_pos);
  }
}

bool up_side_equals_down_side(std::vector<std::string> patter, int &pos,
                              int &act_pos, std::set<int> &positions) {
  std::string tmp = "";
  for (int i = 0; i < patter[0].size(); i++) {
    for (int j = 0; j < patter.size(); j++) {
      tmp += patter[j][i];
    }
    // std::cout << tmp;
    if (left_side_equals_right_side(tmp, pos, tmp.length(), act_pos)) {
      positions.insert(pos);
      // std::cout << "True " << pos << '\n';
    }
    tmp.clear();
  }
  return true;
}

int main() {
  std::vector<std::string> pattern;
  std::string input_line;
  std::set<int> positions;
  int pos = 0;
  int act_pos = 0;
  int total_sum = 0;
  while (getline(std::cin, input_line)) {
    if (input_line == "") {
      for (int i = 0; i < pattern.size(); i++) {
        if (left_side_equals_right_side(pattern[i], pos, pattern[i].length(),
                                        act_pos)) {
          positions.insert(pos);
        }
      }
      // std::cout << '\n';
      if (positions.size() == 1) {
        for (auto a : pattern) {
          std::cout << a << '\n';
        }
        total_sum += act_pos;
        std::cout << "Lines Mirror on index " << act_pos << "\n\n";
      } else {

        //  for (auto a : pattern) {
        //  std::cout << a << '\n';
        // }
        positions.clear();
        up_side_equals_down_side(pattern, pos, act_pos, positions);

        if (positions.size() == 1) {
          for (auto a : pattern) {
            std::cout << a << '\n';
          }
          total_sum += 100 * act_pos;
          std::cout << "Lines Mirror Verticaly on index " << act_pos << "\n\n";
        }
      }
      positions.clear();
      pattern.clear();
    } else {
      pattern.push_back(input_line);
    }
  }
  std::cout << total_sum << "\n";
  return 0;
}
