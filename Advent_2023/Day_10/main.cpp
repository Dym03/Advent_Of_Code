#include <fstream>
#include <iostream>
#include <vector>

#include "../../Utils/utils.cpp"

enum PipeType {
  v_pipe,             // |
  h_pipe,             // -
  down_left_corner,   // L
  down_right_corner,  // J
  upper_left_corner,  // F
  upper_right_corner, // 7
  empty,
  start,
  path,
  inside_loop,
};

enum Directon {
  left,
  right,
  down,
  up,
};

struct Point {
  double x, y;
};

void print_grid(PipeType grid[140][140]) {
  for (int i = 0; i < 140; i++) {
    for (int j = 0; j < 140; j++) {
      if (grid[i][j] == path) {
        std::cout << "x";
      } else {
        std::cout << ".";
      }
    }
    std::cout << "\n";
  }
}

int sum = 0;
std::vector<Point> route_points;

bool find_loop(PipeType grid[140][140], int row, int col, Directon direction) {
  if (row > 140 || col > 140) {
    return false;
  }
  if (grid[row][col] == empty) {
    return false;
  } else if (grid[row][col] == start) {
    grid[row][col] = path;
    Point tmp;
    tmp.x = row;
    tmp.y = col;
    route_points.push_back(tmp);
    return true;
  } else if (grid[row][col] == v_pipe) {
    if (direction == up) {
      Point tmp;
      tmp.x = row;
      tmp.y = col;
      route_points.push_back(tmp);
      grid[row][col] = path;
      sum += 1;
      return find_loop(grid, row - 1, col, up);
    } else if (direction == down) {
      Point tmp;
      tmp.x = row;
      tmp.y = col;
      route_points.push_back(tmp);
      grid[row][col] = path;
      sum += 1;
      return find_loop(grid, row + 1, col, down);
    } else {
      std::cout << "Wrong direction \n";
      return false;
    }
  } else if (grid[row][col] == h_pipe) {
    if (direction == left) {
      Point tmp;
      tmp.x = row;
      tmp.y = col;
      route_points.push_back(tmp);
      grid[row][col] = path;
      sum += 1;
      return find_loop(grid, row, col - 1, left);
    } else if (direction == right) {
      Point tmp;
      tmp.x = row;
      tmp.y = col;
      route_points.push_back(tmp);
      grid[row][col] = path;
      sum += 1;
      return find_loop(grid, row, col + 1, right);
    } else {
      std::cout << "Wrong direction \n";
      return false;
    }
  } else if (grid[row][col] == down_left_corner) {
    if (direction == down) {
      Point tmp;
      tmp.x = row;
      tmp.y = col;
      route_points.push_back(tmp);
      grid[row][col] = path;
      sum += 1;
      return find_loop(grid, row, col + 1, right);
    } else if (direction == left) {
      Point tmp;
      tmp.x = row;
      tmp.y = col;
      route_points.push_back(tmp);
      grid[row][col] = path;
      sum += 1;
      return find_loop(grid, row - 1, col, up);
    } else {
      std::cout << "Wrong direction \n";
      return false;
    }
  } else if (grid[row][col] == down_right_corner) {
    if (direction == down) {
      Point tmp;
      tmp.x = row;
      tmp.y = col;
      route_points.push_back(tmp);
      grid[row][col] = path;
      sum += 1;
      return find_loop(grid, row, col - 1, left);
    } else if (direction == right) {
      Point tmp;
      tmp.x = row;
      tmp.y = col;
      route_points.push_back(tmp);
      grid[row][col] = path;
      sum += 1;
      return find_loop(grid, row - 1, col, up);
    } else {
      std::cout << "Wrong direction \n";
      return false;
    }
  } else if (grid[row][col] == upper_left_corner) {
    if (direction == up) {
      Point tmp;
      tmp.x = row;
      tmp.y = col;
      route_points.push_back(tmp);
      grid[row][col] = path;
      sum += 1;
      return find_loop(grid, row, col + 1, right);
    } else if (direction == left) {
      Point tmp;
      tmp.x = row;
      tmp.y = col;
      route_points.push_back(tmp);
      grid[row][col] = path;
      sum += 1;
      return find_loop(grid, row + 1, col, down);
    } else {
      std::cout << "Wrong direction \n";
      return false;
    }
  } else if (grid[row][col] == upper_right_corner) {
    if (direction == up) {
      Point tmp;
      tmp.x = row;
      tmp.y = col;
      route_points.push_back(tmp);
      grid[row][col] = path;
      sum += 1;
      return find_loop(grid, row, col - 1, left);
    } else if (direction == right) {
      Point tmp;
      tmp.x = row;
      tmp.y = col;
      route_points.push_back(tmp);
      grid[row][col] = path;
      sum += 1;
      return find_loop(grid, row + 1, col, down);
    } else {
      std::cout << "Wrong direction";
      return false;
    }
  }
  return false;
}

