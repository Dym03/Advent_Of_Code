#include "../../Utils/utils.cpp"
#include <algorithm>
#include <limits.h>
#include <set>
#include <vector>

enum State {
  Used,
  Free,
};

std::set<std::vector<int *>> seen_comb;
int min = INT_MAX;
int amth = 0;
int get_combinations(std::vector<std::pair<State, int>> &contrainers,
                     int amounth, std::vector<int *> &seq) {
  int total = 0;
  if (amounth == 0) {
    std::vector<int *> copy = seq;
    std::sort(copy.begin(), copy.end());
    if (seen_comb.find(copy) == seen_comb.end()) {
      seen_comb.insert(copy);
      if (int(seq.size()) == 4) {
        amth += 1;
      }
      std::cout << seq.size() << ' ';
      min = std::min(min, int(seq.size()));
    }
    return 1;
  }

  bool empty = true;
  for (int i = 0; i < contrainers.size(); i++) {
    if (contrainers[i].first != Used) {
      empty = false;
      break;
    }
  }
  if (empty) {
    return 0;
  }

  for (auto &containter : contrainers) {
    if (containter.first == Free && (amounth - containter.second) >= 0) {
      containter.first = Used;
      int tmp = amounth - containter.second;
      seq.push_back(&containter.second);
      total += get_combinations(contrainers, tmp, seq);
      seq.pop_back();
      containter.first = Free;
    }
  }
  return total;
}

int main() {
  std::vector<std::string> lines = read_lines();
  std::vector<std::pair<State, int>> containters;
  for (auto line : lines) {
    containters.push_back({Free, std::stoi(line)});
  }
  std::vector<int *> seq = {};
  int t = get_combinations(containters, 150, seq);
  std::cout << t << '\n'
            << seen_comb.size() << '\n'
            << min << '\n'
            << amth << '\n';

  return 0;
}
