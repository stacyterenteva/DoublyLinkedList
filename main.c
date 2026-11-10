#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "cgraph.h"

const int PZ = 2718;
const int MIN_NUM_OF_ELEMENTS = 5;
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
    DDL_ERROR_PROBLEMS_WITH_FILE
};

struct ddl_t
{
    int* list;
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
void print_list_graphic(ddl_t* list, FILE* graph);
void ddl_push_back(ddl_t* list, int element);
ddl_errors_t ddl_verify(ddl_t* list);
// ddl_iterator
int ddl_get_iterator(ddl_t* list, int num);
// ddl_iterator_end
// ddl_iterator_begin
// ddl_iterator_next
// ddl_iterator_prev
int ddl_size(ddl_t* list);
FILE* open_logfile();
void close_logfile(FILE* logfile);
ddl_t ddl_create_list();

int main()
{

    FILE* logfile = open_logfile();
    if (logfile == NULL) {
        return 1;
    }

    int list[] = {PZ, 10, 20, 30, 40, 50, PZ,  PZ,  PZ};
    int next[] = {0,  2,  3,   4,  5,  -1, 7, 8, 9};
    int prev[] = {0,  -1,  1,   2,  3,  4, -1, -1, -1};

    int wrong_prev[] = {0,  -1,  1,   4,  3,  4, -1, -1, -1};

    ddl_t list1 = {.list = list, .next = next, .prev = prev, .free = 6, .head = 1, .tail = 5, .capacity = 8};

    ddl_t list2 = ddl_create_list();

    ddl_dump(&list2, logfile);

    //ddl_push_back(&list2, 5);
    //ddl_dump(&list2, logfile);

    ddl_t list3 = {.list = list, .next = next, .prev = wrong_prev, .free = 6, .head = 1, .tail = 5, .capacity = 8};

    ddl_dump(&list1, logfile);

    ddl_dump(&list3, logfile);

    ddl_insert(&list1, 35, 3);
    ddl_dump(&list1, logfile);

    ddl_erase(&list1, 3);
    ddl_dump(&list1, logfile);

    fprintf(logfile, "</body>\n"
                      "</html>\n");

    //NOTE: error: permisiion denied, if i call system in dump

    system("dot -Tsvg graph1.dot -o graph1.svg");
    system("dot -Tsvg graph2.dot -o graph2.svg");
    system("dot -Tsvg graph3.dot -o graph3.svg");
    system("dot -Tsvg graph4.dot -o graph4.svg");
    system("dot -Tsvg graph5.dot -o graph5.svg");
    system("dot -Tsvg graph6.dot -o graph6.svg");

    return 0;
}

void ddl_insert(ddl_t* list, int element, int prev)
{
    assert(list);

    list->list[list->free] = element;
    list->next[prev] = list->free;
    list->next[list->free] = prev + 1;
    list->prev[list->free] = prev;
    list->prev[prev + 1] = list->free;
    list->free = list->next[list->free];
}

void ddl_erase(ddl_t* list, int num)
{
    assert(list);

    list->list[num] = PZ;
    list->next[list->prev[num]] = list->next[num];
    list->prev[list->next[num]] = list->prev[num];
    list->next[num] = -1;
    list->prev[num] = -1;
}

ddl_errors_t ddl_dump(ddl_t* list, FILE* logfile)
{
    assert(list);
    assert(logfile);

    static int num_of_call = 0;
    num_of_call++;

    char name_of_file[MAX_STRLEN] = {};
    snprintf(name_of_file, MAX_STRLEN, "graph%d.dot", num_of_call);

    FILE* graph = fopen(name_of_file, "w");
    if (graph == NULL) {
        perror("Error: "); //TODO: error id
        return DDL_ERROR_PROBLEMS_WITH_FILE;
    }

    fprintf(logfile, "<br>\n");

    fprintf(logfile, "<table border = \"1\">\n"
                      "<thead>\n"
                      "<tr>\n"
                      "<th>index</th>\n"
                      "<th>data</th>\n"
                      "<th>next</th>\n"
                      "<th>prev</th>\n"
                      "</tr>\n"
                      "</thead>\n"
                      "<tbody>\n");

    for (int i = 0; i < 8; i++) {
                    fprintf(logfile, "<tr>\n"
                    "  <td>%d</td>\n"
                    "  <td>%d</td>\n"
                    "  <td>%d</td>\n"
                    "  <td>%d</td>\n"
                    "</tr>\n", i, list->list[i], list->next[i], list->prev[i]);
    }

    fprintf(logfile, "</tbody>\n"
                      "</table>\n");

    fprintf(logfile, "<br>\n");

    fprintf(graph, "digraph {\n");

    fprintf(graph, "    rankdir=\"LR\"\n\n");
    fprintf(graph, "    bgcolor=\"#00000000\"\n\n");
    fprintf(graph, "    splines=ortho\n");

    //print_list_graphic(list, graph);


    for (int i = 0; i < list->capacity; i++) {
        if (i == 0) {
            fprintf(graph, "    node%d [shape=Mrecord, style=filled, fillcolor=\"#baab0a\", color=\"#363201\", label=\"index = %d | next = %d | data = %d | prev = %d\"];\n", i, i, list->next[i], list->list[i], list->prev[i]);
        }
        else if (list->list[i] == PZ) {
            fprintf(graph, "    node%d [shape=Mrecord, style=filled, fillcolor=\"#0ca2a2\", color=\"#012d34\", label=\"index = %d | next = %d | data = %d | prev = %d\"];\n", i, i, list->next[i], list->list[i], list->prev[i]);
        }

        else {
            fprintf(graph, "    node%d [shape=Mrecord, style=filled, fillcolor=\"#9ce4ad\", color=\"#035603\", label=\"index = %d | next = %d | data = %d | prev = %d\"];\n", i, i, list->next[i], list->list[i], list->prev[i]);
        }
    }

    //TODO: print_list like img
    for (int i = 0; i < list->capacity - 1; i++) {
        fprintf(graph, "    node%d -> node%d [color = \"#00000000\", weight=2000];\n", i, i + 1);
    }

    for (int i = list->head; i != -1; i = list->next[i]) {
        if (list->prev[list->next[i]] == i) {
            fprintf(graph, "    node%d -> node%d [color = \"#06074f68\", constraint=false, weight = 100, dir=\"both\"];\n", i, list->next[i]);
        }

        else if (list->next[i] != -1) {
            fprintf(graph, "    node%d -> node%d [color = \"#ef0a0a68\", constraint=false, weight = 100];\n", i, list->next[i]);
        }
        else if (list->prev[i] != -1) {
            //fprintf(graph, "    node%d -> node%d [color = \"#ef0a0a68\", constraint=false, weight = 100];\n", i, list->prev[i]);
        }
        /*
        if (list->list[i] != PZ && list->prev[i] != -1) {
            fprintf(graph, "    node%d -> node%d [color = \"#08590e68\", constraint=false, weight = 50];\n", i, list->prev[i]);
        }*/
    }

    fprintf(graph, "}");

    print_list(list, logfile);

    char comand_for_dot[MAX_STRLEN] = {};
    //snprintf(comand_for_dot, MAX_STRLEN, "dot -Tsvg C:\\C\\DoublyLinkedList\\graph%d.dot -o graph%d.svg", num_of_call, );
    //snprintf(comand_for_dot, MAX_STRLEN, "dot -Tsvg graph%d.dot -o graph%d.svg", num_of_call, num_of_call);
    //fprintf(stderr, "%s\n", comand_for_dot);
    //int err = system(comand_for_dot);

    fprintf(logfile,  "<br>\n"
                      "<br>\n"
                      "<img src = \"graph%d.svg\">\n", num_of_call);

/*
    if (err == -1) {
        fprintf(stderr, "Error during system call");
    }
*/

    fclose(graph);
    return DDL_ERROR_SUCCESS;
}


void print_list(ddl_t* list, FILE* logfile)
{
    assert(list);
    assert(logfile);

    fprintf(logfile, "<br\n>");
    fprintf(logfile, "Now the list is looks like:\n");
    for (int i = list->head; i != -1; i = list->next[i]) {
        fprintf(logfile, "%d ", list->list[i]);
    }
    fprintf(logfile, "\n");
}

void print_list_graphic(ddl_t* list, FILE* graph)
{
    assert(list);
    assert(graph);

    for (int i = list->head; i != -1 && list->next[i] != -1; i = list->next[i]) {
        fprintf(graph, "    node%d -> node%d [color = \"#18d90d00\"];\n", i, list->next[i]);
    }
}

ddl_errors_t ddl_verify(ddl_t* list)
{

    if (list->head < 0 ) {
        return DDL_ERROR_NEGATIVE_HEAD;
    }
    else if (list->tail < 0) {
        return DDL_ERROR_NEGATIVE_TAIL;
    }
    else if (list->tail < 0) {
        return DDL_ERROR_NEGATIVE_TAIL;
    }
    return DDL_ERROR_SUCCESS;
}

void ddl_push_back(ddl_t* list, int element)
{
    ddl_insert(list, element, list->tail);
    list->tail = list->next[list->tail];
    printf("%d", list->tail);
    getchar();
}

int ddl_get_iterator(ddl_t* list, int num)
{
    assert(list);
    if (num < 0 || num > ddl_size(list)) {
        return -1;
    }
    int counter = 0;
    int i = 0;
    for (i = list->head; counter != num && i != list->tail; i = list->next[i]) {
        counter++;
    }
    return i;
}

int ddl_size(ddl_t* list)
{
    int counter = 0;
    if (list->head == list->tail) {
        return 1;
    }
    for (int i = list->head; i != -1; i = list->next[i]) {
        counter++;
    }
    return counter;
}

ddl_t ddl_create_list()
{
    int* data = (int*) calloc(MIN_NUM_OF_ELEMENTS, sizeof(int));
    int* next = (int*) calloc(MIN_NUM_OF_ELEMENTS, sizeof(int));
    int* prev = (int*) calloc(MIN_NUM_OF_ELEMENTS, sizeof(int));
    ddl_t list = {.list = data, .next = next, .prev = prev, .free = 1, .head = 0, .tail = 0, .capacity = 1};
    return list;
}

void ddl_erase_by_iterator(ddl_t* list, int num)
{
    ddl_erase(list, ddl_get_iterator(list, num));
}

FILE* open_logfile()
{

    FILE* logfile = fopen("logfile.html", "w");
    if (logfile == NULL) {
        perror("Error: ");
        return NULL;
    }

    fprintf(logfile, "<!DOCTYPE html>\n"
                    "<html>\n"

                    "<head>\n"
                    "<title> logfile for graph </title>\n"
                    "</head>\n"

                    "<body>\n");

    return logfile;
}

void close_logfile(FILE* logfile)
{
    fprintf(logfile, "</body>\n"
                      "</html>\n");
    fclose(logfile);
}
