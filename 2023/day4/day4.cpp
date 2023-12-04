#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <cassert>
#include <sstream>
#include <vector>

struct Card {
  std::size_t id;
  std::vector<std::size_t> winning_nums;
  std::vector<std::size_t> my_nums;
};

std::ostream& operator<<(std::ostream& os, const Card& card) {
  os << card.id << std::endl;
  for (const auto& num : card.winning_nums) {
    os << num << " ";
  }
  os << std::endl;

  for (const auto& num : card.my_nums) {
    os << num << " ";
  }
  os << std::endl;
  return os;
}

Card parse_card(const std::string& line) {
  Card card;

  std::stringstream ss(line);
  std::string token;

  ss >> token;
  assert(token == "Card");

  ss >> card.id;

  ss >> token;
  assert(token == ":");

  while (ss >> token) {
    if (token == "|") {
      break;
    }
    card.winning_nums.push_back(std::stoi(token));
  }

  while (ss >> token) {
    card.my_nums.push_back(std::stoi(token));
  }

  return card;
}

int main() {
  int i = 0;
  std::vector<std::size_t> copies;
  for (std::string line; std::getline(std::cin, line); i++) {
    Card card = parse_card(line);

    std::ranges::sort(card.winning_nums);
    std::ranges::sort(card.my_nums);

    int num_matches = std::transform_reduce(
        std::cbegin(card.winning_nums), std::cend(card.winning_nums), 0, std::plus<>(),
        [&](auto num) { return std::ranges::binary_search(card.my_nums, num); });

    for (int j = copies.size(); j < i + num_matches + 1; j++) {
      copies.push_back(1);
    }

    for (int j = i + 1; j < i + num_matches + 1; ++j) {
      copies.at(j) += copies.at(i);
    }
  }

  std::cout << std::reduce(std::cbegin(copies), std::cend(copies), 0) << std::endl;
}
