#include <sys/wait.h>

#include <set>

#include "../../Utils/utils.cpp"

#define WIDTH 101
#define HEIGHT 103

struct Robot {
    Point position;
    Point velocity;
};

int flodd_fill(Point start, Grid<char>& g, std::set<Point>& seen) {
    if (start.x < 0 || start.x > HEIGHT - 1 || start.y < 0 || start.y > WIDTH - 1) {
        return 0;
    }
    if (seen.count(start) > 0) {
        return 0;
    }
    if (g.get(start.x, start.y) == '.') {
        return 0;
    }
    seen.insert(start);
    Direction directions[4] = {UP, DOWN, LEFT, RIGHT};
    int total = 1;
    for (auto dir : directions) {
        Point d = get_direction_from_enum(dir);
        total += flodd_fill(start + d, g, seen);
    }
    return total;
}

void run_simulation(std::vector<Robot>& robots, bool p_1) {
    int num_seconds = 100;
    int width = WIDTH;
    int height = HEIGHT;
    // for (int i = 0; i < num_seconds; i++) {
    int i = 1;
    while (1) {
        for (auto& robot : robots) {
            robot.position += robot.velocity;
            if (robot.position.x >= 0) {
                robot.position.x = robot.position.x % height;
            } else {
                robot.position.x = robot.position.x + height;
            }
            if (robot.position.y >= 0) {
                robot.position.y = robot.position.y % width;
            } else {
                robot.position.y = robot.position.y + width;
            }
        }
        Grid<char> g{HEIGHT, WIDTH, '.'};
        for (auto robot : robots) {
            g.set(robot.position.x, robot.position.y, '#');
        }
        std::set<Point> seen;
        int max_cluster = 0;
        for (auto robot : robots) {
            if (seen.count(robot.position) == 0) {
                max_cluster = std::max(flodd_fill(robot.position, g, seen), max_cluster);
            }
        }
        if (max_cluster > robots.size() * 0.7 || max_cluster > 20) {
            std::cout << max_cluster << " " << i << std::endl;

            g.print();
            break;
        }

        // sleep(1);
        i += 1;
        if (p_1 && i > 100) {
            break;
        }
    }
}

int get_quadrant_score(Grid<int>& g) {
    int total = 1;
    int quadrant = 0;
    for (int i = 0; i < HEIGHT / 2; i++) {
        for (int j = 0; j < WIDTH / 2; j++) {
            if (g.get(i, j) > 0) {
                quadrant += g.get(i, j);
            }
        }
    }
    std::cout << "Q1 " << quadrant << "\n";
    total *= quadrant;
    quadrant = 0;
    for (int i = 0; i < HEIGHT / 2; i++) {
        for (int j = WIDTH / 2 + 1; j < WIDTH; j++) {
            if (g.get(i, j) > 0) {
                quadrant += g.get(i, j);
            }
        }
    }
    std::cout << "Q2 " << quadrant << "\n";
    total *= quadrant;
    quadrant = 0;
    for (int i = HEIGHT / 2 + 1; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH / 2; j++) {
            if (g.get(i, j) > 0) {
                quadrant += g.get(i, j);
            }
        }
    }
    std::cout << "Q3 " << quadrant << "\n";
    total *= quadrant;
    quadrant = 0;
    for (int i = HEIGHT / 2 + 1; i < HEIGHT; i++) {
        for (int j = WIDTH / 2 + 1; j < WIDTH; j++) {
            if (g.get(i, j) > 0) {
                quadrant += g.get(i, j);
            }
        }
    }
    std::cout << "Q4 " << quadrant << "\n";
    total *= quadrant;
    quadrant = 0;

    return total;
}

int main() {
    std::vector<std::string> lines = read_lines();
    std::vector<Robot> robots;

    for (auto line : lines) {
        std::vector<std::string> tokens = split(' ', line);
        std::string positions = split('=', tokens[0])[1];
        std::vector<std::string> pos = split(',', positions);
        std::string velocities = split('=', tokens[1])[1];
        std::vector<std::string> vel = split(',', velocities);
        Point position = {std::stoi(pos[1]), std::stoi(pos[0])};
        Point velocity = {std::stoi(vel[1]), std::stoi(vel[0])};
        Robot r = {position, velocity};
        robots.push_back(r);
    }

    run_simulation(robots, true);
    Grid<int> g{HEIGHT, WIDTH, 0};
    for (auto robot : robots) {
        std::cout << robot.position << "\n";
        if (robot.position.x != HEIGHT / 2 && robot.position.y != WIDTH / 2) {
            g.set(robot.position.x, robot.position.y, g.get(robot.position.x, robot.position.y) + 1);
        }
    }

    g.print();
    int total = get_quadrant_score(g);
    std::cout << "PART 1 = " << total << std::endl;
    run_simulation(robots, false);

    return 0;
}
