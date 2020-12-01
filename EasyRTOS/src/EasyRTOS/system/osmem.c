#include "osmem.h"
#include "stdlib.h"
#include "stdio.h"

uint8_t *mem_start = 0;
uint8_t *mem_current_end = 0;
int mem_size = 0;

void mem_init(void *start, int size)
{
	mem_start = start;
	mem_current_end = start;
	mem_size = size;
	for(int i = 0; i < size; i++)
	{
		*(mem_start + i) = 0;
	}
}

void * mem_alloc(int size)
{
	for(int i = 0; i < size; i++)
	{
		*(mem_current_end + i) = 0;
	}
	mem_t *new_mem = (mem_t*)mem_current_end;
	new_mem->size = size;
	new_mem->key = MEMSTARTKEYWORK;
	int result = mem_current_end + sizeof(mem_t);
	mem_current_end = mem_current_end + sizeof(mem_t) + size;
	return result;
}

void mem_free(void *mem)
{
	uint8_t *current = (uint8_t *)mem - sizeof(mem_t);
	mem_t *old_mem = (mem_t*)current;
	for(int i = 0; i < old_mem->size; i++)
	{
		*((uint8_t *)mem + i) = 0;
	}
	old_mem->key = 0;
	old_mem->size = 0;
}
