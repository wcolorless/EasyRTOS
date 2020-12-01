#include "os.h"
#include "scheduler.h"
#include "mutex.h"

void init_easyrtos()
{
	add_idle_task_to_list();
	schedule();
}
