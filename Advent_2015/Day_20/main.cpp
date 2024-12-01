#include "../../Utils/utils.cpp"
#include <unordered_map>
#include <vector>

int main() {
  size_t atleast = 34000000;
  std::vector<size_t> elfs{};
  elfs.reserve(3400000);
  std::unordered_map<size_t, size_t> house_presents;
  for (size_t i = 1; i < 3400000; i++) {
    house_presents[i] = 0;
    elfs.emplace_back(i);
    for (auto elf = 1; elf <= i / 2 + 1; elf++) {
      if (i % elf == 0) {
        house_presents[i] += 10 * elf;
      }
    }
    if (i % 100000 == 0) {
      std::cout << i << ' ' << house_presents[i] << '\n';
    }
    if (house_presents[i] >= atleast) {
      std::cout << "House with index " << i << "\n";
      break;
    }
  }

  return 0;
}
