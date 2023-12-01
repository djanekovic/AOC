#include <iostream>
#include <cstring>
#include <string>
#include <ranges>

void replace_if_substring(std::size_t pos, std::string_view substring,
    std::string_view replacement, std::string *line) {
  if (std::strncmp(line->data() + pos, substring.data(), substring.size()) == 0) {
    line->replace(pos, substring.size(), replacement);
  }
}

int main() {
  std::string line;

  std::uint64_t sum = 0;
  while (std::cin >> line) {
    for (std::size_t i = 0; i < line.size(); ++i) {
      switch(line[i]) {
        case 'o':
          replace_if_substring(i, "one", "1", &line);
          break;
        case 'e':
          replace_if_substring(i, "eight", "8", &line);
          break;
        case 'n':
          replace_if_substring(i, "nine", "9", &line);
          break;
        case 'f':
          if (i + 1 < line.size()) {
            if (line[i+1] == 'o') {
              replace_if_substring(i, "four", "4", &line);
            } else if (line[i+1] == 'i') {
              replace_if_substring(i, "five", "5", &line);
            }
          }
          break;
        case 's':
          if (i + 1 < line.size()) {
            if (line[i+1] == 'e') {
              replace_if_substring(i, "seven", "7", &line);
            } else if (line[i+1] == 'i') {
              replace_if_substring(i, "six", "6", &line);
            }
          }
          break;
        case 't':
          if (i + 1 < line.size()) {
            if (line[i+1] == 'h') {
              replace_if_substring(i, "three", "3", &line);
            } else if (line[i+1] == 'w') {
              replace_if_substring(i, "two", "2", &line);
            }
          }
          break;
      }
    }

    auto nums = line
      | std::ranges::views::filter([](char c) { return std::isdigit(c); })
      | std::ranges::views::transform([](char c) { return c - '0';});

    auto first = *nums.begin();
    auto last = *std::ranges::views::reverse(nums).begin();
    sum += (first * 10 + last);
  }

  std::cout << sum << std::endl;
}
