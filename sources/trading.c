#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../lib/sds/sds.h"
#include "trading.h"

uint32_t txn_idx = 0;

Trading trading_new()
{
    SList slist = slist_new();
    Queue* queue = queue_new_ptr(MAX_COMPANY_TYPES);
    Trading trading = {{0}, queue};
    // trading.stocks = &queue;
    // printf("%d, %d, %d, %d\n", trading.stocks->count, trading.stocks->front, trading.stocks->rear, trading.stocks->size);
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
            Transaction txn = {stock->shareName, stock->date, stock->price, quantity, TXN_SELL};
            trading->transactions[txn_idx++] = txn;
        }
    }
    return trading;
}

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

Trading *trading_buy(Trading *trading, sds shareName, uint32_t quantity, TransactionResult *result)
{
    assert(trading != NULL && trading->stocks != NULL && trading->transactions != NULL);
    Queue *stocks = trading->stocks;
    // printf("%d, %d, %d, %d\n", stocks->count, stocks->front, stocks->rear, stocks->size);

    QueueResult q_result;
    if (stocks->count == MAX_COMPANY_TYPES)
    {
        queue_delete(stocks, &q_result);
    }

    Stock* stock = _new_stock(shareName, sdsnew("2020-12-22"), 100.25, quantity);
    queue_add(stocks, stock, &q_result);
    // printf("\n%d", queue_length(stocks));
    // printf("Stock{%s, %f, %d, %s}\n", stock->date, stock->price, stock->quantity, stock->shareName);
    //Assuming same date & price for simplicity
    Transaction txn = {sdsnew(shareName), "2020-12-22", 100.25, quantity, TXN_BUY};
    trading->transactions[txn_idx++] = txn;
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
        // Transaction txn = {shareName, "2020-12-22", 100.25, quantity, TXN_TOPUP};
        // trading->transactions[txn_idx++] = txn;
        result->status = TXN_OK;
    }
    else
    {
        result->status = TXN_STOCK_NOT_FOUND;
    }
    return trading;
}

void trading_to_string(Trading* trading){
    assert(trading != NULL && trading->stocks != NULL && trading->transactions != NULL);
    Queue* q = trading->stocks;
    // printf(">>%d", queue_length(q));
    for (uint32_t i = 0; i < queue_length(q); i++)
    {
        Stock* stock = (Stock*)q->data[i];
        printf("Stock{%s, %f, %d, %s}\n", stock->date, stock->price, stock->quantity, stock->shareName);
    }
    // for (size_t i = 0; i <= txn_idx; i++)
    // {
    //     Transaction txn = trading->transactions[i];
    //     printf("Transaction{%s, %f, %d, %s, %d}", txn.date, txn.price, txn.quantity, txn.shareName, txn.transaction_type);
    // }
}