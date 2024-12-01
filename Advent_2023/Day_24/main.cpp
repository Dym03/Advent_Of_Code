#include "../../Utils/utils.cpp"
#include <vector>

#define MAX_POS 27 // 400000000000000
#define MIN_POS 7  // 200000000000000

struct Hailstone {
  long px;
  long py;
  long pz;
  int vx;
  int vy;
  int vz;
};

enum Outcomes {
  AnyTime,
  NeverOccur,
  Collision_AT_T,
};

void print_hailstone(Hailstone h) {
  std::cout << h.px << ' ' << h.py << ' ' << h.pz << " with velocity " << h.vx
            << ' ' << h.vy << ' ' << h.vz << '\n';
}

bool check_collision(const Hailstone &h1, const Hailstone &h2,
                     double &collision_x, double &collision_y) {
  // Check if paths are parallel
  //
  double slope = (double)h1.vy / h1.vx;
  double slope2 = (double)h2.vy / h2.vx;
  if (slope == slope2) {
    std::cout << "Hailstones' paths are parallel; they never intersect.\n";
    return false;
  }

  double X_collision =
      ((slope2 * h2.px) - (slope * h1.px) + h1.py - h2.py) / (slope2 - slope);
  if (X_collision < MIN_POS || X_collision > MAX_POS) {
    return false;
  }

  double Y_collision = (slope * (X_collision - h1.px)) + h1.py;
  if (Y_collision < MIN_POS || Y_collision > MAX_POS) {
    return false;
  }

  return true;
}

int main() {
  std::vector<std::string> lines = read_lines();
  std::vector<std::string> tokens;
  std::vector<std::string> left_side;
  std::vector<Hailstone> hailstones;
  for (auto line : lines) {
    tokens = split('@', line);
    Hailstone tmp;
    trim(tokens[0]);
    trim(tokens[1]);
    left_side = split(',', tokens[0]);
    for (auto &a : left_side) {
      trim(a);
    }
    tmp.px = std::stol(left_side[0]);
    tmp.py = std::stol(left_side[1]);
    tmp.pz = std::stol(left_side[2]);
    left_side = split(',', tokens[1]);
    for (auto &a : left_side) {
      trim(a);
    }
    tmp.vx = std::stoi(left_side[0]);
    tmp.vy = std::stoi(left_side[1]);
    tmp.vz = std::stoi(left_side[2]);
    print_hailstone(tmp);
    hailstones.push_back(tmp);
  }

  double px = 0;
  double py = 0;
  for (int i = 0; i < hailstones.size(); i++) {
    for (int j = i + 1; j < hailstones.size(); j++) {
      std::cout << i << " " << j << "\n";
      check_collision(hailstones[i], hailstones[j], px, py);
    }
  }

  return 0;
}
