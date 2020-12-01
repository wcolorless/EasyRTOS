#ifndef QUEUEHEAD_H
#define QUEUEHEAD_H

#include "linked_list.h"

typedef struct
{
	linked_list_t *list;
	void *last_item;
}queue_t;

queue_t * create_queue();
void enqueue(queue_t *queue, void *item);
void * dequeue(queue_t *queue);

#endif
