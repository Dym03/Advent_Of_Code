#include "../../Utils/utils.cpp"
#include <algorithm>
#include <chrono>
#include <deque>
#include <set>
#include <utility>
#include <vector>

template <typename T> bool fromHex(const std::string &hexValue, T &result) {
  std::stringstream ss;
  ss << std::hex << hexValue;
  ss >> result;

  return !ss.fail();
}

void print_grid(std::deque<std::deque<bool>> grid, int &count) {
  for (auto a : grid) {
    for (auto b : a) {
      if (b) {
        count += 1;
      }
      std::cout << b;
    }
    std::cout << '\n';
  }
}

std::set<std::pair<int, int>> visited;

void flood_fill(std::deque<std::deque<bool>> &grid, int row, int col) {
  if (row < 0 || row >= grid.size() || col < 0 || col >= grid[0].size() ||
      visited.count({row, col}) > 0 || grid[row][col] == true) {
    return;
  } else {
    visited.insert({row, col});
    grid[row][col] = true;
    flood_fill(grid, row + 1, col);
    flood_fill(grid, row - 1, col);
    flood_fill(grid, row, col - 1);
    flood_fill(grid, row, col + 1);
  }
}

int main() {
  std::vector<std::string> lines = read_lines();
  std::vector<std::string> tokens;
  // std::vector<std::vector<bool>> grid;
  std::deque<std::deque<bool>> grid;
  grid.push_front(std::deque<bool>());
  int height = 0;
  int y = 0;
  int x = 0;
  int width = 0;
  for (auto line : lines) {
    tokens = split(' ', line);
    int amounth = std::stoi(tokens[1]);
    if (tokens[0] == "D") {
      y += amounth;
    } else if (tokens[0] == "R") {
      x += amounth;
    } else if (tokens[0] == "L") {
      x -= amounth;
    } else if (tokens[0] == "U") {
      y -= amounth;
    }
    height = std::max(height, y);
    width = std::max(width, x);
  }
  height += 1;
  width += 1;
  int actual_width = 0;
  int actual_height = 0;
  x = 0;
  y = 0;
  for (auto line : lines) {
    tokens = split(' ', line);
    // Part 1
    int amounth = std::stoi(tokens[1]);
    // Part 2
    // int amounth = 0;
    // tokens[0] = tokens[2][tokens[2].length() - 2];
    // tokens[2] = tokens[2].substr(0, tokens[2].length() - 2);
    // fromHex(tokens[2].substr(2), amounth);
    // if (tokens[0] == "0") {
    //  tokens[0] = "R";
    //} else if (tokens[0] == "1") {
    //  tokens[0] = "D";
    //} else if (tokens[0] == "2") {
    //  tokens[0] = "L";
    //} else if (tokens[0] == "3") {
    //  tokens[0] = "U";
    //}

    if (tokens[0] == "D") {
      for (int i = 0; i < amounth; i++) {
        y += 1;
        if (y < grid.size()) {
          grid[y][x] = true;
        } else {
          grid.push_back(std::deque<bool>(actual_width));
          grid[y][x] = true;
          actual_height += 1;
        }
      }
    } else if (tokens[0] == "R") {
      for (int i = 0; i < amounth; i++) {
        x += 1;
        if (x < grid[y].size()) {
          grid[y][x] = true;
        } else {
          for (int i = 0; i < grid.size(); i++) {
            if (grid[i].size() <= x) {
              grid[i].push_back(false);
            }
          }
          x = grid[y].size() - 1;
          grid[y][x] = true;
          actual_width += 1;
        }
      }
    } else if (tokens[0] == "L") {
      for (int i = 0; i < amounth; i++) {
        x -= 1;
        if (x >= 0) {
          grid[y][x] = true;
        } else {
          x = 0;
          for (int i = 0; i < grid.size(); i++) {
            grid[i].push_front(false);
          }
          grid[y][x] = true;
          actual_width += 1;
        }
      }
    } else if (tokens[0] == "U") {
      for (int i = 0; i < amounth; i++) {
        y -= 1;
        if (y >= 0) {
          grid[y][x] = true;
        } else {
          grid.push_front(std::deque<bool>(actual_width));
          y = 0;
          grid[y][x] = true;
        }
      }
    }
    //  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  }
  // for (int i = 51; i < 56; i++) {
  // grid[319][i] = true;
  //}
  // grid[231][44] = true;
  int c = 0;
  // flood_fill(grid, 5, 216);
  print_grid(grid, c);
  std::cout << c << '\n';

  return 0;
}
