// Copyright 2022 Nioata Alexandra

#ifndef LIBRARY_H_
#define LIBRARY_H_
#include "utils.h"
#include "hashtable.h"
#include "functions.h"

void
add_def(hashtable_t *, int);

void print_book(struct BookInfo *);

void print_def(struct BookInfo *, char*);

void add_definiton
(struct BookInfo *, char *, char *);

void remove_def
(struct BookInfo *, char *);

void show_all_books
(hashtable_t *);

void sort_books
(linked_list_t *);

void add_struct_book
(linked_list_t *, unsigned int, struct UserInfo *);

void free_book
(hashtable_t *, char *);

void remove_definition
(hashtable_t *, char *, char *);

void get_definition
(hashtable_t *, char *, char *);

void add_book
(hashtable_t *, char *, int);
#endif  //  LIBRARY_H_
