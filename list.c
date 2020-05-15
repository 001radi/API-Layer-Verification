//
// Created by d_dab on 15/05/2020.
//

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "list.h"

struct list_t{
    int len;
    listNode *head;
    listNode *tail;
    freeFunction freeFn;
    copyFunction copyFn;
    compareFunction cmpFn;
};

void list_new(List list, int elementSize, freeFunction freeFn)
{
    assert(elementSize > 0);
    list->len = 0;
    list->head = list->tail = NULL;
    list->freeFn = freeFn;
}

void list_destroy(List list)
{
    listNode *current;
    while(list->head != NULL) {
        current = list->head;
        list->head = current->next;

        if(list->freeFn) {
            list->freeFn(current->data);
        }

        free(current->data);
        free(current);
    }
}


void list_add(List list, Element element)
{
    listNode *node = malloc(sizeof(listNode));
    node->data = list->copyFn(element);
    node->next = NULL;

    if(list->len == 0) {
        list->head = list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }

    list->len++;
}

Element list_get_element(List list, Element element)
{
    assert(element != NULL);
    listNode *node = list->head;
    while(node != NULL ) {
        if(!(list->cmpFn(node->data, element))){
            return node->data;
        }
        node = node->next;
    }
    return NULL;
}

void deleteNode(List list, Element element)
{
    // Store head node
    listNode *temp = list->head;
    listNode *prev;

    // If head node itself holds the key to be deleted
    if (temp != NULL && (!(list->cmpFn(temp->data, element)))) {
        list->head = temp->next;   // Changed head
        free(temp);               // free old head
        return;
    }

    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while (temp != NULL && (list->cmpFn(temp->data, element)) ) {
        prev = temp;
        temp = temp->next;
    }

    // If key was not present in linked list
    if (temp == NULL) return;

    // Unlink the node from linked list
    prev->next = temp->next;
    list->freeFn(temp);  // Free memory
    list->len--;
}

int list_size(List list)
{
    return list->len;
}