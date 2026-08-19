#include "transaction.h"

#include "validation.h"

int tx_valid_amount(double amount) {
    return amount > 0;
}

int tx_deposit(Account *acc, double amount) {
    if (!acc || !tx_valid_amount(amount)) return 0;
    acc->balance += amount;
    return 1;
}

int tx_withdraw(Account *acc, double amount) {
    if (!acc || !tx_valid_amount(amount)) return 0;
    if (amount > acc->balance) return 0;
    acc->balance -= amount;
    return 1;
}

double tx_balance(const Account *acc) {
    return acc ? acc->balance : 0.0;
}