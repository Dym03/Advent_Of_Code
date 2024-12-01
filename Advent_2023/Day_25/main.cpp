#include "../../Utils/utils.cpp"
#include <map>
#include <set>
#include <vector>

int main() {
  std::vector<std::string> lines = read_lines();
  std::vector<std::string> tokens;
  std::string from = "";
  std::map<std::string, std::set<std::string>> mappings;
  std::set<std::string> froms;
  for (auto line : lines) {
    tokens = split(':', line);
    from = tokens[0];
    froms.insert(from);
    std::string to = tokens[1];
    trim(to);
    tokens = split(' ', to);
    for (auto t : tokens) {
      mappings[from].insert(t);
      mappings[t].insert(from);
      froms.insert(t);
    }
  }
  for (auto f : froms) {
    std::cout << "From " << f << " to ";
    for (auto t : mappings[f]) {
      std::cout << t << ' ';
    }
    std::cout << '\n';
  }

  return 0;
}
