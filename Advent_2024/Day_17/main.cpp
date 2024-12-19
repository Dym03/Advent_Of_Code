#include <math.h>
#include <semaphore.h>

#include <atomic>
#include <climits>
#include <thread>
#include <unordered_map>

#include "../../Utils/utils.cpp"

struct Program {
    int ip;
    std::vector<int> instructions;
    std::unordered_map<char, unsigned long long> registers;
};

void run(Program program, std::vector<int>& outputs, bool part_2) {
    while (program.ip < program.instructions.size()) {
        int opcode = program.instructions[program.ip];
        unsigned long long operand = program.instructions[program.ip + 1];
        if (operand == 4) {
            operand = program.registers['A'];
        } else if (operand == 5) {
            operand = program.registers['B'];
        } else if (operand == 6) {
            operand = program.registers['C'];
        } else if (operand == 7) {
            std::cout << " Something went wrong" << std::endl;
        }
        if (opcode == 0) {
            unsigned long long numerator = program.registers['A'];
            unsigned long long denominator = std::pow(2, operand);
            unsigned long long result = std::trunc((numerator / denominator));
            program.registers['A'] = result;
        } else if (opcode == 1) {
            unsigned long long result = program.registers['B'] ^ operand;
            program.registers['B'] = result;
        } else if (opcode == 2) {
            unsigned long long mask = 0b00000111;
            unsigned long long result = operand & mask;
            program.registers['B'] = result;
        } else if (opcode == 3) {
            if (program.registers['A'] == 0) {
            } else {
                program.ip = operand;
                continue;
            }
        } else if (opcode == 4) {
            unsigned long long left = program.registers['B'];
            unsigned long long right = program.registers['C'];
            unsigned long long result = left ^ right;
            program.registers['B'] = result;
        } else if (opcode == 5) {
            unsigned long long mask = 0b00000111;
            unsigned long long result = operand & mask;
            outputs.push_back(result);
            if (part_2) {
                int idx = outputs.size() - 1;
                if (outputs[idx] != program.instructions[idx] || outputs.size() == program.instructions.size()) {
                    break;
                }
            }
        } else if (opcode == 6) {
            unsigned long long numerator = program.registers['A'];
            unsigned long long denominator = std::pow(2, operand);
            unsigned long long result = std::trunc(static_cast<double>(numerator) / static_cast<double>(denominator));
            program.registers['B'] = result;
        } else if (opcode == 7) {
            unsigned long long numerator = program.registers['A'];
            unsigned long long denominator = std::pow(2, operand);
            unsigned long long result = std::trunc(static_cast<double>(numerator) / static_cast<double>(denominator));
            program.registers['C'] = result;
        }
        program.ip += 2;
    }
}

std::ostream& operator<<(std::ostream& os, const Program& program) {
    os << "IP: " << program.ip << std::endl;
    os << "Registers: " << std::endl;
    for (auto [key, value] : program.registers) {
        os << key << ": " << value << std::endl;
    }
    os << "Instructions: " << std::endl;
    for (auto instruction : program.instructions) {
        os << instruction << ",";
    }
    std::cout << std::endl;

    return os;
}

sem_t sem;
bool found = false;

struct ThreadArgs {
    Program program;
    long start;
    long end;
    sem_t* sem;
};

long lowest_solution = LONG_MAX;

