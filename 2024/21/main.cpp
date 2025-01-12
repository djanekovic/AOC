#include <iostream>
#include <cassert>
#include <array>
#include <queue>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <unordered_map>

struct NumericKeyboard {
  static constexpr std::pair<int, int> forbidden{3, 0};

  static std::pair<int, int> convert(char c) {
    switch (c) {
      case '7': return {0, 0};
      case '8': return {0, 1};
      case '9': return {0, 2};
      case '4': return {1, 0};
      case '5': return {1, 1};
      case '6': return {1, 2};
      case '1': return {2, 0};
      case '2': return {2, 1};
      case '3': return {2, 2};
      case '0': return {3, 1};
      case 'A': return {3, 2};
      default: throw std::invalid_argument("Not cool");
    }
  }
};

struct DirectionalKeyboard {
  static constexpr std::pair<int, int> forbidden{0, 0};

  static std::pair<int, int> convert(char c) {
    switch(c) {
      case '^': return {0, 1};
      case 'A': return {0, 2};
      case '<': return {1, 0};
      case 'v': return {1, 1};
      case '>': return {1, 2};
      default: throw std::invalid_argument("Not cool");
    }
  }
};

struct StateT {
  std::pair<int, int> pos;
  std::string actions;
};

struct MemoKeyT {
  char position;
  char next;
  int depth;

  bool operator==(const MemoKeyT& other) const {
    return other.position == position &&
      other.next == next &&
      other.depth == depth;
  }
};

struct MemoHasher {
  std::size_t operator()(const MemoKeyT& k) const {
    using std::hash;
    return hash<char>{}(k.position) ^
      hash<char>{}(k.next) ^
      hash<char>{}(k.depth);
  }
};

template<typename KeyboardType, size_t MaxDepth>
int64_t bfs(std::unordered_map<MemoKeyT, int64_t, MemoHasher>& memo,
    std::array<char, MaxDepth>& state,
    char instruction, int depth) {
  auto current = state[state.size()-depth-1];
  if (auto it = memo.find({current, instruction, depth}); it != memo.end()) {
    return it->second;
  }

  std::queue<StateT> q;
  q.push({KeyboardType::convert(state[state.size()-depth-1]), ""});

  int64_t min_num_steps = std::numeric_limits<int64_t>::max();
  auto end = KeyboardType::convert(instruction);
  while(!q.empty()) {
    auto v = q.front();
    q.pop();

    if (v.pos == end) {
      v.actions.push_back('A');
      int64_t num_steps = 0;
      if (depth == 0) {
        num_steps = v.actions.size();
      } else {
        for (auto c : v.actions) {
          auto inc = bfs<DirectionalKeyboard, MaxDepth>(memo, state, c, depth-1);
          num_steps += inc;
          state[state.size()-depth] = c;
        }
      }
      min_num_steps = std::min(min_num_steps, num_steps);
      continue;
    }
    if (v.pos == KeyboardType::forbidden) {
      continue;
    }
    if (v.pos.first < end.first) {
      q.push({{v.pos.first+1, v.pos.second}, v.actions + 'v'});
    }
    if (v.pos.first > end.first) {
      q.push({{v.pos.first-1, v.pos.second}, v.actions + '^'});
    }
    if (v.pos.second < end.second) {
      q.push({{v.pos.first, v.pos.second+1}, v.actions + '>'});
    }
    if (v.pos.second > end.second) {
      q.push({{v.pos.first, v.pos.second-1}, v.actions + '<'});
    }
  }

  memo.insert({{current, instruction, depth}, min_num_steps});
  return min_num_steps;
}

int main(int argc, char **argv) {
  std::ifstream input{argv[1]};

  std::string line;
  int64_t res = 0;
  constexpr auto max_depth = 26;
  while (input >> line) {
    std::unordered_map<MemoKeyT, int64_t, MemoHasher> memo;
    std::string num = line.substr(0, line.size()-1);
    int64_t num_steps = 0;
    std::array<char, max_depth> state;
    state.fill('A');
    for (auto c : line) {
      num_steps += bfs<NumericKeyboard, max_depth>(memo, state, c, max_depth-1);
      state[0] = c;
    }
    res += std::stoi(num) * num_steps;
    std::cout << num << " " << num_steps << std::endl;
  }
  std::cout << res << std::endl;
}
