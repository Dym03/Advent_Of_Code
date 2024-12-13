#include <iostream>
#include <memory>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>

#define DISK_SPACE 70000000
#define NEEDED_SPACE 30000000 

enum class Type {
    DIR,
    FILE,
};

class Node {
   public:
    std::string name;
    Type type;
    int size;
    Node * parent;
    std::vector<Node *> children;
    Node(std::string name, int size, Type type, Node * parent) {
        this->name = name;
        this->size = size;
        this->type = type;
        this->parent = parent;
    }
    ~Node() {
        for (auto child : children) {
            delete child;
        }
    }
};

class Filesystem {
   public:
    Node* root;

    Filesystem() {
        root = new Node ("/", 0, Type::DIR, nullptr);
    }

    Node* find_node(Node * node, std::string name) {
        if (node == nullptr) {
            return nullptr;
        }
        if (node->name == name && node->type == Type::DIR) {
            return node;
        }
        for (auto child : node->children) {
            auto result = find_node(child, name);
            if (result != nullptr && result->type == Type::DIR && result->parent == node) {
                return result;
            }
        }
        return nullptr;
    }

    Node* find_node(std::string name) {
        return find_node(root, name);
    }

    Node * get_parent(Node * node) {
        if (node == nullptr) {
            return nullptr;
        }
        Node * parent = node->parent;
        if (!parent) {
            std::cerr << "Error: Failed to lock weak_ptr in get_parent." << std::endl;
        }
        return parent;
    }

    Node * get_root() {
        return root;
    }

    void add_dir(Node * parent, std::string name) {
        if (parent == nullptr) {
            return;
        }
        Node * node = new Node(name, 0, Type::DIR, parent);
        parent->children.push_back(node);
    }

    void add_file(Node * parent, std::string name, int size) {
        if (parent == nullptr) {
            return;
        }
        Node * node = new Node (name, size, Type::FILE, parent);
        parent->children.push_back(node);
    }

    void print(Node * node, int depth = 0) {
        if (node == nullptr) {
            return;
        }
        for (int i = 0; i < depth; i++) {
            std::cout << " ";
        }
        std::cout << node->name << " (" << (node->type == Type::DIR ? "DIR" : "FILE") << ") " << node->size << std::endl;
        for (auto child : node->children) {
            print(child, depth + 1);
        }
    }

    void calculate_dir_size(Node* node) {
        if (node == nullptr) {
            return;
        }
        int size = 0;
        for (auto child : node->children) {
            if (child->size == 0) {
                calculate_dir_size(child);
            }
            size += child->size;
        }
        node->size = size;
    }

    void find_below_limit(Node * node, int limit, int& sum) {
        if (node == nullptr) {
            return;
        }
        if (node->size <= limit && node->type == Type::DIR) {
            sum += node->size;
        }
        for (auto child : node->children) {
            find_below_limit(child, limit, sum);
        }
    }

    void find_above_limit(Node * node, int limit, std::vector<std::pair<std::string,int>>& can_be_deleted) {
        if (node == nullptr) {
            return;
        }
        if (node->size >= limit && node->type == Type::DIR) {
            // std::cout << node->name << " " << node->size << std::endl;
            can_be_deleted.push_back(std::make_pair(node->name,node->size));
        }
        for (auto child : node->children) {
            find_above_limit(child, limit, can_be_deleted);
        }
    }

};

int main() {
    Filesystem fs;
    std::string input;
    Node* parent = fs.get_root();
    std::vector<std::pair<std::string,int>> can_be_deleted;
    while (getline(std::cin, input)) {
        std::stringstream ss(input);
        std::string token;
        std::vector<std::string> tokens;
        while (getline(ss, token, ' ')) {
            tokens.push_back(token);
        }
        if (tokens[0] == "$") {
            if (tokens[1] == "cd") {
                if (tokens[2] == "..") {
                    if (parent == nullptr) {
                        std::cerr << "Error: Parent is null. tried to go back to " << tokens[2] << std::endl;
                    }else {
                        parent = fs.get_parent(parent);
                    }
                    if (parent == nullptr) {
                        std::cerr << "Error: Parent is null. tried to go back to " << tokens[2] << std::endl;
                        
                    }
                } else if (tokens[2] == "/") {
                    parent = fs.get_root();
                } else {
                    if (parent == nullptr) {
                        std::cerr << "Error: Parent is null. Tried to cd into it " << tokens[2] << std::endl;
                    }else {
                        parent = fs.find_node(parent, tokens[2]);
                    }
                    if (parent == nullptr) {
                        std::cerr << "Error: Parent is null. Tried to cd into it " << tokens[2] << std::endl;
                    }
                }
            } else if (tokens[1] == "ls") {
            }
        } else {
            if (tokens[0] == "dir") {
                if (parent == nullptr){
                    std::cerr << "Error: Parent is null. Tried to add " << token[1] <<  std::endl;
                }else {
                    fs.add_dir(parent, tokens[1]);

                }
            } else {
                fs.add_file(parent, tokens[1], std::stoi(tokens[0]));
            }
        }
    }

    fs.calculate_dir_size(fs.get_root());
    int sum = 0;
    std::cout << DISK_SPACE - fs.get_root()->size << std::endl;
    fs.find_below_limit(fs.get_root(), 100000, sum);
    fs.find_above_limit(fs.get_root(), NEEDED_SPACE - (DISK_SPACE - fs.get_root()->size), can_be_deleted);
    //Part 2
    auto min = std::min_element(can_be_deleted.begin(), can_be_deleted.end(), [](const std::pair<std::string,int>& a, const std::pair<std::string,int>& b) {
        return a.second < b.second;
    });
    std::cout << "Min: " << min->first << " " << min->second << std::endl;
    //Part 1
    std::cout << "Sum that are below limit = " << sum << std::endl;
    fs.print(fs.get_root());

    return 0;
}