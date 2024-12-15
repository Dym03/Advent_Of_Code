#include <sys/wait.h>

#include "../../Utils/utils.cpp"

enum GridType {
    EMPTY,
    ROBOT,
    WALL,
    BOX,
    BOXLEFT,
    BOXRIGHT,
};

Point move_one_step_p2(Point dir, Point act_pos, Grid<GridType>& G);

Point move_one_step(Point dir, Point act_pos, Grid<GridType>& G) {
    Point new_pos = act_pos + dir;
    if (new_pos.x < 0 || new_pos.x > G.get_row_size() - 1 || new_pos.y < 0 || new_pos.y > G.get_col_size() - 1) {
        return act_pos;
    }
    if (G.get(new_pos.x, new_pos.y) == WALL) {
        return act_pos;
    }
    if (G.get(new_pos.x, new_pos.y) == EMPTY) {
        GridType act_type = G.get(act_pos.x, act_pos.y);
        G.set(act_pos.x, act_pos.y, EMPTY);
        G.set(new_pos.x, new_pos.y, act_type);
        return new_pos;
    }
    if (G.get(new_pos.x, new_pos.y) == BOX) {
        if (move_one_step(dir, new_pos, G) != new_pos) {
            GridType act_type = G.get(act_pos.x, act_pos.y);
            G.set(act_pos.x, act_pos.y, EMPTY);
            G.set(new_pos.x, new_pos.y, act_type);
            return new_pos;
        } else {
            return act_pos;
        }
    }
    std::cout << "Something went wrong\n";
    return act_pos;
}

void run_simulation(Point act_pos, Grid<GridType>& G, std::vector<Direction> steps) {
    for (auto dir : steps) {
        Point dx = get_direction_from_enum(dir);
        act_pos = move_one_step(dx, act_pos, G);
    }
}

bool can_move_one_step_p2(Point dir, Point act_pos, Grid<GridType> G) {
    Point new_pos = act_pos + dir;
    if (new_pos.x < 0 || new_pos.x > G.get_row_size() - 1 || new_pos.y < 0 || new_pos.y > G.get_col_size() - 1) {
        return false;
    }
    if (G.get(new_pos.x, new_pos.y) == WALL) {
        return false;
    }
    GridType act_type = G.get(act_pos.x, act_pos.y);
    if (G.get(new_pos.x, new_pos.y) == EMPTY && act_type == ROBOT) {
        GridType act_type = G.get(act_pos.x, act_pos.y);
        G.set(act_pos.x, act_pos.y, EMPTY);
        G.set(new_pos.x, new_pos.y, act_type);
        return true;
    }
    if (G.get(new_pos.x, new_pos.y) == EMPTY && act_type == BOXLEFT) {
        GridType act_type = G.get(act_pos.x, act_pos.y);
        G.set(act_pos.x, act_pos.y, EMPTY);
        G.set(new_pos.x, new_pos.y, act_type);
        return true;
    }
    if (G.get(new_pos.x, new_pos.y) == EMPTY && act_type == BOXRIGHT) {
        GridType act_type = G.get(act_pos.x, act_pos.y);
        G.set(act_pos.x, act_pos.y, EMPTY);
        G.set(new_pos.x, new_pos.y, act_type);
        return true;
    }
    if (G.get(new_pos.x, new_pos.y) == BOXLEFT) {
        if (can_move_one_step_p2(dir, new_pos + Point{0, 1}, G) && can_move_one_step_p2(dir, new_pos, G)) {
            GridType act_type = G.get(act_pos.x, act_pos.y);
            G.set(act_pos.x, act_pos.y, EMPTY);
            G.set(new_pos.x, new_pos.y, act_type);
            return true;
        } else {
            return false;
        }
    }
    if (G.get(new_pos.x, new_pos.y) == BOXRIGHT) {
        if (can_move_one_step_p2(dir, new_pos + Point{0, -1}, G) && can_move_one_step_p2(dir, new_pos, G)) {
            GridType act_type = G.get(act_pos.x, act_pos.y);
            G.set(act_pos.x, act_pos.y, EMPTY);
            G.set(new_pos.x, new_pos.y, act_type);
            return true;
        } else {
            return false;
        }
    }

    std::cout << "Something went wrong\n";
    return false;
}

