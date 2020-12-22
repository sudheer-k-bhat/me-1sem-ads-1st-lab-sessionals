#ifndef TRADING_H
#define TRADING_H
#include <stdint.h>
#include "queue.h"
#include "slist.h"

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
    TXN_STOCK_NOT_FOUND
}TransactionStatus;

typedef struct {
    char* shareName;
    uint32_t quantity;
    TransactionStatus status;
}TransactionResult;

typedef enum {
    TXN_BUY,
    TXN_SELL,
    TXN_TOPUP
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
Trading* trading_sell(Trading *trading, uint32_t quantity, TransactionResult* result);
//Assuming price not under the control of the user
Trading* trading_buy(Trading *trading, char* shareName, uint32_t quantity, TransactionResult* result);
Stock* trading_lookup(Trading *trading, char* shareName);
//Assuming price not under the control of the user
Trading* trading_topup(Trading *trading, char* shareName, uint32_t quantity, TransactionResult* result);

#endif