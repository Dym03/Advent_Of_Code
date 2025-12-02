#include <set>
#include <unordered_map>

#include "../../Utils/utils.cpp"

void part_1(std::unordered_map<std::string, std::set<std::string>>& adj_computers) {
    int i = 0;
    std::set<std::set<std::string>> triplets;
    for (auto computer_1 : adj_computers) {
        std::cout << i++ << " out of " << adj_computers.size() << std::endl;
        for (auto computer_2 : adj_computers) {
            if (computer_1.first == computer_2.first) {
                continue;
            }
            for (auto computer_3 : adj_computers) {
                if (computer_1.first == computer_3.first || computer_2.first == computer_3.first) {
                    continue;
                }
                if (computer_3.second.count(computer_1.first) == 0 || computer_3.second.count(computer_2.first) == 0) {
                    continue;
                }
                if (computer_1.second.count(computer_2.first) == 0 || computer_1.second.count(computer_3.first) == 0) {
                    continue;
                }
                if (computer_2.second.count(computer_1.first) == 0 || computer_2.second.count(computer_3.first) == 0) {
                    continue;
                }
                std::set<std::string> triplet = {computer_1.first, computer_2.first, computer_3.first};
                if (triplets.count(triplet) > 0) {
                    continue;
                }
                triplets.insert(triplet);
            }
        }
    }
    int count = 0;
    for (auto triplet : triplets) {
        for (auto computer : triplet) {
            if (computer[0] == 't') {
                // std::cout << computer << " ";
                count++;
                break;
            }
        }
    }
    std::cout << count << std::endl;
}

std::vector<std::set<std::string>> cliques;

// https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm
void BronKerbosch(std::set<std::string> R, std::set<std::string> P, std::set<std::string> X, std::unordered_map<std::string, std::set<std::string>>& adj_computers) {
    if (P.empty() && X.empty()) {
        cliques.push_back(R);
        return;
    }
    std::set<std::string> pivot_set = P;
    int max_size = 0;
    std::string pivot = "";
    for (auto p : pivot_set) {
        if (adj_computers[p].size() > max_size) {
            max_size = adj_computers[p].size();
            pivot = p;
        }
    }
    std::set<std::string> P_minus_pivot = P;
    for (auto p : P) {
        if (adj_computers[pivot].count(p) > 0) {
            P_minus_pivot.erase(p);
        }
    }
    for (auto v : P_minus_pivot) {
        std::set<std::string> R_v = R;
        R_v.insert(v);
        std::set<std::string> P_v;
        for (auto x : X) {
            P.insert(x);
        }

        for (auto p : P) {
            if (adj_computers[v].count(p) > 0) {
                P_v.insert(p);
            }
        }
        std::set<std::string> X_v;
        for (auto x : X) {
            if (adj_computers[v].count(x) > 0) {
                X_v.insert(x);
            }
        }
        BronKerbosch(R_v, P_v, X_v, adj_computers);
        P.erase(v);
        X.insert(v);
    }
}

int main() {
    std::vector<std::string> lines = read_lines();
    std::unordered_map<std::string, std::set<std::string>> adj_computers;
    for (auto line : lines) {
        std::vector<std::string> l_r = split('-', line);
        adj_computers[l_r[0]].insert(l_r[1]);
        adj_computers[l_r[1]].insert(l_r[0]);
    }
    // part_1(adj_computers);
    std::set<std::string> R;
    std::set<std::string> P;
    for (auto computer : adj_computers) {
        P.insert(computer.first);
    }
    std::set<std::string> X;
    BronKerbosch(R, P, X, adj_computers);
    int max_size = 0;
    std::string code = "";
    for (auto clique : cliques) {
        if (clique.size() > max_size) {
            max_size = clique.size();
            code = "";
            int i = 0;
            for (auto computer : clique) {
                std::cout << computer << " ";
                if (i != clique.size() - 1) {
                    code += computer + ",";

                } else {
                    code += computer;
                }
                i += 1;
            }
            std::cout << std::endl;
        }
    }
    std::cout << max_size << std::endl;
    std::cout << code << std::endl;
    return 0;
}
