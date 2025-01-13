#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <array>
#include <fstream>
#include <vector>
#include <string>

int main(int argc, char **argv) {
  std::ifstream input{argv[1]};

  std::string line;
  std::vector<uint64_t> secrets;
  while (input >> line) {
    uint64_t secret = std::stoi(line);
    secrets.push_back(secret);
  }

  std::unordered_map<int, int> totals;
  auto base = std::array{6859, 361, 19, 1};
  for (int i = 0; i < secrets.size(); i++) {
    std::unordered_map<int, int> seq_to_prices;
    auto secret = secrets[i];
    int price = secret % 10;
    std::array<int, 4> buffer;
    for (int j = 0; j < 2000; j++) {
      secret = (secret ^ (secret * 64)) % 16777216;
      secret = (secret ^ (secret / 32)) % 16777216;
      secret = (secret ^ (secret * 2048)) % 16777216;
      int new_price = (secret % 10);
      int diff = new_price - price+9;
      price = new_price;

      if (j < 4) {
        buffer[j] = diff;
        continue;
      }
      std::rotate(buffer.begin(), buffer.begin()+1, buffer.end());
      buffer[3] = diff;

      int key = 0;
      for (int k = 0; k < 4; k++) {
        key += buffer[k] * base[k];
      }

      if (auto it = seq_to_prices.find(key); it == seq_to_prices.end()) {
        seq_to_prices.insert({key, new_price});
      }
    }

    for (const auto& [k, v]: seq_to_prices) {
      if (auto it = totals.find(k); it != totals.end()) {
        it->second += v;
      } else {
        totals.insert({k, v});
      }
    }
  }

  auto res_it = std::max_element(totals.begin(), totals.end(),
      [](const auto &a, const auto& b) {
        return a.second < b.second;
      });
  std::cout << res_it->second << std::endl;
}
