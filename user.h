// Copyright 2022 Nioata Alexandra

#ifndef USER_H_
#define USER_H_
#include "utils.h"
#include "hashtable.h"
#include "functions.h"

void sort_users
(linked_list_t *);

void show_users
(hashtable_t *);

void add_struct_user
(linked_list_t *, unsigned int, struct UserInfo *);

void add_user
(hashtable_t *, struct UserInfo , char *);

void return_user
(hashtable_t *, hashtable_t *, char *, char *, int, int);

void lost_user
(hashtable_t *, hashtable_t *, char *, char *);

void borrow_user
(hashtable_t *, hashtable_t *, char *, char *, int);

#endif  //  USER_H_
