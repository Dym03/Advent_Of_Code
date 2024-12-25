#include "../../Utils/utils.cpp"

bool is_key(std::vector<std::string> &key) {
    return key[0][0] == '.';
}

std::vector<int> convert_to_height_map(std::vector<std::string> &key) {
    std::vector<int> key_height;

    for (int j = 0; j < key[0].size(); j++) {
        int height = 0;
        for (int i = 1; i < key.size() - 1; i++) {
            if (key[i][j] == '#') {
                height++;
            }
        }
        key_height.push_back(height);
    }

    return key_height;
}

bool do_overlap(std::vector<int> &key, std::vector<int> &pin, int height) {
    for (int i = 0; i < key.size(); i++) {
        if (key[i] + pin[i] > height) {
            return true;
        }
    }
    return false;
}

int main() {
    std::vector<std::string> lines = read_lines();

    std::vector<std::vector<int>> pin_heights;
    std::vector<std::vector<int>> key_heights;
    std::vector<std::string> pin_or_key;
    for (auto line : lines) {
        if (line.size() == 0) {
            continue;
        }
        pin_or_key.push_back(line);

        if (pin_or_key.size() == 7) {
            auto height_map = convert_to_height_map(pin_or_key);
            if (is_key(pin_or_key)) {
                key_heights.push_back(height_map);
            } else {
                pin_heights.push_back(height_map);
            }
            pin_or_key.clear();
        }
    }
    int part_1 = 0;
    for (auto key : key_heights) {
        for (auto pin : pin_heights) {
            if (!do_overlap(key, pin, 5)) {
                part_1++;
            }
        }
    }
    std::cout << "Part 1 = " << part_1 << std::endl;

    return 0;
}
