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
