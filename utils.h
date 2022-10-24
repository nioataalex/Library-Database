// Copyright 2022 Nioata Alexandra

#ifndef UTILS_H_
#define UTILS_H_
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
// useful macro for handling error codes
// and for defines
#define BOOK_SIZE 40
#define USER_SIZE 20
#define STRING_SIZE 50
#define COMMAND_SIZE 60
#define WORD_SIZE 20
#define NO_BOOK "The book is not in the library.\n"
#define NO_DEFINITION "The definition is not in the book.\n"
#define HAS_USER "User is already registered.\n"
#define NO_USER "You are not registered yet.\n"
#define BANNED_USER "You are banned from this library.\n"
#define BORROWED_BOOK "The book is borrowed.\n"
#define HAS_BOOK "You have already borrowed a book.\n"
#define NO_BORROWED_BOOK "You didn't borrow this book.\n"
#define DIE(assertion, call_description)                                       \
    do {                                                                       \
        if (assertion) {                                                       \
            fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);                 \
            perror(call_description);                                          \
            exit(errno);                                                       \
        }                                                                      \
    } while (0)
#endif  //  UTILS_H_
