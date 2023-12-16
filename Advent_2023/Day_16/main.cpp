#include "../../Utils/utils.cpp"
#include <cerrno>
#include <set>
#include <utility>
#include <vector>

enum GridType {
  V_Splitter,
  H_Splitter,
  Empty,
  Left_Mirror,
  Right_Mirror,
};

enum Directon {
  left,
  right,
  down,
  up,
};

void print_grid_n(std::vector<std::vector<GridType>> g) {
  for (auto l : g) {
    for (auto c : l) {
      if (c == Empty) {
        std::cout << '.';
      } else if (c == V_Splitter) {
        std::cout << '|';
      } else if (c == H_Splitter) {
        std::cout << '-';
      } else if (c == Right_Mirror) {
        std::cout << '/';
      } else if (c == Left_Mirror) {
        std::cout << '\\';
      }
    }
    std::cout << '\n';
  }
}

int count_energize(std::vector<std::vector<int>> g) {
  int count = 0;
  for (auto l : g) {
    for (auto c : l) {
      if (c) {
        count += 1;
      } else {
      }
    }
  }
  return count;
}

int print_energize(std::vector<std::vector<int>> g) {
  int count = 0;
  for (auto l : g) {
    for (auto c : l) {
      if (c) {
        std::cout << '#';
      } else {
        std::cout << '.';
      }
    }
    std::cout << '\n';
  }
  return count;
}

std::set<std::vector<std::vector<int>>> set_of_energize;
std::set<std::pair<std::pair<int, int>, Directon>> set_of_points;

bool go_through_grid(std::vector<std::vector<GridType>> grid,
                     std::vector<std::vector<int>> &energize, Directon dir,
                     int row, int col) {
  if (row < 0 || row >= grid.size() || col < 0 || col >= grid[0].size()) {
    return false;
  }

  energize[row][col] = 1;
  // print_energize(energize);
  if (set_of_points.count(std::make_pair(std::make_pair(row, col), dir)) > 0) {
    return true;
  }
  set_of_points.insert(std::make_pair(std::make_pair(row, col), dir));

  if (grid[row][col] == Empty) {
    if (dir == left) {
      go_through_grid(grid, energize, dir, row, col - 1);
    } else if (dir == right) {
      go_through_grid(grid, energize, dir, row, col + 1);
    } else if (dir == down) {
      go_through_grid(grid, energize, dir, row + 1, col);
    } else if (dir == up) {
      go_through_grid(grid, energize, dir, row - 1, col);
    }
  } else if (grid[row][col] == V_Splitter) {
    if (dir == left || dir == right) {
      go_through_grid(grid, energize, down, row + 1, col);
      go_through_grid(grid, energize, up, row - 1, col);
    } else if (dir == up || dir == down) {
      if (dir == down) {
        go_through_grid(grid, energize, dir, row + 1, col);
      } else if (dir == up) {
        go_through_grid(grid, energize, dir, row - 1, col);
      }
    }
  } else if (grid[row][col] == H_Splitter) {
    if (dir == up || dir == down) {
      go_through_grid(grid, energize, left, row, col - 1);
      go_through_grid(grid, energize, right, row, col + 1);
    } else if (dir == left || dir == right) {
      if (dir == left) {
        go_through_grid(grid, energize, dir, row, col - 1);
      } else if (dir == right) {
        go_through_grid(grid, energize, dir, row, col + 1);
      }
    }
  } else if (grid[row][col] == Left_Mirror) {
    if (dir == left) {
      go_through_grid(grid, energize, up, row - 1, col);
    } else if (dir == right) {
      go_through_grid(grid, energize, down, row + 1, col);
    } else if (dir == up) {
      go_through_grid(grid, energize, left, row, col - 1);
    } else if (dir == down) {
      go_through_grid(grid, energize, right, row, col + 1);
    }
  } else if (grid[row][col] == Right_Mirror) {
    if (dir == left) {
      go_through_grid(grid, energize, down, row + 1, col);
    } else if (dir == right) {
      go_through_grid(grid, energize, up, row - 1, col);
    } else if (dir == up) {
      go_through_grid(grid, energize, right, row, col + 1);
    } else if (dir == down) {
      go_through_grid(grid, energize, left, row, col - 1);
    }
  }
  return false;
}

void reset_energize(std::vector<std::vector<int>> &g) {
  for (auto &a : g) {
    for (auto &c : a) {
      c = false;
    }
  }
}

int main() {
  std::vector<std::string> lines = read_lines();
  std::vector<std::vector<int>> energize;
  std::vector<std::vector<GridType>> grid;
  for (auto line : lines) {
    grid.push_back(std::vector<GridType>());
    energize.push_back(std::vector<int>());
    for (auto c : line) {
      if (c == '.') {
        grid.back().push_back(Empty);
      } else if (c == '|') {
        grid.back().push_back(V_Splitter);
      } else if (c == '-') {
        grid.back().push_back(H_Splitter);
      } else if (c == '/') {
        grid.back().push_back(Right_Mirror);
      } else if (c == '\\') {
        grid.back().push_back(Left_Mirror);
      }
      energize.back().push_back(false);
    }
  }
  int max = 0;
  int cnt = 0;
  for (int i = 0; i < grid[0].size(); i++) {
    go_through_grid(grid, energize, down, 0, i);
    cnt = count_energize(energize);
    max = std::max(max, cnt);
    set_of_points.clear();
    reset_energize(energize);
  }
  std::cout << "Done first part \n";
  for (int i = 0; i < grid[0].size(); i++) {
    go_through_grid(grid, energize, up, grid.size() - 1, i);
    cnt = count_energize(energize);
    max = std::max(max, cnt);
    set_of_points.clear();
    reset_energize(energize);
  }
  std::cout << "Done second part \n";
  for (int i = 0; i < grid.size(); i++) {
    go_through_grid(grid, energize, right, i, 0);
    cnt = count_energize(energize);
    max = std::max(max, cnt);
    set_of_points.clear();
    reset_energize(energize);
  }
  std::cout << "Done third part \n";
  for (int i = 0; i < grid.size(); i++) {
    go_through_grid(grid, energize, left, i, grid[0].size() - 1);
    cnt = count_energize(energize);
    max = std::max(max, cnt);
    set_of_points.clear();
    reset_energize(energize);
  }

  std::cout << max << '\n';
  // Part 1
  // go_through_grid(grid, energize, right, 0, 0);
  // int cnt = count_energize(energize);
  // std::cout << cnt << '\n';
  // print_grid_n(grid);
  // std::cout << '\n';
  // print_energize(energize);

  return 0;
}
