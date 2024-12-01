#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void ltrim(std::string &input) {
  input = input.substr(input.find_first_not_of(" "));
}

void rtrim(std::string &input) {
  input = input.substr(0, input.find_last_not_of(" ") + 1);
}

void trim(std::string &input) {
  ltrim(input);
  rtrim(input);
}

std::vector<std::string> read_lines() {
  std::vector<std::string> lines;
  std::string line;
  while (getline(std::cin, line)) {
    lines.push_back(line);
  }

  return lines;
}

std::vector<std::string> split(char delim, std::string input) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream iss(input);
  while (std::getline(iss, token, delim)) {
    if (token.length() != 0) {
      tokens.push_back(token);
    }
  }
  return tokens;
}

template <typename T> void print_vector(std::vector<T> inp, char delim) {
  for (auto a : inp) {
    std::cout << a << delim;
    // if (newline) {
    // std::cout << '\n';
    //} else {
    // std::cout << ' ';
    //}
  }
  std::cout << '\n';
}

template <typename T>
void print_grid(std::vector<std::vector<T>> grid, char delim) {
  for (auto row : grid) {
    print_vector(row, delim);
    // std::cout << '\n';
  }
  std::cout << '\n';
}
