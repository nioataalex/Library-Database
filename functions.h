// Copyright 2022 Nioata Alexandra

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include "utils.h"
#include "hashtable.h"

// struct for  a book
struct BookInfo {
    char name[BOOK_SIZE];
    float rating;
    int purchase;
    float final_rating;
    hashtable_t* book;
    // saves if the book is borrowed or not
    int borrowed;
};

// struct for a user
struct UserInfo {
    char name[USER_SIZE];
    int points;
    int days;
    // saves if the user borroed a book
    int borrowed_book;
    // saves if the user is banned or not
    int banned;
    // saves the name of the borrowed book
    char book_name[STRING_SIZE];
};

char*
read_input(void);

void swap(int *, int *);

void swap_floats(float *, float *);

int count_num(int);

void print_free_everything(hashtable_t *, hashtable_t *);
#endif  //  FUNCTIONS_H_
