#ifndef OSMEMHEAD_H
#define OSMEMHEAD_H

#define MEMSTARTKEYWORK 0x1070FF19

typedef struct
{
	int size;
	int *key;
} mem_t;

void mem_init(void *start, int size);
void * mem_alloc(int size);
void mem_free(void *mem);

#endif
