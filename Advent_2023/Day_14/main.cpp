#include <cerrno>
#include <codecvt>
#include <iostream>
#include <vector>

#include "../../Utils/utils.cpp"

enum GridTile {
  OvalRock,
  RectangleRock,
  EmptySpace,
};

std::vector<std::vector<GridTile>>
initiliaz_grid(std::vector<std::string> lines) {
  std::vector<std::vector<GridTile>> grid;
  for (auto line : lines) {
    grid.push_back(std::vector<GridTile>());
    for (auto c : line) {
      if (c == '.') {
        grid.back().push_back(EmptySpace);
      } else if (c == '#') {
        grid.back().push_back(RectangleRock);
      } else if (c == 'O') {
        grid.back().push_back(OvalRock);
      }
    }
  }
  return grid;
}

void move_rock_north(std::vector<std::vector<GridTile>> &grid, int row,
                     int col) {
  for (int r = row; r > 0; r--) {
    if (grid[r - 1][col] == EmptySpace) {
      grid[r][col] = EmptySpace;
      grid[r - 1][col] = OvalRock;
    } else {
      break;
    }
  }
}

void move_rock_south(std::vector<std::vector<GridTile>> &grid, int row,
                     int col) {
  for (int r = row; r < grid.size() - 1; r++) {
    if (grid[r + 1][col] == EmptySpace) {
      grid[r][col] = EmptySpace;
      grid[r + 1][col] = OvalRock;
    } else {
      break;
    }
  }
}

void move_rock_west(std::vector<std::vector<GridTile>> &grid, int row,
                    int col) {
  for (int c = col; c > 0; c--) {
    if (grid[row][col - 1] == EmptySpace) {
      grid[row][col] = EmptySpace;
      grid[row][col - 1] = OvalRock;
    } else {
      break;
    }
  }
}

void move_rock_east(std::vector<std::vector<GridTile>> &grid, int row,
                    int col) {
  for (int c = col; c < grid[0].size() - 1; c++) {
    if (grid[row][col + 1] == EmptySpace) {
      grid[row][col] = EmptySpace;
      grid[row][col + 1] = OvalRock;
    } else {
      break;
    }
  }
}

void move_rocks(std::vector<std::vector<GridTile>> &grid) {
  for (int c = 0; c < grid[0].size(); c++) {
    for (int r = 0; r < grid.size(); r++) {
      if (grid[r][c] == OvalRock) {
        move_rock_north(grid, r, c);
      }
    }
  }
}

enum Direction {
  North,
  West,
  South,
  East,
};

void move_rocks_p2(std::vector<std::vector<GridTile>> &grid, Direction dir) {
  if (dir == North) {
    for (int c = 0; c < grid[0].size(); c++) {
      for (int r = 0; r < grid.size(); r++) {
        if (grid[r][c] == OvalRock) {
          move_rock_north(grid, r, c);
        }
      }
    }
  } else if (dir == South) {
    for (int c = 0; c < grid[0].size(); c++) {
      for (int r = grid.size() - 1; r >= 0; r--) {
        if (grid[r][c] == OvalRock) {
          move_rock_south(grid, r, c);
        }
      }
    }
  } else if (dir == West) {
    for (int c = 0; c < grid[0].size(); c++) {
      for (int r = 0; r < grid.size(); r++) {
        if (grid[r][c] == OvalRock) {
          move_rock_west(grid, r, c);
        }
      }
    }
  } else if (dir == East) {
    for (int c = grid[0].size() - 1; c > 0; c--) {
      for (int r = 0; r < grid.size(); r++) {
        if (grid[r][c] == OvalRock) {
          move_rock_east(grid, r, c);
        }
      }
    }
  }
}

long calculate_preassure(std::vector<std::vector<GridTile>> grid) {
  long preassure = 0;
  for (int r = 0; r < grid.size(); r++) {
    for (int c = 0; c < grid[0].size(); c++) {
      if (grid[r][c] == OvalRock) {
        preassure += grid.size() - r;
      }
    }
  }
  return preassure;
}

void print_grid(std::vector<std::vector<GridTile>> grid) {
  for (auto row : grid) {
    for (auto colum : row) {
      if (colum == EmptySpace) {
        std::cout << '.';
      } else if (colum == OvalRock) {
        std::cout << 'O';
      } else if (colum == RectangleRock) {
        std::cout << '#';
      }
    }
    std::cout << '\n';
  }
}

int main() {
  std::string input = "";
  std::vector<std::string> lines;
  while (getline(std::cin, input)) {
    lines.push_back(input);
  }
  std::vector<std::vector<GridTile>> grid = initiliaz_grid(lines);
  print_grid(grid);
  Direction dirs[4] = {North, West, South, East};
  std::cout << "\n\n";
  // Part 1 move_rocks(grid);
  for (int i = 0; i < 4; i++) {
    move_rocks_p2(grid, dirs[i]);
    print_grid(grid);
    std::cout << "\n\n";
  }

  long preassure = calculate_preassure(grid);
  std::cout << preassure << '\n';

  return 0;
}
