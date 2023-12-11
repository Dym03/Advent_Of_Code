#include "../../Utils/utils.cpp"
#include <iostream>
#include <string>

void transform_input(std::string &input) {
  std::string tmp;
  char act_char = input[0];
  int lenght = 0;
  for (auto c : input) {
    if (c == act_char) {
      lenght += 1;
    } else {
      tmp += std::to_string(lenght) + act_char;
      act_char = c;
      lenght = 1;
    }
  }
  input = tmp += std::to_string(lenght) + act_char;
}

int main() {
  std::string input = "3113322113";
  std::string test_input = "1";
  for (int i = 0; i < 50; i++) {
    transform_input(input);
  }
  // std::cout << input << '\n';
  std::cout << input.length() << '\n';
}
