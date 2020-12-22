#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "trading.h"

uint32_t txn_idx = 0;

Trading trading_new()
{
    SList slist = slist_new();
    Queue queue = queue_new(MAX_COMPANY_TYPES);
    Trading trading = {{0}, &queue};
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

            //Assuming same date & price for simplicity
            Transaction txn = {stock->shareName, stock->date, stock->price, quantity, TXN_SELL};
            trading->transactions[txn_idx++] = txn;
        }
    }
    return trading;
}

static Stock *q_lookup(Queue *q, char *shareName)
{
    for (size_t i = 0; i < q->count; i++)
    {
        int res = strcmp(((Stock *)q->data[i])->shareName, shareName);
        if (res == 0)
        {
            return (Stock *)q->data[i];
        }
    }
    return NULL;
}

Trading *trading_buy(Trading *trading, char *shareName, uint32_t quantity, TransactionResult *result)
{
    assert(trading != NULL && trading->stocks != NULL && trading->transactions != NULL);
    Queue *stocks = trading->stocks;

    QueueResult q_result;
    if (stocks->count == MAX_COMPANY_TYPES)
    {
        queue_delete(stocks, &q_result);
    }

    Stock stock = {shareName, "2020-12-22", 100.25, quantity};
    queue_add(stocks, &stock, &q_result);
    //Assuming same date & price for simplicity
    Transaction txn = {shareName, "2020-12-22", 100.25, quantity, TXN_BUY};
    trading->transactions[txn_idx++] = txn;
    result->status = TXN_OK;
    return trading;
}

Stock *trading_lookup(Trading *trading, char *shareName)
{
    assert(trading != NULL && trading->stocks != NULL && trading->transactions != NULL);
    return q_lookup(trading->stocks, shareName);
}

Trading *trading_topup(Trading *trading, char *shareName, uint32_t quantity, TransactionResult *result)
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