#include "../../Utils/utils.cpp"
#include <climits>
#include <cstddef>
#include <functional>
#include <list>
#include <queue>
#include <set>
#include <utility>
#include <vector>

enum Direction {
  left,
  right,
  up,
  down,
};

int best_heat = INT_MAX;

// std::set<std::pair<std::pair<int, int>, Direction>> visited_points;

std::set<std::pair<int, int>> visited_points;
bool go_through_grid(std::vector<std::vector<int>> grid, int row, int col,
                     int &heat, Direction dir, int move_lenght) {
  // if (visited_points.count(std::make_pair(std::make_pair(row, col), dir)) >
  if (row < 0 || col < 0 || row >= grid.size() || col >= grid[0].size() ||
      heat >= best_heat) {
    return false;
  }
  // 0) {
  if (visited_points.count(std::make_pair(row, col)) > 0) {
    // visited_points.erase(std::make_pair(std::make_pair(row, col), dir));
    visited_points.erase(std::make_pair(row, col));
    return false;
  } else {
    visited_points.insert((std::make_pair(row, col)));
    // visited_points.insert((std::make_pair(std::make_pair(row, col), dir)));
  }

  heat += grid[row][col];
  if (row == grid.size() - 1 && col == grid[0].size() - 1) {
    best_heat = std::min(best_heat, heat);
    visited_points.erase(std::make_pair(row, col));
    std::cout << best_heat << '\n';
    heat -= grid[row][col];
    return true;
  }

  if (move_lenght > 3) {
    if (dir == left || dir == right) {
      move_lenght = 0;
      go_through_grid(grid, row + 1, col, heat, down, move_lenght + 1);
      move_lenght = 0;
      go_through_grid(grid, row - 1, col, heat, up, move_lenght + 1);
    } else if (dir == up || dir == down) {
      move_lenght = 0;
      go_through_grid(grid, row, col + 1, heat, right, move_lenght + 1);
      move_lenght = 0;
      go_through_grid(grid, row, col - 1, heat, left, move_lenght + 1);
    }
  } else {
    if (dir == left) {
      go_through_grid(grid, row, col - 1, heat, dir, move_lenght + 1);
      move_lenght = 0;
      go_through_grid(grid, row + 1, col, heat, down, move_lenght + 1);
      move_lenght = 0;
      go_through_grid(grid, row - 1, col, heat, up, move_lenght + 1);
    } else if (dir == right) {
      go_through_grid(grid, row, col + 1, heat, dir, move_lenght + 1);
      move_lenght = 0;
      go_through_grid(grid, row + 1, col, heat, down, move_lenght + 1);
      move_lenght = 0;
      go_through_grid(grid, row - 1, col, heat, up, move_lenght + 1);
    } else if (dir == up) {
      go_through_grid(grid, row - 1, col, heat, dir, move_lenght + 1);
      move_lenght = 0;
      go_through_grid(grid, row, col + 1, heat, right, move_lenght + 1);
      move_lenght = 0;
      go_through_grid(grid, row, col - 1, heat, left, move_lenght + 1);
    } else if (dir == down) {
      go_through_grid(grid, row + 1, col, heat, dir, move_lenght + 1);
      move_lenght = 0;
      go_through_grid(grid, row, col + 1, heat, right, move_lenght + 1);
      move_lenght = 0;
      go_through_grid(grid, row, col - 1, heat, left, move_lenght + 1);
    }
  }

  heat -= grid[row][col];
  visited_points.erase(std::make_pair(row, col));
  //   visited_points.erase(std::make_pair(std::make_pair(row, col), dir));
  return false;
}

class Compare {
public:
  bool operator()(std::pair<std::pair<int, int>, int> below,
                  std::pair<std::pair<int, int>, int> above) {
    if (below.second > above.second) {
      return true;
    }

    return false;
  }
};

std::vector<std::pair<int, int>>
get_neighbours(std::vector<std::vector<int>> grid, int row, int col) {
  if (row < 0 || col < 0 || row > grid.size() || col > grid[0].size()) {
    return std::vector<std::pair<int, int>>();
  }
  std::vector<std::pair<int, int>> neigh;

  if (row + 1 < grid.size()) {
    neigh.push_back({row + 1, col});
  }
  if (col + 1 < grid.size()) {
    neigh.push_back({row, col + 1});
  }
  if (row - 1 > 0) {
    neigh.push_back({row - 1, col});
  }
  if (col - 1 > 0) {
    neigh.push_back({row, col - 1});
  }
  return neigh;
}

void bfs(std::vector<std::vector<int>> grid, int s_row, int s_col,
         std::vector<std::vector<int>> &dist,
         std::vector<std::vector<std::pair<int, int>>> &prev) {
  int heat = 0;
  visited_points.insert({s_row, s_col});
  std::priority_queue<std::pair<std::pair<int, int>, int>,
                      std::vector<std::pair<std::pair<int, int>, int>>, Compare>
      q;
  //  std::vector<std::vector<int>> dist;
  // std::vector<std::vector<std::pair<int, int>>> prev;
  for (int r = 0; r < grid.size(); r++) {
    dist.push_back(std::vector<int>());
    prev.push_back(std::vector<std::pair<int, int>>());
    for (int c = 0; c < grid[0].size(); c++) {

      dist.back().push_back(INT_MAX);
      prev.back().push_back({-1, -1});
      q.push({{r, c}, grid[r][c]});
      // q.push({{r, c}, })
    }
  }
  dist[s_row][s_col] = 0;
  // std::queue<std::pair<std::pair<int, int>, int>,
  //          std::list<std::pair<std::pair<int, int>, int>>>
  // q;
  q.push(std::make_pair(std::make_pair(s_row, s_col), grid[s_row][s_col]));
  while (!q.empty()) {
    auto v = q.top();
    q.pop();

    std::vector<std::pair<int, int>> neighbours =
        get_neighbours(grid, v.first.first, v.first.first);
    // std::cout << neighbours.size() << '\n';
    for (auto n : neighbours) {
      std::cout << n.first << ' ' << n.second << ' ' << v.first.first << ' '
                << v.first.second << '\n';
      int tmp = dist[v.first.first][v.first.second] + grid[n.first][n.second];
      if (tmp < dist[n.first][n.second]) {
        dist[n.first][n.second] = tmp;
        prev[n.first][n.second] = v.first;
      }
    }
  }
}

int main() {
  std::vector<std::string> lines = read_lines();
  std::vector<std::vector<int>> grid;
  for (auto line : lines) {
    grid.push_back(std::vector<int>());
    for (auto c : line) {
      grid.back().push_back(c - '0');
    }
  }

  int start_row = 0;
  int start_col = 0;
  int end_row = grid.size() - 1;
  int end_col = grid[0].size() - 1;
  //  print_grid(grid, '\0');
  int heat = 0;
  int move_lenght = 0;
  // go_through_grid(grid, start_row, start_col + 1, heat, right, move_lenght);

  std::vector<std::vector<int>> distances;
  std::vector<std::vector<std::pair<int, int>>> prevs;
  bfs(grid, 0, 0, distances, prevs);

  auto u = prevs[grid.size() - 1][grid[0].size() - 1];
  std::vector<std::pair<int, int>> tmp;
  while (u != std::make_pair(-1, -1)) {
    std::cout << u.first << ' ' << u.second << '\n';
    tmp.push_back(u);
    u = prevs[u.first][u.second];
  }
  std::cout << tmp.size();

  return 0;
}
