#include <iostream>
#include <utility>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <map>

int main(int argc, char **argv) {
  std::ifstream input(argv[1]);
  std::vector<int> left, right;
  int left_num, right_num;
  while (input >> left_num >> right_num) {
    left.push_back(left_num);
    right.push_back(right_num);
  }

  std::sort(std::begin(left), std::end(left));
  std::sort(std::begin(right), std::end(right));

#if PART1
  int sum = std::transform_reduce(std::cbegin(left), std::cend(left),
      std::cbegin(right), 0, std::plus<>(),
      [](const auto a, const auto b) { return std::abs(a - b); });
  std::cout << sum << std::endl;
#else

  std::map<int, int> freq_map;
  for (int i = 0; i < right.size(); i++) {
    ++freq_map[right[i]];
  }

  int sum = std::transform_reduce(std::cbegin(left), std::cend(left), 0,
    std::plus<>(), [&](auto l) {
      if (auto it = freq_map.find(l); it != freq_map.end()) {
        return it->second * l;
      }
      return 0;
    });

  std::cout << sum << std::endl;
#endif
}
