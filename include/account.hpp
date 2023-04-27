#pragma once
/*
  bank.cpp
  this header defines basic classes
*/

#ifndef __ACCOUNT_H_
#define __ACCOUNT_H_

#include <exception>
#include <string>
#include <utility>

namespace bank {

class Account {
  typedef Account self_type;

protected:
  std::string _username = "";
  std::string _password = "";
  std::string _gender = "";
  std::string _telephone = "";
  std::string _ID_number = "";
  // useing long double instead of double to ensure the accuracy
  long double balance;

public:
  Account(std::string username, std::string password, std::string gender,
          std::string telephone, std::string ID_number, long double balance)
      : _username(std::move(username)), _password(std::move(password)),
        _gender(std::move(gender)), _telephone(std::move(telephone)),
        _ID_number(std::move(ID_number)), balance(balance) {}

  bool is_login(const std::string &username,
                const std::string &password) const noexcept;

  virtual const int get_sort_priority() const noexcept = 0;
  const long double get_balance() const noexcept;
  virtual const long double get_minumum_balance() const noexcept = 0;
  virtual const long double get_intresest_rate() const noexcept = 0;
  const std::string &get_username() const noexcept;
  const std::string &get_password() const noexcept;
  const std::string &get_gender() const noexcept;
  const std::string &get_telephone() const noexcept;
  const std::string &get_ID_number() const noexcept;

  // not allow user to deposit or withdraw a wrong amount of money which is less
  // than zero.
  struct moneyLessThanZero : public std::exception {
    const char *what() const noexcept {
      return "The money you deposit/withdraw must be greater than 0.";
    }
  };

  // not allow invalid balace in a account
  struct moneyInvalidInAccount : public std::exception {
    const char *what() const noexcept {
      return "You are trying to withdraw more money than what you are able to "
             "\ntake, please try again.";
    }
  };

  struct dayLessThanZero : public std::exception {
    const char *what() const noexcept {
      return "The number of days cannot be a negative number.";
    }
  };

  void deposit(const long double deposit_money_count);
  virtual void withdraw(const long double withdraw_money_count) = 0;
  virtual long double interest(const int days) const = 0;
  void account_increase_by_interest(const int days) noexcept;

  virtual ~Account() {}
};

class Debit : public Account {
  typedef Debit self_type;

protected:
  static constexpr long double minimum_balance = 0.0;
  static constexpr long double interest_rate = 0.00625;

public:
  Debit(std::string username, std::string password, std::string gender,
        std::string telephone, std::string ID_number, long double balance)
      : Account(std::move(username), std::move(password), std::move(gender),
                std::move(telephone), std::move(ID_number), balance) {}
  const int get_sort_priority() const noexcept { return 0; }
  const long double get_minumum_balance() const noexcept;
  const long double get_intresest_rate() const noexcept;
  void withdraw(const long double withdraw_money_count);
  long double interest(const int days) const;
};

class Compcard : public Account {
  typedef Compcard self_type;

protected:
  static constexpr long double minimum_balance = -7000;
  static constexpr long double interest_rate = 0.00425;

public:
  Compcard(std::string username, std::string password, std::string gender,
           std::string telephone, std::string ID_number, long double balance)
      : Account(std::move(username), std::move(password), std::move(gender),
                std::move(telephone), std::move(ID_number), balance) {}

  const int get_sort_priority() const noexcept { return 1; }
  const long double get_minumum_balance() const noexcept;
  const long double get_intresest_rate() const noexcept;
  void withdraw(const long double withdraw_money_count);
  long double interest(const int days) const;
};

class Credit : public Account {
  typedef Credit self_type;

protected:
  static constexpr long double interest_rate = 0.0;

public:
  Credit(std::string username, std::string password, std::string gender,
         std::string telephone, std::string ID_number, long double balance)
      : Account(std::move(username), std::move(password), std::move(gender),
                std::move(telephone), std::move(ID_number), balance) {}

  const long double get_intresest_rate() const noexcept;
  long double interest(const int days) const;
  virtual ~Credit() {}
};

class NormalCredit : public Credit {
  typedef NormalCredit self_type;

protected:
  static constexpr long double minimum_balance = -7000;

public:
  NormalCredit(std::string username, std::string password, std::string gender,
               std::string telephone, std::string ID_number,
               long double balance)
      : Credit(std::move(username), std::move(password), std::move(gender),
               std::move(telephone), std::move(ID_number), balance) {}
  const int get_sort_priority() const noexcept { return 2; }
  const long double get_minumum_balance() const noexcept;
  void withdraw(const long double withdraw_money_count);
};

class VIPCredit : public Credit {
  typedef VIPCredit self_type;

protected:
  static constexpr long double minimum_balance = -50000;

public:
  VIPCredit(std::string username, std::string password, std::string gender,
            std::string telephone, std::string ID_number, long double balance)
      : Credit(std::move(username), std::move(password), std::move(gender),
               std::move(telephone), std::move(ID_number), balance) {}
  const int get_sort_priority() const noexcept { return 3; }
  const long double get_minumum_balance() const noexcept;
  void withdraw(const long double withdraw_money_count);
};

class VVIPCredit : public Credit {
  typedef VVIPCredit self_type;

protected:
  static constexpr long double minimum_balance = -200000;

public:
  VVIPCredit(std::string username, std::string password, std::string gender,
             std::string telephone, std::string ID_number, long double balance)
      : Credit(std::move(username), std::move(password), std::move(gender),
               std::move(telephone), std::move(ID_number), balance) {}
  const int get_sort_priority() const noexcept { return 4; }
  const long double get_minumum_balance() const noexcept;
  void withdraw(const long double withdraw_money_count);
};

} // namespace bank
#endif // __ACCOUNT_H_