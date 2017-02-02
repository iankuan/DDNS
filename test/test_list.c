#include <stdio.h>
#include <stdlib.h>
#include "list.h"

struct list {
    int ctx;
    struct list_head loc;
};

int main(int argc, char *argv[])
{
    struct list l[3];

    for(int i = 0; i < 3; i++)
    {
        l[i].ctx = i * 10;
        //l[i].loc = (struct list_head *) malloc(sizeof(struct list_head));
    }

    LIST_HEAD(start);

    //INIT_LIST_HEAD(l[0].loc);
    //INIT_LIST_HEAD(l[1].loc);
    //INIT_LIST_HEAD(l[2].loc);

    //struct list_head *start = l[0].loc;

    //list_add(start, l[1].loc);
    //list_add(start, l[2].loc);
    list_add_tail(&l[0].loc, &start);
    list_add_tail(&l[1].loc, &start);
    list_add_tail(&l[2].loc, &start);


    //struct list_head *tmp = start;
    struct list_head *tmp;
    list_for_each(tmp, &start)
    {
        printf("%d\n", list_entry(tmp, struct list, loc)->ctx);
    }
}
