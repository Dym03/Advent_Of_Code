#include "../../Utils/utils.cpp"
#include <cstddef>
#include <cstring>
#include <iostream>

void init_grid(bool grid[1000][1000]) {
  for (int i = 0; i < 1000; i++) {
    memset(grid[i], 0, 1000);
  }
}

void init_grid(int grid[1000][1000]) {
  for (int i = 0; i < 1000; i++) {
    for (int j = 0; j < 1000; j++) {
      grid[i][j] = 0;
    }
  }
}

int main() {
  std::string input;
  std::vector<std::string> tokens;
  std::vector<std::string> starting_nums;
  std::string instruction;
  std::vector<std::string> ending_nums;
  // bool grid[1000][1000];
  int grid[1000][1000];
  init_grid(grid);
  while (getline(std::cin, input)) {
    tokens = split(' ', input);
    if (tokens.size() == 5) {
      instruction.append(tokens[0]);
      instruction.append(tokens[1]);
      starting_nums = split(',', tokens[2]);
      ending_nums = split(',', tokens[4]);
      std::cout << instruction << "\n";
    } else {
      instruction.append(tokens[0]);
      starting_nums = split(',', tokens[1]);
      ending_nums = split(',', tokens[3]);
      std::cout << instruction << "\n";
    }
    if (instruction == "toggle") {
      for (int i = std::stoi(starting_nums[0]); i <= std::stoi(ending_nums[0]);
           i++) {
        for (int j = std::stoi(starting_nums[1]);
             j <= std::stoi(ending_nums[1]); j++) {
          //  if (grid[i][j]) {
          //    grid[i][j] = false;
          //  } else {
          //    grid[i][j] = true;
          //  }
          grid[i][j] += 2;
        }
      }
    } else if (instruction == "turnoff") {
      for (int i = std::stoi(starting_nums[0]); i <= std::stoi(ending_nums[0]);
           i++) {
        for (int j = std::stoi(starting_nums[1]);
             j <= std::stoi(ending_nums[1]); j++) {
          // grid[i][j] = false;
          if (grid[i][j] != 0) {
            grid[i][j] -= 1;
          }
        }
      }
    } else {
      for (int i = std::stoi(starting_nums[0]); i <= std::stoi(ending_nums[0]);
           i++) {
        for (int j = std::stoi(starting_nums[1]);
             j <= std::stoi(ending_nums[1]); j++) {
          // grid[i][j] = true;
          grid[i][j] += 1;
        }
      }
    }
    instruction.clear();
  }
  long sum = 0;
  for (int i = 0; i < 1000; i++) {
    for (int j = 0; j < 1000; j++) {
      if (grid[i][j]) {
        sum += grid[i][j];
      }
    }
  }

  std::cout << sum << "\n";
}
