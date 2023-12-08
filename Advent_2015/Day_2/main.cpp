#include "../../Utils/utils.cpp"
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

int get_product(int x, int y, int z) {
  return 2 * (x * y) + 2 * (y * z) + 2 * (x * z);
}

int get_smalles_side(int x, int y, int z) {
  int a = std::max(x, y);
  a = std::max(a, z);
  if (a == x) {
    return y * z;
  } else if (a == y) {
    return x * z;
  } else {
    return x * y;
  }
}

int get_smalles_side_sum(int x, int y, int z) {
  int a = std::max(x, y);
  a = std::max(a, z);
  if (a == x) {
    return y + z;
  } else if (a == y) {
    return x + z;
  } else {
    return x + y;
  }
}

int main() {
  std::string line;
  std::vector<std::string> tokens;
  int x, y, z = 0;
  long sum = 0;
  long sum_ribbon = 0;
  while (getline(std::cin, line)) {
    tokens = split('x', line);
    std::cout << tokens.size() << "\n";
    x = std::stoi(tokens[0]);
    y = std::stoi(tokens[1]);
    z = std::stoi(tokens[2]);
    sum += get_product(x, y, z) + get_smalles_side(x, y, z);
    sum_ribbon += x * y * z + 2 * get_smalles_side_sum(x, y, z);
  }
  std::cout << sum << " Sum ribbons " << sum_ribbon << "\n";

  return 0;
}
