#include "../../Utils/utils.cpp"

bool is_visible_in_dir(Grid<int>& grid, int i, int j, std::pair<int, int> dir, int& trees_seen) {
    int tree_height = grid.get(i, j);
    i += dir.first;
    j += dir.second;
    while (i >= 0 && i < grid.get_row_size() && j >= 0 && j < grid.get_col_size()) {
        trees_seen += 1;
        if (grid.get(i, j) >= tree_height) {
            return false;
        }
        i += dir.first;
        j += dir.second;
    }
    return true;
}

int find_visible_in_grid(Grid<int>& grid) {
    int visible_count = 0;
    for (int i = 1; i < grid.get_row_size() - 1; i++) {
        for (int j = 1; j < grid.get_col_size() - 1; j++) {
            int trees_seen = 0;
            if (is_visible_in_dir(grid, i, j, {1, 0}, trees_seen)) {
                visible_count++;
                continue;
            }
            if (is_visible_in_dir(grid, i, j, {0, 1}, trees_seen)) {
                visible_count++;
                continue;
            }
            if (is_visible_in_dir(grid, i, j, {0, -1}, trees_seen)) {
                visible_count++;
                continue;
            }
            if (is_visible_in_dir(grid, i, j, {-1, 0}, trees_seen)) {
                visible_count++;
                continue;
            }
        }
    }
    return visible_count;
}

int find_scenic_score_in_grid(Grid<int>& grid) {
    int max_visible_count = 0;
    for (int i = 1; i < grid.get_row_size() - 1; i++) {
        for (int j = 1; j < grid.get_col_size() - 1; j++) {
            int total_trees_seen = 1;
            int trees_seen = 0;
            is_visible_in_dir(grid, i, j, {1, 0}, trees_seen);

            total_trees_seen *= trees_seen;
            trees_seen = 0;
            is_visible_in_dir(grid, i, j, {0, 1}, trees_seen);

            total_trees_seen *= trees_seen;
            trees_seen = 0;
            is_visible_in_dir(grid, i, j, {0, -1}, trees_seen);

            total_trees_seen *= trees_seen;
            trees_seen = 0;

            is_visible_in_dir(grid, i, j, {-1, 0}, trees_seen);

            total_trees_seen *= trees_seen;
            max_visible_count = std::max(max_visible_count, total_trees_seen);
        }
    }
    return max_visible_count;
}

int main() {
    std::vector<std::string> lines;
    lines = read_lines();
    Grid<int> grid(lines.size(), lines[0].size(), 0);
    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[0].size(); j++) {
            grid.set(i, j, lines[i][j] - '0');
        }
    }
    int visible_count = grid.get_row_size() * 2 + grid.get_col_size() * 2 - 4;
    std::cout << visible_count << std::endl;
    int visible_count_interior = find_visible_in_grid(grid);
    int best_scenic_score = find_scenic_score_in_grid(grid);
    std::cout << visible_count << " " << visible_count_interior << std::endl;
    visible_count += visible_count_interior;
    std::cout << "Part 1 = " << visible_count << std::endl;
    std::cout << "Part 2 = " << best_scenic_score << std::endl;
}
