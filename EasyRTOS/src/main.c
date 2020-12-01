#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#include "stdlib.h"
#include "stdio.h"
#include "EasyRTOS/task.h"
#include "EasyRTOS/mutex.h"
#include "EasyRTOS/constants.h"
#include "EasyRTOS/system/osmem.h"
#include "EasyRTOS/system/linked_list.h"

// stacks
uint8_t *zero_stack;
uint8_t *first_stack;
uint8_t second_stack[200];
uint8_t third_stack[200];
// mutexes
mutex_t zero_mutex;
mutex_t first_mutex;
mutex_t second_mutex;
mutex_t third_mutex;
// tasks
task_t *zero_task_struct;
task_t *first_task_struct;
task_t *second_task_struct;
task_t *third_task_struct;
// lists
linked_list_t * first_list;

int message_for_third_mutex = 2;

int main(void)
{

	// init memory
	mem_init((void *)(0x20010000), 2000);

	// create lists
	easy_create_list();
	easy_test_list();

	// init stacks
	stack_init();

	// init tasks
	init_mytasks();

	// init mutexes
	mutex_init(&first_mutex);
    mutex_init(&second_mutex);
    mutex_init(&third_mutex);

    // init leds
    STM_EVAL_LEDInit(LED3);
    STM_EVAL_LEDInit(LED4);
    STM_EVAL_LEDInit(LED5);
    STM_EVAL_LEDInit(LED6);

    // run Easy RTOS
    init_easyrtos();
}


/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}

void zero_task(void)
{
	int index = 0;
	while(1)
	{
		mutex_signal(&first_mutex, 0);
		mutex_wait(&zero_mutex);
	}
}

void first_task(void)
{

	int index1 = 0;
	int index2 = 0;
	int index3 = 0;
	while(true)
	{
		mutex_signal(&second_mutex, 0);
		//STM_EVAL_LEDOn(LED3);
		//STM_EVAL_LEDOn(LED4);
		//STM_EVAL_LEDOn(LED5);
		//STM_EVAL_LEDOn(LED6);
		index1++;
		index2++;
		index3++;
		mutex_wait(&first_mutex);
	}
}

void second_task(void)
{
	while(true)
	{
		mutex_signal(&third_mutex, &message_for_third_mutex);
		STM_EVAL_LEDOff(LED3);
		STM_EVAL_LEDOff(LED4);
		STM_EVAL_LEDOff(LED5);
		STM_EVAL_LEDOff(LED6);
		mutex_wait(&second_mutex);
	}
}

void third_task(void)
{
	int index = 0;
	while(true)
	{
		index++;
		mutex_signal(&first_mutex, 0);
		mutex_t *wait_result = mutex_wait(&third_mutex);
		if(wait_result->message != null)
		{
			index += *((int*)wait_result->message);
		}
	}
}

void init_mytasks()
{
	//zero_task_struct =   add_task(zero_task, 1, zero_stack, 200);
	first_task_struct  = add_task(first_task, 2, first_stack, 200);
	second_task_struct = add_task(second_task, 3, second_stack, sizeof(second_stack));
	third_task_struct =  add_task(third_task, 4, third_stack, sizeof(third_stack));
}

void stack_init()
{
	first_stack = mem_alloc(200);
	zero_stack = mem_alloc(200);
}

void easy_create_list()
{
	first_list = create_list(&message_for_third_mutex);
}

void easy_test_list()
{
	int result[4] = {0,0,0,0};
	int num1 = 1;
	int num2 = 2;
	int num3 = 3;
	insert_to_list(first_list, &num1);
	insert_to_list(first_list, &num2);
	insert_to_list(first_list, &num3);
	int list_size = count_list(first_list);
	for(int i = 0; i < list_size; i++)
	{
		void *data = extract_from_list(first_list);
		result[i] = *((int*)data);
	}
	int list_size_after_extract = count_list(first_list);
}
