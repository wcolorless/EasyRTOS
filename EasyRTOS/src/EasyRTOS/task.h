#ifndef TASKHEAD_H
#define TASKHEAD_H

#include "system/linked_list.h"
#include "mutex.h"
#include <setjmp.h>

typedef struct
{
	enum
	{
		NONE,
		ST_CREATED,
		ST_RUNNING,
		ST_WAITING,
		ST_WAITING_RESUME
	} status;

	int task_id;
	void (*func)(void);
	int priority;
	void *context_buffer;
	void *stack_bottom;
	void *stack_top;
	jmp_buf buf;
	int stack_size;
	int restore;
	void *waitable_mutex;
} task_t;




task_t * add_task(void(*func)(void), int priority,  void *context_buffer, int context_buffer_size);
void save_register_context(task_t *task);
void restore_register_context(task_t *task);
void add_idle_task_to_list();
task_t * get_idle_task_pointer();

#endif
