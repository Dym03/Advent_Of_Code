#include <regex>

#include "../../Utils/utils.cpp"

int main() {
    std::regex re(R"(XMAS)");
    std::regex re_2(R"(SAMX)");
    std::regex re_3(R"(MAS|SAM)");
    size_t count = 0;
    size_t x_max = 0;
    std::vector<std::string> lines = read_lines();
    // Find in line
    for (auto line : lines) {
        std::smatch match;
        auto line_copy = line;
        while (std::regex_search(line_copy, match, re)) {
            line_copy = match.suffix().str();
            count++;
        }
        line_copy = line;
        while (std::regex_search(line_copy, match, re_2)) {
            line_copy = match.suffix().str();
            count++;
        }
    }
    // Find in column
    for (int col = 0; col < lines[0].size(); col++) {
        std::string column;
        for (int row = 0; row < lines.size(); row++) {
            column += lines[row][col];
        }
        std::string line_copy = column;
        std::smatch match;
        while (std::regex_search(line_copy, match, re)) {
            line_copy = match.suffix().str();
            count++;
        }
        line_copy = column;
        while (std::regex_search(line_copy, match, re_2)) {
            line_copy = match.suffix().str();
            count++;
        }
    }
    // Find in diagonal
    for (int row = 0; row < lines.size(); row++) {
        for (int col = 0; col < lines[0].size(); col++) {
            std::string diagonal_right;
            std::string diagonal_left;
            for (int i = 0; i < 4; i++) {
                if (row + i > lines.size() - 1 || col + i > lines[0].size() - 1) {
                    break;
                }
                diagonal_right += lines[row + i][col + i];
            }
            for (int i = 0; i < 4; i++) {
                if (row - i < 0 || col + i > lines[0].size() - 1) {
                    break;
                }
                diagonal_left += lines[row - i][col + i];
            }
            std::string line_copy = diagonal_right;
            std::smatch match;
            while (std::regex_search(line_copy, match, re)) {
                line_copy = match.suffix().str();
                count++;
            }
            line_copy = diagonal_right;
            while (std::regex_search(line_copy, match, re_2)) {
                line_copy = match.suffix().str();
                count++;
            }
            line_copy = diagonal_left;

            while (std::regex_search(line_copy, match, re)) {
                line_copy = match.suffix().str();
                count++;
            }
            line_copy = diagonal_left;
            while (std::regex_search(line_copy, match, re_2)) {
                line_copy = match.suffix().str();
                count++;
            }
        }
    }
    // Find X-MAS
    for (int row = 0; row < lines.size(); row++) {
        for (int col = 0; col < lines[0].size(); col++) {
            std::string diagonal_right;
            std::string diagonal_left;
            bool left = false;
            bool right = false;
            for (int row_i = 0, col_i = 0; row_i < 3; row_i++, col_i++) {
                if (row + row_i > lines.size() - 1 || col + col_i > lines[0].size() - 1) {
                    break;
                }
                diagonal_right += lines[row + row_i][col + col_i];
            }
            for (int row_i = 0, col_i = 2; row_i < 3; row_i++, col_i--) {
                if (row + row_i > lines.size() - 1 || col + col_i > lines[0].size() - 1) {
                    break;
                }
                diagonal_left += lines[row + row_i][col + col_i];
            }
            if (std::regex_search(diagonal_right, re_3) && std::regex_search(diagonal_left, re_3)) {
                x_max++;
            }
        }
    }

    std::cout << count << std::endl;
    std::cout << x_max << std::endl;
    return 0;
}
