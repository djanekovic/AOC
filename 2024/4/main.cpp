#include <iostream>
#include <vector>
#include <string>
#include <fstream>

int main(int argc, char **argv) {
  std::ifstream input{argv[1]};

  std::vector<std::string> matrix;
  std::string line;
  int cnt = 0;
  while (input >> line) {
    matrix.push_back(line);
  }

#if PART2
  // M.S  S.S  M.M  S.M
  // .A.  .A.  .A.  .A.
  // M.S  M.M  S.S  S.M
  for (int i = 1; i < matrix.size()-1; i++) {
    for (int j = 1; j < matrix[i].size()-1; j++) {
      if (matrix[i][j] == 'A') {
        if (matrix[i-1][j-1] == 'M' && matrix[i-1][j+1] == 'S' && matrix[i+1][j-1] == 'M' && matrix[i+1][j+1] == 'S') {
          cnt++;
        } else if (matrix[i-1][j-1] == 'S' && matrix[i-1][j+1] == 'S' && matrix[i+1][j-1] == 'M' && matrix[i+1][j+1] == 'M') {
          cnt++;
        } else if (matrix[i-1][j-1] == 'M' && matrix[i-1][j+1] == 'M' && matrix[i+1][j-1] == 'S' && matrix[i+1][j+1] == 'S') {
          cnt++;
        } else if (matrix[i-1][j-1] == 'S' && matrix[i-1][j+1] == 'M' && matrix[i+1][j-1] == 'S' && matrix[i+1][j+1] == 'M') {
          cnt++;
        }
      }
    }
  }
#else
  for (int i = 0; i < matrix.size(); i++) {
    for (int j = 0; j < matrix[i].size(); j++) {
      if (matrix[i][j] == 'X') {
        // left right
        if (j+3 < matrix[i].size() && matrix[i][j+1] == 'M' && matrix[i][j+2] == 'A' && matrix[i][j+3] == 'S') {
          cnt++;
        }
        if (j-3 >= 0 && matrix[i][j-1] == 'M' && matrix[i][j-2] == 'A' && matrix[i][j-3] == 'S') {
          cnt++;
        }
        // updown
        if (i+3 < matrix.size() && matrix[i+1][j] == 'M' && matrix[i+2][j] == 'A' && matrix[i+3][j] == 'S') {
          cnt++;
        }
        if (i-3 >= 0 && matrix[i-1][j] == 'M' && matrix[i-2][j] == 'A' && matrix[i-3][j] == 'S') {
          cnt++;
        }
        // diagonals
        if (i+3 < matrix.size() && j+3 < matrix[i].size() && matrix[i+1][j+1] == 'M' && matrix[i+2][j+2] == 'A' && matrix[i+3][j+3] == 'S') {
          cnt++;
        }
        if (i+3 < matrix.size() && j-3 >= 0 && matrix[i+1][j-1] == 'M' && matrix[i+2][j-2] == 'A' && matrix[i+3][j-3] == 'S') {
          cnt++;
        }
        if (i-3 >= 0 && j-3 >= 0 && matrix[i-1][j-1] == 'M' && matrix[i-2][j-2] == 'A' && matrix[i-3][j-3] == 'S') {
          cnt++;
        }
        if (i-3 >= 0 && j+3 < matrix[i].size() && matrix[i-1][j+1] == 'M' && matrix[i-2][j+2] == 'A' && matrix[i-3][j+3] == 'S') {
          cnt++;
        }
      }
    }
  }

#endif

  std::cout << cnt << std::endl;
}
