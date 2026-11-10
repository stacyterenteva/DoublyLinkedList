#include "ddl.h"

void ddl_insert(ddl_t* list, int element, int prev)
{
    assert(list);

    list->data[list->free] = element;
    list->next[prev] = list->free;
    list->next[list->free] = prev + 1;
    list->prev[list->free] = prev;
    list->prev[prev + 1] = list->free;
    list->free = list->next[list->free];
}

void ddl_erase(ddl_t* list, int num)
{
    assert(list);

    list->data[num] = PZ;
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
        perror("Error: ");
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

    for (int i = 0; i < list->capacity; i++) {
                    fprintf(logfile, "<tr>\n"
                    "  <td>%d</td>\n"
                    "  <td>%d</td>\n"
                    "  <td>%d</td>\n"
                    "  <td>%d</td>\n"
                    "</tr>\n", i, list->data[i], list->next[i], list->prev[i]);
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
            fprintf(graph, "    node%d [shape=Mrecord, style=filled, fillcolor=\"#baab0a\", color=\"#363201\", label=\"index = %d | next = %d | data = %d | prev = %d\"];\n", i, i, list->next[i], list->data[i], list->prev[i]);
        }
        else if (list->data[i] == PZ) {
            fprintf(graph, "    node%d [shape=Mrecord, style=filled, fillcolor=\"#0ca2a2\", color=\"#012d34\", label=\"index = %d | next = %d | data = %d | prev = %d\"];\n", i, i, list->next[i], list->data[i], list->prev[i]);
        }

        else {
            fprintf(graph, "    node%d [shape=Mrecord, style=filled, fillcolor=\"#9ce4ad\", color=\"#035603\", label=\"index = %d | next = %d | data = %d | prev = %d\"];\n", i, i, list->next[i], list->data[i], list->prev[i]);
        }
    }

    //TODO: print_list like img
    for (int i = 0; i < list->capacity - 1; i++) {
        fprintf(graph, "    node%d -> node%d [color = \"#00000000\", weight=2000];\n", i, i + 1);
    }

    if (list->capacity == 1) {
        fprintf(graph, "}");
        fclose(graph);
        return DDL_ERROR_SUCCESS;
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
        if (list->data[i] != PZ && list->prev[i] != -1) {
            fprintf(graph, "    node%d -> node%d [color = \"#08590e68\", constraint=false, weight = 50];\n", i, list->prev[i]);
        }*/
    }

    fprintf(graph, "}");

    print_list(list, logfile);

    //char comand_for_dot[MAX_STRLEN] = {};
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
        fprintf(logfile, "%d ", list->data[i]);
    }
    fprintf(logfile, "\n");
}

/*
void print_list_graphic(ddl_t* list, FILE* graph)
{
    assert(list);
    assert(graph);

    for (int i = list->head; i != -1 && list->next[i] != -1; i = list->next[i]) {
        fprintf(graph, "    node%d -> node%d [color = \"#18d90d00\"];\n", i, list->next[i]);
    }
} */

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
}

int ddl_iterator(ddl_t* list, int num)
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
    for (int i = 1; i < MIN_NUM_OF_ELEMENTS; i++) {
        next[i] = -1;
        prev[i] = -1;
        data[i] = PZ;
    }
    ddl_t list = {.data = data, .next = next, .prev = prev, .free = 1, .head = 0, .tail = 0, .capacity = 5};
    return list;
}

void ddl_erase_by_iterator(ddl_t* list, int num)
{
    ddl_erase(list, ddl_iterator(list, num));
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

int ddl_iterator_end(ddl_t* list)
{
    return list->tail;
}

int ddl_iterator_begin(ddl_t* list)
{
    return list->head;
}

int ddl_iterator_next(ddl_t* list, int num)
{
    return list->next[num];
}

int ddl_iterator_prev(ddl_t* list, int num)
{
    return list->prev[num];
}
