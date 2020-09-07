//
// Created by d_dab on 15/05/2020.
//

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "helpers.h"

#include "list.h"

#include <seahorn/seahorn.h>



List list_new(freeFunction freeFn, copyFunction copyFn, compareFunction cmpFn)
{   // verf
    assume(freeFn != NULL);
    assume(copyFn != NULL);
    assume(cmpFn != NULL);
    List list =xmalloc(sizeof(*list));
//    if((!list)) {
//    return NULL;
//    }
    sassert(list != NULL);
    list->len = 0;
    list->head = list->tail = NULL;
    list->freeFn = freeFn;
    list->copyFn = copyFn;
    list->cmpFn = cmpFn;
    return list;
}

void list_destroy(List list)
{
    assume(list != NULL);
    listNode *current;
    while(list->head != NULL) {
        current = list->head;
        list->head = current->next;

        if(list->freeFn) {
            list->freeFn(current->data);
        }
        free(current);
    }
    free(list);
}


ListResult list_add(List list, Element element)
{
    assume(element != NULL);
    assume(list != NULL);
    Element element1 = list_get_element(list, element);
//    if(element1 != NULL){
//        return LIST_ALREADY_EXIST;
//    }
    listNode *node = xmalloc(sizeof(listNode));
    sassert(node > 0);
//   if(!node){
//       return LIST_MEMORY_ERROR;
//   }
    node->data = list->copyFn(element);
//   if(!(node->data)){
//      free(node);
//       return LIST_MEMORY_ERROR;
//   }
    node->next = NULL;

    if(list->len == 0) {
        list->head = list->tail = node;
    } else {
        list->tail->next = node;
    }
    list->tail=node;
   // sassert(list->len >=0);
    list->len++;
   //// sassert(list->len >= 0);
    return LIST_SUCCESS;
}

Element list_get_element(List list, Element element)
{
    assume(element > 0);
    assume(list > 0);
    Element res = NULL;
    listNode *node = list->head;
    while(node != NULL ) {
    //   sassert(node != NULL);
        if(!(list->cmpFn(node->data, element))){
            res = node->data;
        }
        node = node->next;
    }
    return res;
}

ListResult list_delete(List list, Element element)
{
    // Store head node
    listNode *temp = list->head;
    listNode *prev;

    // If head node itself holds the key to be deleted
    if (temp != NULL && (!(list->cmpFn(temp->data, element)))) {
        list->head = temp->next;   // Changed head
        list->freeFn(temp->data);
        free(temp);// free old head
        list->len--;
        if(list->len == 0){
            list->tail = NULL;
        }
        return LIST_SUCCESS;
    }

    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while (temp != NULL && (list->cmpFn(temp->data, element)) ) {
        prev = temp;
        temp = temp->next;
    }

    // If key was not present in linked list
    if (temp == NULL) return LIST_NOT_EXIST;

    if(temp == list->tail){
        list->tail = prev;
    }
    // Unlink the node from linked list
    prev->next = temp->next;
    list->freeFn(temp->data);
    free(temp);// Free memory
    list->len--;
    return LIST_SUCCESS;
}

int list_size(List list)
{
    return list->len;
}

listNode* list_get_head(List list){
    assert(list!=NULL);
    return (list->head);
}