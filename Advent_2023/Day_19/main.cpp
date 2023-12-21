#include "../../Utils/utils.cpp"
#include <algorithm>
#include <cstddef>
#include <map>
#include <string>
#include <utility>
#include <vector>

struct Part {
  int x;
  int m;
  int a;
  int s;
};

struct Rule {
  char atr_name;
  char op;
  int limit;
};

struct Workflow {
  std::vector<std::pair<Rule, std::string>> rules;
};

/*
void print_map_workflows() {
  for (auto a : workflows) {
    std::cout << a.first << '\n';
    for (auto b : a.second.rules) {
      std::cout << b.first.atr_name << ' ' << b.first.op << ' ' << b.first.limit
                << ':' << b.second << '\n';
    }
    std::cout << '\n';
  }
}
*/

std::map<char, int> priority_map;

bool valuate_part(Part part, std::string workflow_name,
                  std::map<std::string, Workflow> workflows,
                  std::vector<Rule> &rules_taken) {
  if (workflow_name == "A") {
    return true;
  } else if (workflow_name == "R") {
    return false;
  } else {
    Workflow act_workflow = workflows[workflow_name];
    for (auto rule : act_workflow.rules) {
      if (rule.first.limit != -1) {
        if (rule.first.op == '>') {
          if (rule.first.atr_name == 'x') {
            if (part.x > rule.first.limit) {
              rules_taken.push_back(rule.first);
              return valuate_part(part, rule.second, workflows, rules_taken);
            } else {
              continue;
            }
          } else if (rule.first.atr_name == 'm') {
            if (part.m > rule.first.limit) {
              rules_taken.push_back(rule.first);
              return valuate_part(part, rule.second, workflows, rules_taken);
            } else {
              continue;
            }
          } else if (rule.first.atr_name == 'a') {
            if (part.a > rule.first.limit) {
              rules_taken.push_back(rule.first);
              return valuate_part(part, rule.second, workflows, rules_taken);
            } else {
              continue;
            }
          } else if (rule.first.atr_name == 's') {
            if (part.s > rule.first.limit) {
              rules_taken.push_back(rule.first);
              return valuate_part(part, rule.second, workflows, rules_taken);
            } else {
              continue;
            }
          }
        } else if (rule.first.op == '<') {
          if (rule.first.atr_name == 'x') {
            if (part.x < rule.first.limit) {
              rules_taken.push_back(rule.first);
              return valuate_part(part, rule.second, workflows, rules_taken);
            } else {
              continue;
            }
          } else if (rule.first.atr_name == 'm') {
            if (part.m < rule.first.limit) {
              rules_taken.push_back(rule.first);
              return valuate_part(part, rule.second, workflows, rules_taken);
            } else {
              continue;
            }
          } else if (rule.first.atr_name == 'a') {
            if (part.a < rule.first.limit) {
              rules_taken.push_back(rule.first);
              return valuate_part(part, rule.second, workflows, rules_taken);
            } else {
              continue;
            }
          } else if (rule.first.atr_name == 's') {
            if (part.s < rule.first.limit) {
              rules_taken.push_back(rule.first);
              return valuate_part(part, rule.second, workflows, rules_taken);
            } else {
              continue;
            }
          }
        }

      } else {
        return valuate_part(part, rule.second, workflows, rules_taken);
      }
    }
  }
  return false;
}

struct type_comp {
  // Operator() overloading
  bool operator()(const Rule &h1, const Rule &h2) {
    // new definition
    if (h1.atr_name != h2.atr_name) {
      return priority_map[h1.atr_name] > priority_map[h2.atr_name];
    } else if (h1.op == h2.op) {
      return h1.limit < h2.limit;
    } else {
      return h1.op < h2.op;
    }
  }
};

std::pair<char, int> analyze_rules_taken(std::vector<Rule> rules_taken) {
  type_comp tmp;
  std::sort(rules_taken.begin(), rules_taken.end(), tmp);
  if (!rules_taken.empty()) {
    Rule r = rules_taken[0];
    bool others = false;
    for (auto ra : rules_taken) {
      if (ra.atr_name == r.atr_name && ra.limit > r.limit) {
        r = ra;
      }
      // if (ra.atr_name != r.atr_name && ra.op != '<') {
      // others = true;
      //}
      std::cout << ra.atr_name << ' ' << ra.limit << ' ' << ra.op << '\n';
    }
    // if (!others) {
    //  return {r.atr_name, 4000};
    // }
    if (r.op == '<') {
      return {r.atr_name, r.limit};
    } else {
      return {r.atr_name, 4000};
    }
  } else {
    return {'s', 4000};
  }
}

