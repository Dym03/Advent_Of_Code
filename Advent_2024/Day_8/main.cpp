#include <unordered_map>

#include "../../Utils/utils.cpp"

void compute_antinodes(std::unordered_map<char, std::vector<std::pair<int, int>>> antennas, Grid<int>& antinodes) {
    for (auto k_v : antennas) {
        for (auto antena : k_v.second) {
            for (auto antena_2 : k_v.second) {
                if (antena == antena_2) {
                    continue;
                }
                int drow = antena_2.first - antena.first;
                int dcol = antena_2.second - antena.second;
                int t = 0;
                while (1) {
                    if (antena_2.first + drow * t < 0 || antena_2.first + drow * t > antinodes.get_row_size() - 1 || antena_2.second + dcol * t < 0 || antena_2.second + dcol * t > antinodes.get_col_size() - 1) {
                        break;
                    }
                    antinodes.set(antena_2.first + drow * t, antena_2.second + dcol * t, antinodes.get(antena_2.first + drow * t, antena_2.second + dcol * t) + 1);
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
    compute_antinodes(antennas, antinodes);
    antinodes.print();
    int total = 0;
    for (auto line : antinodes.grid) {
        for (auto num : line) {
            if (num > 0) {
                total += 1;
            }
        }
    }
    std::cout << "Total = " << total << '\n';
    return 0;
}
