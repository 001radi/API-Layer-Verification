//
// Created by d_dab on 15/05/2020.
//

#ifndef PROJECTEMOT_LIST_H
#define PROJECTEMOT_LIST_H

#include <stdbool.h>
// element
typedef void* Element;

typedef enum{
    LIST_SUCCESS,
    LIST_MEMORY_ERROR,
    LIST_ALREADY_EXIST,
    LIST_NOT_EXIST,
    LIST_ITEM2_DOES_NOT_EXIST
}ListResult;

/**
 * the sturcts we have defined may use sort of list or queue, not sure yet which code to include here.
 * */

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

#define FOR_EACH(item, list) \
    for (listNode* (item) = (list); (item); (item) = (item)->next)

List list_new(freeFunction freeFn, copyFunction copyFn, compareFunction cmpFn);
ListResult list_destroy(List list);
ListResult list_add(List list, Element elem);
ListResult list_delete(List list, Element elem);
int list_size(List list);
Element list_get_element(List list, Element element);

#endif //PROJECTEMOT_LIST_H