int main() {
  std::vector<std::string> lines = read_lines();
  std::vector<std::string> tokens;
  std::vector<Part> parts;
  std::map<std::string, Workflow> workflows;
  bool parts_parse = false;
  for (auto line : lines) {
    if (line.empty()) {
      parts_parse = true;
      continue;
    }
    if (!parts_parse) {
      tokens = split('{', line);
      std::string workflow_name = tokens[0];
      tokens[1] = tokens[1].substr(0, tokens[1].length() - 1);
      tokens = split(',', tokens[1]);
      Workflow tmp;
      for (auto r : tokens) {
        if (r.find(':') != std::string::npos) {
          std::vector<std::string> rule = split(':', r);
          Rule tmp_r;
          tmp_r.atr_name = rule[0][0];
          tmp_r.op = rule[0][1];
          tmp_r.limit = std::stoi(rule[0].substr(2));
          tmp.rules.push_back({tmp_r, rule[1]});
        } else {
          if (r == "A") {
            Rule tmp_r = {'\0', '\0', -1};
            tmp.rules.push_back({tmp_r, r});
          } else if (r == "R") {
            Rule tmp_r = {'\0', '\0', -1};
            tmp.rules.push_back({tmp_r, r});
          } else {
            Rule tmp_r = {'\0', '\0', -1};
            tmp.rules.push_back({tmp_r, r});
          }
        }
      }
      workflows[workflow_name] = tmp;
    } else {
      line = line.substr(1, line.length() - 2);
      tokens = split(',', line);
      Part tmp = {
          std::stoi(tokens[0].substr(2)), std::stoi(tokens[1].substr(2)),
          std::stoi(tokens[2].substr(2)), std::stoi(tokens[3].substr(2))};
      parts.push_back(tmp);
    }
  }
  long long c = 167409079868000;
  std::cout << c << '\n';
  std::vector<Rule> rules_taken;
  priority_map['x'] = 0;
  priority_map['m'] = 1;
  priority_map['a'] = 2;
  priority_map['s'] = 3;
  long long sum = 0;
  for (int x = 1; x < 4001; x++) {
    for (int m = 1; m < 4001; m++) {
      for (int a = 1; a < 4001; a++) {
        for (int s = 1; s < 4001; s++) {
          if (valuate_part({x, m, a, s}, "in", workflows, rules_taken)) {
            auto an = analyze_rules_taken(rules_taken);
            long long sum_diff = 0;
            if (an.first == 'x' && an.second > x) {
              sum_diff = an.second - x + 1;
              x = an.second;
            } else if (an.first == 'm' && an.second > m) {
              sum_diff = (an.second - m + 1) * 160000;
              m = an.second;
            } else if (an.first == 'a' && an.second > a) {
              sum_diff = (an.second - a + 1) * 4000;
              a = an.second;
            } else if (an.first == 's' && an.second > s) {
              sum_diff = an.second - s + 1;
              s = an.second;
            }
            sum += sum_diff;
          } else {
            auto an = analyze_rules_taken(rules_taken);
            if (an.first == 'x') {
              x = an.second;
            } else if (an.first == 'm') {
              m = an.second;
            } else if (an.first == 'a') {
              a = an.second;
            } else if (an.first == 's') {
              s = an.second;
            }
          }
          std::cout << x << ' ' << m << ' ' << a << ' ' << s << '\n';
          // std::cout << sum << '\n';
          rules_taken.clear();
        }
      }
    }
  }
  std::cout << sum << '\n';
  /* Part 1
  int sum = 0;
  for (auto part : parts) {
    if (valuate_part(part, "in", workflows, rules_taken)) {
      sum += part.x + part.m + part.a + part.s;
    }
  }
  std::cout << sum << '\n';*/
  // valuate_part(parts[2], "in", workflows, rules_taken);
  // analyze_rules_taken(rules_taken);
  return 0;
}
