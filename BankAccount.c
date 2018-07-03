#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUMTHRDS 2
float saldo = 10000.00;
pthread_t callThd[NUMTHRDS];
pthread_mutex_t mutexsum;
int deposito = 0, saque = 0;

void *balance(void *arg){
    pthread_mutex_lock(&mutexsum);
    int thread = arg;
    float valor;
    int operacao;
    int parar = 1;
    do{
        valor = 100 + rand()%401;
        operacao = rand()%2;
        if(deposito == 5 && saque < 5){
            operacao = 1;
        }
        if(deposito < 5 && saque == 5){
            operacao = 0;
        }
        if(operacao == 0){
	    printf("Thread número: %i\n", thread);
            printf("Operação: Depósito!\nValor: %.2f\nSaldo na Conta: %.2f\nExecutando operação...\n", valor, saldo);
            sleep(1);
            saldo += valor;
            printf("Novo saldo: %.2f\n\n", saldo);
            deposito++;
        }
        if(operacao == 1){
	    printf("Thread número: %i\n", thread);
            printf("Operação: Saque!\nValor: %.2f\nSaldo na Conta: %.2f\nExecutando operação...\n", valor, saldo);
            sleep(1);
            saldo -= valor;
            printf("Novo saldo: %.2f\n\n", saldo);
            saque++;
        }
        if(deposito == 5 && saque == 5){
            deposito = 0;
            saque = 0;
            parar = 0;
        }
    }while(parar == 1);

    pthread_mutex_unlock(&mutexsum);
}

int main (int argc, char *argv[]){
	pthread_attr_t attr;
    int i;

    pthread_mutex_init(&mutexsum, NULL);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    for(i = 0; i < NUMTHRDS; i++){
        pthread_create(&callThd[i], NULL, balance, (void*)i);
    }
    
    pthread_attr_destroy(&attr);

    void *status;
    for(i = 0; i < NUMTHRDS; i++){
        pthread_join(callThd[i], &status);
    }
}
