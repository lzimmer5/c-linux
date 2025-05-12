/* list.c: Singly Linked List */

#include "findit.h"

#include <stdlib.h>

/* Node Functions */

/**
 * Allocate a new Node structure.
 * @param   data        Data value
 * @param   next        Pointer to next Node structure
 * @return  Pointer to new Node structure (must be deleted).
 **/
Node *  node_create(Data data, Node *next) {
    // Use calloc
    Node *newNode = calloc(1, sizeof(Node));
    newNode->data = data;
    newNode->next = next;
    
    return newNode;
}

/**
 * Deallocate Node structure.
 * @param   n           Pointer to Node structure
 * @param   release     Whether or not to free Data string
 * @param   recursive   Whether or not to recursively delete next Node structure
 **/
void    node_delete(Node *n, bool release, bool recursive) {
    // Handle base case, recursive case, and then release data and Node
    if(n == NULL) return;
    
    if (recursive) node_delete(n->next, release, recursive);
    
    if(release && n->data.string != NULL) free(n->data.string);
    free(n);
}

/* List Functions */

/**
 * Append data to end of specified List.
 * @param   l           Pointer to List structure
 * @param   data        Data value to append
 **/
void    list_append(List *l, Data data) {
    // Create new Node Structure and add to end of List
    Node *newNode = node_create(data, NULL);
    
    if(l->head == NULL && l->tail == NULL){
        l->head = newNode;
        l->tail = newNode;
    }
    else{
        l->tail->next = newNode;
        l->tail = newNode;
    }
}

/**
 * Filter list by applying the filter function to each Data string in List with
 * the given options:
 *
 *  - If filter function returns true, then keep current Node.
 *  - Otherwise, remove current Node from List and delete it.
 *
 * @param   l           Pointer to List structure
 * @param   filter      Filter function to apply to each Data string
 * @param   options     Pointer to Options structure to use with filter function
 * @param   release     Whether or not to release data string when deleting Node
 **/
void    list_filter(List *l, Filter filter, Options *options, bool release) {
    // Iterate through List and apply filter function to each Data string
    // to determine whether or not to keep the Node.
    Node *curr = l->head;
    Node *prev = NULL;
    Node *next;

    bool check;
    
    while(curr != NULL){
        next = curr->next;
        
        check = filter(curr->data.string, options);

        if(!check){
            if(prev == NULL){
                l->head = next;
            }
            else{
                prev->next = next;
            }

            if(curr == l->tail){
                l->tail = prev;
            }
             
            node_delete(curr, release, false);
        }
        else prev = curr;

        curr = next;
    }
}

/**
 * Output each Data string in List to specified stream.
 * @param   l           Pointer to List structure
 * @param   stream      File stream to output to
 **/
void    list_output(List *l, FILE *stream) {
    // Iterate though List and output each Data string to given stream
    // (one string per line).
    Node *curr = l->head;
        
    while(curr){
        fprintf(stream, "%s\n", curr->data.string);
        curr = curr->next;
    }
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
