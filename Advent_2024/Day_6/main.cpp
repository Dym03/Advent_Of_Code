#include "../../Utils/utils.cpp"

struct Point {
    int x, y;
};

Point get_new_direction(char at_act_pos) {
    if (at_act_pos == '^') {
        return {0, 1};
    } else if (at_act_pos == 'v') {
        return {0, -1};
    } else if (at_act_pos == '<') {
        return {-1, 0};
    } else if (at_act_pos == '>') {
        return {1, 0};
    }
    std::cout << "Wrong character at actual position: " << at_act_pos << std::endl;
    return {0, 0};
}

int traverse_grid(std::vector<std::vector<char>> grid, Point act_pos, std::vector<std::vector<bool>> &visited) {
    int steps = 0;
    Point direction = {-1, 0};
    visited[act_pos.x][act_pos.y] = true;
    Point start_pos = act_pos;
    Point start_dir = direction;
    while (true) {
        if (act_pos.x < 0 || act_pos.x > grid.size() || act_pos.y < 0 || act_pos.y > grid[0].size()) {
            break;
        }
        if (act_pos.x + direction.x < 0 || act_pos.x + direction.x > grid.size() - 1 || act_pos.y + direction.y < 0 || act_pos.y + direction.y > grid[0].size() - 1) {
            visited[act_pos.x][act_pos.y] = true;
            break;
        }
        if (grid[act_pos.x + direction.x][act_pos.y + direction.y] == '#') {
            direction = get_new_direction(grid[act_pos.x][act_pos.y]);
            char dir_char;
            if (direction.x == 0 && direction.y == -1) {
                dir_char = '<';
            } else if (direction.x == 0 && direction.y == 1) {
                dir_char = '>';
            } else if (direction.x == -1 && direction.y == 0) {
                dir_char = '^';
            } else if (direction.x == 1 && direction.y == 0) {
                dir_char = 'v';
            }
            grid[act_pos.x][act_pos.y] = dir_char;
        } else {
            visited[act_pos.x][act_pos.y] = true;
            char dir_char = grid[act_pos.x][act_pos.y];
            grid[act_pos.x][act_pos.y] = 'X';
            act_pos.x += direction.x;
            act_pos.y += direction.y;
            grid[act_pos.x][act_pos.y] = dir_char;
        }
    }
    return steps;
}

int traverse_grid_p2(std::vector<std::vector<char>> grid, Point act_pos, std::vector<std::vector<std::pair<bool, std::pair<int, int>>>> visited) {
    int steps = 0;
    Point direction = {-1, 0};
    Point start_pos = act_pos;
    Point start_dir = direction;
    while (true) {
        if (act_pos.x < 0 || act_pos.x > grid.size() || act_pos.y < 0 || act_pos.y > grid[0].size()) {
            break;
        }
        if (act_pos.x + direction.x < 0 || act_pos.x + direction.x > grid.size() - 1 || act_pos.y + direction.y < 0 || act_pos.y + direction.y > grid[0].size() - 1) {
            visited[act_pos.x][act_pos.y].first = true;
            break;
        }
        if (visited[act_pos.x][act_pos.y].first && visited[act_pos.x][act_pos.y].second.first == direction.x && visited[act_pos.x][act_pos.y].second.second == direction.y) {
            return -1;
        }

        if (grid[act_pos.x + direction.x][act_pos.y + direction.y] == '#') {
            direction = get_new_direction(grid[act_pos.x][act_pos.y]);
            char dir_char;
            if (direction.x == 0 && direction.y == -1) {
                dir_char = '<';
            } else if (direction.x == 0 && direction.y == 1) {
                dir_char = '>';
            } else if (direction.x == -1 && direction.y == 0) {
                dir_char = '^';
            } else if (direction.x == 1 && direction.y == 0) {
                dir_char = 'v';
            }
            grid[act_pos.x][act_pos.y] = dir_char;
        } else {
            visited[act_pos.x][act_pos.y].first = true;
            visited[act_pos.x][act_pos.y].second = {direction.x, direction.y};
            char dir_char = grid[act_pos.x][act_pos.y];
            grid[act_pos.x][act_pos.y] = 'X';
            act_pos.x += direction.x;
            act_pos.y += direction.y;
            grid[act_pos.x][act_pos.y] = dir_char;
        }
    }
    return 0;
}

int main() {
    std::vector<std::string> lines = read_lines();
    std::vector<std::vector<char>> grid = {};
    std::vector<std::vector<bool>> visited = {};
    std::vector<std::vector<std::pair<bool, std::pair<int, int>>>> visited_p_2 = {};

    Point act_pos;
    for (int i = 0; i < lines.size(); i++) {
        grid.push_back({});
        visited.push_back({});
        visited_p_2.push_back({});
        for (int j = 0; j < lines[i].size(); j++) {
            grid.back().push_back(lines[i][j]);
            visited.back().push_back(false);
            visited_p_2.back().push_back({false, {0, 0}});
            if (lines[i][j] == '^') {
                act_pos = {i, j};
            }
        }
    }
    int count_p_2 = 0;
    traverse_grid(grid, act_pos, visited);
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] != '^' && grid[i][j] != '#' && visited[i][j]) {
                grid[i][j] = '#';
                if (traverse_grid_p2(grid, act_pos, visited_p_2) == -1) {
                    count_p_2++;
                };
                grid[i][j] = '.';
            }
        }
    }
    int count = 0;
    for (int i = 0; i < visited.size(); i++) {
        for (int j = 0; j < visited[i].size(); j++) {
            if (visited[i][j]) {
                count++;
            }
        }
    }
    std::cout << "Visited: " << count << std::endl;
    std::cout << "Visited p2: " << count_p_2 << std::endl;
    return 0;
}
