/* curlit.c Simple HTTP client*/


#include "socket.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <netdb.h>

/* Constants */

#define HOST_DELIMITER  "://"
#define PATH_DELIMITER  '/'
#define PORT_DELIMITER  ':'
#define BILLION         (1000000000.0)
#define MEGABYTES       (1<<20)

/* Macros */

#define streq(a, b) (strcmp(a, b) == 0)

/* Structures */

typedef struct {
    char host[NI_MAXHOST];
    char port[NI_MAXSERV];
    char path[PATH_MAX];
} URL;

/* Functions */

/**
 * Display usage message and exit.
 * @param   status      Exit status.
 **/
void    usage(int status) {
    fprintf(stderr, "Usage: curlit [-h] URL\n");
    exit(status);
}

/**
 * Parse URL string into URL structure.
 * @param   s       URL string
 * @param   url     Pointer to URL structure
 **/
void    parse_url(const char *s, URL *url) {
    // Copy data to local buffer
    char buffer[BUFSIZ];
    strcpy(buffer, s);
    
    // Skip scheme to host
    char *host = strstr(buffer, HOST_DELIMITER);
    if(!host){
        host = buffer;
    }
    else{
        host += 3;
    }
    
    // Split host:port from path
    char *path = strchr(host, PATH_DELIMITER);
    
    if(!path){
        path = "";
    }
    else{
        *path = '\0';
        path++;
    }
    
    // Split host and port
    char *port = strchr(host, PORT_DELIMITER);
    if(!port){
        port = "80";
    }
    else{
        *port = '\0';
        port++;
    }
    
    // Copy components to URL
    strcpy(url->host, host);
    strcpy(url->port, port);
    strcpy(url->path, path);
}

/**
 * Fetch contents of URL and print to standard out.
 *
 * Print elapsed time and bandwidth to standard error.
 * @param   s       URL string
 * @param   url     Pointer to URL structure
 * @return  true if client is able to read all of the content (or if the
 * content length is unset), otherwise false
 **/
bool    fetch_url(URL *url) {
    bool return_val = true;
    
    // Grab start time
    struct timespec start_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    
    // Connect to remote host and port
    FILE *client_socket = socket_dial(url->host, url->port);
    if(!client_socket){
        return false;
    }
    
    // Send request to server
    fprintf(client_socket, "GET /%s HTTP/1.0\r\n", url->path);
    fprintf(client_socket, "Host: %s\r\n", url->host);
    fprintf(client_socket, "\r\n");
    
    // Read status response from server
    char buffer[BUFSIZ];
    fgets(buffer, BUFSIZ, client_socket);

    if(strstr(buffer, "200 OK") == NULL){
        return_val = false;
    }

    // Read response headers from server
    int content_length = 0;
    
    while((fgets(buffer, BUFSIZ, client_socket)) && strlen(buffer) > 2){
        sscanf(buffer, "Content-Length: %d", &content_length);
    }
    
    // Read response body from server
    size_t nread = 0;
    size_t total_bytes = 0;
    while((nread = fread(buffer, 1, BUFSIZ, client_socket)) > 0){
        fwrite(buffer, 1, nread, stdout);
        total_bytes += nread;
    }
    
    // Grab end time
    struct timespec end_time;
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / BILLION;
    double bandwidth = total_bytes / (MEGABYTES*elapsed_time);
    
    // Output metrics
    fprintf(stderr, "Time Elapsed: %0.2f s\n", elapsed_time);
    fprintf(stderr, "Bandwidth: %0.2f MB/s\n", bandwidth);
    
    fclose(client_socket);
    
    if((return_val == false) || ((content_length != 0) && (content_length != total_bytes))){
        return false;
    }
    
    return true;
}

/* Main Execution */

int     main(int argc, char *argv[]) {
    // Parse command line options
    URL url;
    char *user_url;
    
    if(argc == 1){
        usage(1);
    }
    
    for(int i=1; i<argc; i++){
        if(strcmp(argv[i], "-h") == 0){
            usage(0);
        }
        else if(argv[i][0] == '-'){
            usage(1);
        }
        else{
            user_url = argv[i];
        }
    }
    
    // Parse URL
    parse_url(user_url, &url);

    //  Fetch URL
    if(!fetch_url(&url)){
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/* vim set sts=4 sw=4 ts=8 expandtab ft=c */