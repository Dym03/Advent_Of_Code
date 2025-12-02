#include <sys/wait.h>

#include <algorithm>
#include <climits>
#include <functional>
#include <iomanip>
#include <limits>
#include <queue>
#include <set>
#include <stack>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../../Utils/utils.cpp"

enum GridType {
    EMPTY,
    WALL,
    START,
    END,
    PATH,
};

void print_grid(Grid<GridType>& grid) {
    for (int i = 0; i < grid.get_row_size(); i++) {
        for (int j = 0; j < grid.get_col_size(); j++) {
            switch (grid.get(i, j)) {
                case EMPTY:
                    std::cout << ".";
                    break;
                case WALL:
                    std::cout << "#";
                    break;
                case START:
                    std::cout << "S";
                    break;
                case END:
                    std::cout << "E";
                    break;
                case PATH:
                    std::cout << "X";
                    break;
            }
        }
        std::cout << std::endl;
    }
}

int min_score = INT_MAX;

std::set<Point> possible = {};

int shortest = 104516;
long long counter = 0;

int traverse(Grid<GridType>& G, Point start_pos, Point end_pos, Point dir, int score, std::set<std::pair<Point, Point>> visited) {
    if (score > min_score || score > shortest) {
        return INT_MAX;
    }
    if (start_pos == end_pos) {
        if (score <= min_score) {
            // std::cout << start_pos << " " << end_pos << " " << dir << std::endl;
            if (score < min_score) {
                possible.clear();
            }
            min_score = score;
            // std::cout << min_score << std::endl;
            Grid<GridType> grid_copy = G;
            for (auto p : visited) {
                possible.insert(p.first);
                grid_copy.set(p.first.x, p.first.y, PATH);
            }
            print_grid(grid_copy);
        }
        return score;
    }
    if (visited.find({start_pos, dir}) != visited.end() || visited.find({start_pos, {-dir.x, -dir.y}}) != visited.end()) {
        return INT_MAX;
    }
    visited.insert({start_pos, dir});
    counter += 1;

    if (counter % 100000 == 0) {
        std::cout << counter << std::endl;
        // std::cout << "Current state = \n";
        auto g_copy = G;
        for (auto p : visited) {
            g_copy.set(p.first.x, p.first.y, PATH);
        }
        print_grid(g_copy);
        // std::cout << "Current score = " << score << std::endl;
    }
    // sleep(2);
    Point next_pos = start_pos + dir;
    if (next_pos.x < 0 || next_pos.x > G.get_row_size() - 1 || next_pos.y < 0 || next_pos.y > G.get_col_size() - 1) {
        return INT_MAX;
    }

    int best_score = INT_MAX;

    if (G.get(next_pos.x, next_pos.y) == EMPTY) {
        best_score = std::min({traverse(G, next_pos, end_pos, dir, score + 1, visited), traverse(G, start_pos, end_pos, {dir.y, dir.x}, score + 1000, visited), traverse(G, start_pos, end_pos, {-dir.y, -dir.x}, score + 1000, visited)});
        return best_score;
    }
    best_score = INT_MAX;

    if (G.get(next_pos.x, next_pos.y) == WALL) {
        best_score = std::min({traverse(G, start_pos, end_pos, {dir.y, dir.x}, score + 1000, visited), traverse(G, start_pos, end_pos, {-dir.y, -dir.x}, score + 1000, visited)});
        return best_score;
    }
    best_score = INT_MAX;
    if (G.get(next_pos.x, next_pos.y) == END) {
        // std::cout << start_pos << " " << end_pos << " " << dir << std::endl;
        best_score = traverse(G, next_pos, end_pos, dir, score + 1, visited);
        return best_score;
    }
    return INT_MAX;
}

int travel_grid(Grid<GridType>& G, Point start_pos, Point end_pos, Point dir) {
    std::set<std::pair<Point, Point>> visited;
    return traverse(G, start_pos, end_pos, dir, 0, visited);
}

