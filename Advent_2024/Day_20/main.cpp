#include <sys/wait.h>

#include <climits>
#include <iomanip>
#include <queue>
#include <set>
#include <stack>

#include "../../Utils/utils.cpp"
#include "unordered_map"

enum GridType {
    EMPTY,
    WALL,
    START,
    END
};

void print_grid(Grid<GridType>& g) {
    for (int i = 0; i < g.get_row_size(); i++) {
        for (int j = 0; j < g.get_col_size(); j++) {
            if (g.get(i, j) == EMPTY) {
                std::cout << ".";
            } else if (g.get(i, j) == WALL) {
                std::cout << "#";
            } else if (g.get(i, j) == START) {
                std::cout << "S";
            } else if (g.get(i, j) == END) {
                std::cout << "E";
            }
        }
        std::cout << std::endl;
    }
}

bool isValid(int i, int j, Grid<GridType>& g) {
    return i >= 0 && i < g.get_row_size() && j >= 0 && j < g.get_col_size();
}

std::unordered_map<Point, std::vector<Point>> get_adj_map(Grid<GridType>& g) {
    std::unordered_map<Point, std::vector<Point>> adj_map;
    for (int i = 0; i < g.get_row_size(); i++) {
        for (int j = 0; j < g.get_col_size(); j++) {
            for (auto dir : {UP, DOWN, LEFT, RIGHT}) {
                Point dx = get_direction_from_enum(dir);
                Point next = Point(i, j) + dx;
                if (isValid(next.x, next.y, g)) {
                    adj_map[Point(i, j)].push_back(next);
                }
            }
        }
    }
    return adj_map;
}

std::unordered_map<Point, int> get_dist_map(Grid<GridType>& g, Point start, Point end) {
    std::unordered_map<Point, std::vector<Point>> adj_map = get_adj_map(g);
    std::unordered_map<Point, int> dist;
    std::set<Point> visited;
    for (int i = 0; i < g.get_row_size(); i++) {
        for (int j = 0; j < g.get_col_size(); j++) {
            dist[Point(i, j)] = INT_MAX;
        }
    }
    dist[start] = 0;
    std::queue<Point> q;
    q.push(start);
    while (!q.empty()) {
        Point node = q.front();
        q.pop();
        if (visited.count(node) > 0) {
            continue;
        }
        visited.insert(node);
        for (auto v : adj_map[node]) {
            int new_dist = dist[node] + 1;
            if (g.get(v.x, v.y) == WALL) {  // For now, we are not considering walls
                continue;
            }
            if (new_dist < dist[v]) {
                dist[v] = new_dist;
                q.push(v);
            }
        }
    }
    return dist;
}

int solve(std::unordered_map<Point, int>& start_end_dist, std::unordered_map<Point, int>& end_start_dist, int shortest, int distance) {
    int n_saved = 0;
    std::vector<Point> all_reachable = {};
    for (auto [k, v] : start_end_dist) {
        if (v != INT_MAX) {
            all_reachable.push_back(k);
        }
    }

    for (auto p : all_reachable) {
        auto d1 = start_end_dist[p];
        for (auto p2 : all_reachable) {
            if (p == p2) continue;
            if (end_start_dist.count(p2) == 0) {
                continue;
            }
            auto q_dist = end_start_dist[p2];
            auto dist = (abs(p.x - p2.x) + abs(p.y - p2.y));
            if (dist <= distance) {
                if (q_dist != INT_MAX && d1 + q_dist + dist + 99 < shortest) {
                    n_saved += 1;
                }
            }
        }
    }
    return n_saved;
}

int main() {
    std::vector<std::string> lines = read_lines();
    Grid<GridType> grid = Grid<GridType>(lines.size(), lines[0].size(), EMPTY);
    Point start, end;
    std::vector<Point> walls;
    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[0].size(); j++) {
            if (lines[i][j] == '#') {
                grid.set(i, j, WALL);
                walls.push_back(Point(i, j));
            } else if (lines[i][j] == 'S') {
                grid.set(i, j, START);
                start = Point(i, j);
            } else if (lines[i][j] == 'E') {
                grid.set(i, j, END);
                end = Point(i, j);
            } else if (lines[i][j] == '.') {
                grid.set(i, j, EMPTY);
            }
        }
    }

    auto start_end_dist = get_dist_map(grid, start, end);
    auto end_start_dist = get_dist_map(grid, end, start);
    int shortest = start_end_dist[end];
    // This also works for part 1
    // for (int i = 0; i < walls.size(); i++) {
    //     auto p = walls[i];
    //     grid.set(p.x, p.y, EMPTY);
    //     auto dist = dijkstra(grid, start, end);
    //     if (dist[end] + 99 < shortest) {
    //         n_saved += 1;
    //     }
    //     grid.set(p.x, p.y, WALL);
    //     std::cout << i << " out of " << walls.size() << std::endl;
    // }
    // std::cout << n_saved << std::endl;

    std::vector<Point> all_reachable = {};
    for (auto [k, v] : start_end_dist) {
        if (v != INT_MAX) {
            all_reachable.push_back(k);
        }
    }

    std::cout << "Part 1 = " << solve(start_end_dist, end_start_dist, shortest, 2) << std::endl;
    std::cout << "Part 1 = " << solve(start_end_dist, end_start_dist, shortest, 20) << std::endl;
    return 0;
}
