#include <algorithm>
#include <climits>
#include <queue>
#include <set>
#include <unordered_map>

#include "../../Utils/utils.cpp"

template <typename T>
void printBin(const T& t) {
    size_t nBytes = sizeof(T);
    char* rawPtr((char*)(&t));
    for (size_t byte = 0; byte < nBytes; byte++) {
        for (size_t bit = 0; bit < CHAR_BIT; bit++) {
            std::cout << (((rawPtr[byte]) >> bit) & 1);
        }
    }
    std::cout << std::endl;
};

template <typename T>
std::vector<int> get_bin(const T& t) {
    std::vector<int> bits;
    size_t nBytes = sizeof(T);
    char* rawPtr((char*)(&t));
    for (size_t byte = 0; byte < nBytes; byte++) {
        for (size_t bit = 0; bit < CHAR_BIT; bit++) {
            bits.push_back((((rawPtr[byte]) >> bit) & 1));
        }
    }
    return bits;
};

enum Operation {
    AND,
    OR,
    XOR
};

struct Connection {
    std::string left_wire;
    Operation operation;
    std::string right_wire;
    std::string output_wire;
};

std::ostream& operator<<(std::ostream& os, const Connection& conn) {
    os << conn.left_wire << " " << conn.right_wire << " -> " << conn.output_wire;
    return os;
}

std::queue<Connection> parse_input(std::vector<std::string> lines, std::unordered_map<std::string, bool>& wire_values, std::vector<Connection>& connections_vector) {
    std::queue<Connection> connections;
    bool reading_rules = true;
    for (auto line : lines) {
        if (line.size() == 0) {
            reading_rules = false;
            continue;
        }
        if (reading_rules) {
            std::vector<std::string> tokens = split(':', line);
            std::string wire = tokens[0];
            trim(tokens[1]);
            if (tokens[1] == "1") {
                wire_values[wire] = true;
            } else if (tokens[1] == "0") {
                wire_values[wire] = false;
            }
        } else {
            std::vector<std::string> tokens = split(' ', line);
            Connection connection;
            connection.left_wire = tokens[0];
            connection.right_wire = tokens[2];
            connection.output_wire = tokens[4];
            if (tokens[1] == "AND") {
                connection.operation = AND;
            } else if (tokens[1] == "OR") {
                connection.operation = OR;
            } else if (tokens[1] == "XOR") {
                connection.operation = XOR;
            }
            connections.push(connection);
            connections_vector.push_back(connection);
        }
    }
    return connections;
}

void solveP1(std::queue<Connection>& connections, std::unordered_map<std::string, bool>& wire_values) {
    int i = 0;
    while (!connections.empty()) {
        auto act = connections.front();
        connections.pop();
        if (wire_values.count(act.left_wire) > 0 && wire_values.count(act.right_wire) > 0) {
            bool left = wire_values[act.left_wire];
            bool right = wire_values[act.right_wire];
            if (act.operation == AND) {
                wire_values[act.output_wire] = left && right;
            } else if (act.operation == OR) {
                wire_values[act.output_wire] = left || right;
            } else if (act.operation == XOR) {
                wire_values[act.output_wire] = left ^ right;
            }
        } else {
            connections.push(act);
        }
        i++;
    }
    std::cout << "Solved " << i << " connections" << std::endl;
}

long get_value_of_wires(char start_char, std::unordered_map<std::string, bool>& wire_values) {
    long val = 0;
    std::set<std::string> output_wires;
    for (auto [k, v] : wire_values) {
        if (k[0] == start_char) {
            output_wires.insert(k);
        }
    }
    int i = 0;
    long output_val = 0;
    // std::cout << "0b";
    for (auto it = output_wires.rbegin(); it != output_wires.rend(); it++) {
        // std::cout << *it << " " << wire_values[*it] << std::endl;
        // std::cout << wire_values[*it];
        output_val |= wire_values[*it];
        if (i < output_wires.size() - 1) {
            output_val <<= 1;
        }
        i++;
    }
    // std::cout << std::endl;
    return output_val;
}

int get_num_of_difference(std::vector<int>& a, std::vector<int>& b) {
    int diff = 0;
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            diff++;
        }
    }
    return diff;
}

