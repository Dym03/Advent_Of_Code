#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits.h>
#include <queue>
#include <set>
#include <vector>

#include "../../Utils/utils.cpp"

struct Galaxy {
  int x;
  int y;
};

long manhattan_dist(Galaxy source, Galaxy target) {
  long distance = abs(target.x - source.x) + abs(target.y - source.y);
  return distance;
}

long part_2_path(Galaxy source, Galaxy target, std::set<int> free_h_lines,
                 std::set<int> free_lines) {
  long distance = 0;
  long addon;
  while (source.x != target.x) {
    addon = free_h_lines.count(source.x) > 0 ? 1000000 : 1;
    int update = (target.x - source.x) > 0 ? 1 : -1;
    source.x += update;
    distance += addon;
  }
  while (source.y != target.y) {
    addon = free_lines.count(source.y) > 0 ? 1000000 : 1;
    int update = (target.y - source.y) > 0 ? 1 : -1;
    source.y += update;
    distance += addon;
  }
  return distance;
}

int main() {
  std::vector<std::vector<char>> grid;
  std::string input;
  std::set<int> free_lines;
  std::set<int> free_h_lines;
  std::vector<Galaxy> galaxies;
  int actual_galaxy_id = 1;
  while (getline(std::cin, input)) {
    grid.push_back(std::vector<char>());
    std::set<char> inp(input.begin(), input.end());
    for (auto c : input) {
      if (c == '#') {
        grid.back().push_back('#');
        actual_galaxy_id += 1;
      } else {
        grid.back().push_back('.');
      }
    }

    if (inp.size() == 1 && inp.count('#') == 0) {
      free_h_lines.insert(grid.size() - 1);
    }
    // Part 1
    //   if (inp.size() == 1 && inp.count('#') == 0) {
    //     grid.push_back(std::vector<char>());
    //     for (int i = 0; i < 140; i++) {
    //       grid.back().push_back('.');
    //     }
    //   }
  }
  for (int i = 0; i < grid[0].size(); i++) {
    bool found = false;
    for (int j = 0; j < grid.size(); j++) {
      if (grid[j][i] != '.') {
        found = true;
        break;
      }
    }
    if (!found) {
      std::cout << i << "\n";
      free_lines.insert(i);
    }
  }
  // Part 1
  //  int added_lines = 0;
  //  for (auto line : free_lines) {
  //    for (int i = 0; i < grid.size(); i++) {
  //      grid[i].insert(grid[i].begin() + line + added_lines, '.');
  //    }
  //    added_lines += 1;
  //  }
  for (int i = 0; i < grid.size(); i++) {
    for (int j = 0; j < grid[0].size(); j++) {
      if (grid[i][j] == '#') {
        Galaxy tmp = {i, j};
        galaxies.push_back(tmp);
      }
    }
  }

  std::vector<std::vector<int>> distances;
  for (auto gal1 : galaxies) {
    distances.push_back(std::vector<int>());
    for (auto gal2 : galaxies) {
      // distances.back().push_back(
      //   manhattan_dist(gal1, gal2, free_h_lines, free_lines));
      distances.back().push_back(
          part_2_path(gal1, gal2, free_h_lines, free_lines));
    }
  }
  long long total_sum = 0;
  for (int i = 0; i < distances.size(); i++) {
    for (int j = i + 1; j < distances.size(); j++) {
      total_sum += distances[i][j];
    }
  }
  std::cout << total_sum << " "
            << "\n"
            << grid.size() << " " << grid[0].size() << "\n";
  return 0;
}
