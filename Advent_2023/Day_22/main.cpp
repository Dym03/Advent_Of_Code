#include "../../Utils/utils.cpp"
#include <algorithm>
#include <map>
#include <set>
#include <utility>
#include <vector>

struct Position {
  int x;
  int y;
  int z;
};

enum GridTile {
  Brick,
  Empty,
  Ground,
};

class Compare {
public:
  bool operator()(std::pair<std::pair<Position, Position>, int> below,
                  std::pair<std::pair<Position, Position>, int> above) {
    if (below.first.second.z < above.first.second.z) {
      return true;
    }

    return false;
  }
};

void update_grid(std::pair<Position, Position> brick,
                 std::vector<std::vector<std::vector<GridTile>>> &grid,
                 GridTile replacement) {
  for (int x = brick.first.x; x <= brick.second.x; x++) {
    for (int y = brick.first.y; y <= brick.second.y; y++) {
      for (int z = brick.first.z; z <= brick.second.z; z++) {
        grid[x][y][z] = replacement;
      }
    }
  }
}
// Doesnt work as i though
bool is_disintegratable(std::pair<Position, Position> brick,
                        std::vector<std::vector<std::vector<GridTile>>> grid) {
  for (int z = brick.second.z; z < grid[0][0].size() - 1; z++) {
    for (int x = brick.first.x; x <= brick.second.x; x++) {
      for (int y = brick.first.y; y <= brick.second.y; y++) {
        if (grid[x][y][z + 1] == Brick) {
          return false;
        }
      }
    }
  }
  return true;
}

std::set<int> count_supported(
    std::pair<Position, Position> brick,
    std::vector<std::vector<std::vector<GridTile>>> grid,
    std::vector<std::pair<std::pair<Position, Position>, int>> bricks) {
  std::vector<Position> supported_bricks;
  std::set<int> blocks;
  // for (int z = brick.second.z; z < grid[0][0].size() - 1; z++) {
  int z = brick.second.z;
  for (int x = brick.first.x; x <= brick.second.x; x++) {
    for (int y = brick.first.y; y <= brick.second.y; y++) {
      if (grid[x][y][z + 1] == Brick) {
        for (auto b : bricks) {
          if (b.first.first.x == brick.first.x &&
              b.first.first.y == brick.first.y &&
              b.first.first.z == brick.first.z) {
            continue;
          }
          if (x >= b.first.first.x && x <= b.first.second.x &&
              y >= b.first.first.y && y <= b.first.second.y &&
              z + 1 >= b.first.first.z && z + 1 <= b.first.second.z) {
            blocks.insert(b.second);
          }
        }
      }
    }
    //}
  }

  return blocks;
}

void move_brick(std::pair<Position, Position> &brick,
                std::vector<std::vector<std::vector<GridTile>>> &grid) {
  if (brick.first.z == 1) {
    return;
  }
  bool free_below = true;
  for (int z = brick.first.z; z > 1; z--) {
    for (int x = brick.first.x; x <= brick.second.x; x++) {
      for (int y = brick.first.y; y <= brick.second.y; y++) {
        if (grid[x][y][z - 1] == Brick) {
          free_below = false;
          update_grid(brick, grid, Empty);
          int diff = brick.first.z - z;
          brick.first.z = z;
          brick.second.z -= diff;
          update_grid(brick, grid, Brick);
          return;
        }
      }
    }
  }
  if (free_below) {
    update_grid(brick, grid, Empty);
    int diff = brick.first.z - 1;
    brick.first.z = 1;
    brick.second.z -= diff;
    update_grid(brick, grid, Brick);
  }
}

int final_count(int brick_id, std::set<int> &fallen_bricks,
                std::map<int, std::set<int>> id_to_supported) {
  for (auto d : id_to_supported[brick_id]) {
    if (fallen_bricks.count(d) == 0) {
      fallen_bricks.insert(d);
    }
  }
  for (auto supported : id_to_supported[brick_id]) {
    final_count(supported, fallen_bricks, id_to_supported);
  }
  return fallen_bricks.size();
}

int main() {
  std::vector<std::string> lines = read_lines();
  std::vector<std::string> tokens;
  std::vector<std::pair<std::pair<Position, Position>, int>> bricks;
  std::vector<std::vector<std::vector<GridTile>>> grid;
  std::vector<std::string> coords;
  Position max = {0, 0, 0};
  int brick_count = 0;
  for (auto line : lines) {
    tokens = split('~', line);
    coords = split(',', tokens[0]);
    Position start = {std::stoi(coords[0]), std::stoi(coords[1]),
                      std::stoi(coords[2])};
    coords = split(',', tokens[1]);
    Position end = {std::stoi(coords[0]), std::stoi(coords[1]),
                    std::stoi(coords[2])};

    if (end.x < start.x) {
      std::swap(start.x, end.x);
    }
    if (end.y < start.y) {
      std::swap(start.y, end.y);
    }
    if (end.z < start.z) {
      std::swap(start.z, end.z);
    }

    max.x = std::max(max.x, end.x);
    max.y = std::max(max.y, end.y);
    max.z = std::max(max.z, end.z);
    bricks.push_back({{start, end}, brick_count});
    brick_count += 1;
  }

  // std::cout << max.x << ' ' << max.y << ' ' << max.z << '\n';
  for (int i = 0; i <= max.x; i++) {
    grid.push_back(std::vector<std::vector<GridTile>>());
    for (int j = 0; j <= max.y; j++) {
      grid.back().push_back(std::vector<GridTile>());
      for (int k = 0; k <= max.z; k++) {
        if (k == 0) {
          grid.back().back().push_back(Ground);
        } else {
          grid.back().back().push_back(Empty);
        }
      }
    }
  }

  for (auto brick : bricks) {
    Position start = brick.first.first;
    Position end = brick.first.second;
    for (int x = start.x; x <= end.x; x++) {
      for (int y = start.y; y <= end.y; y++) {
        for (int z = start.z; z <= end.z; z++) {
          grid[x][y][z] = Brick;
        }
      }
    }
  }
  Compare cmp;
  std::sort(bricks.begin(), bricks.end(), cmp);
  for (auto &brick : bricks) {
    move_brick(brick.first, grid);
  }
  std::map<int, std::set<int>> id_to_supported;
  std::map<int, int> id_to_num_supporters;
  for (auto brick : bricks) {
    std::set<int> bricks_supported = count_supported(brick.first, grid, bricks);
    id_to_supported[brick.second] = bricks_supported;
    for (auto id : bricks_supported) {
      id_to_num_supporters[id] += 1;
    }
  }
  int cnt = 0;
  std::set<int> crutial_ids;
  for (auto brick : bricks) {
    bool crutial = false;
    std::set<int> bricks_supported = count_supported(brick.first, grid, bricks);
    for (auto id : bricks_supported) {
      if (id_to_num_supporters[id] == 1) {
        crutial = true;
      }
    }
    if (!crutial) {
      cnt += 1;
    } else {
      crutial_ids.insert(brick.second);
    }
  }
  std::cout << "Part 1 : " << cnt << '\n';
  cnt = 0;
  std::set<int> fallen_bricks;
  for (auto brick : crutial_ids) {
    // std::set<int> bricks_supported = id_to_supported[brick];
    // for (auto id : bricks_supported) {
    // for (auto b : id_to_supported[id]) {
    // bricks_supported.insert(b);
    //}
    //}
    cnt += final_count(brick, fallen_bricks, id_to_supported);
    fallen_bricks.clear();
    std::cout << cnt << '\n';
    // cnt += fall.size() - 1;
  }

  std::cout << "Part 2 : " << cnt << '\n';
  return 0;
}
