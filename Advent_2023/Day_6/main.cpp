#include <iostream>
#include <vector>

struct Race {
    long time;
    long distance;
};


long calculate_total(Race r, long lower_bound, long upper_bound) {
    long total = 0;
    for (long i = lower_bound; i < r.time - upper_bound; i++) {
        long speed = i;
        long distance = speed * (r.time - i);
        if (distance > r.distance) {
            total += 1;
        }
    }
    return total;
}

void parse_first_part(std::string line, std::vector<Race>& races, std::string v) {
    std::string nums = "0123456789";
    if (v == "1") {
        while (line.find_first_of(nums) != std::string::npos) {
            int idx = line.find_first_of(nums);
            while (line[idx] != ' ' && idx < line.size()) {
                idx++;
            }
            int time = std::stoi(line.substr(0, idx));
            races.push_back({time, 0});
            if (idx + 1 < line.size()) {
                line = line.substr(idx + 1);
            } else {
                break;
            }
        }
    } else {
        std::string number = "";
        while (line.find_first_of(nums) != std::string::npos) {
            int start_idx = line.find_first_of(nums);
            int idx = line.find_first_of(nums);
            while (line[idx] != ' ' && idx < line.size()) {
                idx++;
            }
        
            number += line.substr(start_idx, idx - start_idx);
            
            if (idx + 1 < line.size()) {
                line = line.substr(idx + 1);
            } else {
                break;
            }
        }
        if (v == "time") {
            Race r2;
            r2.time = std::stol(number);
            races.push_back(r2);
        } else {
            races.at(0).distance = std::stol(number);
        }
    }
}

void parse_second_part(std::string line, std::vector<Race>& races) {
    std::string nums = "0123456789";
    for (auto& a : races) {
        int idx = line.find_first_of(nums);
        while (line[idx] != ' ' && idx < line.size()) {
            idx++;
        }
        a.distance = std::stoi(line.substr(0, idx));
        if (idx + 1 < line.size()) {
            line = line.substr(idx + 1);
        } else {
            break;
        }
    }
}

int main() {
    std::vector<Race> races;
    std::vector<Race> races2;
    std::string line;

    getline(std::cin, line);
    line = line.substr(line.find_first_of(":") + 1);

    parse_first_part(line, races, "1");
    parse_first_part(line, races2, "time");

    getline(std::cin, line);
    line = line.substr(line.find_first_of(":") + 1);
    parse_second_part(line, races);
    parse_first_part(line, races2, "distance");

    int max_total = 1;
    for(auto a : races){
        max_total *= calculate_total(a, 0, 0);
        
    }
    long total_snd;
    for(auto a : races2){
         total_snd = calculate_total(a, 14, 13);
    }
    
    std::cout << max_total << std::endl;
    std::cout << total_snd << std::endl;
    return 0;
}
