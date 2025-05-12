/* node.c: Node Structure */

#include "list.h"

/* Node Functions */

/**
 * Create a Node structure.
 *
 * @param   v       Value (Number or String).
 * @param   next    Pointer to next Node structure.
 * @param   prev    Pointer to previous Node structure.
 *
 * @return  Pointer to new Node structure (must be deleted later).
 **/
Node *  node_create(Value v, Node *next, Node *prev) {
    Node *nodePtr = (Node *)malloc(sizeof(Node));
    
    nodePtr->value = v;
    nodePtr->next = next;
    nodePtr->prev = prev;
    
    return nodePtr;
}

/**
 * Delete Node structure and its contents.
 *
 * @param   n       Pointer to Node structure.
 * @param   release Whether or not to free the string value.
 **/
void    node_delete(Node *n, bool release) {
    if(release) free(n->value.string);
    free(n);
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
