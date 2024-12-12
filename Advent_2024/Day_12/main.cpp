#include <climits>
#include <queue>
#include <set>
#include <unordered_map>

#include "../../Utils/utils.cpp"

std::vector<Point> find_component(Point root, std::set<Point>& seen, std::unordered_map<Point, std::vector<Point>>& adjency_list) {
    std::queue<Point> Q;
    std::vector<Point> component;
    seen.insert(root);
    Q.push(root);
    while (!Q.empty()) {
        Point v = Q.front();
        Q.pop();
        component.push_back(v);
        for (auto point : adjency_list[v]) {
            if (seen.count(point) == 0) {
                seen.insert(point);
                Q.push(point);
            }
        }
    }
    return component;
}

std::vector<std::vector<Point>> find_all_components(Grid<char> G, std::unordered_map<Point, std::vector<Point>>& adjency_list, std::set<Point>& seen) {
    std::vector<std::vector<Point>> components;
    for (int i = 0; i < G.get_row_size(); i++) {
        for (int j = 0; j < G.get_col_size(); j++) {
            Point p = {i, j};
            if (seen.count(p) > 0) {
                continue;
            }
            std::vector<Point> component = find_component(p, seen, adjency_list);
            components.push_back(component);
        }
    }
    return components;
}

void get_adjency_list(Grid<char> G, std::unordered_map<Point, std::vector<Point>>& adjency_list) {
    for (int i = 0; i < G.get_row_size(); i++) {
        for (int j = 0; j < G.get_col_size(); j++) {
            Point p = {i, j};
            std::vector<Direction> dirs = {UP, DOWN, LEFT, RIGHT};
            for (auto dir : dirs) {
                Point d = get_direction_from_enum(dir);
                d += p;
                if (d.x < 0 || d.x > G.get_row_size() - 1 || d.y < 0 || d.y > G.get_col_size() - 1) {
                    continue;
                }
                if (G.get(p.x, p.y) != G.get(d.x, d.y)) {
                    continue;
                }
                adjency_list[p].push_back(d);
            }
        }
    }
}

// Doesnt work for not convex parts
std::tuple<int, int, int, int> find_component_bbox(std::vector<Point>& component) {
    int max_x = INT_MIN;
    int max_y = INT_MIN;
    int min_x = INT_MAX;
    int min_y = INT_MAX;
    for (auto p : component) {
        if (p.x < min_x) {
            min_x = p.x;
        }
        if (p.x > max_x) {
            max_x = p.x;
        }
        if (p.y < min_y) {
            min_y = p.y;
        }
        if (p.y > max_y) {
            max_y = p.y;
        }
    }
    return {min_x, max_x, min_y, max_y};
}

int num_edges(std::vector<Point>& edge_points, std::unordered_map<Point, std::vector<Point>>& adj_list) {
    int num_edges = 0;
    for (auto component : edge_points) {
        if (adj_list[component].size() == 2) {
            num_edges += 2;
        }
        if (adj_list[component].size() == 3) {
            num_edges += 1;
        }
        if (adj_list[component].size() == 1) {
            num_edges += 3;
        }
    }

    return num_edges;
}

std::vector<Point> get_edge_points(std::vector<Point> component, std::unordered_map<Point, std::vector<Point>>& adj_list) {
    std::vector<Point> edge_points = {};
    for (auto point : component) {
        if (adj_list[point].size() == 4) {
            continue;
        }
        edge_points.push_back(point);
    }
    return edge_points;
}

int main() {
    std::vector<std::string> lines = read_lines();
    Grid grid(lines.size(), lines[0].size(), '0');
    std::vector<std::vector<Point>> compontents;
    std::unordered_map<Point, std::vector<Point>> adjency_list;
    compontents.resize(26);
    std::set<Point> seen;
    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[i].size(); j++) {
            grid.set(i, j, lines[i][j]);
        }
    }
    get_adjency_list(grid, adjency_list);
    grid.print();
    auto components = find_all_components(grid, adjency_list, seen);
    size_t total = 0;
    for (auto component : components) {
        print_vector(component, ' ');
        auto bbox = find_component_bbox(component);
        std::vector<Point> edge_points = get_edge_points(component, adjency_list);
        std::cout << "Edge Points : ";
        print_vector(edge_points, ' ');
        int edges_count = num_edges(edge_points, adjency_list);
        int area = component.size();
        std::cout << "Num edges = " << edges_count << std::endl;
        if (area == 1) {
            total += 4;
        } else {
            total += area * edges_count;
        }
    }
    std::cout << total << std::endl;

    return 0;
}
