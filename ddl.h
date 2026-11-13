#ifndef DDL_H
#define DDL_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "cgraph.h"

const int PZ = 2718;
const int MIN_NUM_OF_ELEMENTS = 8;
const int MAX_STRLEN = 100;

#define RED "\033[031m"
#define RESET "\033[0m"

enum ddl_errors_t
{
    DDL_ERROR_NULL_DESTROYED,
    DDL_ERROR_NEGATIVE_HEAD,
    DDL_ERROR_NEGATIVE_TAIL,
    DDL_ERROR_INF_LIST,
    DDL_ERROR_SUCCESS,
    DDL_ERROR_PROBLEMS_WITH_FILE,
    DDL_ERROR_WRONG_LIST_PTR
};

struct ddl_t
{
    int* data;
    int* next;
    int* prev;
    int free;
    int head;
    int tail;
    int capacity;
};

typedef size_t ddl_iterator_t;

void ddl_insert(ddl_t* list, int element, int prev);
void ddl_erase(ddl_t* list, int num);
void ddl_erase_by_iterator(ddl_t* list, int num);
ddl_errors_t ddl_dump(ddl_t* list, FILE* log_file);
void print_list(ddl_t* list, FILE* log_file);
//void print_list_graphic(ddl_t* list, FILE* graph);
void ddl_push_back(ddl_t* list, int element);
ddl_errors_t ddl_verify(ddl_t* list);
int ddl_iterator(ddl_t* list, int num);
int ddl_iterator_end(ddl_t* list);
int ddl_iterator_begin(ddl_t* list);
int ddl_iterator_next(ddl_t* list, int num);
int ddl_iterator_prev(ddl_t* list, int num);
int ddl_size(ddl_t* list);
FILE* open_logfile();
void close_logfile(FILE* logfile);
ddl_t ddl_create_list();

#endif /* DDL_H */
