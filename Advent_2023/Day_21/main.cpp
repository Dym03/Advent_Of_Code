#include "../../Utils/utils.cpp"
#include <chrono>
#include <set>
#include <thread>
#include <utility>
#include <vector>

struct GridPos {
  int row;
  int col;
};

void print_grid_pos(GridPos pos) {
  std::cout << pos.row << ' ' << pos.col << '\n';
}

enum GridTile {
  GardenPlot,
  Rock,
};

std::vector<std::vector<bool>> standing;

int print_standing(std::vector<std::vector<bool>> standing,
                   std::vector<std::vector<GridTile>> grid) {
  int count = 0;
  for (int i = 0; i < standing.size(); i++) {
    for (int j = 0; j < standing[i].size(); j++) {
      if (standing[i][j]) {
        count += 1;
        std::cout << 'O';
      } else if (grid[i][j] == Rock) {
        std::cout << '#';
      } else {
        std::cout << '.';
      }
    }
    std::cout << '\n';
  }
  return count;
}

std::set<int> visited_steps;
std::set<std::pair<std::pair<int, int>, int>> visited_configs;

void go_through_grid(std::vector<std::vector<GridTile>> grid, int row, int col,
                     int step, int max_steps) {
  if (row < 0 || row >= grid.size() || col < 0 || col >= grid[0].size() ||
      grid[row][col] == Rock || visited_configs.count({{row, col}, step}) > 0) {
    return;
  } else if (step == max_steps) {
    standing[row][col] = true;
    return;
  } else {
    visited_configs.insert({{row, col}, step});
    go_through_grid(grid, row + 1, col, step + 1, max_steps);
    go_through_grid(grid, row - 1, col, step + 1, max_steps);
    go_through_grid(grid, row, col - 1, step + 1, max_steps);
    go_through_grid(grid, row, col + 1, step + 1, max_steps);
    // std::cout << "This is the step number" << step << " \n";
    // print_standing(standing, grid);
    // std::cout << '\n';
    // if (visited_steps.count(step) == 0) {
    //  std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // visited_steps.insert(step);
    //}
  }
}

int main() {
  std::vector<std::string> lines = read_lines();
  std::vector<std::vector<GridTile>> grid;
  GridPos start;
  for (int row = 0; row < lines.size(); row++) {
    grid.push_back(std::vector<GridTile>());
    for (int col = 0; col < lines[row].length(); col++) {
      if (lines[row][col] == 'S') {
        start.row = row;
        start.col = col;
        grid.back().push_back(GardenPlot);
      } else if (lines[row][col] == '#') {
        grid.back().push_back(Rock);
      } else if (lines[row][col] == '.') {
        grid.back().push_back(GardenPlot);
      }
    }
  }
  for (int i = 0; i < grid.size(); i++) {
    standing.push_back(std::vector<bool>(grid[i].size()));
  }

  // print_grid(grid, '\0');
  go_through_grid(grid, start.row, start.col, 0, 10);

  // print_grid(standing, '\0');
  int count = print_standing(standing, grid);
  std::cout << count << '\n';

  return 0;
}
