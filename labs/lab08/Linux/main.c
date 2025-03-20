#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_ACCOUNTS 3
#define NUM_ATMS 5
#define NUM_TRANSACTIONS 10

// Estructura para representar una cuenta bancaria
typedef struct {
    int balance;
    pthread_mutex_t lock;  // Mutex  saldo
} BankAccount;

BankAccount accounts[NUM_ACCOUNTS];
FILE *logFile;
pthread_mutex_t logLock; //mutex log

// Función para registrar transacciones en un archivo compartido
void logTransaction(const char *message) {
    pthread_mutex_lock(&logLock);
    time_t now = time(NULL);
    char timeStr[26];
    strftime(timeStr, sizeof(timeStr), "%a %b %d %H:%M:%S %Y", localtime(&now));
    fprintf(logFile, "[%s] %s\n", timeStr, message);
    fflush(logFile);
    pthread_mutex_unlock(&logLock);
}

// Función ejecutada por cada cajero automático (ATM)
void *atmFunction(void *arg) {
    int atm_id = *(int *)arg;
    for (int i = 0; i < NUM_TRANSACTIONS; i++) {
        int acc_id = rand() % NUM_ACCOUNTS;
        int amount = (rand() % 100) + 1;
        int transactionType = rand() % 3;  // 0 = Retiro, 1 = Depósito, 2 = Transferencia
        char logMsg[150];

        if (transactionType == 2) {  // Transferencia
            int dest_acc_id;
            do {
                dest_acc_id = rand() % NUM_ACCOUNTS;
            } while (dest_acc_id == acc_id);

            pthread_mutex_lock(&accounts[acc_id].lock);
            pthread_mutex_lock(&accounts[dest_acc_id].lock);

            if (accounts[acc_id].balance >= amount) {
                accounts[acc_id].balance -= amount;
                accounts[dest_acc_id].balance += amount;
                snprintf(logMsg, sizeof(logMsg), "ATM %d: Transferencia de $%d de Cuenta %d a Cuenta %d", atm_id, amount, acc_id, dest_acc_id);
            } else {
                snprintf(logMsg, sizeof(logMsg), "ATM %d: Transferencia Fallida - Fondos Insuficientes | Cuenta %d", atm_id, acc_id);
            }

            pthread_mutex_unlock(&accounts[dest_acc_id].lock);
            pthread_mutex_unlock(&accounts[acc_id].lock);
        } else {
            pthread_mutex_lock(&accounts[acc_id].lock);
            if (transactionType == 0) {  // Retiro
                if (accounts[acc_id].balance >= amount) {
                    accounts[acc_id].balance -= amount;
                    snprintf(logMsg, sizeof(logMsg), "ATM %d: Retiró $%d de Cuenta %d   | Nuevo Saldo: $%d", atm_id, amount, acc_id, accounts[acc_id].balance);
                } else {
                    snprintf(logMsg, sizeof(logMsg), "ATM %d: Transacción Fallida - Fondos Insuficientes | Cuenta %d", atm_id, acc_id);
                }
            } else {  // Depósito
                accounts[acc_id].balance += amount;
                snprintf(logMsg, sizeof(logMsg), "ATM %d: Depositó $%d en Cuenta %d  | Nuevo Saldo: $%d", atm_id, amount, acc_id, accounts[acc_id].balance);
            }
            pthread_mutex_unlock(&accounts[acc_id].lock);
        }

        logTransaction(logMsg);
        usleep(100000);  // Simulación de tiempo de procesamiento
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    logFile = fopen("transactions.log", "w");
    if (!logFile) {
        perror("Error al abrir el archivo de log");
        return 1;
    }
    
    pthread_mutex_init(&logLock, NULL);
    
    // Inicialización de cuentas
    printf("Saldo Inicial:\n");
    for (int i = 0; i < NUM_ACCOUNTS; i++) {
        accounts[i].balance = 1000;
        pthread_mutex_init(&accounts[i].lock, NULL);
        printf("Cuenta %d: $%d\n", i, accounts[i].balance);
    }

    pthread_t atms[NUM_ATMS];
    int atm_ids[NUM_ATMS];
    // Creación de hilos para cada ATM
    for (int i = 0; i < NUM_ATMS; i++) {
        atm_ids[i] = i;
        pthread_create(&atms[i], NULL, atmFunction, &atm_ids[i]);
    }

    // Esperar a que todos los hilos finalicen
    for (int i = 0; i < NUM_ATMS; i++) {
        pthread_join(atms[i], NULL);
    }

    // Imprimir saldo final de cada cuenta
    printf("\nSaldo Final:\n");
    for (int i = 0; i < NUM_ACCOUNTS; i++) {
        printf("Cuenta %d: $%d\n", i, accounts[i].balance);
        pthread_mutex_destroy(&accounts[i].lock);
    }

    pthread_mutex_destroy(&logLock);
    fclose(logFile);
    return 0;
}