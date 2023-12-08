#include <iostream>
#include <list>
#include <set>
#include <string>

std::set<char> vowels = {'a', 'e', 'i', 'o', 'u'};

bool check_vowels(std::string input) {
  int count = 0;
  for (auto c : input) {
    if (vowels.count(c) > 0) {
      count += 1;
    }
  }
  return count >= 3;
}

bool check_restrained(std::string input) {
  std::list<std::string> restrained = {"ab", "cd", "pq", "xy"};
  for (auto res : restrained) {
    if (input.find(res) != std::string::npos) {
      return false;
    }
  }
  return true;
}

bool check_repetition(std::string input) {
  char prev = ' ';
  for (auto c : input) {
    if (c == prev) {
      return true;
    }
    prev = c;
  }
  return false;
}

bool check_letter_between(std::string input) {
  for (int i = 0; i < input.length() - 2; i++) {
    if (input[i] == input[i + 2]) {
      return true;
    }
  }
  return false;
}

bool check_pairs(std::string input) {
  for (int i = 0; i < input.length() - 3; i++) {
    std::string pair = "";
    pair.push_back(input[i]);
    pair.push_back(input[i + 1]);
    for (int j = i + 2; j < input.length() - 1; j++) {
      std::string comparing = "";
      comparing.push_back(input[j]);
      comparing.push_back(input[j + 1]);
      if (pair == comparing) {
        return true;
      }
    }
  }
  return false;
}

bool check_string(std::string input) {
  return check_vowels(input) && check_repetition(input) &&
         check_restrained(input);
}

bool check_string_p2(std::string input) {
  return check_pairs(input) && check_letter_between(input);
}

int main() {
  std::string input;
  int counter = 0;
  while (std::getline(std::cin, input)) {
    //  if (check_string(input)) {
    //    counter += 1;
    //  }
    if (check_string_p2(input)) {
      counter += 1;
    }
  }
  std::cout << counter << "\n";
}