bool find_0_0(PipeType grid[140][140], int row, int col,
              std::vector<std::vector<bool>> &visited) {
  if (row < 0 || col < 0 || row > 139 || col > 139) {
    return false;
  }
  if (visited[row][col]) {
    return false;
  }
  visited[row][col] = true;
  if ((row == 0 || col == 0 || row == 139 || col == 139)) {
    visited[row][col] = false;
    return true;
  } else if (grid[row][col] == path) {
    visited[row][col] = false;
    return false;
  } else {
    bool res = find_0_0(grid, row + 1, col, visited);
    if (res) {
      visited[row][col] = false;
      return res;
    } else {
      res |= find_0_0(grid, row - 1, col, visited);
      if (res) {
        visited[row][col] = false;
        return res;
      } else {
        res |= find_0_0(grid, row, col + 1, visited);
        if (res) {
          visited[row][col] = false;
          return res;
        } else {
          visited[row][col] = false;
          res |= find_0_0(grid, row, col - 1, visited);
          return res;
        }
      }
    }
  }
}
// Inspired by
// https://www.linkedin.com/pulse/short-formula-check-given-point-lies-inside-outside-polygon-ziemecki
bool is_inside_route(const std::vector<Point> &route, const Point &point) {
  int intersections = 0;
  size_t num_points = route.size();

  for (size_t i = 0; i < num_points; ++i) {
    const Point &p1 = route[i];
    const Point &p2 = route[(i + 1) % num_points];

    // Check for intersection
    if ((p1.y > point.y) != (p2.y > point.y) &&
        point.x < (p2.x - p1.x) * (point.y - p1.y) / (p2.y - p1.y) + p1.x) {
      intersections++;
    }
  }

  // Odd intersections mean the point is inside the route
  return (intersections % 2 == 1);
}

int main() {
  std::string line;
  PipeType grid[140][140];
  std::vector<std::vector<bool>> visited;
  for (int i = 0; i < 140; i++) {
    visited.push_back(std::vector<bool>());
    for (int j = 0; j < 140; j++) {
      visited[i].push_back(false);
    }
  }
  int row = 0;
  std::pair<int, int> start_pos;
  while (getline(std::cin, line)) {
    for (int i = 0; i < line.length(); i++) {
      if (line[i] == '|') {
        grid[row][i] = v_pipe;
      } else if (line[i] == '-') {
        grid[row][i] = h_pipe;
      } else if (line[i] == 'L') {
        grid[row][i] = down_left_corner;
      } else if (line[i] == 'J') {
        grid[row][i] = down_right_corner;
      } else if (line[i] == 'F') {
        grid[row][i] = upper_left_corner;
      } else if (line[i] == '7') {
        grid[row][i] = upper_right_corner;
      } else if (line[i] == '.') {
        grid[row][i] = empty;
      } else if (line[i] == 'S') {
        grid[row][i] = start;
        start_pos = std::make_pair(row, i);
      }
    }
    row += 1;
  }
  if (find_loop(grid, start_pos.first - 1, start_pos.second, up)) {
    std::cout << sum << "\n";
  }
  print_grid(grid);
  std::cout << "\n";
  int sum_p_2 = 0;
  for (int i = 0; i < 140; i++) {
    for (int j = 0; j < 140; j++) {
      if (grid[i][j] != path) {
        Point tmp;
        tmp.x = i;
        tmp.y = j;
        if (is_inside_route(route_points, tmp)) {
          std::cout << i << " " << j << "\n";
          sum_p_2 += 1;
        }
      } else {
        continue;
      }
    }
  }
  std::cout << route_points.size() << " " << sum_p_2 << "\n";
}
