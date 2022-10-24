// Copyright 2022 Nioata Alexandra

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "utils.h"
#include "hashtable.h"
#include "library.h"
#include "user.h"
#define STRING_SIZE 50

int main()
{
    char *command;
    struct UserInfo user_inf;
    struct hashtable_t *library;
    library = ht_create(HMAX, hash_function_string, compare_function_strings);
    struct hashtable_t *users;
    users = ht_create(HMAX, hash_function_string, compare_function_strings);
    int number;
    float rating;
    char user_name[STRING_SIZE];
    while (1) {
        command = read_input();
        char *part = strtok(command, " ");
        if (strcmp(command, "ADD_BOOK") == 0) {
            char book_name[STRING_SIZE] = {0};
            part = strtok(NULL, " ");
            while (part) {
                strcat(book_name, part);
                strcat(book_name, " ");
                number = atoi(part);
                part = strtok(NULL, " ");
            }
            int count = count_num(number);;
            book_name[strlen(book_name) - count - 2] = '\0';
            add_book(library, book_name, number);
        } else if (strcmp(command, "GET_BOOK") == 0) {
            part = strtok(NULL, " ");
            char book_name[STRING_SIZE] = {0};
            while (part) {
                strcat(book_name, part);
                strcat(book_name, " ");
                part = strtok(NULL, " ");
            }
            book_name[strlen(book_name) - 1] = '\0';
            int has_book = ht_has_key(library, book_name);
            if (has_book == 1) {
                int index = ht_get_index(library, book_name);
                ll_node_t *node = library->buckets[index]->head;
                print_book(((info*)node->data)->value);
            } else {
                printf(NO_BOOK);
            }
        } else if (strcmp(command, "RMV_BOOK") == 0) {
            part = strtok(NULL, " ");
            char book_name[STRING_SIZE] = {0};
            while (part) {
                strcat(book_name, part);
                strcat(book_name, " ");
                part = strtok(NULL, " ");
            }
            book_name[strlen(book_name) - 1] = '\0';
            int has_book = ht_has_key(library, book_name);
            if (has_book == 1) {
                free_book(library, book_name);
                ht_remove_entry(library, book_name);
            } else {
                printf(NO_BOOK);
            }
        } else if (strcmp(command, "ADD_DEF") == 0) {
            char book_name[STRING_SIZE] = {0};
            char def_key[STRING_SIZE] = {0};
            char def_val[STRING_SIZE] = {0};
            part = strtok(NULL, " ");
            strcat(book_name, part);
            strcat(book_name, " ");
            int ok = 1;
            if (part[strlen(part) - 1] != '"') {
                part = strtok(NULL, " ");
                while (part && ok) {
                    strcat(book_name, part);
                    strcat(book_name, " ");
                    if (part[strlen(part) - 1] == '"') {
                        ok = 0;
                        part = strtok(NULL, " ");
                    } else {
                        part = strtok(NULL, " ");
                    }
                }
                book_name[strlen(book_name) - 1] = '\0';
                strcat(def_key, part);
                part = strtok(NULL, " ");
                strcat(def_val, part);
            } else {
               book_name[strlen(book_name) - 1] = '\0';
                part = strtok(NULL, " ");
                strcat(def_key, part);
                part = strtok(NULL, " ");
                strcat(def_val, part);
            }
            int has_book = ht_has_key(library, book_name);
            if (has_book == 1) {
                int index = ht_get_index(library, book_name);
                ll_node_t *node = library->buckets[index]->head;
                add_definiton(((info*)node->data)->value, def_key, def_val);
            } else {
                printf(NO_BOOK);
            }
        } else if (strcmp(command, "GET_DEF") == 0) {
            part = strtok(NULL, " ");
            char book_name[STRING_SIZE] = {0};
            char def_name[STRING_SIZE] = {0};
            while (part) {
                strcat(book_name, part);
                strcat(book_name, " ");
                strcpy(def_name, part);
                part = strtok(NULL, " ");
            }
            book_name[strlen(book_name) - strlen(def_name) -2] = '\0';
            get_definition(library, book_name, def_name);
        } else if (strcmp(command, "RMV_DEF") == 0) {
            part = strtok(NULL, " ");
            char book_name[STRING_SIZE] = {0};
            char def_name[STRING_SIZE] = {0};
            while (part) {
                strcat(book_name, part);
                strcat(book_name, " ");
                strcpy(def_name, part);
                part = strtok(NULL, " ");
            }
            book_name[strlen(book_name) - strlen(def_name) -2] = '\0';
            remove_definition(library, book_name, def_name);
        } else if (strcmp(command, "ADD_USER") == 0) {
            part = strtok(NULL, " ");
            strcpy(user_name, part);
            int has_user = ht_has_key(users, user_name);
            if (has_user == 1) {
                printf(HAS_USER);
            } else {
                add_user(users, user_inf, user_name);
            }
        } else if (strcmp(command, "BORROW") == 0) {
            part = strtok(NULL, " ");
            strcpy(user_name, part);
            part = strtok(NULL, " ");
            char book_name[STRING_SIZE] = {0};
            strcat(book_name, part);
            strcat(book_name, " ");
            int ok = 1;
            if (part[strlen(part) - 1] != '"') {
                part = strtok(NULL, " ");
                while (part && ok) {
                    strcat(book_name, part);
                    strcat(book_name, " ");
                    if (part[strlen(part) - 1] == '"') {
                        ok = 0;
                        part = strtok(NULL, " ");
                    } else {
                        part = strtok(NULL, " ");
                    }
                }
                book_name[strlen(book_name) - 1] = '\0';
            } else {
                book_name[strlen(book_name) - 1] = '\0';
                part = strtok(NULL, " ");
                number = atoi(part);
            }
            number = atoi(part);
            borrow_user(library, users, user_name, book_name, number);
        } else if (strcmp(command, "RETURN") == 0) {
            char book_name[STRING_SIZE] = {0};
            part = strtok(NULL, " ");
            strcpy(user_name, part);
            part = strtok(NULL, " ");
            strcat(book_name, part);
            strcat(book_name, " ");
            int ok = 1;
            if (part[strlen(part) - 1] != '"') {
                part = strtok(NULL, " ");
                while (part && ok) {
                    strcat(book_name, part);
                    strcat(book_name, " ");
                    if (part[strlen(part) - 1] == '"') {
                        ok = 0;
                        part = strtok(NULL, " ");
                    } else {
                        part = strtok(NULL, " ");
                    }
                }
                book_name[strlen(book_name) - 1] = '\0';
                number = atoi(part);
                part = strtok(NULL, " ");
                rating = atoi(part);
            } else {
                book_name[strlen(book_name) - 1] = '\0';
                part = strtok(NULL, " ");
                number = atoi(part);
                part = strtok(NULL, " ");
                rating = atoi(part);
            }
            return_user(library, users, user_name, book_name, number, rating);
        } else if (strcmp(command, "LOST") == 0) {
            part = strtok(NULL, " ");
            strcpy(user_name, part);
            part = strtok(NULL, " ");
            char book_name[STRING_SIZE] = {0};
            while (part) {
                strcat(book_name, part);
                strcat(book_name, " ");

                number = atoi(part);
                part = strtok(NULL, " ");
            }
            book_name[strlen(book_name) - 1] = '\0';
            lost_user(library, users, user_name, book_name);
        } else if (strcmp(command, "EXIT") == 0) {
            print_free_everything(library, users);
            free(command);
            break;
        }
        free(command);
    }
    return 0;
}
