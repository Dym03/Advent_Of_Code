#include "../../Utils/utils.cpp"

enum row_type {
    None,
    Increasing,
    Decreasing
};

int main() {
    std::vector<std::string> lines = read_lines();
    int num_saves = 0;
    for (auto line : lines) {
        std::vector<std::string> tokens = split(' ', line);
        if (tokens.size() == 0) {
            continue;
        }
        bool differ = true;
        row_type prev_type = None;
        for (int i = 0; i < tokens.size() - 1; i++) {
            int act = std::stoi(tokens[i]);
            int next = std::stoi(tokens[i + 1]);
            if (act < next) {
                if (prev_type == Decreasing) {
                    differ = false;
                } else {
                    prev_type = Increasing;
                }
            } else if (act > next) {
                if (prev_type == Increasing) {
                    differ = false;
                } else {
                    prev_type = Decreasing;
                }
            } else if (act == next) {
                differ = false;
            }
            if (abs(act - next) > 3) {
                differ = false;
            }
        }
        if ((differ && tokens.size() > 1)) {
            num_saves++;
            continue;
        }
        for (int i = 0; i < tokens.size(); i++) {
            std::vector<std::string> tokens_copy = tokens;
            tokens_copy.erase(tokens_copy.begin() + i);
            differ = true;
            prev_type = None;
            for (int i = 0; i < tokens_copy.size() - 1; i++) {
                int act = std::stoi(tokens_copy[i]);
                int next = std::stoi(tokens_copy[i + 1]);
                if (act < next) {
                    if (prev_type == Decreasing) {
                        differ = false;
                    } else {
                        prev_type = Increasing;
                    }
                } else if (act > next) {
                    if (prev_type == Increasing) {
                        differ = false;
                    } else {
                        prev_type = Decreasing;
                    }
                } else if (act == next) {
                    differ = false;
                }
                if (abs(act - next) > 3) {
                    differ = false;
                }
            }
            if ((differ && tokens.size() > 1)) {
                num_saves++;
                break;
            }
        }
    }

    std::cout << num_saves << std::endl;

    return 0;
}
