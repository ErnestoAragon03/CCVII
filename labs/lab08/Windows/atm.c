#include "windows.h"
#include "atm.h"

void InitializeBankAccount(BankAccount* account, int accountID, int initialBalance){
    account->balance = initialBalance;
    account->accountID = accountID;
    InitializeCriticalSection(&account->lock);
}

void DeleteBankAccount(BankAccount* account){
    DeleteCriticalSection(&account->lock);
}

int deposit(BankAccount* account, int money){
    int balance;
    EnterCriticalSection(&account->lock);
    account->balance += money;
    balance = account->balance;
    LeaveCriticalSection(&account->lock);
    return balance;
}

int withdraw(BankAccount* account, int money){
    int balance;
    EnterCriticalSection(&account->lock);
    if(account->balance >= money){
        account->balance -= money;
        balance = account->balance;
        LeaveCriticalSection(&account->lock);
        return balance;   //Transacción exitosa
    }
    else{
        LeaveCriticalSection(&account->lock);
        return -1;   //Fondos insuficientes
    }
}

int transfer(BankAccount* accountDeparture, BankAccount* accountDestiny, int money){
    EnterCriticalSection(&accountDeparture->lock);
    EnterCriticalSection(&accountDestiny->lock);
    int result = 1; //0: Transferencia exitosa 1: Transferencia fallida
    if(accountDeparture->balance >= money){
        accountDeparture->balance -= money;
        accountDestiny->balance += money;
        result = 0;
    }
    LeaveCriticalSection(&accountDestiny->lock);
    LeaveCriticalSection(&accountDeparture->lock);
    return result;
    
}