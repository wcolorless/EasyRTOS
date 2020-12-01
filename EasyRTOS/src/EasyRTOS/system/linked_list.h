#ifndef LINKEDLISTHEAD_H
#define LINKEDLISTHEAD_H

typedef struct linked_list linked_list_t;

struct linked_list
{
	linked_list_t *prev;
	linked_list_t *next;
	void *data;
};

typedef struct
{
	void *root;
	void *current;
	void *next;
} iterator_t;

void init_list(linked_list_t *list);
linked_list_t * create_list(void *data);
void insert_to_end_list(linked_list_t *node, void *data);
linked_list_t *get_last_node(linked_list_t *root);
void insert_to_list(linked_list_t *root, void *data);
void * extract_from_list(linked_list_t *root);
void clear_list(linked_list_t *root);
int count_list(linked_list_t *root);
iterator_t * get_iterator_from_list(linked_list_t *root);
int iterator_any(iterator_t * iterator);
void * get_current_and_step(iterator_t * iterator);
void remove_item(linked_list_t *root, void *item);


#endif