Point move_one_step_p2(Point dir, Point act_pos, Grid<GridType>& G) {
    Point new_pos = act_pos + dir;
    if (new_pos.x < 0 || new_pos.x > G.get_row_size() - 1 || new_pos.y < 0 || new_pos.y > G.get_col_size() - 1) {
        return act_pos;
    }
    if (G.get(new_pos.x, new_pos.y) == WALL) {
        return act_pos;
    }
    GridType act_type = G.get(act_pos.x, act_pos.y);
    if (G.get(new_pos.x, new_pos.y) == EMPTY && act_type == ROBOT) {
        GridType act_type = G.get(act_pos.x, act_pos.y);
        G.set(act_pos.x, act_pos.y, EMPTY);
        G.set(new_pos.x, new_pos.y, act_type);
        return new_pos;
    }
    if (G.get(new_pos.x, new_pos.y) == EMPTY && act_type == BOXLEFT) {
        GridType act_type = G.get(act_pos.x, act_pos.y);
        G.set(act_pos.x, act_pos.y, EMPTY);
        G.set(new_pos.x, new_pos.y, act_type);
        return new_pos;
    }
    if (G.get(new_pos.x, new_pos.y) == EMPTY && act_type == BOXRIGHT) {
        GridType act_type = G.get(act_pos.x, act_pos.y);
        G.set(act_pos.x, act_pos.y, EMPTY);
        G.set(new_pos.x, new_pos.y, act_type);
        return new_pos;
    }
    if (G.get(new_pos.x, new_pos.y) == BOXLEFT) {
        auto G_copy = G;
        if (move_one_step_p2(dir, new_pos + Point{0, 1}, G_copy) != (new_pos + Point(0, 1)) && move_one_step_p2(dir, new_pos, G_copy) != new_pos) {
            move_one_step_p2(dir, new_pos + Point{0, 1}, G);
            move_one_step_p2(dir, new_pos, G);
            GridType act_type = G.get(act_pos.x, act_pos.y);
            G.set(act_pos.x, act_pos.y, EMPTY);
            G.set(new_pos.x, new_pos.y, act_type);
            return new_pos;
        } else {
            return act_pos;
        }
    }
    if (G.get(new_pos.x, new_pos.y) == BOXRIGHT) {
        auto G_copy = G;
        if (move_one_step_p2(dir, new_pos + Point{0, -1}, G_copy) != (new_pos + Point(0, -1)) && move_one_step_p2(dir, new_pos, G_copy) != new_pos) {
            move_one_step_p2(dir, new_pos + Point{0, -1}, G);
            move_one_step_p2(dir, new_pos, G);
            GridType act_type = G.get(act_pos.x, act_pos.y);
            G.set(act_pos.x, act_pos.y, EMPTY);
            G.set(new_pos.x, new_pos.y, act_type);
            return new_pos;
        } else {
            return act_pos;
        }
    }
    std::cout << "Something went wrong\n";
    return act_pos;
}

void print_grid(Grid<GridType>& G) {
    for (int i = 0; i < G.get_row_size(); i++) {
        for (int j = 0; j < G.get_col_size(); j++) {
            GridType c = G.get(i, j);
            if (c == ROBOT) {
                std::cout << '@';
            } else if (c == WALL) {
                std::cout << '#';
            } else if (c == BOX) {
                std::cout << 'O';
            } else if (c == BOXLEFT) {
                std::cout << '[';
            } else if (c == BOXRIGHT) {
                std::cout << ']';
            } else {
                std::cout << '.';
            }
        }
        std::cout << '\n';
    }
}

void run_simulation_p2(Point act_pos, Grid<GridType>& G, std::vector<Direction> steps) {
    for (auto dir : steps) {
        Point dx = get_direction_from_enum(dir);
        act_pos = move_one_step_p2(dx, act_pos, G);
    }
}

int get_score(Grid<GridType>& G) {
    int score = 0;
    for (int i = 0; i < G.get_row_size(); i++) {
        for (int j = 0; j < G.get_col_size(); j++) {
            GridType c = G.get(i, j);
            if (c == BOX) {
                score += i * 100 + j;
            }
            if (c == BOXLEFT) {
                score += i * 100 + j;
            }
        }
    }
    return score;
}

Grid<GridType> generate_new_grid(Grid<GridType>& G, Point& start_pos) {
    Grid<GridType> new_G = {G.get_row_size(), G.get_col_size() * 2, EMPTY};
    for (int i = 0; i < G.get_row_size(); i++) {
        for (int j = 0; j < G.get_col_size(); j++) {
            int new_j = j * 2;
            GridType c = G.get(i, j);
            if (c == ROBOT) {
                start_pos = {i, new_j};
                new_G.set(i, new_j, ROBOT);
                new_G.set(i, new_j + 1, EMPTY);
            } else if (c == WALL) {
                new_G.set(i, new_j, WALL);
                new_G.set(i, new_j + 1, WALL);
            } else if (c == BOX) {
                new_G.set(i, new_j, BOXLEFT);
                new_G.set(i, new_j + 1, BOXRIGHT);
            } else {
                new_G.set(i, new_j, EMPTY);
                new_G.set(i, new_j + 1, EMPTY);
            }
        }
    }
    return new_G;
}

int main() {
    std::vector<std::string> lines = read_lines();
    bool loading_grid = true;
    std::vector<Direction> moves = {};
    Grid<GridType> G;
    Point start_pos = {0, 0};

    for (int i = 0; i < lines.size(); i++) {
        if (lines[i].size() == 0) {
            loading_grid = false;
            continue;
        }
        if (loading_grid) {
            G.grid.push_back({});
        }
        for (int j = 0; j < lines[i].size(); j++) {
            if (loading_grid) {
                if (lines[i][j] == '@') {
                    start_pos = {i, j};
                    G.grid.back().push_back(ROBOT);
                } else if (lines[i][j] == 'O') {
                    G.grid.back().push_back(BOX);
                } else if (lines[i][j] == '#') {
                    G.grid.back().push_back(WALL);
                } else {
                    G.grid.back().push_back(EMPTY);
                }
            } else {
                if (lines[i][j] == '>') {
                    moves.push_back(RIGHT);
                } else if (lines[i][j] == '^') {
                    moves.push_back(UP);
                } else if (lines[i][j] == 'v') {
                    moves.push_back(DOWN);
                } else if (lines[i][j] == '<') {
                    moves.push_back(LEFT);
                }
            }
        }
    }
    Point start_pos_p2 = {0, 0};
    auto new_G = generate_new_grid(G, start_pos_p2);
    run_simulation(start_pos, G, moves);
    std::cout << "PART 1 " << get_score(G) << std::endl;
    run_simulation_p2(start_pos_p2, new_G, moves);
    std::cout << "PART 2 " << get_score(new_G) << std::endl;
    return 0;
}
