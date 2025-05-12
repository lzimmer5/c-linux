/* list.h: Doubly Linked List Library */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Value Union */

typedef union {
    int64_t number;     // Value as number
    char   *string;     // Value as string
} Value;

/* Node Structure */

typedef struct Node Node;
struct Node {
    Value   value;      // Value
    Node   *next;       // Pointer to next Node
    Node   *prev;       // Pointer to previous Node
};

Node *	node_create(Value v, Node *next, Node *prev);
void	node_delete(Node *n, bool release);

/* List Structure */

typedef struct {
    Node    sentinel;   // Sentinel Node at head and tail of List
    size_t  size;       // Number of Nodes in List
} List;

List *	list_create();
void	list_delete(List *l, bool release);

void	list_append(List *l, Value v);
Value   list_pop(List *l, size_t index);

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */