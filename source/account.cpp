#include "../include/account.hpp"

namespace bank {

// in class Account
bool Account::is_login(const std::string &username,
                       const std::string &password) const noexcept {
  if (_username == username && _password == password) {
    return true;
  }
  return false;
}

const long double Account::get_balance() const noexcept { return balance; }
const std::string &Account::get_username() const noexcept { return _username; }
const std::string &Account::get_password() const noexcept { return _password; }
const std::string &Account::get_gender() const noexcept { return _gender; }
const std::string &Account::get_telephone() const noexcept {
  return _telephone;
}
const std::string &Account::get_ID_number() const noexcept {
  return _ID_number;
}

void Account::deposit(const long double deposit_money_count) {
  if (deposit_money_count < 0) {
    throw moneyLessThanZero();
  }
  balance += deposit_money_count;
}

// virtual function (deprecated, it is pure virtual)
// void Account::withdraw(long double withdraw_money_count) {
//   if (withdraw_money_count < 0) {
//     throw moneyLessThanZero();
//   }
//   balance -= withdraw_money_count;
// }

void Account::account_increase_by_interest(const int days) noexcept {
  balance += interest(days);
}

/* in class Debit--->Account */
void Debit::withdraw(const long double withdraw_money_count) {
  if (withdraw_money_count < 0) {
    throw moneyLessThanZero();
  }
  if (get_balance() - withdraw_money_count < minimum_balance) {
    throw moneyInvalidInAccount();
  }
  balance -= withdraw_money_count;
}

const long double Debit::get_minumum_balance() const noexcept {
  return minimum_balance;
}
const long double Debit::get_intresest_rate() const noexcept {
  return interest_rate;
}

long double Debit::interest(const int days) const {
  if (days < 0) {
    throw dayLessThanZero();
  }
  if (days == 0) {
    return 0;
  }
  long double times = 1.0 + interest_rate;
  long double balanced = balance;
  for (int i = 0; i < days; i++) {
    balanced *= times;
  }
  return balanced - balance;
}

/* in class Compcard--->Account */
void Compcard::withdraw(const long double withdraw_money_count) {
  if (withdraw_money_count < 0) {
    throw moneyLessThanZero();
  }
  if (get_balance() - withdraw_money_count < minimum_balance) {
    throw moneyInvalidInAccount();
  }
  balance -= withdraw_money_count;
}

const long double Compcard::get_minumum_balance() const noexcept {
  return minimum_balance;
}
const long double Compcard::get_intresest_rate() const noexcept {
  return interest_rate;
}

long double Compcard::interest(const int days) const {
  if (days < 0) {
    throw dayLessThanZero();
  }
  if (days == 0) {
    return 0;
  }
  long double times = 1.0 + interest_rate;
  long double balanced = balance;
  for (int i = 0; i < days; i++) {
    balanced *= times;
  }
  return balanced - balance;
}

/* in class Credit--->Account */
long double Credit::interest(const int days) const {
  if (days < 0) {
    throw dayLessThanZero();
  }
  return 0;
}

const long double Credit::get_intresest_rate() const noexcept {
  return interest_rate;
}

/* in class NormalCredit--->Credit--->Account */
void NormalCredit::withdraw(const long double withdraw_money_count) {
  if (withdraw_money_count < 0) {
    throw moneyLessThanZero();
  }
  if (get_balance() - withdraw_money_count < minimum_balance) {
    throw moneyInvalidInAccount();
  }
  balance -= withdraw_money_count;
}

const long double NormalCredit::get_minumum_balance() const noexcept {
  return minimum_balance;
}

/* in class VIPCredit--->Credit--->Account */
void VIPCredit::withdraw(const long double withdraw_money_count) {
  if (withdraw_money_count < 0) {
    throw moneyLessThanZero();
  }
  if (get_balance() - withdraw_money_count < minimum_balance) {
    throw moneyInvalidInAccount();
  }
  balance -= withdraw_money_count;
}

const long double VIPCredit::get_minumum_balance() const noexcept {
  return minimum_balance;
}

/* in class VVIPCredit--->Credit--->Account */
void VVIPCredit::withdraw(const long double withdraw_money_count) {
  if (withdraw_money_count < 0) {
    throw moneyLessThanZero();
  }
  if (get_balance() - withdraw_money_count < minimum_balance) {
    throw moneyInvalidInAccount();
  }
  balance -= withdraw_money_count;
}

const long double VVIPCredit::get_minumum_balance() const noexcept {
  return minimum_balance;
}

} // namespace bank