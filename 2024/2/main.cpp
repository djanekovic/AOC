#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::vector<int> copy_except_nth(const std::vector<int>& src, int n) {
  std::vector<int> res(src.size() - 1);
  auto it = res.begin();
  for (int i = 0; i < src.size(); i++) {
    if (i == n) {
      continue;
    }
    *it = src[i];
    it++;
  }
  return res;
}

bool report_safe(const std::vector<int>& report, bool dampen) {
  bool increasing = report[1] > report[0];
  for (int i = 1; i < report.size(); i++) {
    if ((increasing && report[i] <= report[i-1]) || (!increasing && report[i] >= report[i-1])) {
      if (dampen) {
        // if i == 2, consider also removing the first element.
        // Otherwise try to remove i and i-1
        int n = (i == 2) ? 3 : 2;
        for (int j = 0; j < n; j++) {
          if (report_safe(copy_except_nth(report, i-j), false)) {
            return true;
          }
        }
      }
      return false;
    } else if (std::abs(report[i] - report[i-1]) > 3) {
      if (dampen) {
        for (int j = 0; j < 2; j++) {
          if (report_safe(copy_except_nth(report, i-j), false)) {
            return true;
          }
        }
      }
      return false;
    }
  }
  return true;
}

int main(int argc, char **argv) {
  std::ifstream input(argv[1]);

  int safe = 0;
  std::string line;
  while (std::getline(input, line)) {
    std::stringstream ss(line);
    std::vector<int> report;
    int num;
    while (ss >> num) {
      report.push_back(num);
    }

    if (report_safe(report, true)) {
      safe++;
    }
  }

  std::cout << safe << std::endl;
}
