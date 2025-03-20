#include "windows.h"
#include "atm.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ACCOUNTS 5
#define NUM_ATMS 3

BankAccount accounts[NUM_ACCOUNTS];
CRITICAL_SECTION logLock;
int initialBalance = 0;

void WriteToLog(const char* message){
    EnterCriticalSection(&logLock);
    FILE* logFile = fopen("transactions.log", "a"); //Abrir archivo en modo append
    if (logFile == NULL){
        printf("Error al abrir el archivo transactions.log\n");
        LeaveCriticalSection(&logLock);
        return;
    }
    fprintf(logFile, "%s\n", message);  //Escribir log al archivo
    fclose(logFile);    //Cerrar archivo
    LeaveCriticalSection(&logLock);
}

DWORD WINAPI ATMFunction(LPVOID lpParam){
    srand(GetCurrentThreadId()); //Seed para generar números aleatorios
    int numTransactions = 10;    //Número de transacciones que realiza cada cajero
    for(int i=0; i<numTransactions; i++){
        int accountIndex = rand() % NUM_ACCOUNTS;       //Elegir una cuenta bancaria al azar
        int money = (rand() %100) + 1;                  //Producir cuna cantidad de dinero aleatoria
        int transactionType = rand() % 3;                  //Escoger entre depósito o retiro

        char logMessage[150];  //Buffer para log

        //Crear Timestamp
        time_t now = time(NULL);
        struct tm* timeinfo = localtime(&now);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
        //Depósito
        if(transactionType == 0){
            int balance = deposit(&accounts[accountIndex], money);
            snprintf(logMessage, sizeof(logMessage), "[%s] Deposited $%d.00 to Account %d | New Balance: $%d.00", timestamp, money, accountIndex, balance);
        }
        //Retiro
        else if(transactionType == 1){
            int balance = withdraw(&accounts[accountIndex], money);
            if(balance >= 0){
                snprintf(logMessage, sizeof(logMessage), "[%s] Withdrew $%d.00 from Account %d | New Balance: $%d.00", timestamp, money, accountIndex, balance);
            }
            else{
                snprintf(logMessage, sizeof(logMessage), "[%s] Transaction Withdrawal Failed: Insufficient funds | Account %d", timestamp, accountIndex);
            }
        }
        //Transferencia
        else if(transactionType == 2){
            //Elegir una cuenta bancaria al azar, diferente a la cuenta 'sender'
            int accountDestinyIndex = rand() % NUM_ACCOUNTS;       //Elegir una cuenta bancaria al azar
            while(accountDestinyIndex == accountIndex){
                int accountDestinyIndex = rand() % NUM_ACCOUNTS;       //Elegir una cuenta bancaria al azar
            }
            int result = transfer(&accounts[accountIndex], &accounts[accountDestinyIndex], money);
            if(result >= 0){
                snprintf(logMessage, sizeof(logMessage), "[%s] Transferred $%d.00 from Account %d to Account %d", timestamp, money, accountIndex, accountDestinyIndex);
            }else{
                snprintf(logMessage, sizeof(logMessage), "[%s] Transaction Transfernce Failed: Insufficient funds | Account %d", timestamp, accountIndex);
            }
        }
        WriteToLog(logMessage);
        //int processTime = 100; //Tiempo aleatorio para simular tiempos de carga diferentes entre transacciones
        //Sleep(processTime); //Simular tiempo de procesamiento
    }
    return 0;
}

int main(){

    //Inicializar lock para el archivo log
    InitializeCriticalSection(&logLock);

    HANDLE atmThreads[NUM_ATMS];
    //Inicializar cuentas
    for (int i=0; i<NUM_ACCOUNTS; i++){
        InitializeBankAccount(&accounts[i], i+1 , initialBalance);
    }

    //Crear ATMs (Threads)
    for(int i=0; i<NUM_ATMS; i++){
        atmThreads[i] = CreateThread(NULL, 0, ATMFunction, NULL, 0, NULL);
        if(atmThreads[i] == NULL){
            printf("Error al crear el ATM %d\n", i);
            return 1;
        }
    }

    //Join
    WaitForMultipleObjects(NUM_ATMS, atmThreads, TRUE, 5000);

    char logMessage[150];  //Buffer para log
    //Desplegar balance total
    snprintf(logMessage, sizeof(logMessage), "\n----------Total Balances----------\n");
        WriteToLog(logMessage);
    for (int i = 0; i < NUM_ACCOUNTS; i++) {
        snprintf(logMessage, sizeof(logMessage), "Account %d total Balance: $%d.00", accounts[i].accountID, accounts[i].balance);
        WriteToLog(logMessage);
    }

    //Eliminar Mutex creados
    for(int i = 0; i<NUM_ACCOUNTS; i++){
        DeleteBankAccount(&accounts[i]);
    }

    //Eliminar lock para archivo log
    DeleteCriticalSection(&logLock);

    return 0;

}
    