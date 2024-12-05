#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using GraphT = std::unordered_map<int, std::unordered_set<int>>;

bool validate_update(const std::vector<int>& update, const GraphT& page_order_graph) {
  for (int i = 0; i < update.size(); i++) {
    for (int j = i+1; j < update.size(); j++) {
      const auto& it = page_order_graph.find(update[i]);
      if (it == page_order_graph.cend()) {
        return false;
      }
      if (it->second.find(update[j]) == it->second.cend()) {
        return false;
      }
    }
  }
  return true;
}


int main(int argc, char **argv) {
  std::ifstream input{argv[1]};

  std::string line;
  GraphT page_order_graph;
  while(std::getline(input, line, '\n')) {
    if (line.empty()) {
      break;
    }
    std::stringstream ss(line);
    int x, y;
    char delim;
    ss >> x;
    ss >> delim;
    ss >> y;

    if (auto it = page_order_graph.find(x); it != page_order_graph.end()) {
      it->second.insert(y);
    } else {
      page_order_graph[x] = {y};
    }
  }

  auto res = 0;
  while (input >> line) {
    int num;
    char delim;
    std::stringstream ss(line);
    std::vector<int> update;
    while (ss >> num) {
      update.push_back(num);
      ss >> delim;
    }

    bool valid = validate_update(update, page_order_graph);
    if (valid) {
      //res += update[update.size() / 2];
    } else {
      std::unordered_set<int> update_set(update.begin(), update.end());
      std::vector<std::pair<int, int>> page_to_acceptable_len;
      for (int i = 0; i < update.size(); i++) {
        int cnt = 0;
        // compute size of the set intersection
        auto it = page_order_graph.find(update[i]);
        if (it != page_order_graph.cend()) {
          for (const auto& c : it->second) {
            cnt += update_set.count(c);
          }
        }
        page_to_acceptable_len.push_back({update[i], cnt});
      }

      std::sort(page_to_acceptable_len.begin(), page_to_acceptable_len.end(),
          [](const auto& a, const auto& b) { return a.second > b.second; });

      res += page_to_acceptable_len[page_to_acceptable_len.size() / 2].first;
    }
  }

  std::cout << res << std::endl;
}
