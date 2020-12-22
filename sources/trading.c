#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "trading.h"

Trading trading_new(){
    SList slist = slist_new();
    Queue queue = queue_new(MAX_COMPANY_TYPES);
    Trading trading = {&slist, &queue};
    return trading;
}

void trading_sell(Trading *trading, uint32_t quantity, TransactionResult* result){
    assert(trading != NULL && trading->stocks != NULL && trading->transactions != NULL);
    Queue* stocks = trading->stocks;
    if(stocks->count == 0){
        result->status = TXN_NO_STOCKS;
    }else{
        QueueResult q_result;
        queue_peek(stocks, &q_result);
        Stock* stock = (Stock*)q_result.data;

        if(quantity > stock->quantity){
            result->status = TXN_NOT_ENOUGH_STOCKS;
        } else if(stock->quantity == quantity){
            queue_delete(stocks, &q_result);
        }else{
            stock->quantity -= quantity; 
        }

        //Assuming same date & price for simplicity
        Transaction txn = {stock->shareName, stock->date, stock->price, quantity, TXN_SELL};
        slist_addnode_tail(trading->transactions, &txn);
    }
}

Stock* q_lookup(Queue* q, char* shareName){
    for (size_t i = 0; i < q->count; i++)
    {
        int res = strcmp(((Stock*)q->data[i])->shareName, shareName);
        if(res == 0){
            return q->data[i];
        }
    }
    return NULL;
    
}
void trading_buy(Trading *trading, char* shareName, uint32_t quantity, TransactionResult* result){
    assert(trading != NULL && trading->stocks != NULL && trading->transactions != NULL);
    Queue* stocks = trading->stocks;

    QueueResult q_result;
    if(stocks->count == MAX_COMPANY_TYPES){
        queue_delete(stocks, &q_result);
    }

    Stock stock = {shareName, "2020-12-22", 100.25, quantity};
    queue_add(stocks, &stock, &q_result);
    //Assuming same date & price for simplicity
    Transaction txn = {shareName, "2020-12-22", 100.25, quantity, TXN_BUY};
    slist_addnode_tail(trading->transactions, &txn);
}

uint32_t trading_lookup(Trading *trading, char* shareName){
    assert(trading != NULL && trading->stocks != NULL && trading->transactions != NULL);
    return q_lookup(trading->stocks, shareName) != NULL;
}

void trading_topup(Trading *trading, char* shareName, uint32_t quantity){
    assert(trading != NULL && trading->stocks != NULL && trading->transactions != NULL);
    Stock* stock = q_lookup(trading->stocks, shareName);
    if(stock != NULL){
        stock->quantity += quantity;
        Transaction txn = {shareName, "2020-12-22", 100.25, quantity, TXN_TOPUP};
        slist_addnode_tail(trading->transactions, &txn);
    }
}