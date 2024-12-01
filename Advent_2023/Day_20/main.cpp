#include "../../Utils/utils.cpp"
#include <map>
#include <vector>

struct FlipFlop {
  std::string name;
  bool state;
  std::vector<std::string> next;
  bool next_pulse;
};

struct Conjunction {
  std::string name;
  std::map<std::string, bool> previous;
  std::vector<std::string> next;
};

struct Broadcaster {
  std::vector<std::string> modules;
};

enum ModuleType {
  FlipFlopModule,
  ConjunctionModule,
  BroadcasterModule,
};

std::map<std::string, ModuleType> mapping;
std::map<std::string, FlipFlop> flipflops;
std::map<std::string, Conjunction> conjunctions;

void send_pulse(Broadcaster br) {
  bool pulse = false;
  for (auto module_name : br.modules) {
    if (!flipflops[module_name].state) {
      flipflops[module_name].state = true;
      flipflops[module_name].next_pulse = true;
    } else {
      flipflops[module_name].state = false;
      flipflops[module_name].next_pulse = true;
    }
  }
  for (auto module_name : br.modules) {
    for (auto &a : flipflops[module_name].next) {
      FlipFlop tmp = flipflops[module_name];
      if (mapping[a] == FlipFlopModule) {
        if (tmp.next_pulse == false) {
          flipflops[a].state = !flipflops[a].state;
          flipflops[a].next_pulse = true;
        }
      } else if (mapping[a] == ConjunctionModule) {
        conjunctions[a].previous[module_name] = tmp.next_pulse;
      }
    }
  }
}

int main() {
  std::vector<std::string> lines = read_lines();
  std::vector<std::string> tokens;
  Broadcaster br;
  for (auto line : lines) {
    tokens = split(' ', line);
    if (tokens[0] == "broadcaster") {
      for (int i = 2; i < tokens.size(); i++) {
        br.modules.push_back(tokens[i]);
      }
    } else if (tokens[0][0] == '%') {
      mapping[tokens[0].substr(1)] = FlipFlopModule;
      FlipFlop tmp = {tokens[0].substr(1), false};
      for (int i = 2; i < tokens.size(); i++) {
        if (i != tokens.size() - 1) {
          tmp.next.push_back(tokens[i].substr(0, tokens[i].length() - 1));
        } else {
          tmp.next.push_back(tokens[i]);
        }
      }
      flipflops[tokens[0].substr(1)] = tmp;
    } else if (tokens[0][0] == '&') {
      mapping[tokens[0].substr(1)] = ConjunctionModule;
      Conjunction tmp = {tokens[0].substr(1)};
      for (int i = 2; i < tokens.size(); i++) {
        if (i != tokens.size() - 1) {
          tmp.next.push_back(tokens[i].substr(0, tokens[i].length() - 1));
        } else {
          tmp.next.push_back(tokens[i]);
        }
      }
      conjunctions[tokens[0].substr(1)] = tmp;
    }
  }

  for (auto conj : conjunctions) {
    for (auto n : conj.second.next) {
      if (conjunctions.find(n) != conjunctions.end()) {
        conjunctions[n].previous[conj.first] = false;
      }
    }
  }
  for (auto flop : flipflops) {
    for (auto n : flop.second.next) {
      if (conjunctions.find(n) != conjunctions.end()) {
        conjunctions[n].previous[n] = false;
      }
    }
  }

  //  for (auto conj : conjunctions) {
  //  std::cout << "Conjunction with name : has those previous " << conj.first
  //            << '\n';
  //  for (auto n : conj.second.previous) {
  //    std::cout << n.first << '\n';
  //  }
  //}
  // for (auto flip : flipflops) {
  //  std::cout << "FlipFlop with name : " << flip.first << '\n';
  //  for (auto n : flip.second.next) {
  //    std::cout << n << '\n';
  //  }
  //}

  return 0;
}
