#include <sys/wait.h>

#include <algorithm>
#include <climits>
#include <functional>
#include <iomanip>
#include <limits>
#include <queue>
#include <set>
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
            std::cout << start_pos << " " << end_pos << " " << dir << std::endl;
            if (score < min_score) {
                possible.clear();
            }
            min_score = score;
            std::cout << min_score << std::endl;
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
        std::cout << "Current state = \n";
        auto g_copy = G;
        for (auto p : visited) {
            g_copy.set(p.first.x, p.first.y, PATH);
        }
        print_grid(g_copy);
        std::cout << "Current score = " << score << std::endl;
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
        std::cout << start_pos << " " << end_pos << " " << dir << std::endl;
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

std::pair<std::unordered_map<Point, int>, std::unordered_map<Point, std::vector<Point>>> dijkstra(Grid<GridType>& grid, Point start, Point end, Grid<int>& dist_grid) {
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

    using State = std::tuple<int, Point, Point>;
    std::priority_queue<State, std::vector<State>, std::greater<>> pq;
    std::set<std::pair<Point, Point>> visited;
    Point dir = get_direction_from_enum(RIGHT);
    pq.push({0, start, dir});  // Start with direction (1,0)
    while (!pq.empty()) {
        auto [dist, node, dir] = pq.top();
        pq.pop();

        // Allow revisiting nodes if cost is optimal
        if (dist > distances[node]) {
            continue;
        }

        // Explore neighbors
        for (auto next : adj_map[node]) {
            Point new_dir = next - node;
            int cost = (new_dir == dir) ? 1 : 1000 + 1;
            int new_dist = dist + cost;

            int rotation_cost = (new_dir == dir) ? 0 : 1000;  // Add rotation cost only if turning
            int move_cost = 1;                                // Base cost to move to the next node

            if (new_dist < distances[next]) {
                distances[next] = new_dist;
                prev[next].clear();  // Clear old predecessors
                prev[next].push_back(node);
                pq.push({new_dist, next, new_dir});
            } else if (new_dist == distances[next]) {
                prev[next].push_back(node);  // Add an equally optimal predecessor
                pq.push({new_dist, next, new_dir});
            }

            if (dist_grid.get(next.x, next.y) > new_dist) {
                dist_grid.set(next.x, next.y, new_dist);
            }
        }
    }

    return {distances, prev};
}

std::vector<Point> get_sequence(std::unordered_map<Point, std::vector<Point>>& prev, Point source, Point target) {
    std::vector<Point> sequence;
    if (prev.find(target) == prev.end()) {
        return sequence;
    }
    int num = 0;

    Point u = target;
    while (u != source) {
        sequence.push_back(u);
        u = prev[u][0];
    }
    sequence.push_back(source);
    std::reverse(sequence.begin(), sequence.end());
    return sequence;
}

std::pair<std::unordered_map<Point, int>, std::unordered_map<Point, std::vector<Point>>> bfs(std::unordered_map<Point, std::vector<Point>>& adj_map, Point start, Point end) {
    std::queue<Point> q;
    std::unordered_map<Point, int> dist;                 // Distance map
    std::unordered_map<Point, std::vector<Point>> prev;  // Predecessors map

    // Initialize
    q.push(start);
    dist[start] = 0;
    prev[start] = {};  // Start node has no predecessors
    auto act_dir = get_direction_from_enum(RIGHT);
    while (!q.empty()) {
        Point act = q.front();
        q.pop();

        for (auto next : adj_map[act]) {
            if (dist.find(next) == dist.end()) {  // If the node hasn't been visited

                dist[next] = dist[act] + 1;
                prev[next].push_back(act);
                q.push(next);
            } else if (dist[next] == dist[act] + 1) {
                // If this is another shortest path to the node
                prev[next].push_back(act);
            }
        }
    }
    return {dist, prev};
}

std::set<Point> bfs_backward(Grid<int>& dist_grid, Point start, Point end) {
    std::queue<Point> q;
    std::set<Point> paths;
    q.push(end);
    paths.insert(end);
    while (!q.empty()) {
        auto act = q.front();
        q.pop();
        for (auto dir : {UP, DOWN, LEFT, RIGHT}) {
            auto next = act + get_direction_from_enum(dir);
            if (dist_grid.get(next.x, next.y) == dist_grid.get(act.x, act.y) - 1 || dist_grid.get(next.x, next.y) == dist_grid.get(act.x, act.y) - 1001 || dist_grid.get(next.x, next.y) == dist_grid.get(act.x, act.y)) {
                paths.insert(next);
                q.push(next);
            }
        }
    }
    return paths;
}

struct State {
    std::pair<int, int> cost;  // (rotations, steps)
    Point node;
    Point direction;

    bool operator>(const State& other) const {
        return cost > other.cost;  // Lexicographic: fewer rotations first, then fewer steps
    }
};

