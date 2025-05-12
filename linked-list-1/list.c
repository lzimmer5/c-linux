/* list.c: List Structure */

#include "list.h"

/* List Functions */

/**
 * Create a List structure.
 *
 * @return  Pointer to new List structure (must be deleted later).
 **/
List *	list_create() {
    List *listPtr = (List *)malloc(sizeof(List));
    
    listPtr->sentinel.value.number = 0;
    listPtr->sentinel.next = &listPtr->sentinel;
    listPtr->sentinel.prev = &listPtr->sentinel;
    listPtr->size = 0;

    return listPtr;
}

/**
 * Delete List structure.
 *
 * @param   l       Pointer to List structure.
 * @param   release Whether or not to release the string values.
 **/
void	list_delete(List *l, bool release) {
    Node *curr = l->sentinel.next;
    Node *track;
    
    while(curr != &l->sentinel){
        track = curr->next;
        node_delete(curr, release);
        curr = track;
    }
    
    free(l);
}

/**
 * Add new Value to back of List structure.
 *
 * @param   l       Pointer to List structure.
 * @param   v       Value to add to back of List structure.
 **/
void    list_append(List *l, Value v) {
    Node *newNode = node_create(v, &l->sentinel, l->sentinel.prev);
    
    newNode->prev->next = newNode;
    l->sentinel.prev = newNode;
    
    newNode->value = v;

    (l->size)++;
}

/**
 * Remove Value at specified index from List structure.
 *
 * @param   l       Pointer to List structure.
 * @param   index   Index of Value to remove from List structure.
 *
 * @return  Value at index in List structure (-1 if index is out of bounds).
 **/
Value   list_pop(List *l, size_t index) {
    if (index > l->size - 1) return (Value)-1L;
    
    Node *curr = l->sentinel.next;
    
    for(int i=0; i<index; i++){
        curr = curr->next;
    }
    
    Value v = curr->value;
    
    curr->next->prev = curr->prev;
    curr->prev->next = curr->next;
    
    node_delete(curr, false);
    (l->size)--;
    
    return v;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */