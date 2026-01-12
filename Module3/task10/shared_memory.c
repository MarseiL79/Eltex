#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <signal.h>
#include <time.h>
#include <limits.h>
#include <errno.h>
#include <sys/sem.h>

#define MAX_DATA_SIZE 60
#define MAX_VALUES 12
#define MEMORY_SIZE sizeof(DataBlock)

typedef struct DataBlock 
{
    int elements_count;
    int values_array[100];
    int min_result;
    int max_result;
} DataBlock;

int operations_counter = 0;
pid_t process_id;
int program_active = 1;

void SignalHandler(const char* msg)
{
    perror(msg);
    exit(1);
}

void GenerateValues(DataBlock * memory_area) 
{    
    int values_amount = rand() % MAX_VALUES + 2;
        
    for (int i = 0; i < values_amount; i++) 
    {
         memory_area->values_array[i] = rand() % 1500;
    }
    memory_area->elements_count = values_amount;
}

void AnalyzeData(DataBlock * memory_area) 
{
    if (memory_area->elements_count > 0) 
    {
        int min_val = INT_MAX;
        int max_val = INT_MIN;
                    
        for (int i = 0; i < memory_area->elements_count; i++) 
        {
            if (memory_area->values_array[i] < min_val) min_val = memory_area->values_array[i];
            if (memory_area->values_array[i] > max_val) max_val = memory_area->values_array[i];
        }
                    
        memory_area->min_result = min_val;
        memory_area->max_result = max_val;
    }
}

void TerminationHandler(int sig_num) 
{
    if (sig_num == SIGINT) 
    {
        if (process_id != 0)
        {
            printf("\nProgram terminated\n");
            printf("Processed data sets: %d\n", operations_counter);
        }
        program_active = 0;
    }
}

union SemaphoreUnion 
{
    int sem_value;
    struct semid_ds *sem_info;
    unsigned short *sem_array;
    struct seminfo *sys_info;
};

int main() 
{
    key_t resource_key;
    int shared_mem_id;
    DataBlock *memory_pointer;
    int semaphore_id;
    union SemaphoreUnion sem_arg;
    struct sembuf acquire_sem = {0, -1, SEM_UNDO};
    struct sembuf release_sem = {0, 1, SEM_UNDO};
  
    resource_key = ftok(".", 'X');
    if(resource_key == -1) 
    {
        perror("ftok");
        exit(1);
    }
    
    shared_mem_id = shmget(resource_key, MEMORY_SIZE, IPC_CREAT | 0777);
    if (shared_mem_id == -1)
    {
        perror("shmget");
        exit(1);
    }
    
    memory_pointer = (DataBlock *)shmat(shared_mem_id, NULL, 0);
    
    semaphore_id = semget(resource_key, 1, 0666 | IPC_CREAT);
    if(semaphore_id == -1) SignalHandler("semget");
    
    sem_arg.sem_value = 1;
    int sem_status = semctl(semaphore_id, 0, SETVAL, sem_arg);
    if (sem_status == -1) SignalHandler("semctl(SETVAL)");
    
    signal(SIGINT, TerminationHandler);

    process_id = fork();
    
    switch(process_id)
    {
        case -1:
            perror("fork");
            exit(1);
        
        case 0:
            while (program_active) 
            {
                sleep(3);
                sem_status = semctl(semaphore_id, 0, GETVAL, sem_arg);
                if (sem_status == -1) SignalHandler("semctl(GETVAL)");
                
                if (sem_status == 1) 
                {
                    if (semop(semaphore_id, &acquire_sem, 1) == -1)
                    { 
                        perror("semop:acquire_sem");  
                    }
                    AnalyzeData(memory_pointer);
                }
                
                sem_status = semctl(semaphore_id, 0, GETVAL, sem_arg);
                if (sem_status == -1) SignalHandler("semctl(GETVAL)");
                
                if (sem_status == 0)
                {
                    semop(semaphore_id, &release_sem, 1);
                }
            }
        
        default:
            srand(time(NULL));
            while (program_active)
            {
                sleep(1);
                sem_status = semctl(semaphore_id, 0, GETVAL, sem_arg);
                if (sem_status == -1) SignalHandler("semctl(GETVAL)");
                
                if (sem_status == 1) 
                {
                    if (semop(semaphore_id, &acquire_sem, 1) == -1)
                    { 
                        perror("semop:acquire_sem");  
                    }
                    GenerateValues(memory_pointer);
                }
                
                sem_status = semctl(semaphore_id, 0, GETVAL, sem_arg);
                if (sem_status == -1) SignalHandler("semctl(GETVAL)");
                
                if (sem_status == 0)
                {
                    semop(semaphore_id, &release_sem, 1);
                }
                
                sleep(2);
                
                sem_status = semctl(semaphore_id, 0, GETVAL, sem_arg);
                if (sem_status == -1) SignalHandler("semctl(GETVAL)");
                
                if (sem_status == 1) 
                {
                    if (semop(semaphore_id, &acquire_sem, 1) == -1)
                    { 
                        perror("semop:acquire_sem");  
                    }
                    
                    if (memory_pointer->min_result != INT_MAX && memory_pointer->max_result != INT_MIN) 
                    {
                        if (program_active) printf("Min: %d\nMax: %d\n", memory_pointer->min_result, memory_pointer->max_result);
                        operations_counter++;
                    }
                }
                
                sem_status = semctl(semaphore_id, 0, GETVAL, sem_arg);
                if (sem_status == -1) SignalHandler("semctl(GETVAL)");
                
                if (sem_status == 0)
                {
                    semop(semaphore_id, &release_sem, 1);
                }                      
            }
    }
    
    if (process_id > 0) 
    {
        wait(NULL);
    }

    shmdt(memory_pointer);
    shmctl(shared_mem_id, IPC_RMID, NULL);

    return 0;
}