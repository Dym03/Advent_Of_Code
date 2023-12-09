#include "../../Utils/utils.cpp"
#include <cctype>
#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <sys/types.h>
#include <vector>

int main() {
  std::string input;
  std::map<std::string, uint16_t> wire_value;
  std::vector<std::string> whole_input;
  std::vector<std::string> tokens;
  while (getline(std::cin, input)) {
    whole_input.push_back(input);
  }
  int i = 0;
  while (wire_value["omfg"] == 0) {
    input = whole_input[i];
    tokens = split(' ', input);
    if (tokens[1] == "->") {
      if (std::isdigit(tokens[0][0])) {
        wire_value[tokens[2]] = std::stoi(tokens[0]);
      } else {
        wire_value[tokens[2]] = wire_value[tokens[0]];
      }
    } else if (tokens[0] == "NOT") {
      wire_value[tokens[3]] = ~wire_value[tokens[1]];
    } else {
      if (tokens[1] == "RSHIFT") {
        wire_value[tokens[4]] = wire_value[tokens[0]] >> std::stoi(tokens[2]);
      } else if (tokens[1] == "OR") {
        wire_value[tokens[4]] = wire_value[tokens[0]] | wire_value[tokens[2]];
      } else if (tokens[1] == "AND") {
        if (std::isdigit(tokens[0][0])) {
          wire_value[tokens[4]] = std::stoi(tokens[0]) & wire_value[tokens[2]];
        } else {
          wire_value[tokens[4]] = wire_value[tokens[0]] & wire_value[tokens[2]];
        }
      } else if (tokens[1] == "LSHIFT") {
        wire_value[tokens[4]] = wire_value[tokens[0]] << std::stoi(tokens[2]);
      }
    }
    std::cout << wire_value["a"] << "\n";
    if (wire_value["a"] == 46065) {
      break;
    }
    i += 1;
    if (i == whole_input.size()) {
      i = 0;
    }
  }
}
