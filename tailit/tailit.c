/* tailit.c: Output the last part of files */

#include "list.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Functions */

void usage(int status) {
    fprintf(stderr, "Usage: tailit [-n NUMBER]\n\n");
    fprintf(stderr, "    -n NUMBER  Output the last NUMBER of lines (default is 10)\n");
    exit(status);
}

List *tail_stream(FILE *stream, size_t limit) {
    List *l = list_create();
    Value v;
    char buffer[BUFSIZ];
    
    while(fgets(buffer, BUFSIZ, stream)){
        if(l->size == limit){
            v = list_pop(l, 0);
            free(v.string);
        }
        v.string = strdup(buffer);
        list_append(l, v);
    }
    
    return l;
}

/* Main Execution */

int main(int argc, char *argv[]) {
    
    size_t limit = 10;
    List *lines;
    Node *curr;
    
    // Parse command line arguments
    if(argc > 3){
        usage(1);
    }
    else if(argc == 2){
        if(strcmp(argv[1], "-h") == 0){
            usage(0);
        }
        else{
            usage(1);
        }
    }
    else if(argc == 3){
        limit = strtol(argv[2], NULL, 10);
    }
    
    // Construct tail of stream
    lines = tail_stream(stdin, limit);
    
    // Print out tail
    curr = lines->sentinel.next;
    
    while(curr != &lines->sentinel){
        printf("%s", curr->value.string);
        curr = curr->next;
    }
    
    list_delete(lines, true);
    
    return EXIT_SUCCESS;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
