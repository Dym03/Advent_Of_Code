#include <climits>
#include <queue>
#include <set>
#include <unordered_map>

#include "../../Utils/utils.cpp"

enum GridType {
    FREE,
    WALL,
    START,
    END,
};

void print_grid(Grid<GridType>& g) {
    for (auto r : g.grid) {
        for (auto c : r) {
            if (c == FREE) {
                std::cout << ".";
            } else if (c == WALL) {
                std::cout << "#";
            } else if (c == START) {
                std::cout << "S";
            } else if (c == END) {
                std::cout << "E";
            }
        }
        std::cout << std::endl;
    }
}

int min_lenght = INT_MAX;

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

std::unordered_map<Point, int> dijkstra(Grid<GridType>& grid, Point start, Point end) {
    std::unordered_map<Point, int> distances;
    std::unordered_map<Point, Point> prev;
    auto adj_map = get_adj_map(grid);
    for (int i = 0; i < grid.get_row_size(); i++) {
        for (int j = 0; j < grid.get_col_size(); j++) {
            distances[{i, j}] = INT_MAX;
            prev[{i, j}] = {};
        }
    }
    distances[start] = 0;

    using State = std::tuple<int, Point>;
    std::priority_queue<State, std::vector<State>, std::greater<>> pq;
    std::set<Point> visited;
    pq.push({0, start});
    while (!pq.empty()) {
        auto [dist, node] = pq.top();
        pq.pop();

        // Allow revisiting nodes if cost is optimal
        if (dist > distances[node]) {
            continue;
        }

        // Explore neighbors
        for (auto next : adj_map[node]) {
            int new_dist = dist + 1;
            if (new_dist < distances[next]) {
                distances[next] = new_dist;
                pq.push({new_dist, next});
            }
        }
    }

    return distances;
}

void dfs(Grid<GridType>& g, Point act_pos, Point end, int length, std::set<Point> visited) {
    if (length > min_lenght) {
        return;
    }
    if (act_pos.x < 0 || act_pos.x > g.get_row_size() - 1 || act_pos.y < 0 || act_pos.y > g.get_col_size() - 1) {
        return;
    }
    if (g.get(act_pos.x, act_pos.y) == WALL) {
        return;
    }
    if (act_pos == end) {
        min_lenght = std::min(min_lenght, length);
        std::cout << "New min length = " << min_lenght << std::endl;
    }
    if (visited.count(act_pos) > 0) {
        return;
    }
    visited.insert(act_pos);
    // std::cout << act_pos << std::endl;
    for (auto dir : {UP, DOWN, LEFT, RIGHT}) {
        Point dx = get_direction_from_enum(dir);
        dfs(g, act_pos + dx, end, length + 1, visited);
    }
}

int main() {
    std::vector<std::string> lines = read_lines();
    std::vector<Point> blocks;
    for (std::string line : lines) {
        std::vector<std::string> coords = split(',', line);
        Point p = {std::stoi(coords[0]), std::stoi(coords[1])};
        blocks.push_back(p);
    }
    int grid_size = 71;  // For real input 71
    Grid<GridType> g(grid_size, grid_size, FREE);
    int j = 0;
    Point start = {0, 0};
    g.set(start.x, start.y, START);
    Point end = {grid_size - 1, grid_size - 1};
    g.set(end.x, end.y, END);
    while (1) {
        int num_blocks = 1024 + j;  // For real input 1024;
        for (int i = 0; i < num_blocks; i++) {
            Point block = blocks[i];
            g.set(block.x, block.y, WALL);
        }
        auto dist = dijkstra(g, start, end);
        if (dist[end] == INT_MAX) {
            std::cout << num_blocks;
            std::cout << blocks[num_blocks - 1];
            break;
        }
        std::cout << dist[end] << std::endl;
        j += 1;
    }
    print_grid(g);

    return 0;
}
