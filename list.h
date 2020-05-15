//
// Created by d_dab on 15/05/2020.
//

#ifndef PROJECTEMOT_LIST_H
#define PROJECTEMOT_LIST_H

#include <stdbool.h>
// element
typedef void* Element;
typedef struct list_t* List;
typedef bool (*listIterator)(Element);

// a common function used to free malloc'd objects
typedef void (*freeFunction)(Element);
typedef Element (*copyFunction)(Element);
typedef int (*compareFunction) (Element , Element);


typedef struct _listNode {
    Element data;
    struct _listNode *next;
} listNode;



void list_new(List list, int elementSize, freeFunction freeFn);
void list_destroy(List list);

void list_add(List list, void *element);
void list_delete(List list);
int list_size(List list);

void list_for_each(List list, listIterator iterator);

#endif //PROJECTEMOT_LIST_H
