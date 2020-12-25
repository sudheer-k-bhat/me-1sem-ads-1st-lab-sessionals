#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "queue.h"

Queue queue_new(uint32_t size){
    size = size > 0 && size < Q_LEN ? size : Q_LEN;
    Queue queue = {size, 0, 0, 0, {0}};
    return queue;
}

Queue* queue_new_ptr(uint32_t size){
    size = size > 0 && size < Q_LEN ? size : Q_LEN;
    Queue* queue = malloc(sizeof(Queue));
    queue->size = size;
    queue->data[0] = 0;
    queue->front = 0;
    queue->rear = 0;
    queue->count = 0;
    return queue;
}

QueueStatus queue_full(Queue* q){
    assert(q != NULL);
    return q->count == q->size ? QUEUE_FULL : QUEUE_OK;
}

QueueStatus queue_empty(Queue* q){
    assert(q != NULL);
    return q->count == 0 ? QUEUE_EMPTY : QUEUE_OK;
}

Queue* queue_add(Queue* q, QUEUE_CONTENT_TYPE ele, QueueResult *result){
    assert(q != NULL);
    // printf("%d", q->count);
    if(q->count < q->size){
        q->data[q->rear] = ele;
        ++q->count;
        q->rear = (q->rear + 1) % q->size;

        result->data = ele;
        result->status = QUEUE_OK;
    }else{
        result->status = QUEUE_FULL;
    }
    // printf("%d", q->count);
    return q;
}
Queue* queue_delete(Queue* q, QueueResult *result){
    assert(q != NULL);
    if(q->count != 0){
        result->data = q->data[q->front];
        q->front = (q->front + 1) % q->size;
        --q->count;
        result->status = QUEUE_OK;
    }else{
        result->status = QUEUE_EMPTY;
    }
    return q;
}

Queue* queue_peek(Queue* q, QueueResult *result){
    assert(q != NULL);
    if(q->count != 0){
        result->data = q->data[q->front];
        result->status = QUEUE_OK;
    }else{
        result->status = QUEUE_EMPTY;
    }
    return q;
}

uint32_t queue_length(Queue* q){
    assert(q != NULL);
    return q->count;
}