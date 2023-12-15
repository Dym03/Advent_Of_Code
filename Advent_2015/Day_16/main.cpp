#include "../../Utils/utils.cpp"
#include <map>
#include <vector>

int main() {
  std::map<std::string, int> needle;
  needle["children"] = 3;
  needle["cats"] = 7;
  needle["samoyeds"] = 2;
  needle["pomeranians"] = 3;
  needle["akitas"] = 0;
  needle["vizslas"] = 0;
  needle["goldfish"] = 5;
  needle["trees"] = 3;
  needle["cars"] = 2;
  needle["perfumes"] = 1;
  std::vector<std::string> lines = read_lines();
  std::vector<std::string> tokens;
  int counter = 0;
  for (auto line : lines) {
    tokens = split(' ', line);
    int id = std::stoi(tokens[1]);
    bool found = false;
    for (int i = 2; i < 8; i += 2) {
      std::string atr_name = tokens[i].substr(0, tokens[i].length() - 1);
      int atr_cnt = std::stoi(tokens[i + 1]);
      if (atr_name == "cats" || atr_name == "trees") {
        if (needle[atr_name] >= atr_cnt) {
          found = true;
          break;
        }
      } else if (atr_name == "pomeranians" || atr_name == "goldfish") {
        if (needle[atr_name] <= atr_cnt) {
          found = true;
          break;
        }
      } else if (needle[atr_name] != atr_cnt) {
        found = true;
        break;
      }
    }
    if (!found) {
      std::cout << id << ' ';
      for (int i = 2; i < 8; i += 2) {
        std::cout << tokens[i] << ' ' << tokens[i + 1] << ' ';
      }
    }
  }

  return 0;
}