struct PairHash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        return std::hash<T1>()(p.first) ^ std::hash<T2>()(p.second);
    }
};
// Main Dijkstra function that tracks all shortest paths
std::pair<std::unordered_map<Point, std::pair<int, int>>, std::unordered_map<Point, std::vector<Point>>>
dijkstra_2(Grid<GridType>& grid, Point start, Point end) {
    using Cost = std::pair<int, int>;  // (rotations, steps)

    // Priority queue to explore nodes
    std::priority_queue<State, std::vector<State>, std::greater<>> pq;

    // Distance map: (node, direction) -> (rotations, steps)
    std::unordered_map<std::pair<Point, Point>, Cost, PairHash> distances;

    // Map to reconstruct paths (multiple predecessors for each node)
    std::unordered_map<Point, std::vector<Point>> prev;

    // Visited set: (node, direction)
    std::set<std::pair<Point, Point>> visited;

    // Directions: UP, RIGHT, DOWN, LEFT
    std::vector<Point> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    // Initialize start state
    Point initial_direction = directions[1];  // Start facing RIGHT
    pq.push({{0, 0}, start, initial_direction});
    distances[{start, initial_direction}] = {0, 0};

    while (!pq.empty()) {
        auto [cost, node, dir] = pq.top();
        pq.pop();

        if (visited.find({node, dir}) != visited.end()) {
            continue;
        }
        visited.insert({node, dir});

        // If we reached the end node, stop early
        if (node == end) {
            break;
        }

        // Explore neighbors
        for (const auto& next_dir : directions) {
            Point next = node + next_dir;
            if (next.x < 0 || next.x > grid.get_row_size() - 1 || next.y < 0 || next.y > grid.get_col_size() - 1 || grid.get(next.x, next.y) == WALL) continue;  // Skip invalid positions

            int step_cost = 1;
            int rotation_cost = (next_dir == dir) ? 0 : 1;

            Cost new_cost = {cost.first + rotation_cost, cost.second + step_cost};

            // Update distance if this path is better or the same
            if (!distances.count({next, next_dir}) || new_cost < distances[{next, next_dir}]) {
                distances[{next, next_dir}] = new_cost;
                prev[next].clear();          // Clear previous paths (because we found a better path)
                prev[next].push_back(node);  // Store the current predecessor

                pq.push({new_cost, next, next_dir});
            } else if (new_cost == distances[{next, next_dir}]) {
                prev[next].push_back(node);  // Add the current node as an alternative predecessor
            }
        }
    }

    // Extract final distances for all nodes
    std::unordered_map<Point, std::pair<int, int>> result;
    for (auto& [key, value] : distances) {
        result[key.first] = value;
    }

    return {result, prev};
}

std::set<Point> in_path = {};

void dfs(Grid<GridType>& G, Point start, Point end, std::set<Point>& visited, std::vector<Point>& path, std::vector<Point>& best_path, int& best_dist) {
    if (start.x < 0 || start.x >= G.get_row_size() || start.y < 0 || start.y >= G.get_col_size()) {
        return;
    }
    if (start == end) {
        if (path.size() < best_dist) {
            best_dist = path.size();
            auto act_dir = get_direction_from_enum(RIGHT);
            int score = 0;
            for (int i = 0; i < path.size() - 1; i++) {
                auto dir = path[i + 1] - path[i];
                if (act_dir != dir) {
                    score += 1000;
                    act_dir = dir;
                }
                score += 1;
                std::cout << dir << std::endl;
            }
            std::cout << score << std::endl;
            best_path = path;
            std::cout << best_dist << std::endl;
        }
        return;
    }

    visited.insert(start);
    path.push_back(start);
    for (auto dir : {UP, DOWN, LEFT, RIGHT}) {
        auto next = start + get_direction_from_enum(dir);

        if (visited.find(next) == visited.end()) {
            if (G.get(next.x, next.y) != WALL) {
                dfs(G, next, end, visited, path, best_path, best_dist);
            }
        }
    }
    path.pop_back();
    visited.erase(start);
}

void dfs_caller(Grid<GridType>& G, Point start, Point end) {
    std::set<Point> visited;
    std::vector<Point> path;
    std::vector<Point> best_path;
    int best_dist = INT_MAX;

    dfs(G, start, end, visited, path, best_path, best_dist);
}

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
    std::set<std::pair<Point, Point>> visited;
    // std::cout << travel_grid(grid, start, end, dir) << std::endl;

    // for (auto p : possible) {
    //     grid.set(p.x, p.y, PATH);
    // }

    print_grid(grid);
    // std::cout << possible.size() << std::endl;
    auto adj_map = get_adj_map(grid);

    dfs_caller(grid, start, end);
    // auto result = bfs(adj_map, start, end);
    // auto distances = result.first;
    // auto prev = result.second;
    // auto act = end;
    // while (act != start) {
    //     std::cout << act << std::endl;
    //     print_vector(prev[act], ' ');
    //     act = prev[act][0];
    //     grid.set(act.x, act.y, PATH);
    // }
    // print_grid(grid);
    // Grid<int> dist_grid(grid.get_row_size(), grid.get_col_size(), INT_MAX);
    // auto result = dijkstra(grid, start, end, dist_grid);
    // auto distances = result.first;
    // auto prev = result.second;
    // std::cout << distances[end] << std::endl;

    // for (int i = 0; i < grid.get_row_size(); i++) {
    //     for (int j = 0; j < grid.get_col_size(); j++) {
    //         auto output = (dist_grid.get(i, j) == INT_MAX) ? -1 : dist_grid.get(i, j);
    //         std::cout << std::setw(5) << output;
    //     }
    //     std::cout << std::endl;
    // }

    // auto paths = bfs_backward(dist_grid, start, end);
    // for (auto p : paths) {
    //     grid.set(p.x, p.y, PATH);
    // }
    // std::cout << paths.size() << std::endl;
    // print_grid(grid);

    // auto sequence = get_sequence(prev, start, end);
    // std::cout << sequence.size() << std::endl;
    // for (auto p : sequence) {
    //     grid.set(p.x, p.y, PATH);
    // }

    return 0;
}
