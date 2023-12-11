#include "../../Utils/utils.cpp"
#include <climits>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

struct Location {
  std::string name;
  int distance;
};

bool find_best_flight(
    std::string city, std::set<std::string> &visited,
    std::map<std::string, std::vector<Location>> from_to_location,
    int &distance, int &best_distance, int city_count) {
  if (visited.size() == city_count) {
    // Part 1 is std::min >D
    best_distance = std::max(distance, best_distance);
    return true;
  }
  for (auto location : from_to_location[city]) {
    if (visited.count(location.name) > 0) {
      continue;
    }
    visited.insert(location.name);
    distance += location.distance;
    if (find_best_flight(location.name, visited, from_to_location, distance,
                         best_distance, city_count)) {
    }
    visited.erase(location.name);
    distance -= location.distance;
  }
  return false;
}

int main() {
  std::string input;
  std::vector<std::string> tokens;
  std::set<std::string> cities;
  std::map<std::string, std::vector<Location>> from_to_location;
  while (getline(std::cin, input)) {
    tokens = split(' ', input);
    if (cities.count(tokens[0]) == 0) {
      cities.insert(tokens[0]);
    }
    if (cities.count(tokens[2]) == 0) {
      cities.insert(tokens[2]);
    }
    from_to_location[tokens[0]].push_back({tokens[2], std::stoi(tokens[4])});
    from_to_location[tokens[2]].push_back({tokens[0], std::stoi(tokens[4])});
  }

  int city_count = cities.size();
  std::set<std::string> visited;
  int best_distance = 0 /*INT_MAX for first part */, distance = 0;
  for (auto city : cities) {
    visited.insert(city);
    find_best_flight(city, visited, from_to_location, distance, best_distance,
                     city_count);
    visited.erase(city);
    std::cout << best_distance << '\n';
  }
}
