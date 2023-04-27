#include "../include/ATM_system.hpp"
#include <algorithm>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace bank {

// jump table to seperate different account types
// convenient for input and output
static const std::vector<std::string> jump_table{
    "Debit", "Compcard", "NormalCredit", "VIPCredit", "VVIPCredit",
};

static constexpr const char *slash() noexcept {
  return "----------------------------------------------\n";
}

#ifdef __FORMATTED_
#define __FORMATTED_TEMP_ __FORMATTED_
#undef __FORMATTED_
#endif
#define __FORMATTED_ std::fixed << std::showpoint << std::setprecision(2) <<

/**************************Internal Initialization**************************/
ATM::ATM() {
  for (const auto &row : file_stream.database) {
    if (row.at(0) == jump_table.at(0)) {
      accounts.push_back(
          std::make_shared<Debit>(row.at(1), row.at(2), row.at(3), row.at(4),
                                  row.at(5), std::stold(row.at(6))));
    } else if (row.at(0) == jump_table.at(1)) {
      accounts.push_back(
          std::make_shared<Compcard>(row.at(1), row.at(2), row.at(3), row.at(4),
                                     row.at(5), std::stold(row.at(6))));
    } else if (row.at(0) == jump_table.at(2)) {
      accounts.push_back(std::make_shared<NormalCredit>(
          row.at(1), row.at(2), row.at(3), row.at(4), row.at(5),
          std::stold(row.at(6))));
    } else if (row.at(0) == jump_table.at(3)) {
      accounts.push_back(std::make_shared<VIPCredit>(
          row.at(1), row.at(2), row.at(3), row.at(4), row.at(5),
          std::stold(row.at(6))));
    } else if (row.at(0) == jump_table.at(4)) {
      accounts.push_back(std::make_shared<VVIPCredit>(
          row.at(1), row.at(2), row.at(3), row.at(4), row.at(5),
          std::stold(row.at(6))));
    } else {
      throw accountTypeNotDefined();
    }
  }
  std::sort(accounts.begin(), accounts.end(),
            [](std::shared_ptr<Account> a, std::shared_ptr<Account> b) {
              return a.get()->get_username() < b.get()->get_username();
            });

  user_logged_in_at = accounts.end();
  save_as("database_temp.csv");
}

// main menu, telling user/admin what to do
void ATM::main_menu() noexcept {
  while (true) {
    std::cout << slash() << "Welcome to the ATM system!" << std::endl
              << slash() << std::endl
              << "1. Login as a User (if you are here to withdraw/deposit or"
              << std::endl
              << "check your balance)" << std::endl
              << "2. Login as an Administrator (admin name & password required)"
              << std::endl
              << "Any key else: Exit" << std::endl
              << std::endl
              << "Tips: If you want to create an account or delete your "
              << std::endl
              << "account permanantly, please contact us to help you."
              << std::endl
              << std::endl
              << slash() << "Choose your option: _____\b\b\b";

    char option;
    std::cin.get(option);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (option) {
    case '1':
      user_login_menu();
      break;
    case '2':
      admin_login_menu();
      break;
    default:
      return;
    }
  }
}

/**************************User Side Operations**************************/
// if you are a user, you will be told to input something like below:
void ATM::user_login_menu() noexcept {
  std::string username_input, password_input;

  // tell user to enter username and password
  std::cout << slash() << "Please enter your username: ";
  std::getline(std::cin, username_input);
  std::cout << "Please enter your password: ";
  std::getline(std::cin, password_input);

  if (!user_is_logged_in(username_input, password_input)) {
    std::cout << "Invalid username or password! Please try again." << std::endl;
    return;
  }
  user_menu();
}

// if logged in, do something below
void ATM::user_menu() noexcept {
  while (true) {
    std::cout << slash() << "Welcome to the ATM user subsystem!" << std::endl
              << slash() << std::endl
              << "1. Check your account information" << std::endl
              << "2. Deposit your money" << std::endl
              << "3. Withdraw your money" << std::endl
              << "4. Calculate your interest" << std::endl
              << "Any key else: Exit" << std::endl
              << std::endl
              << slash() << "Choose your option: _____\b\b\b";

    char option;
    std::cin.get(option);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (option) {
    case '1':
      check_info();
      break;
    case '2':
      user_deposit_menu();
      break;
    case '3':
      user_withdraw_menu();
      break;
    case '4':
      user_interest_calculation_menu();
      break;
    default:
      return;
    }
  }
}

