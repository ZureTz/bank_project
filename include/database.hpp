#pragma once
/*
  this header gives the class used to input and output files
  using fstream with the support of database
*/

#ifndef __DATABASE_H_
#define __DATABASE_H_

#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace bank {
namespace io {

// if file not found:
struct fileNotFound : public std::exception {
  const char *what() const noexcept {
    return ("Error: File is not open/not found.");
  }
};

// reading files
class file_read {
protected:
  std::ifstream in_file;

public:
  // database means all data the database.csv include.
  std::vector<std::vector<std::string>> database;
  file_read(std::string str = "database.csv");
  ~file_read() noexcept;
};

// writing files
class file_wirte {
public:
  std::ofstream out_file;
  file_wirte(std::string str = "database.csv");
  ~file_wirte() noexcept;
};

// combination to support both input and output
class file_input_and_output : public file_read, public file_wirte {
public:
  file_input_and_output(std::string instr = "database.csv",
                        std::string outstr = "database.csv")
      : file_read(std::move(instr)), file_wirte(std::move(outstr)) {}
};

} // namespace io
} // namespace bank
#endif // __DATABASE_H_