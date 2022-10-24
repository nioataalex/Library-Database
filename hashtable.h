// Copyright 2022 Nioata Alexandra

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#define HMAX 1000
#define MAX_STRING_SIZE 64

typedef struct ll_node_t ll_node_t;
struct ll_node_t
{
	void* data;
	ll_node_t* next;
};

typedef struct linked_list_t linked_list_t;
struct linked_list_t
{
	ll_node_t* head;
	unsigned int data_size;
	unsigned int size;
};

typedef struct info info;
struct info {
	void *key;
	void *value;
};

typedef struct hashtable_t hashtable_t;
struct hashtable_t {
	linked_list_t **buckets;
	unsigned int size;
	unsigned int hmax;
	unsigned int (*hash_function)(void*);
	int (*compare_function)(void*, void*);
};


linked_list_t*
ll_create(unsigned int);

void
ll_add_nth_node(linked_list_t* , unsigned int, const void*);

ll_node_t*
ll_remove_nth_node(linked_list_t* , unsigned int);

unsigned int
ll_get_size(linked_list_t*);

void
ll_free(linked_list_t**);

int
compare_function_ints(void* , void*);

int
compare_function_strings(void*, void*);

unsigned int
hash_function_int(void*);

unsigned int
hash_function_string(void*);

hashtable_t *
ht_create(unsigned int, unsigned int (*)(void*), int (*)(void*, void*));

int
ht_has_key(hashtable_t* , void*);

void *
ht_get(hashtable_t*, void*);

void
ht_put(hashtable_t*, void*, unsigned int, void*, unsigned int);

void
ht_remove_entry(hashtable_t*, void*);

void
ht_free(hashtable_t*);

unsigned int
ht_get_size(hashtable_t*);

unsigned int
ht_get_hmax(hashtable_t*);

int
ht_get_index(hashtable_t *ht, void *key);

#endif  // HASHTABLE_H_
