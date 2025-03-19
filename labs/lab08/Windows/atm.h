#ifndef ATM_H
#define ATM_H

#include "windows.h"

typedef struct {
    int balance;
    CRITICAL_SECTION lock;
    int accountID;
} BankAccount;

void InitializeBankAccount(BankAccount* account, int accountID, int balance);
void DeleteBankAccount(BankAccount* account);
int deposit(BankAccount* account, int money);
int withdraw(BankAccount* account, int money);
int transfer(BankAccount* accountDeparture, BankAccount* accountDestiny, int money);
#endif