std::vector<int> diffs;

int solveP2(std::queue<Connection>& connections, std::unordered_map<std::string, bool> wire_values, long target_value) {
    int i = 0;
    while (!connections.empty()) {
        auto act = connections.front();
        connections.pop();
        if (wire_values.count(act.left_wire) > 0 && wire_values.count(act.right_wire) > 0) {
            bool left = wire_values[act.left_wire];
            bool right = wire_values[act.right_wire];
            if (act.operation == AND) {
                wire_values[act.output_wire] = left && right;
            } else if (act.operation == OR) {
                wire_values[act.output_wire] = left || right;
            } else if (act.operation == XOR) {
                wire_values[act.output_wire] = left ^ right;
            }
        } else {
            connections.push(act);
        }
        i++;
        if (i > 5000) {
            connections = std::queue<Connection>();
            return -1;
        }
    }

    long output_val = get_value_of_wires('z', wire_values);

    auto T_bits = get_bin(target_value);
    auto O_bits = get_bin(output_val);
    auto difference = get_num_of_difference(T_bits, O_bits);
    diffs.push_back(difference);
    if (difference == 0) {
        print_vector(T_bits, ' ');
        print_vector(O_bits, ' ');
        std::cout << "Difference = " << difference << std::endl;
        std::cout << std::endl;
        std::cout << "Found solution" << std::endl;
        return 1;
    }
    if (difference < 5) {
        print_vector(T_bits, ' ');
        print_vector(O_bits, ' ');
        std::cout << "Difference = " << difference << std::endl;
        std::cout << std::endl;
        return 2;
    }
    return 0;
}

