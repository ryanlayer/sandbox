#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct int_list_list
{
    struct int_list_list *next;
    int *list;
    int size;
};


int main()
{

    struct int_list_list *head = (struct int_list_list *)
            malloc(sizeof(struct int_list_list));
    struct int_list_list *tail = head;

    head->size = 10;
    head->list = (int *) malloc(head->size * sizeof(int));
    head->next = NULL;

    int i;
    for (i = 0; i < head->size; ++i)
        head->list[i] = (i+1) * 10;


    struct int_list_list *copy_head, *copy_tail;
    copy_head = NULL;

    for (i = 0; i < 10; ++i) {
        struct int_list_list *new_list = (struct int_list_list *)
                malloc(sizeof(struct int_list_list));

        new_list->size = 10;
        new_list->list = (int *) malloc(head->size * sizeof(int));
        new_list->next = NULL;
        memcpy(new_list->list, head->list, (i+1)*sizeof(int));

        if (copy_head == NULL) {
            copy_head = new_list;
            copy_tail = new_list;
        } else {
            copy_tail->next = new_list;
            copy_tail = new_list;
        }
    }

    head->next = copy_head;
    tail = copy_tail;


    struct int_list_list *curr= head;
    while (curr != NULL) {
        int i;
        for (i = 0; i < curr->size; ++i) {
            if (i != 0)
                printf(" ");
            printf("%d", curr->list[i]);
        }
        printf("\n");
        curr = curr->next;
    }

    return 0;
}
