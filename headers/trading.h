#ifndef TRADING_H
#define TRADING_H
#include <stdint.h>
#include "../../sandbox/linear-ds/adaptive-queue/headers/queue.h"
#include "../../sandbox/linear-ds/linked-list/headers/slist.h"

#define MAX_COMPANY_TYPES 5

typedef struct {
    char* shareName;
    char* date;
    float price;
    uint32_t quantity;
}Stock;

typedef enum {
    TXN_NO_STOCKS,
    TXN_NOT_ENOUGH_STOCKS,
    TXN_OK,
}TransactionStatus;

typedef struct {
    char* shareName;
    uint32_t quantity;
    TransactionStatus status;
}TransactionResult;

typedef enum {
    TXN_BUY,
    TXN_SELL
}TransactionType;

typedef struct{
    char* shareName;
    char* date;
    float price;
    uint32_t quantity;
    TransactionType transaction_type;
}Transaction;

typedef struct {
    SList* transactions;
    Queue* stocks;
}Trading;


Trading trading_new();
//Oldest stock selling implicit so not passing stock name
void trading_sell(Trading *trading, uint32_t quantity, TransactionResult* result);
//Assuming price not under the control of the user
void trading_buy(Trading *trading, char* shareName, uint32_t quantity, TransactionResult* result);
uint32_t trading_lookup(Trading *trading, char* shareName);
//Assuming price not under the control of the user
void trading_topup(Trading *trading, char* shareName, uint32_t quantity);

#endif