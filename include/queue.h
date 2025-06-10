#ifndef ADVANCED_QUEUE_H
#define ADVANCED_QUEUE_H

#include <stddef.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct queue_node {
    void *data;
    struct queue_node *next;
} queue_node_t;

typedef struct {
    queue_node_t *head;
    queue_node_t *tail;
    size_t size;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} queue_t;

/**
 * Initialize a queue structure.
 * Returns 0 on success.
 */
int queue_init(queue_t *q);

/**
 * Destroy a queue structure. All remaining elements are lost.
 */
void queue_destroy(queue_t *q);

/**
 * Push data to the tail of the queue.
 * Returns 0 on success, -1 on allocation failure.
 */
int queue_push(queue_t *q, void *data);

/**
 * Pop data from the head of the queue. Blocks if the queue is empty.
 * Returns the data pointer, or NULL on error.
 */
void *queue_pop(queue_t *q);

/**
 * Try to pop data from the head of the queue without blocking.
 * Returns the data pointer or NULL if queue is empty.
 */
void *queue_try_pop(queue_t *q);

/**
 * Get current size of the queue.
 */
size_t queue_size(queue_t *q);

#ifdef __cplusplus
}
#endif

#endif /* ADVANCED_QUEUE_H */
