#include "../../Utils/utils.cpp"
#include <iostream>
#include <string>
#include <vector>

long calculate_next(std::vector<long> input) {
  bool found = false;
  for (auto a : input) {
    if (a != 0) {
      found = true;
    }
  }
  if (!found) {
    return 0;
  }
  std::vector<long> next_input;
  for (int i = 0; i < input.size() - 1; i++) {
    next_input.push_back((input[i + 1] - input[i]));
  }
  return (input[input.size() - 1] + calculate_next(next_input));
}

long calculate_prev(std::vector<long> input) {
  bool found = false;
  for (auto a : input) {
    if (a != 0) {
      found = true;
    }
  }
  if (!found) {
    return 0;
  }
  std::vector<long> next_input;
  for (int i = 0; i < input.size() - 1; i++) {
    next_input.push_back((input[i + 1] - input[i]));
  }
  return (input[0] - calculate_prev(next_input));
}

int main() {
  std::string input;
  std::vector<std::string> input_tokens;
  std::vector<long> sequence;
  long sum = 0;
  while (getline(std::cin, input)) {
    input_tokens = split(' ', input);
    for (auto a : input_tokens) {
      sequence.emplace_back(std::stoi(a));
    }
    // sum += calculate_next(sequence);
    sum += calculate_prev(sequence);
    sequence.clear();
  }
  std::cout << sum << "\n";

  return 0;
}
