#include <limits.h>
#include <omp.h>

#include <iostream>
#include <map>
#include <thread>
#include <vector>

enum Map_Classification {
    Seed_Soil,
    Soil_Fertilizer,
    Fertilizer_Water,
    Water_Light,
    Light_Temperature,
    Temperature_Humidity,
    Humidity_Location
};
// Part 1
void read_seeds(std::vector<long> &seeds) {
    std::string input;
    getline(std::cin, input);
    input = input.substr(input.find_first_of(':') + 2);
    input += " ";
    while (input.find_first_of(' ') != std::string::npos) {
        seeds.push_back(std::stol(input.substr(0, input.find_first_of(' '))));
        input = input.substr(input.find_first_of(' ') + 1);
    }
}

std::map<long, long> Seed_Soil_Map;
std::map<long, long> Soil_Fertilizer_Map;
std::map<long, long> Fertilizer_Water_Map;
std::map<long, long> Water_Light_Map;
std::map<long, long> Light_Temperature_Map;
std::map<long, long> Temperature_Humidity_Map;
std::map<long, long> Humidity_Location_Map;
Map_Classification Actual_Classification;
long destination_start = 0;
long source_range_start = 0;
long range_lenght = 0;

void update_map(const std::pair<long, long> k_v, std::map<long, long> &map) {
    if (k_v.second >= source_range_start && k_v.second < source_range_start + range_lenght) {
        map[k_v.first] = destination_start + k_v.second - source_range_start;
    } else if (map[k_v.first] == 0) {
        map[k_v.first] = k_v.second;
    }
}

void solve_part_1(std::string line, std::vector<long> &seeds) {
    if (line == "") {
        return;
    }
    if (line == "seed-to-soil map:") {
        Actual_Classification = Seed_Soil;
    } else if (line == "soil-to-fertilizer map:") {
        Actual_Classification = Soil_Fertilizer;
    } else if (line == "fertilizer-to-water map:") {
        Actual_Classification = Fertilizer_Water;
    } else if (line == "water-to-light map:") {
        Actual_Classification = Water_Light;
    } else if (line == "light-to-temperature map:") {
        Actual_Classification = Light_Temperature;
    } else if (line == "temperature-to-humidity map:") {
        Actual_Classification = Temperature_Humidity;
    } else if (line == "humidity-to-location map:") {
        Actual_Classification = Humidity_Location;
    } else {
        destination_start = std::stol(line.substr(0, line.find_first_of(' ')));
        source_range_start = std::stol(line.substr(line.find_first_of(' ') + 1, line.find_last_of(' ') - line.find_first_of(' ') - 1));
        range_lenght = std::stol(line.substr(line.find_last_of(' ') + 1));
        switch (Actual_Classification) {
            case Seed_Soil:
                for (auto seed : seeds) {
                    std::pair<long, long> p(seed, seed);
                    update_map(p, Seed_Soil_Map);
                }
                break;
            case Soil_Fertilizer:
                for (auto s_s_m : Seed_Soil_Map) {
                    update_map(s_s_m, Soil_Fertilizer_Map);
                }
                break;
            case Fertilizer_Water:
                for (auto s_f_m : Soil_Fertilizer_Map) {
                    update_map(s_f_m, Fertilizer_Water_Map);
                }
                break;
            case Water_Light:
                for (auto f_w_m : Fertilizer_Water_Map) {
                    update_map(f_w_m, Water_Light_Map);
                }
                break;
            case Light_Temperature:
                for (auto w_l_m : Water_Light_Map) {
                    update_map(w_l_m, Light_Temperature_Map);
                }
                break;
            case Temperature_Humidity:
                for (auto l_t_m : Light_Temperature_Map) {
                    update_map(l_t_m, Temperature_Humidity_Map);
                }
                break;
            case Humidity_Location:
                for (auto t_h_m : Temperature_Humidity_Map) {
                    update_map(t_h_m, Humidity_Location_Map);
                }
                break;
        }
    }
}

struct Map {
    long destination_start;
    long source_start;
    long length;
};

