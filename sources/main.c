#include <assert.h>
#include <stdlib.h>

#include "trading.h"

void test_new(){
    Trading trading = trading_new();
    assert(trading.stocks != NULL && trading.transactions != NULL);
}

void test_buy(){
    Trading trading = trading_new();
    Trading* trd = &trading;
    TransactionResult result;
    trading_buy(trd, "INFY", 100, &result);
    assert(result.status == TXN_OK);
}

void test_sell(){
    Trading trading = trading_new();
    Trading* trd = &trading;
    TransactionResult result;
    trading_sell(trd, 100, &result);
    assert(result.status == TXN_NO_STOCKS);
}

void test_sell2(){
    Trading trading = trading_new();
    Trading* trd = &trading;
    TransactionResult result;
    trd = trading_buy(trd, "GOOGL", 10, &result);
    trd = trading_sell(trd, 15, &result);
    assert(result.status == TXN_NOT_ENOUGH_STOCKS);
}

void test_sell3(){
    Trading trading = trading_new();
    Trading* trd = &trading;
    TransactionResult result;
    trd = trading_buy(trd, "GOOGL", 10, &result);
    trading_sell(trd, 10, &result);
    assert(result.status == TXN_OK);
}

void test_lookup(){
    Trading trading = trading_new();
    Trading* trd = &trading;
    TransactionResult result;
    trading_buy(trd, "INFY", 100, &result);
    trading_buy(trd, "GOOGL", 10, &result);
    assert(trading_lookup(trd, "INFY") != NULL);
}

void test_topup(){
    Trading trading = trading_new();
    Trading* trd = &trading;
    TransactionResult result;
    trading_buy(trd, "INFY", 100, &result);
    trading_topup(trd, "INFY", 20, &result);
    // assert(result.status == TXN_OK);
}

int main(){
    test_new();
    test_buy();
    test_sell();
    test_sell2();
    test_sell3();
    // test_lookup();
    test_topup();
    return 0;
}