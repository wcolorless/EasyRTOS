#include "queue.h"
#include "linked_list.h"
#include "osmem.h"

queue_t * create_queue()
{
	linked_list_t *new_list_node = create_list(0);
	queue_t * new_queue = (queue_t*)mem_alloc(sizeof(queue_t));
	new_queue->list = new_list_node;
	new_queue->last_item = 0;
	return new_queue;
}

void enqueue(queue_t *queue, void *item)
{
	if(item != 0 && queue != 0)
	{
		insert_to_list(queue->list, item);
	}
}

void * dequeue(queue_t *queue)
{
	if(queue != 0)
	{
		void *item = extract_from_list(queue->list);
		queue->last_item = item;
		return item;
	}
	else return 0;
}
