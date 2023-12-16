#include "../../Utils/utils.cpp"
#include <memory>
#include <set>
#include <utility>
#include <vector>

enum LightState {
  On,
  Off,
};

int num_on_neighbours(std::vector<std::vector<LightState>> grid, int row,
                      int col) {
  int count = 0;
  for (int r = row - 1; r <= row + 1; r++) {
    for (int c = col - 1; c <= col + 1; c++) {
      if ((r == row && c == col) || c < 0 || r < 0 || r >= grid.size() ||
          c >= grid[0].size()) {
        continue;
      } else {
        if (grid[r][c] == On) {
          count += 1;
        }
      }
    }
  }
  return count;
}

int count_on(std::vector<std::vector<LightState>> grid) {
  int counter = 0;
  for (auto row : grid) {
    for (auto col : row) {
      if (col == On) {
        counter += 1;
      }
    }
  }
  return counter;
}

std::set<std::pair<int, int>> stuck;

void update_grid(
    std::vector<std::vector<LightState>> &grid,
    std::vector<std::pair<std::pair<int, int>, LightState>> &update_lights) {
  for (auto update : update_lights) {
    if (stuck.count(update.first) > 0) {
      continue;
    }
    int row = update.first.first;
    int col = update.first.second;
    grid[row][col] = update.second;
  }
  update_lights.clear();
}

int main() {
  std::vector<std::string> lines = read_lines();
  std::vector<std::vector<LightState>> grid;

  for (auto line : lines) {
    grid.push_back(std::vector<LightState>());
    for (auto c : line) {
      if (c == '.') {
        grid.back().push_back(Off);
      } else if (c == '#') {
        grid.back().push_back(On);
      }
    }
  }
  // Part 2
  grid[0][0] = On;
  stuck.insert({0, 0});
  grid[0][grid[0].size() - 1] = On;
  stuck.insert({0, grid[0].size() - 1});
  grid[grid.size() - 1][0] = On;
  stuck.insert({grid.size() - 1, 0});
  grid[grid.size() - 1][grid[0].size() - 1] = On;
  stuck.insert({grid.size() - 1, grid[0].size() - 1});

  int num_steps = 100;
  int on_neighbours = 0;
  std::vector<std::pair<std::pair<int, int>, LightState>> update_lights;
  for (int s = 0; s < num_steps; s++) {
    //  print_grid(grid);
    for (int row = 0; row < grid.size(); row++) {
      for (int col = 0; col < grid[0].size(); col++) {
        if (grid[row][col] == On) {
          on_neighbours = num_on_neighbours(grid, row, col);
          // std::cout << on_neighbours << '\n';
          if (on_neighbours < 2 || on_neighbours > 3) {
            // grid[row][col] = Off;
            update_lights.push_back(
                std::make_pair(std::make_pair(row, col), Off));
          }
        } else if (grid[row][col] == Off) {
          on_neighbours = num_on_neighbours(grid, row, col);
          if (on_neighbours == 3) {
            // grid[row][col] = On;
            update_lights.push_back(
                std::make_pair(std::make_pair(row, col), On));
          }
        }
      }
    }
    update_grid(grid, update_lights);
  }
  int tmp = count_on(grid);
  std::cout << tmp << '\n';

  return 0;
}
