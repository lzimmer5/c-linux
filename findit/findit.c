/* findit.c: Search for files in a directory hierarchy */

#include "findit.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <unistd.h>

/* Macros */

#define	streq(a, b) (strcmp(a, b) == 0)

/* Functions */

/**
 * Print usage message and exit with status
 * @param   status      Exit status
 **/
void usage(int status) {
    fprintf(stderr, "Usage: findit PATH [OPTIONS]\n\n");
    fprintf(stderr, "Options:\n\n");
    fprintf(stderr, "   -type [f|d]	File is of type f for regular file or d for directory\n");
    fprintf(stderr, "   -name pattern	Name of file matches shell pattern\n");
    fprintf(stderr, "   -executable	File is executable or directory is searchable by user\n");
    fprintf(stderr, "   -readable	File is readable by user\n");
    fprintf(stderr, "   -writable	File is writable by user\n");
    exit(status);
}

/**
 * Recursively walk specified directory, adding all file system entities to
 * specified files list.
 * @param   root        Directory to walk
 * @param   files       List of files found
 **/
void	find_files(const char *root, List *files) {
    // Add root to files

    // Walk directory
    //  - Skip current and parent directory entries
    //  - Form full path to entry
    //  - Recursively walk directories or add entry to files list
    
    Data root_data;
    root_data.string = strdup(root);
    
    list_append(files, root_data);
    
    DIR *d = opendir(root);
    if (!d){
        perror("opendir");
        return;
    }
    
    struct dirent *e;
    while ((e = readdir(d)) != NULL){
        
        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0){
            continue;
        }
        
        char path[BUFSIZ];
        snprintf(path, BUFSIZ, "%s/%s", root, e->d_name);
        
        if (e->d_type == DT_DIR){
            find_files(path, files);
        }
        
        else{
            Data data;
            data.string = strdup(path);
            if (data.string) {
                list_append(files, data);
            }
        }
    }
    
    closedir(d);
}
    
/**
 * Iteratively filter list of files with each filter in list of filters.
 * @param   files       List of files
 * @param   filters     List of filters
 * @param   options     Pointer to options structure
 **/
void	filter_files(List *files, List *filters, Options *options) {
    // Apply each filter to list of files
    Node *currFilter = filters->head;
    
    while(currFilter){
        list_filter(files, currFilter->data.function, options, true);
        currFilter = currFilter->next;
    }
}

void print_song() {
    char song[] = "Now we are running in a pack to the place you don't know\n"
    "And I want you to know that I'll always be around\n"
    "Down where the summer and the late nights last forever\n"
    "There's a house on the hill and we can't travel now\n"
    "Alice D, you're on your way up\n"
    "Way up, way up\n"
    "I feel most times we're high and low (high and low)\n"
    "If I had my way, never let you go (never let you go)\n"
    "I feel most times we're high and low (high and low)\n"
    "If I had my way, never let you go (never let you go)\n"
    "Let's get together and forget all the troubles and just float\n"
    "I don't want you to go\n"
    "I need to be closer to now\n"
    "Alice D, you're on your way up\n"
    "Way up, way up\n"
    "I feel most times we're high and low (high and low)\n"
    "If I had my way, never let you go (never let you go)\n"
    "I feel most times we're high and low (high and low)\n"
    "If I had my way, never let you go (never let you go)\n"
    "Alice D is on will\n"
    "She's been out all night again\n"
    "She don't want nobody's help now\n"
    "She sees stars and rocket ships\n"
    "I feel most times we're high and low (high and low)\n"
    "If I had my way, never let you go (never let you go)\n"
    "I feel most times we're high and low (high and low)\n"
    "If I had my way, never let you go (never let you go)\n"
    "I feel most times we're high and low (high and low)\n"
    "If I had my way, never let you go (never let you go)\n"
    "I feel most times we're high and low (high and low)\n";

    char *line = strtok(song, "\n");
    
    while(line != NULL) {
        printf("%s\n", line); 
        usleep(1000000);
        line = strtok(NULL, "\n");
    }
}


/* Main Execution */

int main(int argc, char *argv[]) {
    // Parse command line arguments */
    
    List files = {0};
    List filters = {0};
    Options options = {0};
    Data data = {0};
    char root[BUFSIZ];
    
    if(argc == 1) usage(1);
    
    for(int i=1; i<argc; i++){
        if(strcmp(argv[i], "-type") == 0){
            data.function = filter_by_type;
            list_append(&filters, data);
            i++;

            if(strcmp(argv[i], "f") == 0) options.type = S_IFREG;
    
            else if(strcmp(argv[i], "d") == 0) options.type = S_IFDIR;
        }
        else if(strcmp(argv[i], "-name") == 0){
            options.name = argv[i+1];
            i++;
            data.function = filter_by_name;
            list_append(&filters, data);
        }
        else if(strcmp(argv[i], "-executable") == 0){
            options.mode |= 0x1;
            data.function = filter_by_mode;
            list_append(&filters, data);
        }
        else if(strcmp(argv[i], "-writable") == 0){
            options.mode |= 0x2;
            data.function = filter_by_mode;
            list_append(&filters, data);
        }
        else if(strcmp(argv[i], "-readable") == 0){
            options.mode |= 0x4;
            data.function = filter_by_mode;
            list_append(&filters, data);
        }
        else if(strcmp(argv[i], "luke") == 0){
            printf("Congratulations! You found the hidden Easter Egg. Here's High and Low by Empire of the Sun\n\n");
            print_song();
            return EXIT_SUCCESS;
        }
        else{
            strcpy(root, argv[i]);
        }
    }
    
    // Find files, filter files, print files
    
    find_files(root, &files);
    filter_files(&files, &filters, &options);
    list_output(&files, stdout);
    
    node_delete(files.head, true, true);
    node_delete(filters.head, false, true);
    
    return EXIT_SUCCESS;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
