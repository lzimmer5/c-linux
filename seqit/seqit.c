/* seqit.c: Print a sequence of numbers */

#include "list.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* Functions */

void usage(int status) {
    fprintf(stderr, "Usage: seqit LAST\n");
    fprintf(stderr, "       seqit FIRST LAST\n");
    fprintf(stderr, "       seqit FIRST INCREMENT LAST\n");
    exit(status);
}

List *generate_sequence(ssize_t first, ssize_t increment, ssize_t last) {
    

    List *l = list_create();
    Value v;
    
    if(increment > 0){
        for(int i=first; i<=last; i+=increment){
            v.number = i;
            list_append(l, v);
        }
    }
    else if(increment < 0){
        for(int i=first; i>= last; i+=increment){
            v.number = i;
            list_append(l, v);
        }
    }
    
    return l;
}

/* Main Execution */

int main(int argc, char *argv[]) {

    ssize_t first = 1;
    ssize_t increment = 1;
    ssize_t last;
    
    
    // Parse command line arguments
    if(argc == 1 || argc > 4){
        usage(1);
    }
    else if(argc == 2){
        last = strtol(argv[1], NULL, 10);
    }
    else if(argc == 3){
        first = strtol(argv[1], NULL, 10);
        last = strtol(argv[2], NULL, 10);
    }
    else if(argc == 4){
        first = strtol(argv[1], NULL, 10);
        increment = strtol(argv[2], NULL, 10);
        last = strtol(argv[3], NULL, 10);
    }
    // Generate sequence
    List *sequence = generate_sequence(first, increment, last);
    
    // Print out sequence
    Node *curr = sequence->sentinel.next;
    
    while(curr != &sequence->sentinel){
        printf("%ld\n", curr->value.number);
        curr = curr->next;
    }
    
    list_delete(sequence, false);
    
    return EXIT_SUCCESS;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
