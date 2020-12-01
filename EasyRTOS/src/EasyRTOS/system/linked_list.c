#include "linked_list.h"
#include "osmem.h"
#include "../constants.h"


void init_list(linked_list_t *list)
{
	list->prev = 0;
	list->next = 0;
	list->data = 0;
}

linked_list_t * create_list(void *data)
{
	linked_list_t *new_list_node = (linked_list_t*)mem_alloc(sizeof(linked_list_t));
	init_list(new_list_node);
	new_list_node->data = data;
	return new_list_node;
}

void insert_to_end_list(linked_list_t *node, void *data)
{
	linked_list_t *new_list_node = (linked_list_t*)mem_alloc(sizeof(linked_list_t));
	new_list_node->data = data;
	new_list_node->prev = node;
	new_list_node->next = 0;
	node->next = new_list_node;
}

linked_list_t *get_last_node(linked_list_t *root)
{
	if(root == 0) return 0;
	linked_list_t *end_node = 0;
	while(end_node == 0)
	{
		if(root->next == 0)
		{
			end_node = root;
			break;
		}
		else
		{
			root = root->next;
		}
	}
	return end_node;
}

void insert_to_list(linked_list_t *root, void *data)
{
	if(root != 0 && root->data == 0)
	{
		root->data = data;
		return;
	}
	linked_list_t *last_node = get_last_node(root);
	insert_to_end_list(last_node, data);
}

void * extract_from_list(linked_list_t *root)
{
   if(root->data == 0) return 0;
   void *data = root->data;
   if(root->next != 0)
   {
	   linked_list_t *next = root->next;
	   root->data = next->data;
	   root->prev = 0;
	   root->next = next->next;
	   if(next->next != 0)
	   {
		   linked_list_t *anext = next->next;
		   anext->prev = root;
	   }
	   mem_free(next);
   }
   else
   {
	   root->data = 0;
   }
   return data;
}

void clear_list(linked_list_t *root)
{
	linked_list_t *node = 0;
	root->data = 0;
	while(node == 0)
	{
		if(root->next != 0)
		{
			node = root->next;
		}
		else
		{
			mem_free(root);
			break;
		}
		mem_free(root);
		root = node;
		node = 0;
	}
}

int count_list(linked_list_t *root)
{
	int count = 0;
	if(root->data == 0) return count;
	while(1)
	{
		count++;
		if(root->next != 0)
		{
			root = root->next;
		}
		else
		{
			break;
		}
	}
	return count;
}

iterator_t * get_iterator_from_list(linked_list_t *root)
{
	if(root == 0) return 0;
	if(root->data == 0) return 0;
	iterator_t * new_iterator = (iterator_t*)mem_alloc(sizeof(iterator_t));
	new_iterator->root = root;
	new_iterator->current = root;
	new_iterator->next = root->next;
	return new_iterator;
}

int iterator_any(iterator_t * iterator)
{
	if(iterator == 0) return 0;
	if(iterator->current == 0) return 0;
	else return 1;
}

void * get_current_and_step(iterator_t * iterator)
{
	if(iterator->current == 0) return 0;
	linked_list_t *current_node = (linked_list_t *)iterator->current;
	void *current_data = current_node->data;
	linked_list_t * next_node = 0;
	if(current_node->next != 0)
	{
		next_node = current_node->next;
		iterator->current = next_node;
		iterator->next = next_node->next;
	}
	else
	{
		iterator->current = 0;
		iterator->next = 0;
	}
	return current_data;
}

void remove_item(linked_list_t *root, void *item)
{
	while(1)
	{
		if(root->data == item)
		{
		   if(root->next != 0)
		   {
			   linked_list_t *next = root->next;
			   root->data = next->data;
			   root->next = next->next;
			   if(next->next != 0)
			   {
				   linked_list_t *anext = next->next;
				   anext->prev = root;
			   }
			   mem_free(next);
		   }
		   else
		   {
			   root->data = 0;
		   }
		   break;
		}
		else
		{
			if(root->next == 0) break;

			linked_list_t *next = root->next;
			root = next;
			continue;
		}
	}
}
