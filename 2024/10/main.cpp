#include <iostream>
#include <string>
#include <fstream>
#include <set>

struct Topomap {
  std::vector<char> grid;
  int width;
  int height;

  static Topomap parse(std::ifstream& input) {
    Topomap topomap{};

    std::string line;
    while (input >> line) {
      topomap.width = line.size();
      topomap.height++;
      for (auto c : line) {
        topomap.grid.push_back(c);
      }
    }

    return topomap;
  }

  std::vector<int> get_trailheads(int row, int col) const {
    std::vector<int> trailheads;
    std::queue<std::pair<int, int>> q;
    q.push({row, col});
    while (q.size()) {
      const auto& [parent_row, parent_col] = q.front();
      auto v = grid[parent_row * width + parent_col];
      //std::cout << parent_row << " " << parent_col << " " << v << std::endl;
      q.pop();

      if (v == '9') {
        trailheads.push_back(parent_row*width+parent_col);
        continue;
      }

      if (parent_row - 1 >= 0) {
        auto candidate = grid[(parent_row-1)*width+parent_col];
        if (candidate == v+1) {
          q.push({parent_row-1, parent_col});
        }
      }
      if (parent_col - 1 >= 0) {
        auto candidate = grid[parent_row*width+parent_col-1];
        if (candidate == v+1) {
          q.push({parent_row, parent_col-1});
        }
      }
      if (parent_row + 1 < height) {
        auto candidate = grid[(parent_row+1)*width+parent_col];
        if (candidate == v+1) {
          q.push({parent_row+1, parent_col});
        }
      }
      if (parent_col + 1 < width) {
        auto candidate = grid[parent_row*width+parent_col+1];
        if (candidate == v+1) {
          q.push({parent_row, parent_col+1});
        }
      }
    }

    return trailheads;

  }
};

int main(int argc, char **argv) {
  std::ifstream input{argv[1]};

  Topomap topomap = Topomap::parse(input);
  auto res = 0;
  for (int i = 0; i < topomap.height; i++) {
    for (int j = 0; j < topomap.width; j++) {
      if (topomap.grid[i * topomap.width + j] == '0') {
        // remove duplicates if you want the unique trails
        auto trailheads = topomap.get_trailheads(i, j);
        res += trailheads.size();
      }
    }
  }

  std::cout << res << std::endl;
}
