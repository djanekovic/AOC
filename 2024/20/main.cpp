#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <map>

struct Grid {
  std::vector<char> grid;
  int width;
  int height;
};


std::vector<std::pair<std::pair<int, int>, int>> dfs(const Grid& g, const std::pair<int, int>& start, const std::pair<int, int>& end) {
  std::queue<std::pair<int, int>> q{};
  q.push(start);

  std::vector<char> visited(g.grid.size(), false);
  std::map<int, std::pair<int, int>> parent;
  while (q.size()) {
    auto n = q.front();
    q.pop();

    if (n == end) {
      std::vector<std::pair<std::pair<int, int>, int>> path;
      int i = 0;
      path.push_back({n, i++});
      while (n != start) {
        n = parent[n.first * g.width + n.second];
        path.push_back({n, i++});
      }
      return path;
    }

    auto idx = n.first * g.width + n.second;
    if (visited[idx]) {
      continue;
    }
    visited[idx] = true;

    if (n.first - 1 >= 0 && g.grid[(n.first-1)*g.width+n.second] != '#') {
      q.push({n.first-1, n.second});
      parent.insert({(n.first-1)*g.width+n.second, n});
    }
    if (n.second - 1 >= 0 && g.grid[n.first*g.width+n.second-1] != '#') {
      q.push({n.first, n.second-1});
      parent.insert({n.first*g.width+n.second-1, n});
    }
    if (n.first + 1 < g.height && g.grid[(n.first+1)*g.width+n.second] != '#') {
      q.push({n.first+1, n.second});
      parent.insert({(n.first+1)*g.width+n.second, n});
    }
    if (n.second + 1 < g.width && g.grid[n.first*g.width+n.second+1] != '#') {
      q.push({n.first, n.second+1});
      parent.insert({n.first*g.width+n.second+1, n});
    }
  }

  return {};
}

auto cnt_jumps(const Grid &g, const std::vector<std::pair<std::pair<int, int>, int>> &path, int dist_threshold, int time_threshold) {
  int cnt = 0;
  for (const auto& [src, cost1] : path) {
    for (const auto& [dst, cost2]: path) {
      if (src == dst || cost1 <= cost2) {
        continue;
      }

      int d = std::abs(dst.first - src.first) + std::abs(dst.second - src.second);
      if (d <= dist_threshold && cost1-cost2-d >= time_threshold) {
        cnt++;
      }
    }
  }

  return cnt;
}

int main(int argc, char **argv) {
  std::ifstream input{argv[1]};

  Grid grid{};
  std::string line;
  std::pair<int, int> start, end;
  while (input >> line) {
    grid.width = line.size();
    if (auto pos = line.find('S'); pos != std::string::npos) {
      line[pos] = '.';
      start = {grid.height, pos};
    }

    if (auto pos = line.find('E'); pos != std::string::npos) {
      line[pos] = '.';
      end = {grid.height, pos};
    }

    grid.height++;
    for (auto c : line) {
      grid.grid.push_back(c);
    }
  }
  auto path = dfs(grid, start, end);
  auto jumps = cnt_jumps(grid, path, 20, 100);
  std::cout << jumps << std::endl;
}
