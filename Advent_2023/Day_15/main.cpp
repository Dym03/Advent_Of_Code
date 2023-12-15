#include "../../Utils/utils.cpp"
#include <cstdio>
#include <vector>

int get_hash_code(std::string str) {
  int hash = 0;
  for (char c : str) {
    hash += c;
    hash *= 17;
    hash %= 256;
  }
  std::cout << hash << '\n';
  return hash;
}

int main() {
  std::string line;
  std::vector<std::string> tokens;
  getline(std::cin, line);
  tokens = split(',', line);
  long sum = 0;
  for (auto tok : tokens) {
    // part 1 sum += get_hash_code(tok);
  }

  std::cout << sum << '\n';
  return 0;
}
