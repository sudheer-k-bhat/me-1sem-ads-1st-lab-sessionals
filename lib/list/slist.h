#include <stdint.h>

#ifndef SLIST_CONTENT_TYPE
#define SLIST_CONTENT_TYPE int32_t
#endif

typedef struct _node_ Node;
struct _node_
{
    SLIST_CONTENT_TYPE data;
    Node *next;
};

typedef struct _slist_ SList;
struct _slist_
{
    Node *head;
    Node *tail;
    uint32_t length;
};

SList slist_new();

//No modifications are neeed, so using `const`
//not using `stdbool` bcoz not supported across compilers & platforms.
uint8_t slist_lookup(const SList *list, SLIST_CONTENT_TYPE key);

int32_t slist_length(const SList *list);

SList* slist_addnode_head(SList *list, SLIST_CONTENT_TYPE val);

SList* slist_addnode_tail(SList *list, SLIST_CONTENT_TYPE val);

SList* slist_delnode_head(SList *list);

SList* slist_delnode_tail(SList *list);
