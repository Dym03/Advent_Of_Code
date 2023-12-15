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

struct Lens {
  std::string label;
  int focal_lenght;
};

int main() {
  std::string line;
  std::vector<std::string> tokens;
  std::vector<std::vector<Lens>> hash_map;
  for (int i = 0; i < 256; i++) {
    hash_map.push_back(std::vector<Lens>());
  }
  getline(std::cin, line);
  tokens = split(',', line);
  std::string label;
  long sum = 0;
  for (auto tok : tokens) {
    if (tok.find('-') != std::string::npos) {
      label = tok.substr(0, tok.find('-'));
      int hash_code = get_hash_code(label);
      for (int i = 0; i < hash_map[hash_code].size(); i++) {
        if (hash_map[hash_code][i].label == label) {
          hash_map[hash_code].erase(hash_map[hash_code].cbegin() + i);
        }
      }
    } else if (tok.find('=') != std::string::npos) {
      label = tok.substr(0, tok.find('='));
      int focal_lenght = std::stoi(tok.substr(tok.find('=') + 1));
      int hash_code = get_hash_code(label);
      bool found = false;
      for (int i = 0; i < hash_map[hash_code].size(); i++) {
        if (hash_map[hash_code][i].label == label) {
          hash_map[hash_code][i].focal_lenght = focal_lenght;
          found = true;
        }
      }
      if (!found) {
        Lens tmp = {label, focal_lenght};
        hash_map[hash_code].push_back(tmp);
      }

      // std::cout << label << ' ' << focal_lenght << '\n';
    }
    // part 1 sum += get_hash_code(tok);
  }
  int box_counter = 1;
  for (auto v : hash_map) {
    int lens_counter = 1;
    if (!v.empty()) {
      std::cout << "Box " << box_counter - 1 << ' ';
    }
    for (auto b : v) {
      sum += box_counter * lens_counter * b.focal_lenght;
      std::cout << "Lens " << lens_counter << ' ' << '[' << b.label << ' '
                << b.focal_lenght << "]" << ' ';
      lens_counter += 1;
    }
    if (!v.empty()) {
      std::cout << '\n';
    }
    box_counter += 1;
  }

  std::cout << sum << '\n';
  return 0;
}
