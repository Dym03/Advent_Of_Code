#include <climits>
#include <cmath>
#include <numeric>

#include "../../Utils/utils.cpp"

struct Button {
    int x;
    int y;
    int current_x;
    int current_y;
};

struct Prize {
    long long X;
    long long Y;
};

Button get_button(std::vector<std::string> line) {
    Button b = {};
    b.current_x = 0;
    b.current_y = 0;
    std::vector<std::string> x_y = split(',', line[1]);
    std::string x = x_y[0];
    std::string y = x_y[1];
    b.x = std::stoi(split('+', x)[1]);
    b.y = std::stoi(split('+', y)[1]);
    return b;
}

Prize get_prize(std::vector<std::string> line) {
    Prize p = {};
    p.X = 0;
    p.Y = 0;
    std::vector<std::string> x_y = split(',', line[1]);
    std::string x = x_y[0];
    std::string y = x_y[1];
    p.X = std::stoi(split('=', x)[1]) + 10000000000000;
    p.Y = std::stoi(split('=', y)[1]) + 10000000000000;
    return p;
}

std::ostream& operator<<(std::ostream& os, Button b) {
    os << b.x << " " << b.y << " Curr value " << b.current_x << "," << b.current_y;
    return os;
}

std::ostream& operator<<(std::ostream& os, Prize p) {
    os << p.X << " " << p.Y;
    return os;
}

int find_lowest_token_win(Button A, Button B, Prize P) {
    int lowest_token_count = INT_MAX;
    for (int i = 0; i <= 100; i++) {
        if (A.current_x > P.X || A.current_y > P.Y) {
            break;
        }
        B.current_x = 0;
        B.current_y = 0;
        for (int j = 0; j <= 100; j++) {
            if (A.current_x + B.current_x == P.X && A.current_y + B.current_y == P.Y) {
                int token_count = i * 3 + j * 1;
                lowest_token_count = std::min(token_count, lowest_token_count);
            }
            if (A.current_x + B.current_x > P.X || A.current_y + B.current_y > P.Y) {
                break;
            }
            B.current_x += B.x;
            B.current_y += B.y;
        }
        A.current_x += A.x;
        A.current_y += A.y;
    }
    return lowest_token_count;
}

long long find_lowest_cramer(Button A, Button B, Prize P) {
    long long lowest_token_count = LONG_LONG_MAX;
    long long determinant = (long long)A.x * (long long)B.y - (long long)A.y * (long long)B.x;

    if (determinant != 0) {
        long long delta1 = (long long)P.X * B.y - (long long)B.x * P.Y;
        long long delta2 = (long long)P.Y * A.x - (long long)P.X * A.y;

        if (delta1 % determinant == 0 && delta2 % determinant == 0) {
            long long x = delta1 / determinant;
            long long y = delta2 / determinant;
            long long token_count = x * 3 + y * 1;
            lowest_token_count = std::min(token_count, lowest_token_count);
        }
    }

    return lowest_token_count;
}

std::pair<double, double> get_button_press_count(Button A, Button B, Prize P) {
    double ca = (P.X * B.y - P.Y * B.x) / (A.x * B.y - A.y * B.x);
    double cb = (P.X - A.x * ca) / B.x;
    double ipart;
    double ipart_1;
    if (std::modf(ca, &ipart) == 0 && std::modf(cb, &ipart_1) == 0) {
        if (std::fmod(ca, 1.0) == 0 && std::fmod(cb, 1.0) == 0) {
            if (trunc(ca) == ca && trunc(cb) == cb) {
                return {ca, cb};
            }
        }
    }

    return {0, 0};
}

int main() {
    std::vector<std::string> lines = read_lines();
    long long total = 0;
    long long cramer_total = 0;
    for (int i = 0; i < lines.size(); i += 4) {
        std::vector<std::string> line = split(':', lines[i]);
        Button A = get_button(line);
        std::vector<std::string> line_1 = split(':', lines[i + 1]);
        Button B = get_button(line_1);
        std::vector<std::string> line_2 = split(':', lines[i + 2]);
        Prize P = get_prize(line_2);

        // int token_count = find_lowest_token_win(A, B, P);
        auto c = get_button_press_count(A, B, P);
        auto cramer = find_lowest_cramer(A, B, P);
        auto lin = c.first * 3 + c.second;
        total += lin;
        if (cramer != LONG_LONG_MAX) {
            cramer_total += cramer;
        }

        if (lin != cramer && lin != 0) {
            std::cout << A << " " << B << " " << P << std::endl;
            std::cout << lin << " " << cramer << std::endl;
        }
        // Part 1
        // if (token_count != INT_MAX) {
        //     total += token_count;
        // }
    }
    std::cout << "CRAMER TOTAL = " << cramer_total << "\n";
    std::cout << "Total = " << total << std::endl;
    return 0;
}
