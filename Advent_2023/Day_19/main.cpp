#include "../../Utils/utils.cpp"
#include <cstddef>
#include <map>
#include <string>
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

bool valuate_part(Part part, std::string workflow_name,
                  std::map<std::string, Workflow> workflows) {
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
              return valuate_part(part, rule.second, workflows);
            } else {
              continue;
            }
          } else if (rule.first.atr_name == 'm') {
            if (part.m > rule.first.limit) {
              return valuate_part(part, rule.second, workflows);
            } else {
              continue;
            }
          } else if (rule.first.atr_name == 'a') {
            if (part.a > rule.first.limit) {
              return valuate_part(part, rule.second, workflows);
            } else {
              continue;
            }
          } else if (rule.first.atr_name == 's') {
            if (part.s > rule.first.limit) {
              return valuate_part(part, rule.second, workflows);
            } else {
              continue;
            }
          }
        } else if (rule.first.op == '<') {
          if (rule.first.atr_name == 'x') {
            if (part.x < rule.first.limit) {
              return valuate_part(part, rule.second, workflows);
            } else {
              continue;
            }
          } else if (rule.first.atr_name == 'm') {
            if (part.m < rule.first.limit) {
              return valuate_part(part, rule.second, workflows);
            } else {
              continue;
            }
          } else if (rule.first.atr_name == 'a') {
            if (part.a < rule.first.limit) {
              return valuate_part(part, rule.second, workflows);
            } else {
              continue;
            }
          } else if (rule.first.atr_name == 's') {
            if (part.s < rule.first.limit) {
              return valuate_part(part, rule.second, workflows);
            } else {
              continue;
            }
          }
        }

      } else {
        return valuate_part(part, rule.second, workflows);
      }
    }
  }
  return false;
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
  int sum = 0;
  for (auto part : parts) {
    if (valuate_part(part, "in", workflows)) {
      sum += part.x + part.m + part.a + part.s;
    }
  }
  std::cout << sum << '\n';

  return 0;
}
