#include <fstream>
#include <iostream>
#include <string>

int parse_number(const std::string& line, std::string::size_type& pos) {
    int n = line.size();
    auto start_it = line.cbegin() + pos;
    while (pos < n && line[pos] >= '0' && line[pos] <= '9') {
      pos += 1;
    }

    std::string_view number_str(start_it, line.cbegin() + pos);

    int number = 0;
    int power = 1;
    for (int i = 0; i < number_str.size(); i++) {
      number += static_cast<int>(number_str[number_str.size() - i - 1] - '0') * power;
      power *= 10;
    }
    return number;
}

int main(int argc, char **argv) {
  std::ifstream input{argv[1]};

  std::string line;
  int res = 0;
  bool enabled = true;
  while (input >> line) {
    for (std::string::size_type pos = 0; pos < line.size();) {
      if (line[pos] == 'm' && enabled) {
        // eat mul(X,Y)
        int ret = line.compare(pos, 3, "mul");
        if (ret != 0) {
          pos++;
          continue;
        }

        pos += 3;
        if (line[pos++] != '(') {
          continue;
        }

        int number1 = parse_number(line, pos);
        if (line[pos++] != ',') {
          continue;
        }

        int number2 = parse_number(line, pos);
        if (line[pos++] != ')') {
          continue;
        }

        res += number1 * number2;
      } else if (line[pos] == 'd') {
        // eat do() or don't() commands
        if (line.compare(pos, 4, "do()") == 0) {
          enabled = true;
          pos += 4;
        } else if (line.compare(pos, 7, "don\'t()") == 0) {
          enabled = false;
          pos += 7;
        } else {
          pos++;
        }
      } else {
        pos++;
      }
    }
  }
  std::cout << res << std::endl;
}
