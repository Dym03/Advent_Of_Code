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

void move_rock(std::vector<std::vector<GridTile>> &grid, int row, int col) {
  for (int r = row; r > 0; r--) {
    if (grid[r - 1][col] == EmptySpace) {
      grid[r][col] = EmptySpace;
      grid[r - 1][col] = OvalRock;
    } else {
      break;
    }
  }
}

void move_rocks(std::vector<std::vector<GridTile>> &grid) {
  for (int c = 0; c < grid[0].size(); c++) {
    for (int r = 0; r < grid.size(); r++) {
      if (grid[r][c] == OvalRock) {
        move_rock(grid, r, c);
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

int main() {
  std::string input = "";
  std::vector<std::string> lines;
  while (getline(std::cin, input)) {
    lines.push_back(input);
  }
  std::vector<std::vector<GridTile>> grid = initiliaz_grid(lines);
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

  std::cout << "\n\n";
  move_rocks(grid);

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

  long preassure = calculate_preassure(grid);
  std::cout << preassure << '\n';

  return 0;
}
