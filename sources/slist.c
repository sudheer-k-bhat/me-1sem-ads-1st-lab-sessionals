#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "slist.h"

SList slist_new()
{
    SList slist = {NULL, NULL, 0};
    return slist;
}

int32_t slist_length(const SList *list)
{
    assert(list != NULL);
    return list->length;
}

//private function via `static`
static Node *_get_new_node(SLIST_CONTENT_TYPE val)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = val;
    node->next = NULL;
    return node;
}

SList *slist_addnode_head(SList *list, SLIST_CONTENT_TYPE val)
{
    assert(list != NULL);
    Node *node = _get_new_node(val);
    node->next = list->head;
    list->head = node;
    if (list->tail == NULL)
    {
        list->tail = node;
    }
    list->length += 1;
    assert((list->length == 1 && list->head == list->tail) ||
           (list->length > 1 && list->head != list->tail));
    return list;
}

SList *slist_addnode_tail(SList *list, SLIST_CONTENT_TYPE val)
{
    assert(list != NULL);
    Node *node = _get_new_node(val);
    if (list->tail != NULL)
    {
        list->tail->next = node;
        list->tail = node;
    }
    else
    {
        list->head = list->tail = node;
    }
    list->length += 1;
    printf(">>>%d<<<", list->length);
    if(list->head == list->tail){
        printf("yes");
    }
    // assert((list->length == 1 && list->head == list->tail) ||
    //        (list->length > 1 && list->head != list->tail));
    return list;
}

//Avoid multiple return statements when possible.
uint8_t slist_lookup(const SList *list, SLIST_CONTENT_TYPE key){
    assert(list != NULL);
    Node* cur = list->head;
    for(cur = list->head; cur != NULL; cur = cur->next){
        if(cur->data == key){
            break;
        }
    }
    return cur != NULL;
}

SList* slist_delnode_head(SList *list){
    assert(list != NULL);
    if(list->head != NULL){
        Node* temp = list->head;
        list->head = list->head->next;
        if(list->head == NULL){
            list->tail = NULL;
        }
        list->length -= 1;
        free(temp);
    }
    return list;
}

SList* slist_delnode_tail(SList *list){
    assert(list != NULL);
    if(list->tail != NULL){
        Node* last = list->tail;
        if(list->tail == list->head){
            list->tail = list->head = NULL;
        }else{
            Node* cur = list->head;
            for(cur = list->head; cur->next != last; cur = cur->next);
            list->tail = cur;
            cur->next = NULL;
        }
        list->length -= 1;
        free(last);
    }
    return list;
}
