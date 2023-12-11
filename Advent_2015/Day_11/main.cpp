#include "../../Utils/utils.cpp"
#include <iostream>
#include <set>

bool check_first_rule(std::string password) {
  for (int i = 0; i < password.length() - 2; i++) {
    if (password[i] == password[i + 1] - 1 &&
        password[i + 1] == password[i + 2] - 1) {
      return true;
    }
  }
  return false;
}

bool check_second_rule(std::string password) {
  std::set<char> char_set(password.begin(), password.end());
  if (char_set.count('i') > 0 || char_set.count('o') > 0 ||
      char_set.count('l') > 0) {
    return false;
  }
  return true;
}

bool check_third_rule(std::string password) {
  int count = 0;
  for (int i = 0; i < password.length() - 1; i++) {
    if (password[i] == password[i + 1]) {
      count += 1;
      if (count == 2) {
        return true;
      }
      i += 1;
    }
  }
  return false;
}

bool check_password(std::string password) {
  return check_first_rule(password) && check_second_rule(password) &&
         check_third_rule(password);
}

void generate_new_password(std::string &password) {
  for (int i = password.length() - 1; i >= 0; i--) {
    for (int j = password[i]; j <= 'z'; j++) {
      if (check_password(password) && password != "hepxxyzz") {
        return;
      } else {
        password[i] = j;
      }
    }
    if (i >= 1 && password[i - 1] < 'z') {

      password[i - 1] += 1;
      for (int k = i; k < password.length(); k++) {
        password[k] = 'a';
        i += 1;
      }
    }
  }
}

int main() {
  std::string password = "hepxcrrq";
  generate_new_password(password);
  std::cout << password;
  return 0;
}
