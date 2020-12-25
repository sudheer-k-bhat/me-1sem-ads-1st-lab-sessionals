#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../lib/sds/sds.h"
#include "trading.h"

uint32_t txn_idx = 0;

static Stock *q_lookup(Queue *q, sds shareName)
{
    for (size_t i = 0; i < q->count; i++)
    {
        if (sdscmp(((Stock *)q->data[i])->shareName, shareName) == 0)
        {
            return (Stock *)q->data[i];
        }
    }
    return NULL;
}

static Stock* _new_stock(sds shareName, sds date, float price, uint32_t quantity){
    Stock* stock = malloc(sizeof(Stock));
    stock->date = sdsdup(date);
    stock->price = price;
    stock->quantity = quantity;
    stock->shareName = sdsdup(shareName);
    return stock;
}

static Transaction* _new_txn(sds shareName, sds date, float price, uint32_t quantity, TransactionType txn_type){
    Transaction* txn = malloc(sizeof(Transaction));
    txn->date = sdsdup(date);
    txn->price = price;
    txn->quantity = quantity;
    txn->shareName = sdsdup(shareName);
    txn->transaction_type = txn_type;
    return txn;
}

Trading trading_new()
{
    SList slist = slist_new();
    Queue* stocks = queue_new_ptr(MAX_COMPANY_TYPES);
    SList* transactions = slist_new_ptr();
    Trading trading = {transactions, stocks};
    return trading;
}

Trading *trading_sell(Trading *trading, uint32_t quantity, TransactionResult *result)
{
    assert(trading != NULL && trading->stocks != NULL && trading->transactions != NULL);
    Queue *stocks = trading->stocks;
    if (stocks->count == 0)
    {
        result->status = TXN_NO_STOCKS;
    }
    else
    {
        QueueResult q_result;
        queue_peek(stocks, &q_result);
        Stock *stock = (Stock *)q_result.data;

        if (quantity > stock->quantity)
        {
            result->shareName = sdsdup(stock->shareName);
            result->quantity = stock->quantity;
            result->status = TXN_NOT_ENOUGH_STOCKS;
        }
        else
        {
            if (stock->quantity == quantity)
            {
                queue_delete(stocks, &q_result);
            }
            else
            {
                stock->quantity -= quantity;
            }
            result->shareName = sdsdup(stock->shareName);
            result->quantity = quantity;
            result->status = TXN_OK;

            //Assuming same date & price for simplicity
            sds date = sdsnew("2020-12-22");
            Transaction* txn = _new_txn(result->shareName, date, stock->price, quantity, TXN_SELL);
            slist_addnode_tail(trading->transactions, txn);
        }
    }
    return trading;
}

Trading *trading_buy(Trading *trading, sds shareName, uint32_t quantity, TransactionResult *result)
{
    assert(trading != NULL && trading->stocks != NULL && trading->transactions != NULL);
    Queue *stocks = trading->stocks;

    QueueResult q_result;
    if (stocks->count == MAX_COMPANY_TYPES)
    {
        queue_delete(stocks, &q_result);
    }

    sds date = sdsnew("2020-12-22");
    float price = 100.25;
    Stock* stock = _new_stock(shareName, date, price, quantity);
    queue_add(stocks, stock, &q_result);
    //Assuming same date & price for simplicity
    Transaction* txn = _new_txn(shareName, date, price, quantity, TXN_BUY);
    slist_addnode_tail(trading->transactions, txn);
    // printf("Transaction{%s, %f, %d, %s, %d}", txn.date, txn.price, txn.quantity, txn.shareName, txn.transaction_type);

    result->status = TXN_OK;
    return trading;
}

Stock *trading_lookup(Trading *trading, sds shareName)
{
    assert(trading != NULL && trading->stocks != NULL && trading->transactions != NULL);
    return q_lookup(trading->stocks, shareName);
}

Trading *trading_topup(Trading *trading, sds shareName, uint32_t quantity, TransactionResult *result)
{
    assert(trading != NULL && trading->stocks != NULL && trading->transactions != NULL);
    Stock *stock = q_lookup(trading->stocks, shareName);
    if (stock != NULL)
    {
        stock->quantity += quantity;
        sds date = sdsnew("2020-12-22");
        Transaction* txn = _new_txn(shareName, date, stock->price, quantity, TXN_TOPUP);
        slist_addnode_tail(trading->transactions, txn);
        result->status = TXN_OK;
    }
    else
    {
        result->status = TXN_STOCK_NOT_FOUND;
    }
    return trading;
}

void print_txn(Transaction* txn){
    printf("Transaction{%s, %f, %d, %s, %d}\n", txn->date, txn->price, txn->quantity, txn->shareName, txn->transaction_type);
}

void trading_to_string(Trading* trading){
    assert(trading != NULL && trading->stocks != NULL && trading->transactions != NULL);
    Queue* q = trading->stocks;
    for (uint32_t i = 0; i < queue_length(q); i++)
    {
        Stock* stock = (Stock*)q->data[i];
        printf("Stock{%s, %f, %d, %s}\n", stock->date, stock->price, stock->quantity, stock->shareName);
    }
    SList* list = trading->transactions;
    Node* cur = list->head;
    for(cur = list->head; cur != NULL; cur = cur->next){
        print_txn((Transaction*)cur->data);
    }
}