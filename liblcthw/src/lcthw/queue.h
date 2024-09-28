#ifndef _LCTHW_QUEUE_H
#define _LCTHW_QUEUE_H

#include <stdlib.h>  // For malloc and free functions

// Define the node structure for the queue
typedef struct QueueNode {
    struct QueueNode *next;  // Pointer to the next node in the queue
    void *value;             // Pointer to the value stored in the node
} QueueNode;

// Define the queue structure
typedef struct Queue {
    int count;         // Number of elements in the queue
    QueueNode *first;  // Pointer to the first node (front) of the queue
    QueueNode *last;   // Pointer to the last node (rear) of the queue
} Queue;

// Macro to get the current count of the queue
#define Queue_count(A) ((A)->count)

/**
 * Creates a new queue and initializes its members.
 */
static inline Queue *Queue_create()
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    if (queue == NULL) return NULL;  // Allocation failed
    queue->count = 0;
    queue->first = NULL;
    queue->last = NULL;
    return queue;
}

/**
 * Destroys the queue and frees all allocated memory.
 */
static inline void Queue_destroy(Queue *queue)
{
    if (queue) {
        QueueNode *current = queue->first;
        QueueNode *next = NULL;

        // Iterate over each node and free it
        while (current) {
            next = current->next;
            free(current);
            current = next;
        }

        // Free the queue structure itself
        free(queue);
    }
}

/**
 * Adds a new value to the end of the queue.
 */
static inline void Queue_send(Queue *queue, void *value)
{
    // Create a new node
    QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));
    if (node == NULL) return;  // Allocation failed

    node->value = value;
    node->next = NULL;

    if (queue->last) {
        // Queue is not empty; append to the end
        queue->last->next = node;
    } else {
        // Queue is empty; new node is the first node
        queue->first = node;
    }

    queue->last = node;  // Update the last pointer to the new node
    queue->count++;      // Increment the count
}

/**
 * Removes and returns the value from the front of the queue.
 */
static inline void *Queue_recv(Queue *queue)
{
    if (queue->first == NULL) return NULL;  // Queue is empty

    QueueNode *node = queue->first;
    void *value = node->value;

    queue->first = node->next;  // Move the first pointer to the next node

    if (queue->first == NULL) {
        // Queue is now empty; update the last pointer
        queue->last = NULL;
    }

    free(node);    // Free the old first node
    queue->count--;  // Decrement the count

    return value;
}

/**
 * Peeks at the value at the front of the queue without removing it.
 */
static inline void *Queue_peek(Queue *queue)
{
    if (queue->first == NULL) return NULL;  // Queue is empty
    return queue->first->value;             // Return the first value
}

/**
 * Macro to iterate over each node in the queue.
 */
#define QUEUE_FOREACH(Q, V) \
    for (QueueNode *V = (Q)->first; V != NULL; V = V->next)

#endif
