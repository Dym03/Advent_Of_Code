#include <climits>
#include <queue>
#include <set>
#include <unordered_map>

#include "../../Utils/utils.cpp"

std::set<Point> find_component(Point root, std::set<Point>& seen, std::unordered_map<Point, std::vector<Point>>& adjency_list) {
    std::queue<Point> Q;
    std::set<Point> component;
    seen.insert(root);
    Q.push(root);
    while (!Q.empty()) {
        Point v = Q.front();
        Q.pop();
        component.insert(v);
        for (auto point : adjency_list[v]) {
            if (seen.count(point) == 0) {
                seen.insert(point);
                Q.push(point);
            }
        }
    }
    return component;
}

std::vector<std::set<Point>> find_all_components(Grid<char> G, std::unordered_map<Point, std::vector<Point>>& adjency_list, std::set<Point>& seen) {
    std::vector<std::set<Point>> components;
    for (int i = 0; i < G.get_row_size(); i++) {
        for (int j = 0; j < G.get_col_size(); j++) {
            Point p = {i, j};
            if (seen.count(p) > 0) {
                continue;
            }
            std::set<Point> component = find_component(p, seen, adjency_list);
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

int num_edges(std::set<Point>& edge_points, std::unordered_map<Point, std::vector<Point>>& adj_list) {
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

std::set<Point> get_edge_points(std::set<Point> component, std::unordered_map<Point, std::vector<Point>>& adj_list) {
    std::set<Point> edge_points = {};
    for (auto point : component) {
        if (adj_list[point].size() == 4) {
            continue;
        }
        edge_points.insert(point);
    }
    return edge_points;
}

// Solution for P2 inspired by https://www.youtube.com/watch?v=glNiVe_Rztg
std::set<std::pair<Point, Point>> get_perimetr(std::set<Point> component, Grid<char>& G) {
    std::set<std::pair<Point, Point>> perimeter;
    std::vector<Direction> directions = {UP, DOWN, LEFT, RIGHT};
    for (auto node : component) {
        for (auto dir : directions) {
            auto dir_point = get_direction_from_enum(dir);
            Point d_node = node + dir_point;
            // If the node is on the border of the grid, or it is not touching other node in the direction then add it to the list, for p1 just perimetr += 1
            if (d_node.x < 0 || d_node.x > G.get_row_size() || d_node.y < 0 || d_node.y > G.get_col_size() || component.count(d_node) == 0) {
                perimeter.insert({node, d_node});
            }
        }
    }

    // Now we have to get rid of the perimetrs that are not needed, we just keep the last of the perimetr in the direction
    std::set<std::pair<Point, Point>> perimeter_2;
    directions.clear();
    directions = {DOWN, RIGHT};
    for (auto node : perimeter) {
        bool keep = true;
        for (auto dir : directions) {
            auto dir_point = get_direction_from_enum(dir);
            Point p1_n = node.first + dir_point;
            Point p2_n = node.second + dir_point;
            if (perimeter.count({p1_n, p2_n}) > 0) {
                keep = false;
            }
        }
        if (keep) {
            perimeter_2.insert({node.first, node.second});
        }
    }
    return perimeter_2;
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
    size_t total_p_1 = 0;
    size_t total_p_2 = 0;
    for (auto component : components) {
        std::set<Point> edge_points = get_edge_points(component, adjency_list);
        std::cout << "Edge Points : ";
        int edges_count = num_edges(edge_points, adjency_list);
        auto perim_2 = get_perimetr(component, grid);
        std::cout << "Perimetr 2 = " << perim_2.size() << std::endl;
        int area = component.size();
        std::cout << "Num edges = " << edges_count << std::endl;
        if (area == 1) {
            total_p_1 += 4;
        } else {
            total_p_1 += area * edges_count;
        }
        total_p_2 += area * perim_2.size();
    }
    std::cout << total_p_1 << std::endl;
    std::cout << total_p_2 << std::endl;

    return 0;
}
