#include <iostream>
#include <cassert>
#include <charconv>
#include <array>
#include <algorithm>
#include <vector>

// defined as [start, start + length)
struct NumberView {
  std::pair<std::size_t, std::size_t> start;
  std::size_t length;
};

int main() {
  std::vector<std::string> schematic;
  for (std::string line; std::getline(std::cin, line); schematic.push_back(line));

  std::vector<NumberView> numbers;
  for (std::size_t line_idx = 0; line_idx < schematic.size(); ++line_idx) {
    const auto& line = schematic[line_idx];

    for (auto last_it = std::cbegin(line);;) {
      const auto num_start_it = std::find_if(last_it, std::cend(line),
          [](auto c) { return std::isdigit(c); });
      if (num_start_it == std::cend(line)) {
        break;
      }

      const auto num_end_it = std::find_if_not(num_start_it, std::cend(line),
          [](auto c) { return std::isdigit(c); });

      auto j = std::distance(std::cbegin(line), num_start_it);
      auto len = std::distance(num_start_it, num_end_it);
      NumberView num{
        .start = std::make_pair(line_idx, j),
        .length = static_cast<std::size_t>(len),
      };
      numbers.push_back(num);
      last_it = num_end_it;
    }
  }

  int result = 0;
  std::vector<bool> checked(numbers.size(), false);
  for (std::size_t line_idx = 0; line_idx < schematic.size(); ++line_idx) {
    const auto& line = schematic[line_idx];

    for (auto last_it = std::cbegin(line); ;) {
      const auto gear_it = std::find_if(last_it, std::cend(line),
          [](auto c) { return c == '*'; });
      if (gear_it == std::cend(line)) {
        break;
      }

      auto j = static_cast<std::size_t>(std::distance(std::cbegin(line), gear_it));

      std::vector<std::size_t> adjacent_nums;
      adjacent_nums.reserve(4);
      for (std::size_t k = 0; k < numbers.size(); ++k) {
        const auto& number = numbers[k];
        auto last_digit_pos = number.start.second + number.length - 1;
        const auto is_part_number =
          !checked[k] &&
          number.start.first <= line_idx + 1 &&
          number.start.first >= line_idx - 1 &&
          ((number.start.second >= j-1 && number.start.second <= j+1) ||
           (last_digit_pos >= j-1 && last_digit_pos <= j+1) ||
           (number.start.second <= j-1 && last_digit_pos >= j+1));

        if (is_part_number) {
          int num;
          auto [ptr, ec] = std::from_chars(
              schematic[number.start.first].data() + number.start.second,
              schematic[number.start.first].data() + number.start.second + number.length,
              num);
          assert(ec == std::errc());

          checked[k] = true;
          adjacent_nums.push_back(num);
        }
      }

      if (adjacent_nums.size() == 2) {
        result += (adjacent_nums[0] * adjacent_nums[1]);
      }

      last_it = gear_it + 1;
    }
  }

  std::cout << result << std::endl;
}