int main() {
    std::vector<std::string> lines = read_lines();
    bool reading_rules = true;
    std::unordered_map<std::string, bool> wire_values;
    std::vector<Connection> connections_vector;
    std::queue<Connection> connections = parse_input(lines, wire_values, connections_vector);
    std::cout << connections.size() << std::endl;
    auto wire_values_p2 = wire_values;
    solveP1(connections, wire_values);
    std::queue<Connection> connections_copy;
    long output_val = get_value_of_wires('z', wire_values);
    std::cout << "Part 1 = " << output_val << std::endl;
    long x_val = get_value_of_wires('x', wire_values);
    long y_val = get_value_of_wires('y', wire_values);

    long target_value = x_val + y_val;
    std::cout << "Target value = " << target_value << " output value = " << output_val << std::endl;
    std::cout << "Target 1 Output 2\n";
    std::cout << "0b";
    printBin(target_value);
    std::cout << "0b";
    printBin(output_val);
    for (int i = 0; i < 66; i++) {
        std::cout << "-";
    }
    std::cout << std::endl
              << "  ";
    for (int i = 0; i < 64; i++) {
        std::cout << i / 10;
    }
    std::cout << std::endl
              << "  ";
    for (int i = 0; i < 64; i++) {
        std::cout << i % 10;
    }
    std::cout << std::endl;
    std::set<std::pair<std::string, std::string>> cannot_be_swapped;

    auto a = get_bin(target_value);
    auto b = get_bin(output_val);
    std::cout << "Base = " << get_num_of_difference(a, b) << std::endl;

    for (int i = 0; i < connections_vector.size(); i++) {
        std::cout << i << " out of " << connections_vector.size() << std::endl;
        for (int i_2 = i + 1; i_2 < connections_vector.size(); i_2++) {
            if (cannot_be_swapped.count({connections_vector[i].output_wire, connections_vector[i_2].output_wire}) > 0) {
                continue;
            }
            std::swap(connections_vector[i].output_wire, connections_vector[i_2].output_wire);
            for (auto conn : connections_vector) {
                connections_copy.push(conn);
            }
            if (solveP2(connections_copy, wire_values_p2, (x_val + y_val)) == -1) {
                std::swap(connections_vector[i].output_wire, connections_vector[i_2].output_wire);
                connections_copy = std::queue<Connection>();
                continue;
            } else {
                connections_copy = std::queue<Connection>();
            }
            for (int j = i_2 + 1; j < connections_vector.size(); j++) {
                for (int j_2 = j + 1; j_2 < connections_vector.size(); j_2++) {
                    if (cannot_be_swapped.count({connections_vector[j].output_wire, connections_vector[j_2].output_wire}) > 0) {
                        continue;
                    }
                    std::swap(connections_vector[j].output_wire, connections_vector[j_2].output_wire);
                    for (auto conn : connections_vector) {
                        connections_copy.push(conn);
                    }
                    if (solveP2(connections_copy, wire_values_p2, (x_val + y_val)) == -1) {
                        std::swap(connections_vector[j].output_wire, connections_vector[j_2].output_wire);
                        connections_copy = std::queue<Connection>();
                        cannot_be_swapped.insert({connections_vector[j].output_wire, connections_vector[j_2].output_wire});
                        continue;
                    } else {
                        connections_copy = std::queue<Connection>();
                    }
                    for (int k = j_2 + 1; k < connections_vector.size(); k++) {
                        for (int k_2 = k + 1; k_2 < connections_vector.size(); k_2++) {
                            if (cannot_be_swapped.count({connections_vector[k].output_wire, connections_vector[k_2].output_wire}) > 0) {
                                continue;
                            }
                            std::swap(connections_vector[k].output_wire, connections_vector[k_2].output_wire);
                            for (auto conn : connections_vector) {
                                connections_copy.push(conn);
                            }
                            if (solveP2(connections_copy, wire_values_p2, (x_val + y_val)) == -1) {
                                std::swap(connections_vector[k].output_wire, connections_vector[k_2].output_wire);
                                connections_copy = std::queue<Connection>();
                                cannot_be_swapped.insert({connections_vector[k].output_wire, connections_vector[k_2].output_wire});
                                continue;
                            } else {
                                connections_copy = std::queue<Connection>();
                            }
                            for (int l = k_2 + 1; l < connections_vector.size(); l++) {
                                for (int l_2 = l + 1; l_2 < connections_vector.size(); l_2++) {
                                    if (cannot_be_swapped.count({connections_vector[l].output_wire, connections_vector[l_2].output_wire}) > 0) {
                                        continue;
                                    }
                                    std::swap(connections_vector[l].output_wire, connections_vector[l_2].output_wire);
                                    for (auto conn : connections_vector) {
                                        connections_copy.push(conn);
                                    }
                                    // print_vector(connections_vector, ' ');
                                    auto result = solveP2(connections_copy, wire_values_p2, (x_val + y_val));
                                    if (result == 1) {
                                        std::cout
                                            << "Found solution" << std::endl;
                                        std::cout << connections_vector[i].output_wire << "," << connections_vector[i_2].output_wire << "," << connections_vector[j].output_wire << "," << connections_vector[j_2].output_wire << "," << connections_vector[k].output_wire << "," << connections_vector[k_2].output_wire << "," << connections_vector[l].output_wire << "," << connections_vector[l_2].output_wire << std::endl;
                                        return 0;
                                    } else if (result == 2) {
                                        std::cout << connections_vector[i].output_wire << "," << connections_vector[i_2].output_wire << "," << connections_vector[j].output_wire << "," << connections_vector[j_2].output_wire << "," << connections_vector[k].output_wire << "," << connections_vector[k_2].output_wire << "," << connections_vector[l].output_wire << "," << connections_vector[l_2].output_wire << std::endl;
                                    } else if (result == -1) {
                                        cannot_be_swapped.insert({connections_vector[l].output_wire, connections_vector[l_2].output_wire});
                                    }
                                    connections_copy = std::queue<Connection>();

                                    std::swap(connections_vector[l].output_wire, connections_vector[l_2].output_wire);
                                }
                            }
                            std::swap(connections_vector[k].output_wire, connections_vector[k_2].output_wire);
                        }
                    }
                    std::swap(connections_vector[j].output_wire, connections_vector[j_2].output_wire);
                }
            }
            std::swap(connections_vector[i].output_wire, connections_vector[i_2].output_wire);
        }
    }

 

    return 0;
}

// nnw,kfb,npc,hkm

// z42,pqj,htg,gdd,ttm,kfb,npc,hkm

// z42,pqj,htg,gdd,qgp,kfb,bpv,kqw