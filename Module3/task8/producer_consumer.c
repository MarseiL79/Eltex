#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <limits.h>

#define MAX_STRING_SIZE 60
#define MAX_VALUES_COUNT 12

void ReportError(const char* msg)
{
    perror(msg);
    exit(1);
}

void GenerateData(const char *datafile) 
{
    FILE *data_stream = fopen(datafile, "a+");
    if(data_stream == NULL) ReportError("fopen");
    
    char generated_string[MAX_STRING_SIZE] = ""; 
    int values_amount = rand() % MAX_VALUES_COUNT + 1;
        
    for (int i = 0; i < values_amount; i++) 
    {
        snprintf(generated_string + strlen(generated_string),
        sizeof(generated_string)-strlen(generated_string), "%d ", rand() % 200);
    }
        
    fprintf(data_stream, "%s\n", generated_string);
    fflush(data_stream);
    sleep(rand()%2);
    
    fclose(data_stream);
}

void AnalyzeString(char *input_line) 
{
    char *value_token = strtok(input_line, " ");
    int smallest = INT_MAX, largest = INT_MIN;
    
    while (value_token != NULL) 
    {
        int current_value = atoi(value_token);
        if (current_value > largest) largest = current_value;
        if (current_value < smallest && current_value != 0) smallest = current_value;
        value_token = strtok(NULL, " ");
    }
    
    printf("Min: %d Max: %d\n", smallest, largest);
}

void ProcessData(const char *datafile) 
{
    FILE *data_stream = fopen(datafile, "r");
    if(data_stream == NULL) ReportError("fopen(data_stream)");
    
    size_t name_len = strlen(datafile) + 10; 
    char position_file[name_len];
    snprintf(position_file, name_len, "%s_position", datafile);
    FILE *pos_stream = fopen(position_file, "r+");
    if (pos_stream == NULL) 
    {
        pos_stream = fopen(position_file, "w+");
        if (pos_stream == NULL) ReportError("fopen(pos_stream)");
    }
    char position_buffer[15];
    fgets(position_buffer, sizeof(position_buffer), pos_stream);
    int file_position = atoi(position_buffer);
    if (file_position > 0) fseek(data_stream, file_position, SEEK_SET);

    char line_buffer[MAX_STRING_SIZE];
    while(fgets(line_buffer, sizeof(line_buffer), data_stream)) 
    {
        AnalyzeString(line_buffer);
    }
    
    int current_position = ftell(data_stream);
    
    rewind(pos_stream);  
    fprintf(pos_stream, "%d\n", current_position);
    fclose(pos_stream);
    
    fclose(data_stream);
    
}

union SemaphoreControl 
{
    int value;
    struct semid_ds *sem_stat;
    unsigned short *array_values;
    struct seminfo *info_buffer;
};

int main(int argc, char *argv[])
{
    pid_t process_id;
    key_t sem_key;
    int semaphore_id;
    union SemaphoreControl sem_arg;
    struct sembuf acquire_lock = {0, -1, SEM_UNDO};
    struct sembuf release_lock = {0, 1, SEM_UNDO};
    const char *data_filename = argv[2];
    
    if(argc < 3) 
    {
        printf("Usage: %s mode datafile\n", argv[0]);
        exit(0);
    }
    
    if (!fopen(data_filename, "a"))
    {
        FILE *tmp_file = fopen(data_filename, "a");
        fclose(tmp_file);
    }
  
    sem_key = ftok(data_filename, getpid());
    if(sem_key == -1) ReportError("ftok");

    semaphore_id = semget(sem_key, 1, 0666 | IPC_CREAT);
    if(semaphore_id == -1) ReportError("semget");
    
    sem_arg.value = 1;
    int sem_value = semctl(semaphore_id, 0, SETVAL, sem_arg);
    if (sem_value == -1) ReportError("semctl(SETVAL)");

    if (strcmp(argv[1], "generate") == 0) 
    {
        printf("Generator started with file <%s>\n", data_filename);
        
        while (1)
        {
            sem_value = semctl(semaphore_id, 0, GETVAL, sem_arg);
            if (sem_value == -1) ReportError("semctl(GETVAL)");
            
            if (sem_value == 1) 
            {
                if (semop(semaphore_id, &acquire_lock, 1) == -1)
                { 
                    perror("semop:acquire_lock");  
                }
                    
                GenerateData(data_filename);
                printf("Data written to <%s>\n", data_filename);
            }
            
            sem_value = semctl(semaphore_id, 0, GETVAL, sem_arg);
            if (sem_value == -1) ReportError("semctl(GETVAL)");
            
            if (sem_value == 0)
            {
                semop(semaphore_id, &release_lock, 1);
            }
        }        
    } 
    else if (strcmp(argv[1], "process") == 0) 
    {
        printf("Processor started with file <%s>\n", data_filename);
        
        while (1)
        {
            sleep(rand()%3);
            sem_value = semctl(semaphore_id, 0, GETVAL, sem_arg);
            if (sem_value == -1) ReportError("semctl(GETVAL)");
                    
            if (sem_value == 1)
            {
                if (semop(semaphore_id, &acquire_lock, 1) == -1)
                { 
                    perror("semop:acquire_lock"); 
                }
      
                ProcessData(data_filename);
            }
            
            sem_value = semctl(semaphore_id, 0, GETVAL, sem_arg);
            if (sem_value == -1) ReportError("semctl(GETVAL)");

            if (sem_value == 0)
            {
                semop(semaphore_id, &release_lock, 1);
            }
            
            sleep(rand()%3);
        }
    } 
    else 
    {
        ReportError("Invalid mode. Use 'generate' or 'process'.\n");
    }
   
    return 0;
}