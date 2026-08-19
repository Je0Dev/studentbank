#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "account.h"

int tx_deposit(Account *acc, double amount);
int tx_withdraw(Account *acc, double amount);
double tx_balance(const Account *acc);
int tx_valid_amount(double amount);

#endif