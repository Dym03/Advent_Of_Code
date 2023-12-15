#include "../../Utils/utils.cpp"
#include <limits.h>
#include <vector>

enum State {
  Flying,
  Resting,
};

struct Reindeer {
  std::string name;
  int act_distance;
  int speed;
  int fly_duration;
  int rest_duration;
  int num_sec_state;
  State cur_state;
};

int main() {
  std::vector<std::string> lines = read_lines();
  std::vector<std::string> tokens;
  std::vector<Reindeer> reindeers;
  std::vector<int> points;
  int speed;
  for (auto line : lines) {
    tokens = split(' ', line);

    Reindeer tmp = {tokens[0],
                    0,
                    std::stoi(tokens[3]),
                    std::stoi(tokens[6]),
                    std::stoi(tokens[13]),
                    0,
                    Flying};
    reindeers.push_back(tmp);
    points.push_back(0);
  }

  int loop_duration = 2503;
  int max = 0;
  /* Part 1
  for (auto reinder : reindeers) {
    int distance =
        (loop_duration / (reinder.fly_duration + reinder.rest_duration)) *
        (reinder.fly_duration * reinder.speed);
    distance +=
        loop_duration % (reinder.fly_duration + reinder.rest_duration) <=
                reinder.fly_duration
            ? loop_duration % (reinder.fly_duration + reinder.rest_duration) *
                  reinder.speed
            : reinder.fly_duration * reinder.speed;
    std::cout << loop_duration % (reinder.fly_duration + reinder.rest_duration)
              << ' ' << distance << '\n';
    max = std::max(max, distance);
  }*/

  // Part 2
  int best_pos = 0;
  for (int i = 0; i < loop_duration; i++) {
    for (auto &r : reindeers) {
      if (r.cur_state == Flying) {
        r.act_distance += r.speed;
        r.num_sec_state += 1;
        if (r.num_sec_state >= r.fly_duration) {
          r.cur_state = Resting;
          r.num_sec_state = 0;
        }
      } else if (r.cur_state == Resting) {
        r.num_sec_state += 1;
        if (r.num_sec_state >= r.rest_duration) {
          r.cur_state = Flying;
          r.num_sec_state = 0;
        }
      }
      if (max < r.act_distance) {
        max = r.act_distance;
      }
    }
    int pos = 0;
    for (auto &r : reindeers) {
      if (r.act_distance == max) {
        points[pos] += 1;
      }
      pos += 1;
    }
  }
  for (auto a : reindeers) {
    std::cout << a.act_distance << '\n';
  }

  print_vector(points, false);

  std::cout << '\n' << max << '\n';
  return 0;
}
