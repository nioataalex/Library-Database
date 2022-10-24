// Copyright 2022 Nioata Alexandra

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "utils.h"
#include "hashtable.h"

// sorts the users after the number of points
// if the points are equal they
// will be sorted after their name
void sort_users(linked_list_t *users)
{
    ll_node_t *current = users->head, *index = NULL;
    char temp[STRING_SIZE] = {0};
    if (users->head == NULL) {
            return;
    } else {
        while (current != NULL) {
            index = current->next;
            while (index != NULL) {
                struct UserInfo *curr_info = (struct UserInfo *) current->data;
                struct UserInfo *index_info = (struct UserInfo *) index->data;
                if (curr_info->points < index_info->points){
                    strcpy(temp, curr_info->name);
                    strcpy(curr_info->name, index_info->name);
                    strcpy(index_info->name, temp);
                    swap(&curr_info->points, &index_info->points);
                } else if (curr_info->points == index_info->points) {
                    if (strcmp(curr_info->name, index_info->name) > 0) {
                        strcpy(temp, curr_info->name);
                        strcpy(curr_info->name, index_info->name);
                        strcpy(index_info->name, temp);
                    }
                }
                index = index->next;
            }
            current = current->next;
        }
    }
}

// add a struct to a linked list
void add_struct_user(linked_list_t *list, unsigned int n, struct UserInfo *info)
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
    new_node->data = malloc(sizeof(struct UserInfo));
	DIE(!new_node->data, "malloc data");
	memcpy(new_node->data, info, sizeof(struct UserInfo));
    new_node->next = curr;
    if (prev == NULL) {
        list->head = new_node;
    } else {
        prev->next = new_node;
    }
    list->size++;
}

// makes linked list with the users
// verifies if the user is banned or not
// prints the user in the given order and format
void show_users(hashtable_t *users)
{
    int size = ht_get_hmax(users);
    linked_list_t *top_users = ll_create(sizeof(linked_list_t *));
    for (int i = 0; i < size; i++) {
        ll_node_t* node = users->buckets[i]->head;
        if (node != NULL) {
            struct info* inform = (info*)node->data;
            struct UserInfo* user_inf  = (struct UserInfo *)inform->value;
            if (user_inf->points > 0)
                add_struct_user(top_users, 1, (struct UserInfo *)user_inf);
        }
    }
    sort_users(top_users);
    ll_node_t* node = top_users->head;
    int i = 1;
	for (; node; node = node->next) {
        printf("%d. ", i);
        printf("Name:");
        printf("%s ", ((struct UserInfo*)node->data)->name);
        printf("Points:");
        printf("%d", ((struct UserInfo*)node->data)->points);
        i++;
	    printf("\n");
    }
	ll_free(&top_users);
}

// add user in a hashtable
void add_user(hashtable_t *users, struct UserInfo user_inf, char *user_name)
{
    user_inf.points = 100;
    user_inf.borrowed_book = 0;
    user_inf.days = 0;
    user_inf.banned = 0;
    strcpy(user_inf.name, user_name);
    ht_put(users, user_name, strlen(user_name) + 1,
            &user_inf, sizeof(user_inf));
}

// verifies all the cases for the return command
void return_user
(hashtable_t *library, hashtable_t *users,
    char *user_name, char *book_name, int number, int rating)
{
    int index = ht_get_index(users, user_name);
    ll_node_t *node = users->buckets[index]->head;
    struct info* inform = (info*)node->data;
    struct UserInfo* user_inf  = (struct UserInfo *)inform->value;
    if (user_inf->banned == 1) {
            printf(BANNED_USER);
    } else {
        if (user_inf->points < 0) {
            user_inf->banned = 1;
            printf(BANNED_USER);
        } else if (user_inf->borrowed_book == 0) {
            printf(NO_BORROWED_BOOK);
        } else if (strcmp(user_inf->book_name, book_name) != 0) {
            printf(NO_BORROWED_BOOK);
        } else {
            if (user_inf->days < number) {
                int late_days = number - user_inf->days;
                user_inf->points -= late_days * 2;
                if (user_inf->points < 0) {
                    user_inf->banned = 1;
                    printf("The user %s has ", user_name);
                    printf("been banned from this library.\n");
                }
            } else if (user_inf->days > number) {
                int no_late_days = user_inf->days - number;
                user_inf->points += no_late_days;
            }
            index = ht_get_index(library, book_name);
            ll_node_t *node1 = library->buckets[index]->head;
            struct info* inf = (info*)node1->data;
            struct BookInfo* book_inf  = (struct BookInfo *)inf->value;
            book_inf->borrowed = 0;
            user_inf->borrowed_book = 0;
            book_inf->rating += rating;
            book_inf->purchase++;
        }
    }
}

// verifies all the cases for lost command
void lost_user
(hashtable_t *library, hashtable_t *users,
    char *user_name, char *book_name)
{
    int has_user = ht_has_key(users, user_name);
    if (has_user == 0) {
        printf(NO_USER);
    } else {
        int index = ht_get_index(users, user_name);
        ll_node_t *node = users->buckets[index]->head;
        struct info* inform = (info*)node->data;
        struct UserInfo* user_inf  = (struct UserInfo *)inform->value;
        if (user_inf->points < 0) {
            printf(BANNED_USER);
            user_inf->banned = 1;
        } else {
            index = ht_get_index(library, book_name);
            ll_node_t *node1 = library->buckets[index]->head;
            struct info* inf = (info*)node1->data;
            struct BookInfo* book_inf  = (struct BookInfo *)inf->value;
            ht_free(book_inf->book);
            user_inf->points -= 50;
            if (user_inf-> points < 0) {
                printf("The user %s has ", user_name);
                printf("been banned from this library.\n");
            }
            ht_remove_entry(library, book_name);
            user_inf->borrowed_book = 0;
        }
    }
}

// verifies all the cases for borrow command
void borrow_user
(hashtable_t *library, hashtable_t *users,
    char *user_name, char *book_name, int number)
{
    int has_user = ht_has_key(users, user_name);
    if (has_user == 0) {
        printf(NO_USER);
    } else {
        int index = ht_get_index(users, user_name);
        ll_node_t *node = users->buckets[index]->head;
        struct info* inform = (info*)node->data;
        struct UserInfo* user_inf  = (struct UserInfo *)inform->value;
        if (user_inf->points < 0) {
            printf(BANNED_USER);
        } else {
            int has_book = ht_has_key(library, book_name);
            if (has_book == 0) {
                printf(NO_BOOK);
            } else {
                index = ht_get_index(library, book_name);
                ll_node_t *node1 = library->buckets[index]->head;
                struct info* inf = (info*)node1->data;
                struct BookInfo* book_inf  = (struct BookInfo *)inf->value;
                if (user_inf->borrowed_book != 0) {
                    printf(HAS_BOOK);
                } else if (book_inf->borrowed != 0) {
                    printf(BORROWED_BOOK);
                } else  {
                    book_inf->borrowed = 1;
                    user_inf->borrowed_book = 1;
                    user_inf->days = number;
                    strcpy(user_inf->book_name, book_name);
                }
            }
        }
    }
}
