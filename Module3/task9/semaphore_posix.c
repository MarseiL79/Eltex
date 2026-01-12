#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <errno.h>
#include <limits.h>

#define MAX_ENTRY_SIZE 60
#define MAX_VALUES 15
#define SEMAPHORE_NAME "/data_sync_sem"

void HandleError(const char* message)
{
    perror(message);
    exit(1);
}

void DataGenerator(const char *output_file) 
{
    FILE *file_handle = fopen(output_file, "a+");
    if(file_handle == NULL) HandleError("fopen");
    
    char data_string[MAX_ENTRY_SIZE] = ""; 
    int value_count = rand() % MAX_VALUES + 1;
        
    for (int i = 0; i < value_count; i++) 
    {
        snprintf(data_string + strlen(data_string),
        sizeof(data_string)-strlen(data_string), "%d ", rand() % 150);
    }
        
    fprintf(file_handle, "%s\n", data_string);
    fflush(file_handle);
    sleep(rand()%2);
    
    fclose(file_handle);
}

void ParseDataString(char *input_line) 
{
    char *value_ptr = strtok(input_line, " ");
    int minimum = INT_MAX, maximum = INT_MIN;
    
    while (value_ptr != NULL) 
    {
        int current_value = atoi(value_ptr);
        if (current_value > maximum) maximum = current_value;
        if (current_value < minimum && current_value != 0) minimum = current_value;
        value_ptr = strtok(NULL, " ");
    }
    
    printf("Min: %d Max: %d\n", minimum, maximum);
}

void DataProcessor(const char *input_file) 
{
    FILE *file_handle = fopen(input_file, "r");
    if(file_handle == NULL) HandleError("fopen(file_handle)");
    
    size_t name_length = strlen(input_file) + 12; 
    char position_file[name_length];
    snprintf(position_file, name_length, "%s_offset", input_file);
    FILE *offset_handle = fopen(position_file, "r+");
    if (offset_handle == NULL) 
    {
        offset_handle = fopen(position_file, "w+");
        if (offset_handle == NULL) HandleError("fopen(offset_handle)");
    }
    char offset_buffer[15];
    fgets(offset_buffer, sizeof(offset_buffer), offset_handle);
    int read_position = atoi(offset_buffer);
    if (read_position > 0) fseek(file_handle, read_position, SEEK_SET);

    char line_buffer[MAX_ENTRY_SIZE];
    while(fgets(line_buffer, sizeof(line_buffer), file_handle)) 
    {
        ParseDataString(line_buffer);
    }
    
    int current_offset = ftell(file_handle);
    
    rewind(offset_handle);  
    fprintf(offset_handle, "%d\n", current_offset);
    fclose(offset_handle);
    
    fclose(file_handle);
    
}

int main() 
{
    const char *data_file = "shared_data";
    sem_t *semaphore = sem_open(SEMAPHORE_NAME, O_CREAT | O_EXCL, 0644, 1);
    if (semaphore == SEM_FAILED) 
    {
        if (errno == EEXIST) 
        {
            sem_unlink(SEMAPHORE_NAME);
            semaphore = sem_open(SEMAPHORE_NAME, O_CREAT | O_EXCL, 0644, 1);
        }
        if (semaphore == SEM_FAILED) HandleError("sem_open");
    }

    FILE *initial_file = fopen(data_file, "a");
    if (!initial_file) 
    {
        perror("fopen");
        sem_close(semaphore);
        sem_unlink(SEMAPHORE_NAME);
        exit(1);
    }
    fclose(initial_file);

    pid_t child_pid = fork();
    
    switch(child_pid)
    {
        case -1:
            perror("fork");
            sem_close(semaphore);
            sem_unlink(SEMAPHORE_NAME);
            exit(1);
        
        case 0:
            while (1) 
            {
                sem_wait(semaphore);
                
                DataProcessor(data_file);

                sem_post(semaphore);
                sleep(2);
            }
            
            sem_close(semaphore);
            exit(0);
        
        default:
            srand(time(NULL) ^ getpid());

            while (1)
            {
                sem_wait(semaphore);

                DataGenerator(data_file);

                sem_post(semaphore);
                sleep(1);
            }

            wait(NULL);

            sem_close(semaphore);
            sem_unlink(SEMAPHORE_NAME);
    }

    return 0;
}