std::unordered_map<Point, std::vector<Point>> get_adj_map(Grid<GridType>& g) {
    std::unordered_map<Point, std::vector<Point>> adj_map;
    std::vector<Direction> dirs = {UP, DOWN, LEFT, RIGHT};
    for (int i = 0; i < g.get_row_size(); i++) {
        for (int j = 0; j < g.get_col_size(); j++) {
            for (auto dir : dirs) {
                Point dx = get_direction_from_enum(dir);
                Point next = {i + dx.x, j + dx.y};
                if (next.x < 0 || next.x > g.get_row_size() - 1 || next.y < 0 || next.y > g.get_col_size() - 1) {
                    continue;
                }
                if (g.get(next.x, next.y) != WALL) {
                    adj_map[{i, j}].push_back(next);
                }
            }
        }
    }
    return adj_map;
}
std::set<Point> rotated = {};

int best_dist = INT_MAX;

std::pair<std::unordered_map<Point, int>, std::unordered_map<Point, std::vector<Point>>> dijkstra(Grid<GridType>& grid, Point start, Point end) {
    std::unordered_map<Point, int> distances;
    std::unordered_map<Point, std::vector<Point>> prev;
    auto adj_map = get_adj_map(grid);
    for (int i = 0; i < grid.get_row_size(); i++) {
        for (int j = 0; j < grid.get_col_size(); j++) {
            distances[{i, j}] = INT_MAX;
            prev[{i, j}] = {};
        }
    }
    distances[start] = 0;
    struct State {
        int dist;
        Point pos;
        Point dir;
        bool operator<(const State& other) const {
            return dist > other.dist;
        }
    };
    std::priority_queue<State, std::vector<State>> pq;
    pq.push({0, start, get_direction_from_enum(RIGHT)});
    std::set<std::pair<Point, Point>> visited;
    while (!pq.empty()) {
        auto act = pq.top();
        pq.pop();
        if (visited.find({act.pos, act.dir}) != visited.end()) {
            continue;
        }
        visited.insert({act.pos, act.dir});
        for (auto next : adj_map[act.pos]) {
            Point act_dir = next - act.pos;
            if (act_dir != act.dir) {
                distances[act.pos] = distances[act.pos] + 1000;
                pq.push({distances[next], act.pos, act_dir});

            } else {
                int new_dist = distances[act.pos] + 1;
                if (new_dist < distances[next]) {
                    distances[next] = new_dist;
                    prev[next].push_back(act.pos);
                    pq.push({new_dist, next, act.dir});
                } else if (new_dist == distances[next]) {
                    prev[next].push_back(act.pos);
                }
            }
        }
    }
    return {distances, prev};
}

const std::unordered_map<char, Point> delta = {
    {'E', {0, 1}}, {'W', {0, -1}}, {'N', {-1, 0}}, {'S', {1, 0}}};

// Updated Dijkstra function
std::unordered_map<std::pair<Point, char>, int, hash_pair> dijkstra_2(Grid<GridType>& grid, Point start, char startDir, Point end) {
    // Distance map with direction
    std::unordered_map<std::pair<Point, char>, int, hash_pair> distances;

    for (int i = 0; i < grid.get_row_size(); ++i) {
        for (int j = 0; j < grid.get_col_size(); ++j) {
            for (char dir : {'E', 'W', 'N', 'S'}) {
                distances[{{i, j}, dir}] = INT_MAX;
            }
        }
    }

    // Priority queue (distance, position, direction)
    using State = std::tuple<int, Point, char>;
    auto compare = [](const State& a, const State& b) { return std::get<0>(a) > std::get<0>(b); };
    std::priority_queue<State, std::vector<State>, decltype(compare)> pq(compare);

    distances[{start, startDir}] = 0;
    pq.push({0, start, startDir});

    while (!pq.empty()) {
        auto [d, pos, dir] = pq.top();
        pq.pop();

        if (pos == end) {
            continue;  // Stop condition
        }

        if (distances[{pos, dir}] < d) {
            continue;  // Skip already processed states
        }

        // Explore all directions except current
        for (char nextDir : {'E', 'W', 'N', 'S'}) {
            if (nextDir == dir) continue;  // Skip the same direction

            std::pair<Point, char> state = {pos, nextDir};
            if (distances[state] > d + 1000) {
                distances[state] = d + 1000;
                pq.push({d + 1000, pos, nextDir});
            }
        }

        // Move forward in the current direction
        Point deltaMove = delta.at(dir);
        Point nextPos = {pos.x + deltaMove.x, pos.y + deltaMove.y};

        if (nextPos.x >= 0 && nextPos.x < grid.get_row_size() &&
            nextPos.y >= 0 && nextPos.y < grid.get_col_size() &&
            grid.get(nextPos.x, nextPos.y) != WALL) {
            std::pair<Point, char> state = {nextPos, dir};
            if (distances[state] > d + 1) {
                distances[state] = d + 1;
                pq.push({d + 1, nextPos, dir});
            }
        }
    }

    return distances;
}