void ATM::user_deposit_menu() noexcept {
INPUT_DEPOSIT:
  std::cout << slash() << "Your current balance is: "
            << __FORMATTED_ user_logged_in_at->get()->get_balance() << std::endl
            << "How much do you want to deposit? _______\b\b\b\b\b\b\b";
  long double money_input = 0.;
  std::cin >> money_input;
  while (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cerr << "Bad entry or the money your input is less than 0."
              << std::endl
              << "Please try again. _______\b\b\b\b\b\b\b";
    std::cin >> money_input;
  }
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  try {
    user_logged_in_at->get()->deposit(money_input);
    std::cout << "Deposit suceessful." << std::endl
              << "Your current balance is: "
              << __FORMATTED_ user_logged_in_at->get()->get_balance() << std::endl;
    std::cout << "Press Enter to continue.";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  } catch (const std::exception &err) {
    std::cerr << "Error: " << err.what() << std::endl;
    std::cout << "Please try again." << std::endl;
    goto INPUT_DEPOSIT;
  }
}

void ATM::user_withdraw_menu() noexcept {
INPUT_WITHDRAW:
  std::cout << slash() << "The minimum balance you can reach is: "
            << __FORMATTED_ user_logged_in_at->get()->get_minumum_balance()
            << std::endl
            << "The money you are able to withdraw is: "
            << __FORMATTED_ user_logged_in_at->get()->get_balance() -
                   user_logged_in_at->get()->get_minumum_balance()
            << std::endl
            << "Your current balance is: "
            << __FORMATTED_ user_logged_in_at->get()->get_balance() << std::endl
            << "How much do you want to withdraw? _______\b\b\b\b\b\b\b";

  long double money_input = 0.;
  std::cin >> money_input;
  while (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cerr << "Bad entry." << std::endl
              << "Please try again. _______\b\b\b\b\b\b\b";
    std::cin >> money_input;
  }
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  try {
    user_logged_in_at->get()->withdraw(money_input);
    std::cout << "Withdrawal suceessful." << std::endl
              << "Your current balance is: "
              << __FORMATTED_ user_logged_in_at->get()->get_balance() << std::endl;
    std::cout << "Press Enter to continue.";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  } catch (const std::exception &err) {
    std::cerr << "Input Error: " << err.what() << std::endl;
    std::cout << "Please try again." << std::endl;
    goto INPUT_WITHDRAW;
  }
}

void ATM::user_interest_calculation_menu() noexcept {
INPUT_INTEREST:
  std::cout << slash()
            << "How much days you want to input? _______\b\b\b\b\b\b\b";
  int input_days;
  std::cin >> input_days;
  while (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cerr << "Bad entry." << std::endl
              << "Please try again. _______\b\b\b\b\b\b\b";
    std::cin >> input_days;
  }
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  try {
    std::cout << "Your interest is: "
              << __FORMATTED_ user_logged_in_at->get()->interest(input_days)
              << std::endl;
    std::cout << "Press Enter to continue.";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  } catch (const std::exception &err) {
    std::cerr << "Input Error: " << err.what() << std::endl;
    std::cout << "Please try again." << std::endl;
    goto INPUT_INTEREST;
  }
}

// to check if OK to login
bool ATM::user_is_logged_in(const std::string &username,
                            const std::string &password) noexcept {
  for (auto it = accounts.begin(); it != accounts.end(); it++) {
    if (it->get()->is_login(username, password)) {
      user_logged_in_at = it;
      return true;
    }
  }
  return false;
}

