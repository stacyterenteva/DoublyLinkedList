#include <stdio.h>
#include <stdlib.h>

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

struct elem_t
{
    int next;
    int value;
    int prev;
};

void add_new_element(ddl_t* list, int element, int prev);
void delete_element(ddl_t* list, int num);
void ddl_dump(ddl_t* list, FILE* graph, FILE* log_file);
void print_list(ddl_t* list);
void push_back(ddl_t* list, int element);
ddl_errors_t ddl_verify(ddl_t* list);

int main()
{
    int list[] = {pz, 10, 20, 30, 40, 50, pz,  pz,  pz};
    int next[] = {0,  2,  3,   4,  5,  -1, -1, -1, -1};
    int prev[] = {0,  -1,  1,   2,  3,  4, -1, -1, -1};

    //35 beetween 30 and 40

    ddl_t list1 = {.list = list, .next = next, .prev = prev, .head = 1, .tail = 5};

    FILE* logfile_1 = fopen("logfile_1.html", "w");
    FILE* graph1 = fopen("graph1.dot", "w");

    ddl_dump(&list1, graph1, logfile_1);
    system("dot -Tpng graph1.dot -o graph1.png");
    printf("\n\n");

    add_new_element(&list1, 35, 3);

    FILE* logfile_2 = fopen("logfile_2.html", "w");
    FILE* graph2 = fopen("graph2.dot", "w");

    ddl_dump(&list1, graph2, logfile_2);
    system("dot -Tpng graph2.dot -o graph2.png");
    printf("\n\n");

    delete_element(&list1, 2);

    FILE* logfile_3 = fopen("logfile_3.html", "w");
    FILE* graph3 = fopen("graph3.dot", "w");

    ddl_dump(&list1, graph3, logfile_3);
    system("dot -Tpng graph3.dot -o graph3.png");

    push_back(&list1, 60);
    //ddl_dump(&list1);

    return 0;
}


//free, not element after tail
void add_new_element(ddl_t* list, int element, int prev)
{
    list->list[list->tail + 1] = element;
    list->next[prev] = list->tail + 1;
    list->next[list->tail + 1] = prev + 1;
    list->prev[list->tail + 1] = prev;
    list->prev[prev + 1] = list->tail + 1;
}

void delete_element(ddl_t* list, int num)
{
    list->list[num] = pz;
    list->next[list->prev[num]] = list->next[num];
    list->next[num] = -1;
}

void ddl_dump(ddl_t* list, FILE* graph, FILE* log_file)
{
    fprintf(log_file, "<!DOCTYPE html>\n"
                    "<html>\n"

                    "<head>\n"
                    "<title> logfile for graph </title>\n"
                    "</head>\n"

                    "<body>\n"
                    "<pre>\n");

    printf(RED "---------------------------\n" RESET);
    printf("head = %d\n", list->head);
    printf("tail = %d\n\n", list->tail);

    printf("num | list | next | prev\n");
    printf("---------------------------\n");
    for (int i = 0; i < 8; i++) {
        printf("%3d | %4d | %4d | %4d  \n", i, list->list[i], list->next[i], list->prev[i]);
    }

    fprintf(graph, "digraph {\n");

    fprintf(graph, "    rankdir=\"LR\"\n\n");
    fprintf(graph, "    bgcolor=\"#f8f9fa\"\n\n");
    fprintf(graph, "    splines=ortho\n");

    for (int i = 0; i < 8; i++) {
        if (i == 0) {
            fprintf(graph, "    node%d [shape=Mrecord, style=filled, fillcolor=\"#baab0a\", color=\"#363201\", label=\"index = %d | next = %d | data = %d | prev = %d\"];\n", i, i, list->next[i], list->list[i], list->prev[i]);
        }
        else if (list->list[i] == pz) {
            fprintf(graph, "    node%d [shape=Mrecord, style=filled, fillcolor=\"#0ca2a2\", color=\"#012d34\", label=\"index = %d | next = %d | data = %d | prev = %d\"];\n", i, i, list->next[i], list->list[i], list->prev[i]);
        }
        else {
            fprintf(graph, "    node%d [shape=Mrecord, style=filled, fillcolor=\"#9ce4ad\", color=\"#035603\", label=\"index = %d | next = %d | data = %d | prev = %d\"];\n", i, i, list->next[i], list->list[i], list->prev[i]);
        }
    }


    for (int i = 0; i < 7; i++) {
        fprintf(graph, "    node%d -> node%d [color = \"#00000000\", weight=2000];\n", i, i + 1);
    }

    /*
    for (int i = 2; i < 7; i++) {
        fprintf(graph, "    node%d -> node%d [color = \"#3a3cc268\", weight=100];\n", i, list->next[i]);
        fprintf(graph, "    node%d -> node%d [color = \"#a30bab78\"];\n", i, list->prev[i]);
    }
    fprintf(graph, "}"); */

    for (int i = list->head; i != -1; i = list->next[i]) {
        if (list->list[i] != pz && list->next[i] != -1) {
            fprintf(graph, "    node%d -> node%d [color = \"#06074f68\", constraint=false];\n", i, list->next[i]);
        }
        if (list->list[i] != pz && list->prev[i] != -1) {
            fprintf(graph, "    node%d -> node%d [color = \"#08590e68\", constraint=false];\n", i, list->prev[i]);
        }
    }

    fprintf(graph, "}");

    //system("dot -Tpng graph.dot -o graph.png");

    //printf("\n");
    print_list(list);
    printf(RED "---------------------------\n" RESET);

    fclose(graph);
}


void print_list(ddl_t* list)
{
    printf("Now the list is looks like:\n");
    for (int i = list->head; i != -1; i = list->next[i]) {
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

void push_back(ddl_t* list, int element)
{
    add_new_element(list, element, list->tail - 1);
}
