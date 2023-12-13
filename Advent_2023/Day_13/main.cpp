#include "../../Utils/utils.cpp"
#include <iostream>
#include <set>
#include <vector>

// Source "https://www.youtube.com/watch?v=Jzp8INWz5Z0"
int check_column_symetry(std::vector<std::string> pattern, int i) {
  int rows = pattern.size();
  int cols = pattern[0].length();
  int baddness = 0;
  for (int c = 0; c < cols; c++) {
    for (int r = 0; r < rows; r++) {
      int r2 = i * 2 + 1 - r;
      if (r2 >= 0 && r2 < rows) {
        if (pattern[r][c] != pattern[r2][c]) {
          return false;
        }
      }
    }
  }
  return true;
}

std::vector<std::string> transpose(std::vector<std::string> &pattern) {
  std::vector<std::string> copy;
  for (int i = 0; i < pattern[0].size(); i++) {
    copy.push_back("");
    for (int j = 0; j < pattern.size(); j++) {
      copy[i] += pattern[j][i];
    }
  }
  return copy;
}

int main() {
  std::vector<std::string> pattern;
  std::string input_line;
  std::set<int> positions;
  int total_sum = 0;
  while (getline(std::cin, input_line)) {
    if (input_line == "") {
      int h_pos = -1;
      int v_pos = -1;
      for (int i = 0; i < pattern.size() - 1; i++) {
        if (check_column_symetry(pattern, i)) {
          v_pos = i;
          break;
        }
      }
      std::vector<std::string> T = transpose(pattern);
      for (int i = 0; i < T.size() - 1; i++) {
        if (check_column_symetry(T, i)) {
          h_pos = i;
          break;
        }
      }
      total_sum += h_pos + 1 + (v_pos + 1) * 100;
      pattern.clear();
    }

    else {
      pattern.push_back(input_line);
    }
  }
  std::cout << total_sum << '\n';
  return 0;
}
