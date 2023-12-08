#include "../../Utils/utils.cpp"
#include <iostream>
#include <string>

int main() {
  std::string input;
  int floor;
  int instruction = 0;
  getline(std::cin, input);
  for (auto c : input) {
    instruction += 1;
    if (c == '(') {
      floor += 1;
    } else if (c == ')') {
      floor -= 1;
    }
    if (floor < 0) {
      std::cout << instruction << "\n";
      break;
    }
  }
  std::cout << floor << "\n";
}
