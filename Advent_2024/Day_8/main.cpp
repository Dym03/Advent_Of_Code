#include <unordered_map>

#include "../../Utils/utils.cpp"

void compute_antinodes(std::unordered_map<char, std::vector<std::pair<int, int>>> antennas, Grid<int>& antinodes, bool p_2 = false) {
    for (auto k_v : antennas) {
        for (auto antena : k_v.second) {
            for (auto antena_2 : k_v.second) {
                if (antena == antena_2) {
                    continue;
                }
                int drow = antena_2.first - antena.first;
                int dcol = antena_2.second - antena.second;
                int t = !p_2 ? 1 : 0;
                while (1) {
                    if (antena_2.first + drow * t < 0 || antena_2.first + drow * t > antinodes.get_row_size() - 1 || antena_2.second + dcol * t < 0 || antena_2.second + dcol * t > antinodes.get_col_size() - 1) {
                        break;
                    }
                    antinodes.set(antena_2.first + drow * t, antena_2.second + dcol * t, antinodes.get(antena_2.first + drow * t, antena_2.second + dcol * t) + 1);
                    if (!p_2) {
                        break;
                    }
                    t += 1;
                }
            }
        }
    }
}

int main() {
    std::vector<std::string> lines = read_lines();
    Grid<char> grid(lines.size(), lines[0].size(), ' ');
    std::unordered_map<char, std::vector<std::pair<int, int>>> antennas;
    Grid<int> antinodes(lines.size(), lines[0].size(), 0);
    for (int row = 0; row < lines.size(); row++) {
        for (int col = 0; col < lines[row].size(); col++) {
            grid.set(row, col, lines[row][col]);
            if (lines[row][col] != '.') {
                antennas[lines[row][col]].push_back({row, col});
            }
        }
    }
    auto antinodes_p_2 = antinodes;
    compute_antinodes(antennas, antinodes);
    compute_antinodes(antennas, antinodes_p_2, true);
    int total = 0;
    int total_p_2 = 0;
    for (int row = 0; row < antinodes.get_row_size(); row++) {
        for (int col = 0; col < antinodes.get_col_size(); col++) {
            if (antinodes.get(row, col) > 0) {
                total += 1;
            }
            if (antinodes_p_2.get(row, col) > 0) {
                total_p_2 += 1;
            }
        }
    }
    std::cout << "Total Part 1 = " << total << '\n';
    std::cout << "Total Part 2 = " << total_p_2 << '\n';
    return 0;
}
