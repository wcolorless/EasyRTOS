#ifndef MUTEXHEAD_H
#define MUTEXHEAD_H

#include "task.h"

typedef struct
{
	int signal_counter;
	void *message;
	int is_lock;
	void *blocking_task;
    linked_list_t *task_list;
} mutex_t;


void mutex_init(mutex_t *mutex);
mutex_t* mutex_wait(mutex_t *mutex);
void mutex_signal(mutex_t *input, void *message);
void mutex_lock(mutex_t *mutex);
void mutex_unlock(mutex_t *mutex);


#endif
