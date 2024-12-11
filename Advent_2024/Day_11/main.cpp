#include <unordered_map>

#include "../../Utils/utils.cpp"

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const {
        // Hash the first element
        size_t hash1 = std::hash<T1>{}(p.first);
        // Hash the second element
        size_t hash2 = std::hash<T2>{}(p.second);
        // Combine the two hash values
        return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
    }
};

void apply_rules_p1(std::string stone, std::vector<std::string>& new_stones) {
    if (stone == "0") {
        new_stones.emplace_back("1");
    } else if (stone.size() % 2 == 0) {
        new_stones.emplace_back(stone.substr(0, stone.size() / 2));
        new_stones.emplace_back(std::to_string(std::stol((stone.substr(stone.size() / 2)))));
    } else {
        unsigned long long stone_val = std::stoull(stone);
        stone_val *= 2024;
        new_stones.emplace_back(std::to_string(stone_val));
    }
}

void apply_rules_p2(unsigned long long stone, std::vector<unsigned long long>& new_stones) {
    std::string stone_string = std::to_string(stone);
    if (stone == 0) {
        new_stones.emplace_back(1);
    } else if (stone_string.size() % 2 == 0) {
        new_stones.emplace_back(std::stoull(stone_string.substr(0, stone_string.size() / 2)));
        new_stones.emplace_back(std::stoull((stone_string.substr(stone_string.size() / 2))));
    } else {
        stone *= 2024;
        new_stones.emplace_back(stone);
    }
}

int main() {
    std::vector<std::string> lines = read_lines();
    std::string line = lines[0];
    std::vector<std::string> stones = split(' ', line);
    print_vector(stones, ' ');
    int num_blinks = 25;
    std::vector<std::string> new_stones;

    std::vector<unsigned long long> stones_long;
    for (auto stone : stones) {
        stones_long.push_back(std::stoull(stone));
    }
    std::vector<unsigned long long> new_stones_long;
    std::unordered_map<std::pair<unsigned long long, int>, unsigned long long, hash_pair> stone_to_num_stones;
    new_stones.reserve(stones.size());
    // Part 1
    for (int i = 0; i < num_blinks; i++) {
        for (auto stone : stones) {
            apply_rules_p1(stone, new_stones);
        }
        stones = new_stones;
        new_stones.clear();
        new_stones.reserve(stones.size());
        // print_vector(stones, ' ');
        // std::cout << i << std::endl;
    }
    std::cout << "Part 1 = " << stones.size() << std::endl;
    std::vector<unsigned long long> stones_p2;
    num_blinks = 40;
    std::vector<unsigned long long> cached_stones = {
        0,
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        2024,
        4048,
        8096,
    };
    for (auto stone : cached_stones) {
        stones_p2.push_back(stone);
        for (int i = 0; i < num_blinks; i++) {
            for (auto stone_p2 : stones_p2) {
                apply_rules_p2(stone_p2, new_stones_long);
            }
            stones_p2 = new_stones_long;
            new_stones_long.clear();
            new_stones_long.reserve(stones_p2.size());
            stone_to_num_stones[{stone, i}] = stones_p2.size();
        }
        stones_p2.clear();
    }

    // Part 2
    unsigned long long num_stones = 0;
    num_blinks = 75;
    stones_p2.clear();
    for (auto stone : stones_long) {
        stones_p2.push_back(stone);
        for (int i = 0; i < num_blinks; i++) {
            for (auto stone_p2 : stones_p2) {
                if (stone_to_num_stones.count({stone_p2, 74 - i}) > 0) {
                    num_stones += stone_to_num_stones[{stone_p2, 74 - i}];
                    continue;
                }
                apply_rules_p2(stone_p2, new_stones_long);
            }
            stones_p2 = new_stones_long;
            new_stones_long.clear();
            new_stones_long.reserve(stones_p2.size());
        }
        num_stones += stones_p2.size();
        stones_p2.clear();
    }
    std::cout << "Part 2 = " << num_stones << std::endl;
    return 0;
}
