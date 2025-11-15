#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

int main() {
    char input[MAX_INPUT];
    char* args [MAX_ARGS];

    while(1) {
        printf("myshell> ");
        fflush(stdout);

        if(fgets(input, sizeof(input), stdin) != NULL) {
            
            input[strcspn(input, "\n")] = 0;
            if (strlen(input) == 0) {
                continue;
            }

            int i = 0;
            args[i] = strtok(input, " ");      
            while (args[i] != NULL && i < MAX_ARGS - 1) {
                i++;
                args[i] = strtok(NULL, " "); 
            }
            args[i] = NULL;  

            if (strcmp(args[0], "exit") == 0) {
                printf("Выход из shell\n");
                break;
            }

            pid_t pid = fork();

            if (pid == 0) {
                execvp(args[0], args);
                
                printf("Ошибка: команда '%s' не найдена\n", args[0]);
                exit(1);
            }
            else if (pid > 0) {
                wait(NULL);
            }
            else {
                printf("Ошибка: не удалось создать процесс\n");
            }
        }
        else {
            printf("ошибка чтения");
        }
    }
}