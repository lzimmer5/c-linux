/* timeit.c: Run command with a time limit */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <fcntl.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

/* Macros */

#define streq(a, b) (strcmp(a, b) == 0)
#define strchomp(s) (s)[strlen(s) - 1] = 0
#define debug(M, ...) \
    if (Verbose) { \
        fprintf(stderr, "%s:%d:%s: " M, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
    }

#define BILLION 1000000000.0

/* Globals */

int  Timeout  = 10;
bool Verbose  = false;
int  ChildPid = 0;

/**
  * Display usage message and exit.
  * @param   status      Exit status.
  **/

void    usage(int status) {
    fprintf(stderr, "Usage: timeit [options] command...\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "    -t SECONDS  Timeout duration before killing command (default is %d)\n", Timeout);
    fprintf(stderr, "    -v          Display verbose debugging output\n");
    exit(status);
}

/**
  * Parse command line options.
  * @param   argc        Number of command line arguments.
  * @param   argv        Array of command line argument strings.
  * @return  Array of strings representing command to execute (must be freed).
  **/
char ** parse_options(int argc, char **argv){
    // Iterate through command line arguments to determine Timeout and
    // Verbose flags
    
    int counter = 1;

    if(argc == 1){
        usage(1);
    }
    else if(strcmp(argv[1], "-h") == 0){
        usage(0);
    }

    for(; counter<argc; counter++){
        if(streq(argv[counter],"-t")){
            if (argc > counter+1){
                counter++;
                Timeout = atoi(argv[counter]);
            }
            else{
                usage(1);
            }
        }
        else if (streq(argv[counter], "-v")) {
            Verbose = true;
        }
        else{
            break;
        }
    }

    debug("Timeout = %d\n", Timeout);
    debug("Verbose = %d\n", Verbose);

    if(counter >= argc){
        usage(1);
    }

    char **command = calloc(argc - counter + 1, sizeof(char *));
    
    // Copy remaining arguments into new array of strings
    for(int j=0; j < argc-counter; j++){
        command[j] = argv[counter + j];
    }

    command[argc - counter] = NULL;

    if (Verbose) {
        // Print out new array of strings (to stderr)
        debug("Command =");

        for(int n=0; command[n]; n++){
            fprintf(stderr, " %s", command[n]);
        }
        fprintf(stderr, "\n");
    }

    return command;
}

/**
 * Handle signal.
 * @param   signum      Signal number.
 **/
void    handle_signal(int signum) {
    // Kill child process gracefully, then forcefully
    debug("Killing child %d...\n", ChildPid);
    
    if (kill(ChildPid, SIGTERM) < 0) {
        debug("Failed to kill child with SIGTERM: %s\n", strerror(errno));
    }
    
    if(kill(ChildPid, SIGKILL) < 0){
        debug("Failed to kill child with SIGKILL: %s\n", strerror(errno));
    }
}


/* Main Execution */

int     main(int argc, char *argv[]) {
    // Parse command line options
    char **command = parse_options(argc, argv);
    
    // Register alarm handler and save start time
    debug("Registering handlers...\n");
    signal(SIGALRM, handle_signal);

    debug("Grabbing start time...\n");
    struct timespec start_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    
    // Fork child process:
    pid_t pid = fork();
    
    if(pid < 0){
        fprintf(stderr, "Unable to fork: %s\n", strerror(errno));
        free(command);
        exit(EXIT_FAILURE);
    }
    
    //  1. Child executes command parsed from command line
    if(pid == 0){
        execvp(command[0], command);
        fprintf(stderr, "Unable to execvp: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    //  2. Parent sets alarm based on Timeout and waits for child
    if(pid > 0){
        ChildPid = pid;
        debug("Executing child %d...\n", ChildPid);
        
        alarm(Timeout);
        
        debug("Sleeping for %d seconds...\n", Timeout);
        debug("Waiting for child %d...\n", ChildPid);
        
        int status;
        wait(&status);
        
        alarm(0);
        
        // Print out child's exit status or termination signal
        if (WIFEXITED(status)) {
            debug("Child exit status: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            // went ahead and added anothe debug call if child was sent a signal
            debug("Child terminated by signal: %d\n", WTERMSIG(status));
        }
        
        // Print elapsed time
        debug("Grabbing end time...\n");

        struct timespec end_time;
        clock_gettime(CLOCK_MONOTONIC, &end_time);

        double elapsed_time =
            (end_time.tv_sec - start_time.tv_sec) +
            (end_time.tv_nsec - start_time.tv_nsec) / BILLION;

        printf("Time Elapsed: %.1f\n", elapsed_time);
        
        // Cleanup
        free(command);
        
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)){
            // return the signal
            return WTERMSIG(status);
        }
        
        return status;
    }

    free(command);
    return EXIT_FAILURE;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
