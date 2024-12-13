#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void ltrim(std::string &input) {
    input = input.substr(input.find_first_not_of(" "));
}

void rtrim(std::string &input) {
    input = input.substr(0, input.find_last_not_of(" ") + 1);
}

void trim(std::string &input) {
    ltrim(input);
    rtrim(input);
}

std::vector<std::string> read_lines() {
    std::vector<std::string> lines;
    std::string line;
    while (getline(std::cin, line)) {
        lines.push_back(line);
    }

    return lines;
}

std::vector<std::string> split(char delim, std::string input) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream iss(input);
    while (std::getline(iss, token, delim)) {
        if (token.length() != 0) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

template <typename T>
void print_vector(std::vector<T> inp, char delim) {
    for (auto a : inp) {
        std::cout << a << delim;
    }
    std::cout << '\n';
}

template <typename T>
void print_grid(std::vector<std::vector<T>> grid, char delim) {
    for (auto row : grid) {
        print_vector(row, delim);
    }
    std::cout << '\n';
}

struct Point {
    int x;
    int y;
    Point(int x, int y) : x(x), y(y) {}

    Point operator+(const Point &other) const {
        return Point(this->x + other.x, this->y + other.y);
    }

    Point operator-(const Point &other) const {
        return Point(this->x - other.x, this->y - other.y);
    }

    Point &operator+=(const Point &other) {
        *this = *this + other;
        return *this;
    }

    Point &operator-=(const Point &other) {
        *this = *this - other;
        return *this;
    }

    bool operator==(const Point &other) const {
        return this->x == other.x && this->y == other.y;
    }
    bool operator!=(const Point &other) const { return !(*this == other); }

    bool operator<(const Point &other) const {
        return this->x < other.x || (this->x == other.x && this->y < other.y);
    }

    bool operator>(const Point &other) const {
        return this->x > other.x || (this->x == other.x && this->y > other.y);
    }
};

template <>
struct std::hash<Point> {
    size_t operator()(const Point &p) const {
        size_t hash1 = std::hash<int>{}(p.x);
        // Hash the second element
        size_t hash2 = std::hash<int>{}(p.y);
        return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
    }
};

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2> &p) const {
        // Hash the first element
        size_t hash1 = std::hash<T1>{}(p.first);
        // Hash the second element
        size_t hash2 = std::hash<T2>{}(p.second);
        // Combine the two hash values
        return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
    }
};

std::ostream &operator<<(std::ostream &os, const Point &point) {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}

enum Direction { UP,
                 DOWN,
                 LEFT,
                 RIGHT };

Point get_direction_from_enum(Direction dir) {
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

template <typename T>
class Grid {
   public:
    std::vector<std::vector<T>> grid;
    Grid(int rows, int cols, T fill_value) {
        grid = std::vector<std::vector<T>>(rows, std::vector<T>(cols, fill_value));
    }

    void print(char delim) {
        for (auto row : grid) {
            print_vector(row, delim);
        }
    }

    void print() {
        for (auto row : grid) {
            print_vector(row, ' ');
        }
    }

    void add_row(std::vector<T> row) { grid.push_back(row); }

    void set_row(int row, std::vector<T> values) { grid[row] = values; }

    T get(int row, int col) { return grid[row][col]; }

    void set(int row, int col, T value) { grid[row][col] = value; }

    void set_col(int col, std::vector<T> values) {
        for (int i = 0; i < grid.size(); i++) {
            grid[i][col] = values[i];
        }
    }

    int get_row_size() { return this->grid.size(); }

    int get_col_size() {
        if (grid.size() > 0) {
            return this->grid[0].size();
        }
        return 0;
    }
};