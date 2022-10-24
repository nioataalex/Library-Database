// Copyright 2022 Nioata Alexandra

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "utils.h"
#include "hashtable.h"

// read every line with definitions
// puts in hashtable
void
add_def(hashtable_t *book, int number)
{
    char def_key[WORD_SIZE], def_val[WORD_SIZE];
    while (number) {
        char *line = read_input();
        char *part = strtok(line, " ");
        strcpy(def_key, part);
        part = strtok(NULL, " ");
        strcpy(def_val, part);
        int length_key = strlen(def_key) + 1;
        int length_val = strlen(def_val) + 1;
        ht_put(book, def_key, length_key , def_val, length_val);
        number--;
		free(line);
    }
}

// print a book with the given format
void print_book(struct BookInfo *b_info)
{
    float final_rating = 0;
    char new_name[STRING_SIZE] = {0};
    if (b_info->name[strlen(b_info->name) - 1] == '"')
        b_info->name[strlen(b_info->name) - 1] = '\0';
    strcpy(new_name, b_info->name + 1);
    if (b_info->purchase != 0)
        final_rating = (b_info->rating) / b_info->purchase;
	printf("Name:%s Rating:%.3f ", new_name, final_rating);
    printf("Purchases:%d\n", b_info->purchase);
}

// verifies if the definition exits
// prints the definition
void print_def(struct BookInfo *b_info, char *def_name)
{
    int has_def = ht_has_key(b_info->book, def_name);
    if (has_def == 1) {
        int index = ht_get_index(b_info->book, def_name);
        ll_node_t *node = b_info->book->buckets[index]->head;
        while (node) {
            char *key = (char *)((info*)node->data)->key;
            if (strcmp(key, def_name) == 0) {
                printf("%s\n", (char *)(((info*)node->data)->value));
                break;
            }
            node = node->next;
        }
    } else {
        printf(NO_DEFINITION);
    }
}

// add a new definition to the book
void add_definiton(struct BookInfo *b_info, char *def_key, char *def_val)
{
    ht_put(b_info->book, def_key, strlen(def_key) + 1,
                def_val, strlen(def_val) + 1);
}

// verifies is the definitions exists
// removes the definition from the book
void remove_def(struct BookInfo *b_info, char *def_name)
{
    int has_def = ht_has_key(b_info->book, def_name);
    if (has_def == 1) {
        int index = ht_get_index(b_info->book, def_name);
        ll_node_t *node = b_info->book->buckets[index]->head;
        while (node) {
            char *key = (char *)((info*)node->data)->key;
            if (strcmp(key, def_name) == 0) {
                ht_remove_entry(b_info->book, def_name);
                break;
            }
            node = node->next;
        }
    } else {
        printf(NO_DEFINITION);
    }
}

// sort books
// first after rating, if the rating is equal
// it will be sorted after purchases
// if the purchases are equal too, they will be sorted after
// their name
void sort_books(linked_list_t *books) {
        ll_node_t *curr = books->head, *ind = NULL;
        char temp[STRING_SIZE] = {0};
        if (books->head == NULL) {
            return;
        } else {
            while (curr != NULL) {
                ind = curr->next;
                while (ind != NULL) {
                    struct BookInfo *curr_inf = (struct BookInfo *) curr->data;
                    struct BookInfo *ind_info = (struct BookInfo *) ind->data;
                    float rating1;
                    float rating2;
                    if (curr_inf->purchase != 0)
                            rating1 = curr_inf->rating / curr_inf->purchase;
                    else
                        rating1 = curr_inf->rating;
                    if (ind_info->purchase != 0)
                            rating2 = ind_info->rating / ind_info->purchase;
                    else
                        rating2 = ind_info->rating;
                    if (rating1 < rating2) {
                        strcpy(temp, curr_inf->name);
                        strcpy(curr_inf->name, ind_info->name);
                        strcpy(ind_info->name, temp);
                        swap_floats(&curr_inf->rating, &ind_info->rating);
                        swap(&curr_inf->purchase, &ind_info->purchase);
                    } else if (rating1 == rating2) {
                        if (curr_inf->purchase < ind_info->purchase) {
                            strcpy(temp, curr_inf->name);
                            strcpy(curr_inf->name, ind_info->name);
                            strcpy(ind_info->name, temp);
                            swap_floats(&curr_inf->rating,
                                                &ind_info->rating);
                            swap(&curr_inf->purchase,
                                                &ind_info->purchase);
                        } else if (curr_inf->purchase == ind_info->purchase) {
                            if (strcmp(curr_inf->name,
                                            ind_info->name) > 0) {
                                strcpy(temp, curr_inf->name);
                                strcpy(curr_inf->name, ind_info->name);
                                strcpy(ind_info->name, temp);
                                swap_floats(&curr_inf->rating,
                                                &ind_info->rating);
                                swap(&curr_inf->purchase,
                                    &ind_info->purchase);
                            }
                        }
                    }
                    ind = ind->next;
                }
                curr = curr->next;
            }
        }
}

