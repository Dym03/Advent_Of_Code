#include "../../Utils/utils.cpp"

void reoder_memory(std::vector<int>& memory) {
    auto front = memory.begin();
    auto back = memory.end() - 1;
    while (front < back) {
        if (*front == -1 && *back != -1) {
            std::swap(*front, *back);
            front++;
            back--;
        }
        if (*front != -1) {
            front++;
        }
        if (*back == -1) {
            back--;
        }
    }
}

void reoder_memory_p_2(std::vector<std::pair<int, int>>& memory) {
    auto front = memory.begin();
    auto back = memory.end() - 1;
    auto front_copy = front;
    for (auto it = memory.end() - 1; it >= memory.begin(); it--) {
        if (it->first != -1) {
            for (auto it_2 = memory.begin(); it_2 < it; it_2++) {
                if (it_2->first == -1 && it_2->second >= it->second) {
                    std::swap(*it_2, *it);
                    if (it->second > it_2->second) {
                        std::pair<int, int> new_block = {it->first, it->second - it_2->second};
                        it->second -= new_block.second;
                        memory.insert(it_2 + 1, new_block);
                    }
                    std::vector<int> memory_p_2;
                    for (auto block : memory) {
                        for (int i = 0; i < block.second; i++) {
                            memory_p_2.push_back(block.first);
                        }
                    }
                    break;
                }
            }
        }
    }
}

size_t count_check_sum(const std::vector<int>& memory) {
    size_t check_sum = 0;
    int id = 0;
    for (auto block : memory) {
        if (block != -1) {
            check_sum += id * block;
        }
        id += 1;
    }
    return check_sum;
}
int main() {
    std::vector<std::string> lines = read_lines();
    int id = 0;
    std::vector<int> memory;
    memory.reserve(1000);
    std::vector<std::pair<int, int>> blocks;
    bool free_memory = false;
    for (auto line : lines) {
        for (auto c : line) {
            int block_size = c - '0';
            for (int i = 0; i < block_size; i++) {
                if (!free_memory) {
                    memory.push_back(id);
                } else {
                    memory.push_back(-1);
                }
            }
            if (!free_memory) {
                blocks.push_back({id, block_size});
                id += 1;
            } else {
                blocks.push_back({-1, block_size});
            }
            free_memory = !free_memory;
        }
    }

    reoder_memory(memory);
    reoder_memory_p_2(blocks);
    std::vector<int> memory_p_2;
    for (auto block : blocks) {
        for (int i = 0; i < block.second; i++) {
            memory_p_2.push_back(block.first);
        }
    }
    size_t check_sum = count_check_sum(memory);
    std::cout << "Check sum p_1: " << check_sum << std::endl;
    check_sum = count_check_sum(memory_p_2);
    std::cout << "Check sum p_2: " << check_sum << std::endl;

    return 0;
}