void load_vectors_of_maps(std::string line, std::vector<std::vector<Map>> &maps) {
    if (line == "") {
        return;
    }
    if (line == "seed-to-soil map:") {
        Actual_Classification = Seed_Soil;
    } else if (line == "soil-to-fertilizer map:") {
        Actual_Classification = Soil_Fertilizer;
    } else if (line == "fertilizer-to-water map:") {
        Actual_Classification = Fertilizer_Water;
    } else if (line == "water-to-light map:") {
        Actual_Classification = Water_Light;
    } else if (line == "light-to-temperature map:") {
        Actual_Classification = Light_Temperature;
    } else if (line == "temperature-to-humidity map:") {
        Actual_Classification = Temperature_Humidity;
    } else if (line == "humidity-to-location map:") {
        Actual_Classification = Humidity_Location;
    } else {
        destination_start = std::stol(line.substr(0, line.find_first_of(' ')));
        source_range_start = std::stol(line.substr(line.find_first_of(' ') + 1, line.find_last_of(' ') - line.find_first_of(' ') - 1));
        range_lenght = std::stol(line.substr(line.find_last_of(' ') + 1));
        switch (Actual_Classification) {
            case Seed_Soil:
                if (maps.size() == 0) {
                    maps.push_back(std::vector<Map>());
                }
                maps[maps.size() - 1].push_back(Map{destination_start, source_range_start, range_lenght});
                break;
            case Soil_Fertilizer:
                if (maps.size() == 1) {
                    maps.push_back(std::vector<Map>());
                }
                maps[maps.size() - 1].push_back(Map{destination_start, source_range_start, range_lenght});
                break;
            case Fertilizer_Water:
                if (maps.size() == 2) {
                    maps.push_back(std::vector<Map>());
                }
                maps[maps.size() - 1].push_back(Map{destination_start, source_range_start, range_lenght});
                break;
            case Water_Light:
                if (maps.size() == 3) {
                    maps.push_back(std::vector<Map>());
                }
                maps[maps.size() - 1].push_back(Map{destination_start, source_range_start, range_lenght});
                break;
            case Light_Temperature:
                if (maps.size() == 4) {
                    maps.push_back(std::vector<Map>());
                }
                maps[maps.size() - 1].push_back(Map{destination_start, source_range_start, range_lenght});
                break;
            case Temperature_Humidity:
                if (maps.size() == 5) {
                    maps.push_back(std::vector<Map>());
                }
                maps[maps.size() - 1].push_back(Map{destination_start, source_range_start, range_lenght});
                break;
            case Humidity_Location:
                if (maps.size() == 6) {
                    maps.push_back(std::vector<Map>());
                }
                maps[maps.size() - 1].push_back(Map{destination_start, source_range_start, range_lenght});
                break;
        }
    }
}

void process_inner_loop(const int i, const std::vector<long> &seeds, const std::vector<std::vector<Map>> &maps, long &minimal_location) {
    for (long j = seeds[i]; j < seeds[i] + seeds[i + 1]; j++) {
        if (j == (seeds[i] + seeds[i + 1]) / 2) {
            std::cout << "Thread "
                      << " is at " << j << std::endl;
        } else if (j == seeds[i] + seeds[i + 1] - 1) {
            std::cout << "Thread with initial value " << seeds[i] << " ended" << std::endl;
        }
        long location = j;
        for (auto map : maps) {
            for (auto m : map) {
                if (location >= m.source_start && location < m.source_start + m.length) {
                    location = m.destination_start + location - m.source_start;
                    break;
                } else {
                    location = location;
                }
            }
        }
        if (location < minimal_location) {
            minimal_location = location;
        }
    }
}

int main() {
    std::vector<long> seeds;
    std::vector<long> seeds2;
    std::vector<std::vector<Map>> maps;

    read_seeds(seeds);
    std::string line;

    while (getline(std::cin, line)) {
        solve_part_1(line, seeds);
        load_vectors_of_maps(line, maps);
    }

    // Part 1
    long min_location = LONG_MAX;
    for (auto h_l_m : Humidity_Location_Map) {
        if (h_l_m.second < min_location) {
            min_location = h_l_m.second;
        }
    }
    for (auto seed : seeds) {
        std::cout << seed << " " << Seed_Soil_Map[seed] << " " << Soil_Fertilizer_Map[seed] << " " << Fertilizer_Water_Map[seed] << " " << Water_Light_Map[seed] << " " << Light_Temperature_Map[seed] << " " << Temperature_Humidity_Map[seed] << " " << Humidity_Location_Map[seed] << std::endl;
    }
    std::cout << "Minimal location for part 1 :" << min_location << std::endl;

    std::cout << maps.size() << std::endl;
    int size = seeds.size();
    long minimal_location = LONG_MAX;
    const int num_threads = std::thread::hardware_concurrency();
    std::cout << num_threads << std::endl;
    std::vector<std::thread> threads;
    // Part 2
    for (int i = 0; i < size; i += 2) {
        std::cout << "Started " << seeds[i] << " " << seeds[i + 1] << std::endl;
        threads.emplace_back(process_inner_loop, i, std::ref(seeds), std::ref(maps), std::ref(minimal_location));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    std::cout << "Minimal location for part 2 :" << minimal_location << std::endl;

    return 0;
}
