#ifndef TRADING_H
#define TRADING_H
#include <stdint.h>

#include "queue.h"
#include "slist.h"
#include "../lib/sds/sds.h"

#define MAX_COMPANY_TYPES 5

typedef struct {
    sds shareName;
    sds date;
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
    sds shareName;
    uint32_t quantity;
    TransactionStatus status;
}TransactionResult;

typedef enum {
    TXN_BUY,
    TXN_SELL,
    TXN_TOPUP
}TransactionType;

typedef struct{
    sds shareName;
    sds date;
    float price;
    uint32_t quantity;
    TransactionType transaction_type;
}Transaction;

typedef struct {
    SList* transactions;
    Queue* stocks;
}Trading;


Trading trading_new();
void trading_free(Trading* trading);
//Oldest stock selling implicit so not passing stock name
Trading* trading_sell(Trading *trading, uint32_t quantity, TransactionResult* result);
//Assuming price not under the control of the user
Trading* trading_buy(Trading *trading, sds shareName, uint32_t quantity, TransactionResult* result);
Stock* trading_lookup(Trading *trading, sds shareName);
//Assuming price not under the control of the user
Trading* trading_topup(Trading *trading, sds shareName, uint32_t quantity, TransactionResult* result);
void trading_to_string(Trading* trading);

#endif