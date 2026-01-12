#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

volatile int sigint_count = 0; 
volatile sig_atomic_t should_exit = 0;  
FILE *file = NULL;

void signal_handler(int sig) {
    const char *msg = NULL;
    
    switch(sig) {
        case SIGINT:
            sigint_count++;
            msg = "Received and handled SIGINT\n";
            fprintf(file, "%s", msg); //лучше без принтф, а через флаги
            fflush(file);
            
            if (sigint_count >= 3) {
                should_exit = 1; 
            }
            break;
            
        case SIGQUIT:
            msg = "Received and handled SIGQUIT\n";
            fprintf(file, "%s", msg);
            fflush(file);
            break;
    }
}

int main() {
    int counter = 1;
    char filename[] = "output.txt";
    
    file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file");
        exit(1);
    }
    
    printf("Program started. PID: %d\n", getpid());
    printf("Writing to file: %s\n", filename);
    printf("Send 3 SIGINT signals to stop the program.\n");
    
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);
    
    while (!should_exit) {

        fprintf(file, "%d\n", counter++);
        fflush(file);  
        
        sleep(1);
    }
    
    printf("Received 3 SIGINT signals. Exiting...\n");
    fclose(file);
    
    return 0;
}