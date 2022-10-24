// Copyright 2022 Nioata Alexandra

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "functions.h"
#include "hashtable.h"
#include "library.h"
#include "user.h"

// reads a line
char*
read_input(void)
{
	char *command;
	command = malloc(COMMAND_SIZE);
	fgets(command, COMMAND_SIZE, stdin);
	if (command[strlen(command) - 1] == '\n')
		command[strlen(command) - 1] = '\0';
	return command;
}

// swap 2 variables that are floats
void swap_floats(float *xp, float *yp)
{
    float temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// swap2 variables that are ints
void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// counts the digits from a number
int count_num(int x) {
    int count = 0;
    do {
        x = x / 10;
        count++;
    }while(x);
    return count;
}

// prints the books from the library an the users
// frees the hashtables
void print_free_everything(hashtable_t *library, hashtable_t *users) {
    printf("Books ranking:\n");
            show_all_books(library);
            printf("Users ranking:\n");
            show_users(users);
            int size = ht_get_hmax(library);
            for (int i = 0; i < size; i++) {
                ll_node_t* node = library->buckets[i]->head;
                while (node != NULL) {
                    struct info* inf = (info*)node->data;
                    struct BookInfo* book_inf  = (struct BookInfo *)inf->value;
                    ht_free(book_inf->book);
                    node = node->next;
                }
            }
            ht_free(library);
            ht_free(users);
}
