#pragma once
/*
  ATM System is a simple
  "operating system"
  acting as an simple ATM Terminal Machine
*/

#ifndef __ATM_SYS_H_
#define __ATM_SYS_H_

#include "account.hpp"
#include "database.hpp"
#include <exception>
#include <memory>
#include <string>
#include <vector>

namespace bank {
class ATM {
  typedef io::file_input_and_output file_io;
  typedef ATM self_type;

private:
  const std::string admin_username = "admin";
  const std::string admin_password = "1337Code";
  file_io file_stream;

  // a united interface
  std::vector<std::shared_ptr<Account>> accounts;
  // an iterator to tell where user is
  std::vector<std::shared_ptr<Account>>::iterator user_logged_in_at;

public:
  // if there was an undefined account type...
  struct accountTypeNotDefined : public std::exception {
    const char *what() const noexcept { return "Undefined account type."; }
  };

  /* Initialization and* menus */
  ATM();                // input
  void save() noexcept; // output
  ~ATM();
  void main_menu() noexcept;

  // what a user can do
  void user_login_menu() noexcept;
  void user_menu() noexcept;
  void user_deposit_menu() noexcept;
  void user_withdraw_menu() noexcept;
  void user_interest_calculation_menu() noexcept;

  // what an administrator can do
  void admin_login_menu() noexcept;
  void admin_menu() noexcept;
  void admin_check_all_info() noexcept;
  void admin_add_account_menu() noexcept;
  void admin_remove_account_menu() noexcept;

private:
  /*********User Operations*********/

  // to check if a user logged in
  bool user_is_logged_in(const std::string &username,
                         const std::string &password) noexcept;

  // print out the user info
  void check_info() const noexcept;

  /*********Admin Operations*********/
  // to check if an admin is logged in
  bool admin_is_logged_in(const std::string &username,
                          const std::string &password) const noexcept;
  self_type &add_account(std::string account_type, std::string username,
                         std::string password, std::string gender,
                         std::string telephone, std::string ID_number);
  self_type &
  remove_account(std::vector<std::shared_ptr<Account>>::iterator pos);

  // no "read()" because the ATM autpmatically reads data from a given data
  // name(as an augument) while initialization
  void read_as(std::string file_name);

  // save as "file_name".csv
  void save_as(std::string file_name) const noexcept;

  // friend class io::file_input_and_output;
}; // class ATM
} // namespace bank
#endif // __ATM_SYS_H_