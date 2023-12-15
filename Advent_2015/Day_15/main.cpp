#include "../../Utils/utils.cpp"
#include <string>
#include <vector>

struct Ingrediet {
  std::string name;
  int capacity;
  int durability;
  int flavour;
  int texture;
  int calorie;
};

long calculate_score(int amounts[4], std::vector<Ingrediet> ingrediens) {
  int capacity = 0;
  int durability = 0;
  int flavour = 0;
  int texture = 0;
  int calories = 0;
  for (int i = 0; i < 4; i++) {
    int act_amounth = amounts[i];
    Ingrediet tmp = ingrediens[i];
    capacity += tmp.capacity * act_amounth;
    durability += tmp.durability * act_amounth;
    flavour += tmp.flavour * act_amounth;
    texture += tmp.texture * act_amounth;
    calories += tmp.calorie * act_amounth;
  }
  if (capacity <= 0 || durability <= 0 || texture <= 0 || flavour <= 0 ||
      calories != 500) {
    return 0;
  } else {
    return capacity * durability * flavour * texture;
  }
}

int main() {
  std::vector<std::string> lines = read_lines();
  std::vector<std::string> tokens;
  std::vector<Ingrediet> ingrediens;
  for (auto line : lines) {
    tokens = split(' ', line);
    Ingrediet tmp = {
        tokens[0],
        std::stoi(tokens[2]),
        std::stoi(tokens[4]),
        std::stoi(tokens[6]),
        std::stoi(tokens[8]),
        std::stoi(tokens[10]),
    };
    ingrediens.push_back(tmp);
  }
  int amounts[4] = {0};
  long max = 0;
  for (int i = 100; i >= 0; i--) {
    amounts[0] = i;
    for (int j = 100 - i; j >= 0; j--) {
      amounts[1] = j;
      for (int k = 100 - j - i; k >= 0; k--) {
        amounts[2] = k;
        for (int l = 100 - k - j - i; l >= 0; l--) {
          if (i + j + k + l != 100) {
            break;
          }
          amounts[3] = l;
          max = std::max(max, calculate_score(amounts, ingrediens));
        }
      }
    }
  }

  std::cout << max << '\n';

  return 0;
}
