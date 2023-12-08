#include "../../Utils/utils.cpp"

#include <iostream>
#include <map>
#include <utility>

void move_santa(std::pair<int, int> &location, char a) {
  if (a == '>') {
    location.second += 1;
  } else if (a == '<') {
    location.second -= 1;
  } else if (a == '^') {
    location.first += 1;
  } else if (a == 'v') {
    location.first -= 1;
  }
}

int main() {
  std::map<std::pair<int, int>, int> location_visited;
  std::pair<int, int> location = std::make_pair(0, 0);
  std::pair<int, int> robo_location = std::make_pair(0, 0);
  std::string input;
  location_visited[location] += 1;
  getline(std::cin, input);
  for (int i = 0; i < input.length(); i += 2) {
    move_santa(location, input[i]);
    move_santa(robo_location, input[i + 1]);
    location_visited[robo_location] += 1;
    location_visited[location] += 1;
  }
  std::cout << location_visited.size() << "\n";

  return 0;
}
