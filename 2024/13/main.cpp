#include <iostream>
#include <string>
#include <fstream>

std::pair<int64_t, int64_t> euler_div(int64_t a, int64_t b) {
  return std::make_pair(a/b, a%b);
}

int main(int argc, char **argv) {
  std::ifstream input{argv[1]};

  int64_t res = 0;
  std::string line;
  while (std::getline(input, line)) {
    if (line.empty()) {
      continue;
    }

    int64_t xa, xb, ya, yb, px, py;
    sscanf(line.data(), "Button A: X%ld, Y%ld", &xa, &ya);

    std::getline(input, line);
    sscanf(line.data(), "Button B: X%ld, Y%ld", &xb, &yb);

    std::getline(input, line);
    sscanf(line.data(), "Prize: X=%ld, Y=%ld", &px, &py);

    if (xa*yb - xb*ya == 0) {
      continue;
    }

    px += 10'000'000'000'000L;
    py += 10'000'000'000'000L;
    auto [a, aq] = euler_div(px*yb - xb*py, xa*yb - xb*ya);
    auto [b, bq] = euler_div(xa*py - px*ya, xa*yb - xb*ya);
    if (aq == 0 && bq == 0) {
      res += 3 * a + b;
    }
  }
  std::cout << res << std::endl;
}
