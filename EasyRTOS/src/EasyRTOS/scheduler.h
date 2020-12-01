#include "mutex.h"

enum {
    INIT=0,
    SCHEDULE,
    EXIT_TASK,
};

typedef struct {
    jmp_buf buf;
    struct task_t *current;
} scheduler_status;

void save_context();
void restore_context(mutex_t *mutex);
void run_next_task();
int have_tasks_to_switch();
task_t * get_current_task();
