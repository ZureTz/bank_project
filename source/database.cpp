#include "../include/database.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace bank {

namespace io {
// in class file_read
file_read::file_read(std::string file_name) {
  in_file.open(std::move(file_name), std::ios::in);
  if (!in_file.is_open()) {
    throw fileNotFound();
  }

  // row means a original row inputed from a .csv file
  std::vector<std::string> row;
  // in_line means a vector of strings, containing formmated info
  // word means every word
  std::string in_line, word;

  // ignore the first row
  std::getline(in_file, in_line);
  in_line.clear();

  while (std::getline(in_file, in_line)) {
    row.clear();
    std::stringstream str(in_line);
    while (std::getline(str, word, ',')) {
      row.push_back(word);
    }
    // accounts.push_back(Account(row.at(0),row.at(1),row.at(2),row.at(3),row.at(4)));
    database.push_back(row);
  }
// debugger, to know if successful
#if 0
    for (const auto & row : database) {
      for (const auto & column : row) {
        std::cout << column << " ";
      }
      std::cout << std::endl;
    }
#endif
}

file_read::~file_read() noexcept { in_file.close(); }

// in class file_write
file_wirte::file_wirte(std::string str) {
  out_file.open(std::move(str), std::ios::out | std::ios::trunc);
  if (!out_file.is_open()) {
    throw fileNotFound();
  }
}
file_wirte::~file_wirte() noexcept { out_file.close(); }

} // namespace io

} // namespace bank