// print out the user info
void ATM::check_info() const noexcept {
  std::cout << slash() << std::endl
            << "Username: " << user_logged_in_at->get()->get_username()
            << std::endl
            << "Password: " << user_logged_in_at->get()->get_password()
            << std::endl
            << "Gender: " << user_logged_in_at->get()->get_gender() << std::endl
            << "Telephone #: " << user_logged_in_at->get()->get_telephone()
            << std::endl
            << "ID #: " << user_logged_in_at->get()->get_ID_number()
            << std::endl
            << "Current Balance: "
            << __FORMATTED_ user_logged_in_at->get()->get_balance() << std::endl
            << std::endl
            << slash();
  std::cout << "Press Enter to continue.";
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**************************Admin Side Operations**************************/
void ATM::admin_login_menu() noexcept {
  std::string username_input, password_input;

  // tell an administrator to enter username and password
  std::cout << slash() << "Please enter your username: ";
  std::getline(std::cin, username_input);
  std::cout << "Please enter your password: ";
  std::getline(std::cin, password_input);

  if (!admin_is_logged_in(username_input, password_input)) {
    std::cout << "Invalid username or password! Please try again." << std::endl;
    return;
  }
  admin_menu();
}

// if logged in successfully
void ATM::admin_menu() noexcept {
  while (true) {
    std::cout << slash() << "Welcome to the ATM Admin subsystem!" << std::endl
              << slash() << std::endl
              << "1. List of all accounts" << std::endl
              << "2. Create a new account" << std::endl
              << "3. Erase an account and transfer/withdraw all money."
              << std::endl
              << "Any key else: Exit" << std::endl
              << std::endl
              << slash() << "Choose your option: _____\b\b\b";

    char option;
    std::cin.get(option);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (option) {
    case '1':
      admin_check_all_info();
      break;
    case '2':
      admin_add_account_menu();
      break;
    case '3':
      admin_remove_account_menu();
      break;
    default:
      return;
    }
  }
}

void ATM::admin_check_all_info() noexcept {
  std::cout << slash();
  if (accounts.empty()) {
    std::cout << "No accounts!" << std::endl;
    return;
  }
  std::cout << '|' << std::left << std::setw(3) << '#' << '|' << std::left
            << std::setw(15) << "Username" << '|' << std::left << std::setw(15)
            << "Password" << '|' << std::left << std::setw(15) << "Account Type"
            << '|' << std::left << std::setw(7) << "Gender" << '|' << std::left
            << std::setw(15) << "Telephone" << '|' << std::left << std::setw(20)
            << "ID number" << '|' << std::left << std::setw(12) << "Balance"
            << '|' << std::endl;
  for (auto it = accounts.begin(); it != accounts.end(); ++it) {
    std::cout << '|' << std::left << std::setw(3) << it - accounts.begin()
              << '|' << std::left << std::setw(15) << it->get()->get_username()
              << '|' << std::left << std::setw(15) << it->get()->get_password()
              << '|' << std::left << std::setw(15)
              << jump_table.at(it->get()->get_sort_priority());
    std::cout << '|' << std::left << std::setw(7) << it->get()->get_gender()
              << '|' << std::left << std::setw(15) << it->get()->get_telephone()
              << '|' << std::left << std::setw(20) << it->get()->get_ID_number()
              << '|' << std::left << std::setw(12)
              << __FORMATTED_ it->get()->get_balance() << '|' << std::endl;
  }
  std::cout << slash();
  std::cout << "Press Enter to continue.";
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void ATM::admin_add_account_menu() noexcept {
INPUT_ADD_ACCOUNT:
  std::string account_type, username, password, gender, telephone, ID_number;
  std::cout << "If you want to create an account: " << std::endl;
  std::cout << "1. Enter the type of the "
               "account:_______________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
  std::getline(std::cin, account_type);
  std::cout
      << "2. Enter the username:_______________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
  std::getline(std::cin, username);
  std::cout
      << "3. Enter the password:_______________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
  std::getline(std::cin, password);
  std::cout
      << "4. Enter the gender:_______________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
  std::getline(std::cin, gender);
  std::cout << "5. Enter the telephone "
               "number:_______________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
  std::getline(std::cin, telephone);
  std::cout
      << "6. Enter the ID number:_______________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
  std::getline(std::cin, ID_number);

  try {
    add_account(account_type, username, password, gender, telephone, ID_number);
    std::cout << "Account has been added successfully." << std::endl;
    std::cout << "Press Enter to continue.";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  } catch (const std::exception &err) {
    std::cerr << "Error: " << err.what() << std::endl;
    std::cout << "Please try again." << std::endl;
    goto INPUT_ADD_ACCOUNT;
  }
}

void ATM::admin_remove_account_menu() noexcept {
INPUT_REMOVE_ACCOUNT:
  std::cout << "What account do you want to remove? ____\b\b\b\b";

  int index;
  std::cin >> index;
  while (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cerr << "Bad entry." << std::endl << "Please try again. ____\b\b\b\b";
    std::cin >> index;
  }
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  try {
    remove_account(accounts.begin() + index);
    std::cout << "Account has been removed successfully." << std::endl;
    std::cout << "Press Enter to continue.";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  } catch (const std::exception &err) {
    std::cerr << "Error: " << err.what() << std::endl;
    std::cout << "Please try again." << std::endl;
    goto INPUT_REMOVE_ACCOUNT;
  }
}

ATM &ATM::add_account(std::string account_type, std::string username,
                      std::string password, std::string gender,
                      std::string telephone, std::string ID_number) {
  if (account_type == jump_table.at(0)) {
    accounts.push_back(std::make_shared<Debit>(username, password, gender,
                                               telephone, ID_number, 0));
  } else if (account_type == jump_table.at(1)) {
    accounts.push_back(std::make_shared<Compcard>(username, password, gender,
                                                  telephone, ID_number, 0));
  } else if (account_type == jump_table.at(2)) {
    accounts.push_back(std::make_shared<NormalCredit>(
        username, password, gender, telephone, ID_number, 0));
  } else if (account_type == jump_table.at(3)) {
    accounts.push_back(std::make_shared<VIPCredit>(username, password, gender,
                                                   telephone, ID_number, 0));
  } else if (account_type == jump_table.at(4)) {
    accounts.push_back(std::make_shared<VVIPCredit>(username, password, gender,
                                                    telephone, ID_number, 0));
  } else {
    throw accountTypeNotDefined();
  }
  std::sort(accounts.begin(), accounts.end(),
            [](std::shared_ptr<Account> a, std::shared_ptr<Account> b) {
              return a.get()->get_username() < b.get()->get_username();
            });

  return *this;
}

ATM &ATM::remove_account(std::vector<std::shared_ptr<Account>>::iterator pos) {
  accounts.erase(pos);
  return *this;
}

// if you are an admin, you can use this function
bool ATM::admin_is_logged_in(const std::string &username,
                             const std::string &password) const noexcept {
  if (admin_username == username && admin_password == password) {
    return true;
  }
  return false;
}

void ATM::read_as(std::string file_name) {

  // clear original data (in"database.csv") first
  accounts.clear();
  io::file_read in_file(file_name);

  for (const auto &row : in_file.database) {
    if (row.at(0) == jump_table.at(0)) {
      accounts.push_back(
          std::make_shared<Debit>(row.at(1), row.at(2), row.at(3), row.at(4),
                                  row.at(5), std::stold(row.at(6))));
    } else if (row.at(0) == jump_table.at(1)) {
      accounts.push_back(
          std::make_shared<Compcard>(row.at(1), row.at(2), row.at(3), row.at(4),
                                     row.at(5), std::stold(row.at(6))));
    } else if (row.at(0) == jump_table.at(2)) {
      accounts.push_back(std::make_shared<NormalCredit>(
          row.at(1), row.at(2), row.at(3), row.at(4), row.at(5),
          std::stold(row.at(6))));
    } else if (row.at(0) == jump_table.at(3)) {
      accounts.push_back(std::make_shared<VIPCredit>(
          row.at(1), row.at(2), row.at(3), row.at(4), row.at(5),
          std::stold(row.at(6))));
    } else if (row.at(0) == jump_table.at(4)) {
      accounts.push_back(std::make_shared<VVIPCredit>(
          row.at(1), row.at(2), row.at(3), row.at(4), row.at(5),
          std::stold(row.at(6))));
    } else {
      throw accountTypeNotDefined();
    }
  }
  std::sort(accounts.begin(), accounts.end(),
            [](std::shared_ptr<Account> a, std::shared_ptr<Account> b) {
              return a.get()->get_username() < b.get()->get_username();
            });
}

// write files
#ifdef __COMMA_
#define __COMMA_TEMP_ __COMMA_
#undef __COMMA_
#endif
#define __COMMA_ ',' <<
void ATM::save() noexcept {
  file_stream.out_file
      << "Account_type,username,password,gender,telephone,ID_number,balance"
      << std::endl;

  for (const auto &account : accounts) {
    file_stream.out_file << jump_table.at(account->get_sort_priority())
                         << __COMMA_ account->get_username()
                         << __COMMA_ account->get_password()
                         << __COMMA_ account->get_gender()
                         << __COMMA_ account->get_telephone()
                         << __COMMA_ account->get_ID_number()
                         << __COMMA_ __FORMATTED_ account->get_balance()
                         << std::endl;
  }
}
ATM::~ATM() { save(); }

void ATM::save_as(std::string file_name) const noexcept {
  std::ofstream out_file(file_name, std::ios::out | std::ios::trunc);
  out_file
      << "Account_type,username,password,gender,telephone,ID_number,balance"
      << std::endl;

  for (const auto &account : accounts) {
    out_file << jump_table.at(account->get_sort_priority())
             << __COMMA_ account->get_username()
             << __COMMA_ account->get_password()
             << __COMMA_ account->get_gender()
             << __COMMA_ account->get_telephone()
             << __COMMA_ account->get_ID_number()
             << __COMMA_ __FORMATTED_ account->get_balance() << std::endl;
  }
}
#undef __COMMA_
#ifdef __COMMA_TEMP_
#define __COMMA_ __COMMA_TEMP_
#undef __COMMA_TEMP_
#endif
#undef __FORMATTED_
#ifdef __FORMATTED_TEMP_
#define __FORMATTED_ __FORMATTED_TEMP_
#undef __FORMATTED_TEMP_
#endif

} // namespace bank