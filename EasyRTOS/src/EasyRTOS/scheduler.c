#include "scheduler.h"
#include "task.h"
#include "constants.h"
#include "settings.h"
#include "mutex.h"

task_t *current_task = 0;
extern task_t all_task[TASK_QUANTITY];
extern mutex_t *mutexes[MUTEX_SIZE];
int *sp_pointer = 0;
int was_restore = 0;
linked_list_t *tasks = 0;
scheduler_status scheduler_info;

// Task Manager
void schedule()
{
	task_t *selected_task = 0;
	task_t * idle_task = get_idle_task_pointer();
	for(int i = 0; i < TASK_QUANTITY; i++) // find all task which not started
	{
		task_t *find_task = &all_task[i];
		if(find_task == current_task) continue;
		if(find_task->status == ST_CREATED && find_task != idle_task) // if not started
		{
			selected_task = find_task;
			current_task = find_task;
			register void *sp asm ("sp");
			sp = selected_task->stack_top;
			selected_task->status = ST_RUNNING;
			selected_task->func();
		}
	}

	if(tasks == 0)
	{
		tasks = (linked_list_t *)mem_alloc(sizeof(linked_list_t));
	}
	else
	{
		clear_list(tasks);
	}

	for(int i = 0; i < MUTEX_SIZE; i++)
	{
		mutex_t * selected_mutex = mutexes[i];
		if(selected_mutex->signal_counter > 0)
		{
			iterator_t * task_list_iterator = get_iterator_from_list(selected_mutex->task_list);
			while(iterator_any(task_list_iterator))
			{
				task_t *waiting_task = (task_t*)get_current_and_step(task_list_iterator);
				if(waiting_task != 0)
				{
					waiting_task->waitable_mutex = selected_mutex; // !!!
					insert_to_list(tasks, waiting_task);
				}
			}
			mem_free(task_list_iterator);
		}
	}

	task_t *max_priority_task = 0;
	iterator_t * task_list_iterator = get_iterator_from_list(tasks);
	while(iterator_any(task_list_iterator))
	{
		task_t *waiting_task = (task_t*)get_current_and_step(task_list_iterator);
		if(waiting_task != 0)
		{
			if(max_priority_task == 0)
			{
				max_priority_task = waiting_task;
			}
			else
			{
				if(waiting_task->priority < max_priority_task->priority)
				{
					max_priority_task = waiting_task;
				}
			}
		}
	}
	mem_free(task_list_iterator);
	selected_task = max_priority_task;

	if(selected_task != 0)
	{
		if(selected_task->status == ST_WAITING && selected_task != idle_task) // task is wait signal from mutex
		{
			current_task = selected_task;
			mutex_t * waitable_mutex = current_task->waitable_mutex;
			remove_item(waitable_mutex->task_list, current_task);
			waitable_mutex->signal_counter = 0;
			current_task->waitable_mutex = 0;
			current_task->restore = 1;
			current_task->status = ST_RUNNING;
			register void *sp asm ("sp");
			sp = current_task->stack_top;
			longjmp(current_task->buf, 1);
		}
	}
	else
	{
		for(int i = 0; i < TASK_QUANTITY; i++)
		{
			task_t *find_task = &all_task[i];
			if(find_task == current_task) continue;
			if(find_task->status == ST_WAITING_RESUME && find_task != idle_task)
			{
				current_task = find_task;
				current_task->waitable_mutex = 0;
				current_task->restore = 1;
				current_task->status = ST_RUNNING;
				register void *sp asm ("sp");
				sp = current_task->stack_top;
				longjmp(current_task->buf, 1);
			}
		}

		selected_task = idle_task;
		current_task = idle_task;
		if(idle_task->status == ST_CREATED)
		{
			register void *sp asm ("sp");
			sp = selected_task->stack_top;
			selected_task->status = ST_RUNNING;
			selected_task->func();
		}
		else
		{
			idle_task->restore = 1;
			idle_task->status = ST_RUNNING;
			register void *sp asm ("sp");
			sp = idle_task->stack_top;
			longjmp(idle_task->buf, 1);
		}
	}
}

int have_tasks_to_switch()
{
	task_t *selected_task = 0;
	task_t * idle_task = get_idle_task_pointer();
	for(int i = 0; i < TASK_QUANTITY; i++) // find all task which not started
	{
		task_t *find_task = &all_task[i];
		if(find_task == current_task) continue;
		if(find_task->status == ST_WAITING && find_task != idle_task && find_task->priority < current_task->priority) // task is wait signal from mutex
		{
			mutex_t * mutex = find_task->waitable_mutex;
			if(mutex->signal_counter > 0)
			{
				selected_task = find_task;
				break;
			}
		}
	}
	if(selected_task == 0) return 0;
	else return 1;
}

task_t * get_current_task()
{
	return current_task;
}
