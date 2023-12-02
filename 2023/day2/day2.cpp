#include <algorithm>
#include <iostream>
#include <numeric>
#include <cassert>
#include <sstream>
#include <vector>
#include <array>

enum class Color {
  RED,
  GREEN,
  BLUE
};

using GameSet = std::array<std::size_t, 3>;

struct Game {
  std::size_t id;
  // N x 3 matrix where each row is one game set
  std::vector<GameSet> sets;
};

std::ostream& operator<<(std::ostream& os, const Game& game) {
  os << game.id << std::endl;
  for (const auto& set : game.sets) {
    os << set[0] << " " << set[1] << " " << set[2] << std::endl;
  }
  return os;
}

Game parse_game(const std::string& game) {
  Game g;
  std::string token;
  std::stringstream ss(game);

  ss >> token; assert(token == "Game");
  ss >> g.id;
  ss >> token; assert(token == ":");

  do {
    GameSet set = {0};
    do {
      int num;
      ss >> num;

      ss >> token;
      if (token.compare(0, 3, "red") == 0) {
        set[static_cast<int>(Color::RED)] += num;
      } else if (token.compare(0, 4, "blue") == 0) {
        set[static_cast<int>(Color::BLUE)] += num;
      } else if (token.compare(0, 5, "green") == 0) {
        set[static_cast<int>(Color::GREEN)] += num;
      } else {
        std::cerr << token << std::endl; assert(false);
      }
    } while (token.back() == ',');
    g.sets.push_back(set);
  } while (token.back() == ';');

  return g;
}


int main() {
  int result = 0;
  for (std::string game; std::getline(std::cin, game); ) {
    Game g = parse_game(game);

    GameSet minset = std::accumulate(std::cbegin(g.sets), std::cend(g.sets), g.sets[0],
        [](const auto& acc, const auto& set) {
          GameSet res;
          std::transform(std::cbegin(acc), std::cend(acc), std::cbegin(set),
                         std::begin(res), [](auto a, auto b) { return std::max(a, b); });
          return res;
        });

    result += (minset[0] * minset[1] * minset[2]);
  }

  std::cout << result << std::endl;
}
