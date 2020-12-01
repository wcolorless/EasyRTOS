#include <stdio.h>
#include "task.h"
#include "settings.h"
#include <setjmp.h>
#include "constants.h"

task_t all_task[TASK_QUANTITY];
task_t *idle_task_pointer;
int usage_index = 0;
int lr = 0;

task_t * add_task(void(*func)(void), int priority,  void *context_buffer, int context_buffer_size)
{
	int current_index = usage_index++;
	all_task[current_index].func = func;
	all_task[current_index].task_id = current_index;
	all_task[current_index].priority = priority;
	all_task[current_index].status = ST_CREATED;
	all_task[current_index].stack_size = context_buffer_size;
	all_task[current_index].stack_bottom = context_buffer;
	all_task[current_index].stack_top = all_task[current_index].stack_bottom + context_buffer_size;
	return &all_task[current_index];
}


void tick_timer()
{
  for(int i = 0; i < 100000; i++);
}

void idle_task()
{
	while(1)
	{
		int have_task = have_tasks_to_switch();
		if(have_task == true)
		{
			idle_task_pointer->status = ST_WAITING_RESUME;
			if(setjmp(idle_task_pointer->buf))
			{

			}
			else
			{
				schedule();
			}
		}
		else
		{
			tick_timer();
		}
	}
}

void add_idle_task_to_list()
{
	uint8_t idle_task_stack[100];
	idle_task_pointer = add_task(idle_task, 256, idle_task_stack, sizeof(idle_task_stack));
}

task_t * get_idle_task_pointer()
{
	return idle_task_pointer;
}

