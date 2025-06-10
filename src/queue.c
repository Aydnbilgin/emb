#include "queue.h"
#include <stdlib.h>

int queue_init(queue_t *q) {
    if (!q) return -1;
    q->head = q->tail = NULL;
    q->size = 0;
    if (pthread_mutex_init(&q->lock, NULL) != 0)
        return -1;
    if (pthread_cond_init(&q->cond, NULL) != 0) {
        pthread_mutex_destroy(&q->lock);
        return -1;
    }
    return 0;
}

void queue_destroy(queue_t *q) {
    if (!q) return;
    pthread_mutex_lock(&q->lock);
    queue_node_t *node = q->head;
    while (node) {
        queue_node_t *tmp = node;
        node = node->next;
        free(tmp);
    }
    q->head = q->tail = NULL;
    q->size = 0;
    pthread_mutex_unlock(&q->lock);

    pthread_mutex_destroy(&q->lock);
    pthread_cond_destroy(&q->cond);
}

int queue_push(queue_t *q, void *data) {
    queue_node_t *node = malloc(sizeof(queue_node_t));
    if (!node) return -1;
    node->data = data;
    node->next = NULL;

    pthread_mutex_lock(&q->lock);
    if (q->tail) {
        q->tail->next = node;
        q->tail = node;
    } else {
        q->head = q->tail = node;
    }
    q->size++;
    pthread_cond_signal(&q->cond);
    pthread_mutex_unlock(&q->lock);
    return 0;
}

void *queue_pop(queue_t *q) {
    pthread_mutex_lock(&q->lock);
    while (q->size == 0) {
        pthread_cond_wait(&q->cond, &q->lock);
    }
    queue_node_t *node = q->head;
    q->head = node->next;
    if (!q->head)
        q->tail = NULL;
    q->size--;
    pthread_mutex_unlock(&q->lock);

    void *data = node->data;
    free(node);
    return data;
}

void *queue_try_pop(queue_t *q) {
    pthread_mutex_lock(&q->lock);
    if (q->size == 0) {
        pthread_mutex_unlock(&q->lock);
        return NULL;
    }
    queue_node_t *node = q->head;
    q->head = node->next;
    if (!q->head)
        q->tail = NULL;
    q->size--;
    pthread_mutex_unlock(&q->lock);

    void *data = node->data;
    free(node);
    return data;
}

size_t queue_size(queue_t *q) {
    pthread_mutex_lock(&q->lock);
    size_t size = q->size;
    pthread_mutex_unlock(&q->lock);
    return size;
}