// add a struct to a linked list
void add_struct_book(linked_list_t *list, unsigned int n, struct BookInfo *info)
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
    new_node->data = malloc(sizeof(struct BookInfo));
	DIE(!new_node->data, "malloc data");
	memcpy(new_node->data, info, sizeof(struct BookInfo));
    new_node->next = curr;
    if (prev == NULL) {
        list->head = new_node;
    } else {
        prev->next = new_node;
    }
    list->size++;
}

// makes a linked list with the books
// sorts them
// prints them with the  given format
void show_all_books(hashtable_t *library)
{
    int size = ht_get_hmax(library);
    linked_list_t *books = ll_create(sizeof(linked_list_t *));
    for (int i = 0; i < size; i++) {
        ll_node_t* node = library->buckets[i]->head;
        if (node != NULL) {
        struct info* inform = (info*)node->data;
        struct BookInfo* book_inf  = (struct BookInfo *)inform->value;
        add_struct_book(books, 1, book_inf);
        }
    }
    sort_books(books);
    ll_node_t* node = books->head;
    int i = 1;
	for (; node; node = node->next) {
        printf("%d. ", i);
        print_book((struct BookInfo*)node->data);
        i++;
    }
    ll_free(&books);
}

// frees a book from the library with the given name
void free_book(hashtable_t *library, char *book_name)
{
    int index = ht_get_index(library, book_name);
    ll_node_t *node1 = library->buckets[index]->head;
    struct info* inform1 = (info*)node1->data;
    struct BookInfo* book_inf  = (struct BookInfo *)inform1->value;
    ht_free(book_inf->book);
}

// function for the remove_def command
// removes the given definition
// verifies  if the book exists
void remove_definition(hashtable_t *library, char *book_name, char *def_name)
{
    int has_book = ht_has_key(library, book_name);
    if (has_book == 1) {
        int index = ht_get_index(library, book_name);
        ll_node_t *node = library->buckets[index]->head;
        while (node) {
            char *key = (char *)((info*)node->data)->key;
            if (strcmp(key, book_name) == 0) {
                remove_def(((info*)node->data)->value, def_name);
                break;
            }
            node = node->next;
        }
    } else {
        printf(NO_BOOK);
    }
}

// function for get_def command
// prints the given definition
// verifies if the books exists
void get_definition(hashtable_t *library, char *book_name, char *def_name)
{
    int has_book = ht_has_key(library, book_name);
    if (has_book == 1) {
        int index = ht_get_index(library, book_name);
        ll_node_t *node = library->buckets[index]->head;
        print_def(((info*)node->data)->value, def_name);
    } else {
        printf(NO_BOOK);
    }
}

// function for add_book command
// verifies of the book exists or not
// if it exist it will be free
// put the book and his struct with infos in the hashtable
void add_book(hashtable_t *library, char* book_name, int number)
{
    struct BookInfo book_inf;
    int has_book = ht_has_key(library, book_name);
    if (has_book == 1) {
            free_book(library, book_name);
    }
    strcpy(book_inf.name, book_name);
    book_inf.book = ht_create(HMAX, hash_function_string,
                        compare_function_strings);
    add_def(book_inf.book, number);
    book_inf.rating = 0;
    book_inf.purchase = 0;
    book_inf.borrowed = 0;
    ht_put(library, book_inf.name, strlen(book_inf.name) + 1,
                &book_inf, sizeof(book_inf));
}
