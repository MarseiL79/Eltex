#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>

void process_argument(char* arg) {
    char* endptr1;
    char* endptr2;

    long num1 = strtol(arg, &endptr1, 10);
    double num2 = strtod(arg, &endptr2);

        if (*endptr1 == '\0')
        {
            printf("Аргумент %ld - целое число, удваиваем: %ld ", num1, num1 * 2);
        }

        else if (*endptr2 == '\0')
        {
            printf("Аргумент %.3f - вещественное число, удваиваем: %.3f ", num2, num2 * 2);
        } 

        else
        {
            printf("Аргумент %s не число ", arg);
        }

    return;
}

int main(int argc, char *argv[]) 
{
    if(argc > 1) {

        printf("Список аргументов: ");
        for (int i = 1; i < argc; i++)
        {
            printf("%s,  ", argv[i]);
        }
        printf("\n");
        

        pid_t pid = fork();

        if(pid > 0) {
            for (int i = 1; i <= argc/2; i++)
            {
                printf(" Задачу выполнил процесс #%d ", getpid());
                process_argument(argv[i]);
                printf("\n");
            }
        }

        else if (pid == 0)
        {
            for (int i = argc/2 + 1; i < argc; i++)
            {
                printf(" Задачу выполнил процесс #%d ", getpid());
                process_argument(argv[i]);
                printf("\n");
            }
        } 
    }
}