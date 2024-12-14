#include <fstream>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <vector>

struct File {
  int id;
  int start;
  int size;
};

struct Filemap {
  std::vector<File> files;

  static Filemap parse(std::ifstream& input) {
    Filemap filemap{};
    std::string filemap_str;
    input >> filemap_str;
    int id = 0;
    int start = 0;
    for (int i = 0; i < filemap_str.size(); i++) {
      if (i % 2 == 0) {
        filemap.files.emplace_back(id++, start, filemap_str[i]-'0');
      } else {
        if (filemap_str[i] > '0') {
          filemap.files.emplace_back(-1, start, filemap_str[i]-'0');
        }
      }
      start += filemap_str[i]-'0';
    }
    return filemap;
  }

  std::string debug() const {
    std::string vis;
    for (const auto& f : files) {
      if (f.id == -1) {
        for (int i = 0; i < f.size; i++) {
          vis.push_back('.');
        }
      } else {
        for (int i = 0; i < f.size; i++) {
          vis.push_back(f.id + '0');
        }
      }
    }
    return vis;
  }

  void compress() {
    while (true) {
      while (files.back().id == -1) {
        files.erase(std::prev(files.end()));
      }

      // find free space in the front
      auto free_it = std::find_if(files.begin(), files.end(), [](const auto &f) { return f.id == -1;});
      if (free_it == files.end()) {
        return;
      }
      // find file at the back
      auto file_it = files.insert(free_it, {files.back().id, 0, 0});
      free_it++;
      // file fits in the free space
      if (free_it->size > files.back().size) {
        free_it->size -= files.back().size;
        file_it->size += files.back().size;
        // remove last
        files.erase(std::prev(files.end()));
      } else if (free_it->size == files.back().size) {
        file_it->size += files.back().size;
        free_it->size = 0;
        // remove last and free
        files.erase(std::prev(files.end()));
        files.erase(free_it);
      } else if (free_it->size < files.back().size) {
        files.back().size -= free_it->size;
        file_it->size = free_it->size;
        free_it->size = 0;
        files.erase(free_it);
      }
    }
  }


  void compress2() {
    auto rfiles_it = files.rbegin();
    while (true) {
      while (files.back().id == -1) {
        files.erase(std::prev(files.end()));
      }

      rfiles_it = std::find_if(rfiles_it, files.rend(), [](const auto &f) { return f.id != -1; });
      if (rfiles_it == files.rend()) {
        break;
      }
      auto free_search_end = std::next(rfiles_it).base();

      // find free space in the front that can fit this file
      auto free_it = std::find_if(files.begin(), free_search_end, [&](const auto &f) { return f.id == -1 && f.size >= rfiles_it->size;});
      if (free_it == free_search_end) {
        rfiles_it++;
        continue;
      }

      // file fits in the free space
      if (free_it->size > rfiles_it->size) {
        // add a new file
        auto file_it = files.insert(free_it, {rfiles_it->id, 0, 0});
        free_it++;
        rfiles_it--;

        free_it->size -= rfiles_it->size;
        file_it->size += rfiles_it->size;
        rfiles_it->id = -1;
      } else if (free_it->size == rfiles_it->size) {
        std::swap(free_it->id, rfiles_it->id);
      }
    }
  }
};

int main(int argc, char **argv) {
  std::ifstream input{argv[1]};
  Filemap filemap = Filemap::parse(input);

  //std::cout << filemap.debug() << std::endl;
  //filemap.compress();
  filemap.compress2();

  uint64_t res = 0;
  int pos = 0;
  for (const auto& f : filemap.files) {
    for (int i = 0; i < f.size; i++) {
      if (f.id != -1) {
        res += pos++ * f.id;
      } else {
        pos++;
      }
    }
  }

  std::cout << res << std::endl;
}
