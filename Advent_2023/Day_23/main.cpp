#include "../../Utils/utils.cpp"
#include <algorithm>
#include <map>
#include <set>
#include <vector>

enum GridTile {
  Path,
  Forest,
  IcyDown,
  IcyUp,
  IcyLeft,
  IcyRight,
};

typedef std::vector<std::vector<GridTile>> Grid;

std::set<std::pair<int, int>> step_taken;
std::map<std::pair<int, int>, int> position_to_max_path;
int max_sub_lenght = 0;
void go_through_grid(Grid grid, int row, int col,
                     std::vector<std::vector<bool>> &paths, int lenght,
                     int &max_lenght) {
  if (row < 0 || row >= grid.size() || col < 0 || col >= grid[0].size() ||
      grid[row][col] == Forest || step_taken.count({row, col}) > 0) {
    return;
  }
  step_taken.insert({row, col});
  //  if (position_to_max_path[{row, col}] != 0) {
  //    std::cout << row << ' ' << col << ' ' << position_to_max_path[{row,
  //    col}]
  //              << '\n';
  //    lenght += position_to_max_path[{row, col}];
  //    max_lenght = std::max(max_lenght, lenght);
  //    step_taken.erase({row, col});
  //    return;
  //  }
  // End of the trip
  if (grid[row][col] == Path && row == grid.size() - 1) {
    max_lenght = std::max(max_lenght, lenght);
    std::cout << max_lenght << '\n';
    step_taken.erase({row, col});
    position_to_max_path[{row, col}] = 1;
    return;
  }

  if (grid[row][col] == Path || grid[row][col] == IcyDown ||
      grid[row][col] == IcyLeft || grid[row][col] == IcyUp ||
      grid[row][col] == IcyRight) {
    go_through_grid(grid, row - 1, col, paths, lenght + 1, max_lenght);
    //    if (position_to_max_path[{row - 1, col}]) {
    //      position_to_max_path[{row, col}] =
    //          std::max(position_to_max_path[{row, col}],
    //                   position_to_max_path[{row - 1, col}] + 1);
    //    }
    go_through_grid(grid, row + 1, col, paths, lenght + 1, max_lenght);
    //    if (position_to_max_path[{row + 1, col}]) {
    //      position_to_max_path[{row, col}] =
    //          std::max(position_to_max_path[{row, col}],
    //                   position_to_max_path[{row + 1, col}] + 1);
    //    }
    go_through_grid(grid, row, col - 1, paths, lenght + 1, max_lenght);
    //    if (position_to_max_path[{row, col - 1}]) {
    //      position_to_max_path[{row, col}] =
    //          std::max(position_to_max_path[{row, col}],
    //                   position_to_max_path[{row, col - 1}] + 1);
    //    }
    go_through_grid(grid, row, col + 1, paths, lenght + 1, max_lenght);
    // if (position_to_max_path[{row, col + 1}]) {
    //   position_to_max_path[{row, col}] =
    //       std::max(position_to_max_path[{row, col}],
    //                position_to_max_path[{row, col + 1}] + 1);
    // }
    // std::cout << position_to_max_path[{row, col}] << '\n';
  } /* Part 1
  else if (grid[row][col] == IcyDown) {
    go_through_grid(grid, row + 1, col, paths, lenght + 1, max_lenght);
  } else if (grid[row][col] == IcyRight) {
    go_through_grid(grid, row, col + 1, paths, lenght + 1, max_lenght);
  } else if (grid[row][col] == IcyLeft) {
    go_through_grid(grid, row, col - 1, paths, lenght + 1, max_lenght);
  } else if (grid[row][col] == IcyUp) {
    go_through_grid(grid, row - 1, col, paths, lenght + 1, max_lenght);
  }
  */
  step_taken.erase({row, col});
}

int main() {
  std::vector<std::string> lines = read_lines();
  Grid grid;
  std::vector<std::vector<bool>> paths;
  for (auto line : lines) {
    grid.push_back(std::vector<GridTile>());
    paths.push_back(std::vector<bool>());
    for (auto ch : line) {
      if (ch == '.') {
        grid.back().push_back(Path);
      } else if (ch == '#') {
        grid.back().push_back(Forest);
      } else if (ch == '>') {
        grid.back().push_back(IcyRight);
      } else if (ch == '^') {
        grid.back().push_back(IcyUp);
      } else if (ch == '<') {
        grid.back().push_back(IcyLeft);
      } else if (ch == 'v') {
        grid.back().push_back(IcyDown);
      }
      paths.back().push_back(false);
    }
  }
  int start_x = 0;
  for (int i = 0; i < grid[0].size(); i++) {
    if (grid[0][i] == Path) {
      start_x = i;
      break;
    }
  }
  int max_lenght = 0;
  go_through_grid(grid, 0, start_x, paths, 0, max_lenght);
  std::cout << max_lenght << '\n';
  return 0;
}
