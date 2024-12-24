#include <queue>
#include <set>
#include <unordered_map>

#include "../../Utils/utils.cpp"

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

std::queue<Connection> parse_input(std::vector<std::string> lines, std::unordered_map<std::string, bool>& wire_values) {
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
        }
    }
    return connections;
}

void solveP1(std::queue<Connection>& connections, std::unordered_map<std::string, bool>& wire_values) {
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
    }
}

int main() {
    std::vector<std::string> lines = read_lines();
    bool reading_rules = true;
    std::unordered_map<std::string, bool> wire_values;
    std::queue<Connection> connections = parse_input(lines, wire_values);
    std::cout << connections.size() << std::endl;
    solveP1(connections, wire_values);
    std::set<std::string> output_wires;
    for (auto [k, v] : wire_values) {
        if (k[0] == 'z') {
            output_wires.insert(k);
        }
    }
    int i = 0;
    long long output_val = 0;
    for (auto it = output_wires.rbegin(); it != output_wires.rend(); it++) {
        std::cout << *it << " " << wire_values[*it] << std::endl;
        output_val |= wire_values[*it];
        if (i < output_wires.size() - 1) {
            output_val <<= 1;
        }
        i++;
    }
    std::cout << output_val << std::endl;

    return 0;
}