bool same_vector(std::vector<int> a, std::vector<int> b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

bool same_vector_p2(std::vector<int> a, std::vector<int> b) {
    for (int i = 0; i < std::min(a.size(), b.size()); i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

void thread_run(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*)args;
    Program program = thread_args->program;
    for (long i = thread_args->start; i < thread_args->end; i++) {
        // if (found) {
        //     break;
        // }
        program.registers['A'] = i;
        std::vector<int> outputs;
        run(program, outputs, true);
        if (same_vector(program.instructions, outputs)) {
            sem_wait(thread_args->sem);
            std::cout << "Found the solution i = " << i << std::endl;
            print_vector(outputs, ',');
            found = true;
            lowest_solution = std::min(lowest_solution, i);
            sem_post(thread_args->sem);
            break;
        }
        outputs.clear();
    }
    sem_wait(thread_args->sem);
    std::cout << "Thread finished start = " << thread_args->start << " end =  " << thread_args->end << std::endl;
    sem_post(thread_args->sem);
}

// Inspired by https://github.com/anominos/AdventOfCode/blob/main/2024/d17.py Source
long rec(long n, long a, Program prog) {
    if (n == -1) {
        return a;
    }
    a <<= 3;
    for (int x = 0; x < 8; x++) {
        prog.registers['A'] = a + x;
        std::vector<int> outputs;
        run(prog, outputs, false);
        auto sample_vector = std::vector(prog.instructions.begin() + n, prog.instructions.end());
        if (same_vector(outputs, sample_vector)) {
            std::cout << "Outputs: ";
            print_vector(outputs, ',', false);
            std::cout << " Sample vector: ";
            print_vector(sample_vector, ',', true);
            long s = rec(n - 1, a + x, prog);
            if (s != -1) {
                return s;
            }
        }
    }
    return -1;
}

int main() {
    sem_init(&sem, 0, 1);
    std::vector<std::string> lines = read_lines();
    Program program;
    program.ip = 0;
    bool reading_registers = true;

    for (auto line : lines) {
        if (line.size() == 0) {
            reading_registers = false;
            continue;
        }
        if (reading_registers) {
            auto tokens = split(':', line);
            auto key = *(tokens[0].end() - 1);
            auto value = std::stol(tokens[1]);
            program.registers[key] = value;
        } else {
            auto tokens = split(':', line);
            trim(tokens[1]);
            auto instructions = split(',', tokens[1]);
            for (auto instruction : instructions) {
                program.instructions.push_back(std::stoi(instruction));
            }
        }
    }
    std::cout << program << std::endl;
    // P1 = 1,5,3,0,2,5,2,5,3
    // P2 = 108107566389757

    // std::vector<std::thread> threads;
    // int num_threads = 14;
    // // long start = 0;
    // long start = 108107516389757;
    // long step = (108107596389757 - start) / num_threads;
    // for (int i = 0; i < num_threads; i++) {
    //     ThreadArgs* args = new ThreadArgs();
    //     args->program = program;
    //     args->start = start + i * step;
    //     args->end = start + (i + 1) * step;
    //     std::cout << "Thread " << i << " start: " << args->start << " end: " << args->end << std::endl;
    //     args->sem = &sem;
    //     threads.push_back(std::thread(thread_run, args));
    // }

    // for (auto& thread : threads) {
    //     thread.join();
    // }
    // std::cout << "Lowest solution: " << lowest_solution << std::endl;
    // for (int i = 73307000; i < INT_MAX; i++) {
    //     if (i % 10000 == 0) {
    //         std::cout << "Iteration: " << i << std::endl;
    //     }

    //     program.registers['A'] = i;
    auto p_2 = rec(program.instructions.size() - 1, 0, program);
    std::cout << "Solution: " << p_2 << std::endl;
    std::vector<int> outputs;
    // This is the solution :D
    program.registers['A'] = 108107566389757;
    // 108107566389757;
    //  MIN NUBER = 35184372088832;
    //  88888888888888
    //  281474976710656
    //  108107566389757
    //  35184372088832
    //  2413754113035530;
    run(program, outputs, false);
    print_vector(outputs, ',');
    //     if (std::equal(program.instructions.begin(), program.instructions.end(), outputs.begin())) {
    //         std::cout << "Found the solution i = " << i << std::endl;
    //         print_vector(outputs, ',');
    //         break;
    //     }
    //     outputs.clear();
    // }
    // parallel_find_solution(program, 0, 1174412);
    return 0;
}
