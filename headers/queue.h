#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include <stdint.h>

#include "constants.h"

#ifndef QUEUE_CONTENT_TYPE
#define QUEUE_CONTENT_TYPE int32_t
#endif

typedef enum
{
    QUEUE_OK,
    QUEUE_EMPTY,
    QUEUE_FULL
} QueueStatus;

typedef struct
{
    uint32_t size;
    uint32_t count;
    uint32_t rear;
    uint32_t front;
    QUEUE_CONTENT_TYPE data[Q_LEN];
} Queue;

typedef struct
{
    QUEUE_CONTENT_TYPE data;
    QueueStatus status;
} QueueResult;

// Public interfaces
Queue queue_new(uint32_t size);
QueueStatus queue_full(Queue *q);
QueueStatus queue_empty(Queue *q);
Queue *queue_add(Queue *q, QUEUE_CONTENT_TYPE ele, QueueResult *result); //aka push
Queue *queue_delete(Queue *q, QueueResult *result);           //aka pop
Queue *queue_peek(Queue *q, QueueResult *result);           //aka peek
uint32_t queue_length(Queue *q);

#endif