#include <math.h>

#include <climits>
#include <set>
#include <unordered_map>

#include "../../Utils/utils.cpp"
#define Modulus 16777216

long pseudo_random_step(long value) {
    value = ((value * 64) ^ value) % Modulus;
    value = (static_cast<int>(std::floor((value) / 32.0)) ^ value) % Modulus;
    value = ((value * 2048) ^ value) % Modulus;
    return value;
}

struct VectorHasher {
    int operator()(const std::vector<int> &V) const {
        int hash = V.size();
        for (auto &i : V) {
            hash ^= i + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

int main() {
    std::vector<std::string> lines = read_lines();
    long long secret_sum = 0;
    std::unordered_map<std::vector<int>, int, VectorHasher> diff_to_price = {};
    for (auto line : lines) {
        std::vector<int> diffs = {};
        long secret_num = std::stol(line);
        std::set<std::vector<int>> diff_set = {};
        for (int i = 0; i < 2000; i++) {
            int price = secret_num % 10;
            secret_num = pseudo_random_step(secret_num);
            int new_price = secret_num % 10;

            diffs.push_back(new_price - price);
            if (diffs.size() == 4) {
                if (diff_set.count(diffs) == 0) {
                    diff_to_price[diffs] += new_price;
                    diff_set.insert(diffs);
                }
                diffs.erase(diffs.begin());
            }
        }
        secret_sum += secret_num;
    }
    int max_count = 0;
    for (auto [k, v] : diff_to_price) {
        if (v > max_count) {
            max_count = v;
        }
    }

    std::cout << "Secret sum: " << secret_sum << std::endl;
    std::cout << max_count << std::endl;
    return 0;
}
