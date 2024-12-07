#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

std::int64_t concat(int64_t a, int b) {
  // C++ doesn't have log10 that works on int64_t..
  int64_t power = 1;
  for (;; power *= 10) {
    if (b / power == 0) {
      break;
    }
  }
  return a * power + b;
}

void search(std::int64_t test, const std::vector<int>& nums, std::vector<int>& ops) {
  if (ops.size() == nums.size() - 1) {
    std::int64_t res = nums[0];
    for (int i = 1; i < nums.size(); i++) {
      if (ops[i-1] == 0) {
        res += nums[i];
      } else if (ops[i-1] == 1) {
        res *= nums[i];
      } else {
        res = concat(res, nums[i]);
      }
    }
    if (res == test) {
      throw true;
    }
  } else {
    for (int i = 0; i < 3; i++) {
      ops.push_back(i);
      search(test, nums, ops);
      ops.pop_back();
    }
  }
}

bool is_valid(std::int64_t test, const std::vector<int>& nums) {
  std::vector<int> ops;
  ops.reserve(nums.size() - 1);
  try {
    search(test, nums, ops);
  } catch (...) {
    return true;
  }
  return false;
}

int main(int argc, char **argv) {
  std::ifstream input{argv[1]};

  std::int64_t res = 0;
  std::string line;
  while (std::getline(input, line)) {
    std::stringstream ss(line);

    std::int64_t num;
    char delim;
    ss >> num;
    ss >> delim;

    std::int64_t test = num;
    std::vector<int> nums;
    while (ss >> num) {
      nums.push_back(num);
    }

    bool valid = is_valid(test, nums);
    if (valid) {
      //std::cout << test << " valid" << std::endl;
      res += test;
    }
  }

  std::cout << res << std::endl;
}
