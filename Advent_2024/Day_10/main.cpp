#include <set>

#include "../../Utils/utils.cpp"

std::pair<int, int> get_direction_from_enum_2(Direction dir) {
    if (dir == UP) {
        return {-1, 0};
    } else if (dir == DOWN) {
        return {1, 0};
    } else if (dir == LEFT) {
        return {0, -1};
    } else if (dir == RIGHT) {
        return {0, 1};
    }
    std::cout << "Invalid direction" << std::endl;
    return {0, 0};
}

int travel_one_trail(Grid<int>& grid, std::pair<int, int> act_pos, Direction dir, int prev_value, std::set<std::pair<std::pair<int, int>, Direction>> visited, std::set<std::pair<int, int>>& visited_nines) {
    if (visited.find({act_pos, dir}) != visited.end()) {
        return 0;
    }
    std::pair<int, int> direction = get_direction_from_enum_2(dir);
    visited.insert({act_pos, dir});
    act_pos.first += direction.first;
    act_pos.second += direction.second;
    if (act_pos.first < 0 || act_pos.first > grid.get_row_size() - 1 || act_pos.second < 0 || act_pos.second > grid.get_col_size() - 1) {
        return 0;
    }
    int act_value = grid.get(act_pos.first, act_pos.second);
    if (act_value - prev_value != 1) {
        return 0;
    }
    if (grid.get(act_pos.first, act_pos.second) == 9) {
        visited_nines.insert(act_pos);
        visited.insert({act_pos, dir});
        return 1;
    }

    return travel_one_trail(grid, act_pos, UP, act_value, visited, visited_nines) + travel_one_trail(grid, act_pos, DOWN, act_value, visited, visited_nines) + travel_one_trail(grid, act_pos, LEFT, act_value, visited, visited_nines) + travel_one_trail(grid, act_pos, RIGHT, act_value, visited, visited_nines);
}

std::pair<int, int> get_trail_sizes(Grid<int>& grid, std::pair<int, int> trail_head) {
    std::set<std::pair<std::pair<int, int>, Direction>> visited = {};
    std::set<std::pair<int, int>> visited_nines = {};
    int visited_nines_score = 0;
    int total_score = travel_one_trail(grid, trail_head, UP, 0, visited, visited_nines);
    total_score += travel_one_trail(grid, trail_head, DOWN, 0, visited, visited_nines);
    total_score += travel_one_trail(grid, trail_head, LEFT, 0, visited, visited_nines);
    total_score += travel_one_trail(grid, trail_head, RIGHT, 0, visited, visited_nines);
    visited_nines_score += visited_nines.size();
    return {visited_nines_score, total_score};
}

int main() {
    std::vector<std::string> lines = read_lines();
    std::vector<std::pair<int, int>> trail_heads = {};
    Grid grid = Grid(lines.size(), lines[0].size(), 0);
    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[i].size(); j++) {
            grid.set(i, j, lines[i][j] - '0');
            if (lines[i][j] == '0') {
                trail_heads.push_back({i, j});
            }
        }
    }
    int part_1_score = 0;
    int part_2_score = 0;
    for (auto trail_head : trail_heads) {
        auto scores = get_trail_sizes(grid, trail_head);
        part_1_score += scores.first;
        part_2_score += scores.second;
    }
    std::cout << "Part 1: " << part_1_score << '\n';
    std::cout << "Part 2: " << part_2_score << '\n';

    return 0;
}
