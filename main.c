#include "ddl.h"

int main()
{

    FILE* logfile = open_logfile();
    if (logfile == NULL) {
        return 1;
    }

    int data[] = {PZ, 10, 20, 30, 40, 50, PZ,  PZ,  PZ};
    int next[] = {0,  2,  3,   4,  5,  -1, 7, 8, -1};
    int prev[] = {0,  -1,  1,   2,  3,  4, -1, -1, -1};

    int wrong_prev[] = {0,  -1,  1,   4,  3,  4, -1, -1, -1};

    ddl_t list1 = {.data = data, .next = next, .prev = prev, .free = 6, .head = 1, .tail = 5, .capacity = 8};
    ddl_dump(&list1, logfile);

    ddl_t list3 = {.data = data, .next = next, .prev = wrong_prev, .free = 6, .head = 1, .tail = 5, .capacity = 8};
    ddl_dump(&list3, logfile);

    ddl_insert(&list1, 35, 3);
    ddl_dump(&list1, logfile);

    ddl_erase(&list1, 3);
    ddl_dump(&list1, logfile);

    for (int i = ddl_iterator_begin(&list1); i != ddl_iterator_end(&list1); i = ddl_iterator_next(&list1, i)) {
        printf("%d\n", list1.data[i]);
    }

    close_logfile(logfile);

    //NOTE: error: permisiion denied, if i call system in dump

    system("dot -Tsvg graph1.dot -o graph1.svg");
    system("dot -Tsvg graph2.dot -o graph2.svg");
    system("dot -Tsvg graph3.dot -o graph3.svg");
    system("dot -Tsvg graph4.dot -o graph4.svg");

    return 0;
}
