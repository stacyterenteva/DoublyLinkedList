#include "ddl.h"

int main()
{

    FILE* logfile = open_logfile();
    if (logfile == NULL) {
        return 1;
    }

    // FIXME: use only functions
    int data[] = {PZ, 10, 20, 30, 40, 50, PZ,  PZ,  PZ};
    int next[] = {0,  2,  3,   4,  5,  -1, 7, 8, -1};
    int prev[] = {0,  -1,  1,   2,  3,  4, -1, -1, -1};

    int wrong_prev[] = {0,  -1,  1,   4,  3,  4, -1, -1, -1};

    ddl_t list4 = ddl_create_list();

    FILE* logfile1 = fopen("logfile1.html", "w");
    ddl_dump(&list4, logfile1);

    ddl_push_back(&list4, 10);
    ddl_push_back(&list4, 20);
    ddl_push_back(&list4, 30);
    ddl_push_back(&list4, 40);
    ddl_push_back(&list4, 50);

    ddl_dump(&list4, logfile1);

    ddl_insert(&list4, 35, 3);
    ddl_dump(&list4, logfile1);

    ddl_erase(&list4, 3);
    ddl_dump(&list4, logfile1);

    ddl_t list1 = {.data = data, .next = next, .prev = prev, .free = 6, .head = 1, .tail = 5, .capacity = 8};
    ddl_dump(&list1, logfile);

    ddl_t list3 = {.data = data, .next = next, .prev = wrong_prev, .free = 6, .head = 1, .tail = 5, .capacity = 8};
    ddl_dump(&list3, logfile);

    ddl_insert(&list1, 35, 3);
    printf("tail = %d\n", list1.tail);
    printf("free = %d\n", list1.free);

    ddl_dump(&list1, logfile);

    ddl_erase(&list1, 3);
    printf("tail = %d\n", list1.tail);
    printf("free = %d\n", list1.free);

    ddl_dump(&list1, logfile);

    for (int i = ddl_iterator_begin(&list1); i != ddl_iterator_end(&list1); i = ddl_iterator_next(&list1, i)) {
        //printf("%d\n", element_by_iterator(num));
    }

    ddl_push_back(&list1, 57);
    ddl_dump(&list1, logfile);

    close_logfile(logfile);

    return 0;
}
