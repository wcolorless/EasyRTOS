#include <setjmp.h>
#include "mutex.h"
#include "task.h"
#include "settings.h"
#include "scheduler.h"
#include "constants.h"
#include "system/osmem.h"

mutex_t *mutexes[MUTEX_SIZE];
int mutex_index = 0;
extern int was_restore;
extern scheduler_status scheduler_info;

void mutex_init(mutex_t *mutex)
{
	mutex->signal_counter = 0;
	mutex->message = 0;
	linked_list_t *new_list_node = (linked_list_t*)mem_alloc(sizeof(linked_list_t));
	init_list(new_list_node);
	mutex->task_list = new_list_node;
	mutexes[mutex_index++] = mutex;
}


mutex_t* mutex_wait(mutex_t *mutex)
{
	task_t *current_task = get_current_task();
	current_task->restore = 0;
	mutex->signal_counter = 0;
	mutex->message = 0;
	insert_to_list(mutex->task_list, current_task); // add task to task list into mutex_t
	if(setjmp(current_task->buf))
	{
		return mutex;
	}
	else
	{
		if(current_task->restore == 0)
		{
			current_task->waitable_mutex = mutex;
			current_task->status = ST_WAITING;
			schedule();
		}
	}
	return mutex;
}

void mutex_signal(mutex_t *input, void *message)
{
	input->signal_counter++;
	input->message = message;
	int wait_result = have_tasks_to_switch();
	if(wait_result == true)
	{
		task_t *current_task = get_current_task();
		if(setjmp(current_task->buf))
		{
			return;
		}
		else
		{
			current_task->status = ST_WAITING_RESUME;
			schedule();
		}
	}
}

void mutex_lock(mutex_t *mutex)
{
	task_t *current_task = get_current_task();
	current_task->restore = 0;
	mutex->signal_counter = 0;
	mutex->message = 0;
	if(mutex->is_lock == false)
	{
		mutex->is_lock = true;
		mutex->blocking_task = current_task;
	}
	else
	{
		while(mutex->is_lock == true)
		{
			mutex->signal_counter = 0;
			mutex->message = 0;
			insert_to_list(mutex->task_list, current_task); // add task to task list into mutex_t
			if(setjmp(current_task->buf))
			{
				// mutex lock after return control this task
				if(mutex->is_lock == true) continue;
				mutex->is_lock = true;
				mutex->blocking_task = current_task;
				return;
			}
			else
			{
				current_task->status = ST_WAITING;
				schedule();
				break;
			}
		}
	}
}

void mutex_unlock(mutex_t *mutex)
{

	if(mutex->is_lock == true)
	{
		if(mutex->blocking_task == get_current_task())
		{
			mutex->is_lock = false;
			mutex->blocking_task = 0;
			mutex->signal_counter++;
		}
	}
}
