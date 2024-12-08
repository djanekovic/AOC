#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

struct Grid {
  int width;
  int height;
  std::unordered_map<char, std::vector<std::pair<int, int>>> antennas;

  static Grid parse(std::ifstream& input) {
    Grid grid;
    std::string line;
    grid.height = 0;
    while (input >> line) {
      for (int col = 0; col < line.size(); col++) {
        if (line[col] != '.') {
          auto it = grid.antennas.find(line[col]);
          if (it != grid.antennas.end()) {
            it->second.push_back({grid.height, col});
          } else {
            grid.antennas.insert({line[col], {std::make_pair(grid.height, col)}});
          }
        }
      }
      grid.width = line.size();
      grid.height++;
    }
    return grid;
  }

  void print() const {
    std::vector<char> grid(width * height, '.');
    for (const auto& kv : antennas) {
      auto antenna = kv.first;
      for (const auto& pos : kv.second) {
        grid[pos.first * width + pos.second] = antenna;
      }
    }

    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        std::cout << grid[i * width + j];
      }
      std::cout << std::endl;
    }
  }

  auto get_antinodes() const {
    std::unordered_map<char, std::vector<std::pair<int, int>>> antinodes;
    for (const auto& kv : antennas) {
      auto antenna_freq = kv.first;
      const auto& antenna_positions = kv.second;
      for (int i = 0; i < antenna_positions.size(); i++) {
        auto a1 = antenna_positions[i];
        for (int j = 0; j < antenna_positions.size(); j++) {
          if (i == j) {
            continue;
          }
          auto a2 = antenna_positions[j];
          for (int k = 0;; k++) {
            std::pair<int, int> n = std::make_pair(
                a1.first + k*(a1.first - a2.first),
                a1.second + k*(a1.second - a2.second));
            if (n.first < 0 || n.first >= height || n.second < 0 || n.second >= width) {
              break;
            }
            auto it = antinodes.find(antenna_freq);
            if (it == antinodes.end()) {
              antinodes.insert({antenna_freq, {n}});
            } else {
              it->second.push_back(n);
            }
          }
        }
      }
    }
    return antinodes;
  }
};

struct PositionHasher {
  std::uint64_t operator()(const std::pair<int, int>& p) const {
    return std::hash<int>{}(p.first) ^ std::hash<int>{}(p.second);
  }
};

int main(int argc, char **argv) {
  std::ifstream input{argv[1]};
  Grid grid = Grid::parse(input);

  auto antinodes = grid.get_antinodes();
  std::unordered_set<int> antinodes_pos_set;
  for (const auto& kv: antinodes) {
    for (const auto& pos : kv.second) {
      antinodes_pos_set.insert(pos.first * grid.width + pos.second);
    }
  }
  std::cout << antinodes_pos_set.size() << std::endl;
}
