#include <iostream>
#include <cassert>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

int64_t check(std::unordered_map<std::string_view, int64_t>& DP, const std::string_view& design, const std::vector<std::string> &patterns) {
  if (const auto it = DP.find(design); it != DP.end()) {
    return it->second;
  }

  int64_t nsol = 0;
  if (design.empty()) {
    nsol = 1;
  } else {
    for (const auto& pattern : patterns) {
      if (design.starts_with(pattern)) {
        nsol += check(DP, design.substr(pattern.size()), patterns);
      }
    }
  }

  DP.insert(std::make_pair(design, nsol));
  return nsol;
}

int main(int argc, char **argv) {
  std::ifstream input{argv[1]};

  std::string line;
  std::vector<std::string> patterns;
  std::getline(input, line);
  {
    std::istringstream ss(line);
    for (;!ss.eof();) {
      std::string value;
      while (ss.peek() != ',' && !ss.eof()) {
        value.push_back(ss.get());
      }
      patterns.push_back(value);
      ss.ignore(2);
    }
  }

  int64_t sum = 0;
  while (input >> line) {
    std::unordered_map<std::string_view, int64_t> DP;
    auto ok = check(DP, line, patterns);
    sum += ok;
  }

  std::cout << sum << std::endl;
}