struct PairHash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        return std::hash<T1>()(p.first) ^ std::hash<T2>()(p.second);
    }
};

int main() {
    std::vector<std::string> lines = read_lines();
    Grid<GridType> grid(lines.size(), lines[0].size(), EMPTY);
    Point start = {0, 0}, end = {0, 0};
    Point dir = get_direction_from_enum(RIGHT);
    std::cout << dir << std::endl;
    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[0].size(); j++) {
            if (lines[i][j] == '#') {
                grid.set(i, j, WALL);
            } else if (lines[i][j] == 'S') {
                grid.set(i, j, START);
                start = {i, j};
            } else if (lines[i][j] == 'E') {
                grid.set(i, j, END);
                end = {i, j};
            }
        }
    }
    print_grid(grid);
    std::cout << start << " " << end << std::endl;
    auto dist_grid = Grid<int>(grid.get_row_size(), grid.get_col_size(), INT_MAX);
    auto dist_from_start = dijkstra_2(grid, start, 'E', end);
    auto dist_from_end = dijkstra_2(grid, end, 'W', start);
    int min_dist = INT_MAX;
    for (auto d : {'E', 'W', 'N', 'S'}) {
        min_dist = std::min(dist_from_start[{end, d}], min_dist);
    }
    std::cout << min_dist << std::endl;
    std::unordered_map<char, char> dir_flip = {{'E', 'W'}, {'W', 'E'}, {'N', 'S'}, {'S', 'N'}};
    for (int i = 0; i < grid.get_row_size(); i++) {
        for (int j = 0; j < grid.get_col_size(); j++) {
            for (auto d : {'E', 'W', 'N', 'S'}) {
                auto state_from_start = std::make_pair(Point{i, j}, d);
                auto state_from_end = std::make_pair(Point{i, j}, dir_flip[d]);
                if (dist_from_start[state_from_start] != INT_MAX && dist_from_end[state_from_end] != INT_MAX) {
                    std::cout << dist_from_start[state_from_start] << " " << dist_from_end[state_from_end] << std::endl;
                    if (dist_from_start[state_from_start] + dist_from_end[state_from_end] == min_dist) {
                        grid.set(i, j, START);
                    }
                }
            }
        }
    }
    std::cout << dist_from_start[{(start + Point(-1, 0)), 'E'}] << " " << dist_from_end[{start + Point(-1, 0), 'S'}] << std::endl;
    print_grid(grid);
    // s
    // std::cout << travel_grid(grid, start, end, dir) << std::endl;
    // auto dist = bfs(grid, adj_map, start, end, get_direction_from_enum(RIGHT));
    // // std::cout << dist[end] << std::endl;
    // for (int i = 0; i < grid.get_row_size(); i++) {
    //     for (int j = 0; j < grid.get_col_size(); j++) {
    //         if (dist[{i, j}] == INT_MAX) {
    //             std::cout << std::setw(5) << -1;
    //             continue;
    //         }
    //         std::cout << std::setw(5) << dist[{i, j}];
    //     }
    //     std::cout << std::endl;
    // }

    // auto end_start_dist = bfs(grid, adj_map, end, start, get_direction_from_enum(DOWN));
    // for (int i = 0; i < grid.get_row_size(); i++) {
    //     for (int j = 0; j < grid.get_col_size(); j++) {
    //         if (end_start_dist[{i, j}] == INT_MAX) {
    //             std::cout << std::setw(5) << -1;
    //             continue;
    //         }
    //         std::cout << std::setw(5) << end_start_dist[{i, j}];
    //     }
    //     std::cout << std::endl;
    return 0;
}
