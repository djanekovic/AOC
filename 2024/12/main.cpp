#include <iostream>
#include <numeric>
#include <array>
#include <algorithm>
#include <queue>
#include <vector>
#include <string>
#include <fstream>

struct Grid {
  std::vector<char> grid;
  int width;
  int height;

  static Grid parse(std::ifstream& input) {
    Grid g{};
    std::string line;
    while (input >> line) {
      g.height++;
      g.width = line.size();
      for (auto c : line) {
        g.grid.push_back(c);
      }
    }
    return g;
  }
};

auto dfs(const Grid& g, int i, int j, std::vector<char>& visited) {
  std::queue<std::pair<int, int>> q{};
  q.push({i, j});

  int region_size = 0;
  std::array<std::vector<std::pair<int, int>>, 4> sides;
  auto v = g.grid[i * g.width + j];
  while (q.size()) {
    auto n = q.front();
    q.pop();

    auto idx = n.first * g.width + n.second;
    if (visited[idx]) {
      continue;
    }
    region_size++;
    visited[idx] = true;

    if (n.first - 1 >= 0 && g.grid[(n.first-1)*g.width+n.second] == v) {
      q.push({n.first-1, n.second});
    } else {
      sides[0].push_back({n.first-1, n.second});
    }
    if (n.second - 1 >= 0 && g.grid[n.first*g.width+n.second-1] == v) {
      q.push({n.first, n.second-1});
    } else {
      sides[1].push_back({n.first, n.second-1});
    }
    if (n.first + 1 < g.height && g.grid[(n.first+1)*g.width+n.second] == v) {
      q.push({n.first+1, n.second});
    } else {
      sides[2].push_back({n.first+1, n.second});
    }
    if (n.second + 1 < g.width && g.grid[n.first*g.width+n.second+1] == v) {
      q.push({n.first, n.second+1});
    } else {
      sides[3].push_back({n.first, n.second+1});
    }
  }

  return std::make_pair(sides, region_size);
}

int part1(const Grid& g, int i, int j, std::vector<char>& visited) {
  const auto& [sides, region_size] = dfs(g, i, j, visited);
  int perimiter_cnt = 0;
  for (int i = 0; i < 4; i++) {
    perimiter_cnt += sides[i].size();
  }
  return perimiter_cnt * region_size;
}


int part2(const Grid& g, int i, int j, std::vector<char>& visited) {
  auto [sides, region_size] = dfs(g, i, j, visited);
  auto row_sortf = [](const auto &a, const auto &b) {
    if (a.first == b.first) {
      return a.second < b.second;
    }
    return a.first < b.first;
  };
  auto col_sortf = [](const auto &a, const auto &b) {
    if (a.second == b.second) {
      return a.first < b.first;
    }
    return a.second < b.second;
  };
  std::sort(sides[0].begin(), sides[0].end(), row_sortf);
  std::sort(sides[2].begin(), sides[2].end(), row_sortf);
  std::sort(sides[1].begin(), sides[1].end(), col_sortf);
  std::sort(sides[3].begin(), sides[3].end(), col_sortf);

  int sides_cnt = 0;
  for (int j = 0; j < 4; j++) {
    sides_cnt += 1;
    auto start = sides[j][0];
    for (auto i = 1UL; i < sides[j].size(); i++) {
      if (j % 2 == 0) {
        if (start.first != sides[j][i].first || start.second + 1 != sides[j][i].second) {
          sides_cnt++;
        }
        start = sides[j][i];
      }
      if (j % 2 == 1) {
        if (start.second != sides[j][i].second || start.first + 1 != sides[j][i].first) {
          sides_cnt++;
        }
        start = sides[j][i];
      }
    }
  }

  return sides_cnt * region_size;
}

int main(int argc, char **argv) {
  std::ifstream input{argv[1]};

  Grid grid = Grid::parse(input);

  std::vector<char> visited(grid.grid.size(), false);
  int res = 0;
  for (int i = 0; i < grid.height; i++) {
    for (int j = 0; j < grid.width; j++) {
      if (visited[i*grid.width + j] == false) {
        //res += part1(grid, i, j, visited);
        res += part2(grid, i, j, visited);
      }
    }
  }
  std::cout << res << std::endl;
}
