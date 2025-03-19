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
    EnterCriticalSection(&account->lock);
    account->balance += money;
    LeaveCriticalSection(&account->lock);
    return account->balance;
}

int withdraw(BankAccount* account, int money){
    EnterCriticalSection(&account->lock);
    if(account->balance >= money){
        account->balance -= money;
        LeaveCriticalSection(&account->lock);
        return account->balance;   //Transacción exitosa
    }
    else{
        LeaveCriticalSection(&account->lock);
        return -1;   //Fondos insuficientes
    }
}

int transfer(BankAccount* accountDeparture, BankAccount* accountDestiny, int money){
    EnterCriticalSection(&accountDeparture->lock);
    if(accountDeparture->balance >= money){
        accountDeparture->balance -= money;
        EnterCriticalSection(&accountDestiny->lock);
        accountDestiny->balance += money;
        LeaveCriticalSection(&accountDestiny->lock);
        LeaveCriticalSection(&accountDeparture->lock);
        return 0;
    }
    //Fondos insuficientes
    else {
        LeaveCriticalSection(&accountDeparture->lock);
        return -1;
    }
    
}