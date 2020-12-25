#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "trading.h"

void test_new(){
    Trading trading = trading_new();
    assert(trading.stocks != NULL && trading.transactions != NULL);
}

//TODO free sharenames
void test_buy(){
    Trading trading = trading_new();
    Trading* trd = &trading;
    TransactionResult result;
    // printf("%d, %d, %d, %d\n", stocks->count, stocks->front, stocks->rear, stocks->size);
    trd = trading_buy(trd, sdsnew("INFY"), 100, &result);
    assert(result.status == TXN_OK);
    trd = trading_buy(trd, sdsnew("GOOGL"), 10, &result);
    assert(result.status == TXN_OK);
    trd = trading_buy(trd, sdsnew("AMZN"), 5, &result);
    assert(result.status == TXN_OK);
    // trading_to_string(trd);
    Queue *stocks = trd->stocks;
    assert(stocks->count == 3);
    trading_free(trd);
}

void test_sell(){
    Trading trading = trading_new();
    Trading* trd = &trading;
    TransactionResult result;
    trading_sell(trd, 100, &result);
    // trading_to_string(trd);
    assert(result.status == TXN_NO_STOCKS);
    trading_free(trd);
}

void test_sell2(){
    Trading trading = trading_new();
    Trading* trd = &trading;
    TransactionResult result;
    sds infy_share_name = sdsnew("INFY");
    trd = trading_buy(trd, infy_share_name, 100, &result);
    trd = trading_buy(trd, sdsnew("GOOGL"), 10, &result);
    trd = trading_buy(trd, sdsnew("AMZN"), 5, &result);
    trd = trading_sell(trd, 105, &result);
    // trading_to_string(trd);
    assert(result.status == TXN_NOT_ENOUGH_STOCKS && sdscmp(result.shareName, infy_share_name) == 0);
    trading_free(trd);
}

void test_sell3(){
    Trading trading = trading_new();
    Trading* trd = &trading;
    TransactionResult result;
    sds infy_share_name = sdsnew("INFY");
    trd = trading_buy(trd, infy_share_name, 100, &result);
    trd = trading_buy(trd, sdsnew("GOOGL"), 10, &result);
    trd = trading_buy(trd, sdsnew("AMZN"), 5, &result);
    int32_t sell_quantity = 10;
    trading_sell(trd, sell_quantity, &result);
    // trading_to_string(trd);
    assert(result.status == TXN_OK && sdscmp(result.shareName, infy_share_name) == 0 && result.quantity == sell_quantity);
    trading_free(trd);
}

void test_lookup(){
    Trading trading = trading_new();
    Trading* trd = &trading;
    TransactionResult result;
    sds infy_share_name = sdsnew("INFY");
    trd = trading_buy(trd, infy_share_name, 100, &result);
    trd = trading_buy(trd, sdsnew("GOOGL"), 10, &result);
    trd = trading_buy(trd, sdsnew("AMZN"), 5, &result);
    // trading_to_string(trd);
    assert(trading_lookup(trd, infy_share_name) != NULL);
    assert(trading_lookup(trd, sdsnew("TSLA")) == NULL);
    trading_free(trd);
}

void test_topup(){
    Trading trading = trading_new();
    Trading* trd = &trading;
    TransactionResult result;
    sds infy_share_name = sdsnew("INFY");
    trd = trading_buy(trd, infy_share_name, 100, &result);
    trd = trading_buy(trd, sdsnew("GOOGL"), 10, &result);
    trd = trading_buy(trd, sdsnew("AMZN"), 5, &result);
    trading_topup(trd, infy_share_name, 20, &result);
    assert(result.status == TXN_OK);
    trading_topup(trd, sdsnew("TSLA"), 20, &result);
    // trading_to_string(trd);
    assert(result.status == TXN_STOCK_NOT_FOUND);
    trading_free(trd);
}

int main(){
    test_new();
    test_buy();
    test_sell();
    test_sell2();
    test_sell3();
    test_lookup();
    test_topup();
    return 0;
}