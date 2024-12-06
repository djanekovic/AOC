#include <iostream>
#include <algorithm>
#include <tuple>
#include <fstream>
#include <vector>

enum class Direction {
  UP, DOWN, LEFT, RIGHT
};

struct Guard {
  int row;
  int col;
  Direction direction;

  std::tuple<bool, int, int> walk(std::vector<char>& grid, int grid_width, int grid_height) {
    int steps = 0;
    int distance = 0;
    switch(direction) {
      case Direction::UP:
        for (int i = row-1; i >= 0; i--) {
          auto& v = grid.at(i * grid_width + col);
          if (v == '#') {
            row = i+1;
            direction = Direction::RIGHT;
            return std::make_tuple(true, steps, distance);
          }
          if (v != 'X') {
            v = 'X';
            steps++;
          }
          distance++;
        }
        break;
      case Direction::DOWN:
        for (int i = row+1; i < grid_height; i++) {
          auto& v = grid.at(i * grid_width + col);
          if (v == '#') {
            row = i-1;
            direction = Direction::LEFT;
            return std::make_tuple(true, steps, distance);
          }

          if (v != 'X') {
            v = 'X';
            steps++;
          }
          distance++;
        }
        break;
      case Direction::LEFT:
        for (int i = col-1; i >= 0; i--) {
          auto& v = grid.at(row * grid_width + i);
          if (v == '#') {
            col = i+1;
            direction = Direction::UP;
            return std::make_tuple(true, steps, distance);
          }

          if (v != 'X') {
            v = 'X';
            steps++;
          }
          distance++;
        }
        break;
      case Direction::RIGHT:
        for (int i = col+1; i < grid_width; i++) {
          auto& v = grid.at(row * grid_width + i);
          if (v == '#') {
            col = i-1;
            direction = Direction::DOWN;
            return std::make_tuple(true, steps, distance);
          }
          if (v != 'X') {
            v = 'X';
            steps++;
          }
          distance++;
        }
        break;
    }

    return {false, steps, distance};
  }
};

bool operator==(const Guard& a, const Guard& b) {
  return a.col == b.col && a.direction == b.direction && a.row == b.row;
}

void print_state(const Guard& g, const std::vector<char>& grid, int grid_width, int grid_height) {
  std::cout << g.row << " " << g.col << " " << static_cast<int>(g.direction) << std::endl;
  for (int i = 0; i < grid_height; i++) {
    for (int j = 0; j < grid_width; j++) {
      std::cout << grid.at(i * grid_width + j);
    }
    std::cout << std::endl;
  }
}

int main(int argc, char **argv) {
  std::ifstream input{argv[1]};

  std::vector<char> grid;
  int grid_width = 0;
  int grid_height = 0;
  Guard guard;
  {
    std::string line;
    while (input >> line) {
      grid_width = line.size();
      for (int col = 0; col < line.size(); col++) {
        if (line[col] == '^') {
            guard = {grid_height, col, Direction::UP};
            line[col] = 'X';
        }
        grid.push_back(line[col]);
      }
      grid_height++;
    }
  }

  auto empty_grid = grid;
  auto start_guard = guard;
  int steps = 1;
  for (;;) {
    const auto [found, steps_made, distance] = guard.walk(grid, grid_width, grid_height);
    steps += steps_made;
    if (!found) {
      break;
    }
  }

  // extract all potential positions for new obstacle
  std::vector<int> step_locations;
  step_locations.reserve(steps);
  for (int i = 0; i < grid_height; i++) {
    for (int j = 0; j < grid_width; j++) {
      if (i == start_guard.row && j == start_guard.col) {
        continue;
      }
      if (grid[i * grid_width + j] == 'X') {
        step_locations.push_back(i * grid_width + j);
      }
    }
  }

  int loop_counter = 0;
  for (auto obstacle_candidate : step_locations) {
    grid = empty_grid;
    guard = start_guard;
    grid[obstacle_candidate] = '#';

    //print_state(guard, grid, grid_width, grid_height);

    std::vector<Guard> position_buffer{guard};
    for (int position_idx = 0;; position_idx++) {
      //print_state(guard, grid, grid_width, grid_height);
      const auto [found, steps_made, distance] = guard.walk(grid, grid_width, grid_height);
      if (!found) {
        break;
      }

      auto it = std::find(position_buffer.begin(), position_buffer.end(), guard);
      if (it != position_buffer.end()) {
        loop_counter++;
        break;
      }
      position_buffer.push_back(guard);
    }
  }

  std::cout << loop_counter << std::endl;
}
