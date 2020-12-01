#include "timer.h"



void timer_init(timer_t *timer)
{
	timer->initial_time = 0;
	timer->period  = 1; // 1 ms period
}
