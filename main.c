#include <stdio.h>
#include "cgraph.h"

const int pz = 2718;

#define RED "\033[031m"
#define RESET "\033[0m"

enum ddl_errors_t
{
    NULL_DESTROYED,
    NEGATIVE_HEAD,
    NEGATIVE_TAIL,
    INF_LIST,
    WRONG_HASH,
    OK
};

struct ddl_t
{
    int* list;
    int* next;
    int* prev;
    int head;
    int tail;
    int hash;
};

void add_new_element(ddl_t* list, int element, int prev);
void delete_element(ddl_t* list, int num);
void ddl_dump(ddl_t* list, FILE* log_file);
void print_list(ddl_t* list);
ddl_errors_t ddl_verify(ddl_t* list);

int main()
{
    int list[] = {pz, 10, 20, 30, 40, 50, pz,  pz,  pz};
    int next[] = {1,  2,  3,   4,  5,  0, -1, -1, -1};
    int prev[] = {0,  0,  1,   2,  3,  4, -1, -1, -1};

    //35 beetween 30 and 40

    FILE* log_file = fopen("log_file.dot", "w");


    ddl_t list1 = {.list = list, .next = next, .prev = prev, .head = 0, .tail = 5};

    //print_list(&list1);
    ddl_dump(&list1);
    printf("\n\n");

    add_new_element(&list1, 35, 3);
    ddl_dump(&list1);
    printf("\n\n");

    delete_element(&list1, 2);
    ddl_dump(&list1);

    return 0;
}



void add_new_element(ddl_t* list, int element, int prev)
{
    list->list[++list->tail] = element;
    list->next[prev] = list->tail;
    list->next[list->tail] = prev + 1;
}

void delete_element(ddl_t* list, int num)
{
    list->list[num] = pz;
    list->next[list->prev[num]] = list->next[num];
    list->next[num] = -1;
}

void ddl_dump(ddl_t* list)
{
    printf(RED "---------------------------\n" RESET);
    printf("head = %d\n", list->head);
    printf("tail = %d\n\n", list->tail);

    printf("num | list | next | prev\n");
    printf("---------------------------\n");
    for (int i = 0; i < 8; i++) {
        printf("%3d | %4d | %4d | %4d  \n", i, list->list[i], list->next[i], list->prev[i]);
    }

    //printf("\n");
    print_list(list);
    printf(RED "---------------------------\n" RESET);


}


void print_list(ddl_t* list)
{
    printf("Now the list is looks like:\n");
    for (int i = list->head + 1; i <= list->tail && i != 0; i = list->next[i]) {
        printf("%d ", list->list[i]);
    }
    putchar('\n');
}

ddl_errors_t ddl_verify(ddl_t* list)
{

    if (list->head < 0 ) {
        return NEGATIVE_HEAD;
    }
    else if (list->tail < 0) {
        return NEGATIVE_TAIL;
    }
    else if (list->tail < 0) {
        return NEGATIVE_TAIL;
    }
    return OK;
}
