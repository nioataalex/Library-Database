// Copyright 2022 Nioata Alexandra

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "hashtable.h"

// linked list implementation from lab4
linked_list_t*
ll_create(unsigned int data_size)
{
	linked_list_t* ll = calloc( 1, sizeof(*ll));
	DIE(!ll, "calloc list");
	ll->data_size = data_size;
	ll->head = NULL;
    ll->data_size = data_size;
    ll->size = 0;
	return ll;
}

void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{
	ll_node_t *prev, *curr;
    ll_node_t* new_node;
    if (!list) {
        return;
    }
    if (n > list->size) {
        n = list->size;
    }
    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }
    new_node = malloc(sizeof(*new_node));
    new_node->data = malloc(list->data_size);
    memcpy(new_node->data, new_data, list->data_size);
    new_node->next = curr;
    if (prev == NULL) {
        list->head = new_node;
    } else {
        prev->next = new_node;
    }
    list->size++;
}

ll_node_t*
ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
	ll_node_t *prev, *curr;
    if (!list || !list->head) {
        return NULL;
    }
    if (n > list->size - 1) {
        n = list->size - 1;
    }
    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }
    if (prev == NULL) {
        list->head = curr->next;
    } else {
        prev->next = curr->next;
	}
    list->size--;
    return curr;
}

unsigned int
ll_get_size(linked_list_t* list)
{
	if (!list) {
        return -1;
    }
    return list->size;
}

void
ll_free(linked_list_t** pp_list)
{
	ll_node_t* currNode;
    if (!pp_list || !*pp_list) {
        return;
    }
    while (ll_get_size(*pp_list) > 0) {
        currNode = ll_remove_nth_node(*pp_list, 0);
        free(currNode->data);
        currNode->data = NULL;
        free(currNode);
        currNode = NULL;
    }
    free(*pp_list);
    *pp_list = NULL;
}

// hashtable implementation made at lab4
int
compare_function_ints(void *a, void *b)
{
	int int_a = *((int *)a);
	int int_b = *((int *)b);
	if (int_a == int_b) {
		return 0;
	} else if (int_a < int_b) {
		return -1;
	} else {
		return 1;
	}
}

int
compare_function_strings(void *a, void *b)
{
	char *str_a = (char *)a;
	char *str_b = (char *)b;
	return strcmp(str_a, str_b);
}

unsigned int
hash_function_int(void *a)
{
	unsigned int uint_a = *((unsigned int *)a);
	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = (uint_a >> 16u) ^ uint_a;
	return uint_a;
}

unsigned int
hash_function_string(void *a)
{
	unsigned char *puchar_a = (unsigned char*) a;
	unsigned int hash = 5381;
	int c;
	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */
	return hash;
}

// creates a hashtable
// makes initializations
hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*))
{
	if(!hash_function || !compare_function)
        return 0;
	hashtable_t *ht = malloc(sizeof(*ht));
	if(!ht) {
        printf("malloc failed\n");
        return 0;
    }
    ht->hmax = hmax;
    ht->size = 0;
	ht->hash_function = hash_function;
	ht->compare_function = compare_function;
	ht->buckets = malloc(hmax * sizeof(*ht->buckets));
	if(!ht->buckets) {
        printf("malloc failed\n");
        return 0;
    }
	for (unsigned int i = 0; i < hmax; i++)
		ht->buckets[i] = ll_create(sizeof(info));
	return ht;
}

// verifies if it has key
int
ht_has_key(hashtable_t *ht, void *key)
{
	if(!ht || !key) {
        return -1;
    }
    int index = ht->hash_function(key) % ht->hmax;
    ll_node_t *node = ht->buckets[index]->head;
    while (node != NULL) {
        info *inf = (info *)node->data;
        if (!ht->compare_function(key, inf->key))
            return 1;
        node = node->next;
    }
	return 0;
}

// gets the index from the hashtable for a certain key
int
ht_get_index(hashtable_t *ht, void *key)
{
	if(!ht || !key) {
        return 0;
    }
    int index = ht->hash_function(key) % ht->hmax;
    return index;
}

// puts key and value in the hashtable
void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size)
{
	if(!ht || !key)
        return;
	info infos;
	int index = ht->hash_function(key) % ht->hmax;
	linked_list_t *bucket = ht->buckets[index];
	int ok = 0;
	ll_node_t *node_t = bucket->head;
    ll_node_t *node = NULL;
    for (unsigned int i = 0; i < bucket->size && ok == 0; i++) {
		if (ht->compare_function(key, ((info *)node_t->data)->key) == 0) {
            node = node_t;
            ok = 0;
        }
		node_t  = node_t->next;
	}
	if (!node) {
		infos.key = malloc(key_size);
		if(!key_size) {
            printf("malloc failed\n");
            return;
        }
		memcpy(infos.key, key, key_size);
		infos.value = malloc(value_size);
		if(!value_size) {
            printf("malloc failed\n");
            return;
        }
		memcpy(infos.value, value, value_size);
		ll_add_nth_node(bucket, 0, &infos);
	} else {
		free(((info *)node->data)->value);
		((info *)node->data)->value = malloc(value_size);
		memcpy(((info *)node->data)->value, value, value_size);
	}
	ht->size++;
}

// removes a given key
void
ht_remove_entry(hashtable_t *ht, void *key)
{
	if(!ht || !key)
        return;
	int index = ht->hash_function(key) % ht->hmax;
	linked_list_t *bucket = ht->buckets[index];
    int n = 0;
    int ok = 0;
	ll_node_t *node_t = bucket->head;
    ll_node_t *node = NULL;
    ll_node_t *temp_node;
    for (unsigned int i = 0; i < bucket->size && ok == 0; i++) {
		if (ht->compare_function(key, ((info *)node_t->data)->key) == 0) {
            node = node_t;
            ok = 1;
        }
		if(ok == 0)
        	n++;
		node_t  = node_t->next;
	}
	if (node) {
		temp_node = ll_remove_nth_node(bucket, n);
		free(((info *)temp_node->data)->value);
		free(((info *)temp_node->data)->key);
		free(temp_node->data);
		free(temp_node);
		ht->size--;
	}
}

// frees the hashtable
void
ht_free(hashtable_t *ht)
{
    ll_node_t *node;
	for (unsigned int i = 0; i < ht->hmax; i++) {
		if (ht->buckets[i]->head != NULL) {
			node = ht->buckets[i]->head;
			while (node != NULL) {
				free(((info *)node->data)->value);
				free(((info *)node->data)->key);
				node = node->next;
			}
		}
		ll_free(&ht->buckets[i]);
	}
	free(ht->buckets);
	free(ht);
}

// gets size of the hashtable
unsigned int
ht_get_size(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;
	return ht->size;
}

// gets number of buckets of the
// hashtable
unsigned int
ht_get_hmax(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;
	return ht->hmax;
}
