#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

#define MAX_ENTRY_SIZE 60
#define MAX_VALUES 12
#define MEMORY_NAME "/shared_data_area"

#define SEMAPHORE_NAME "/data_sync_semaphore"

typedef struct MemoryBlock 
{
    int items_count;
    int data_array[100];
    int min_result;
    int max_result;
} MemoryBlock;

int processed_sets = 0;
pid_t child_id;
int running = 1;

void HandleError(const char* message)
{
    perror(message);
    exit(1);
}

void GenerateData(MemoryBlock * mem_block) 
{    
    int values_amount = rand() % MAX_VALUES + 2;
        
    for (int i = 0; i < values_amount; i++) 
    {
         mem_block->data_array[i] = rand() % 1200;
    }
    mem_block->items_count = values_amount;
}

void ProcessData(MemoryBlock * mem_block) 
{
    if (mem_block->items_count > 0) 
    {
        int min_val = INT_MAX;
        int max_val = INT_MIN;
                    
        for (int i = 0; i < mem_block->items_count; i++) 
        {
            if (mem_block->data_array[i] > max_val) max_val = mem_block->data_array[i];
            if (mem_block->data_array[i] < min_val) min_val = mem_block->data_array[i];
        }
                    
        mem_block->min_result = min_val;
        mem_block->max_result = max_val;
    }
}

void SignalHandler(int sig_num) 
{
    if (sig_num == SIGINT) 
    {
        if (child_id != 0)
        {
            printf("\nProgram terminated\n");
            printf("Processed data sets: %d\n", processed_sets);
        }
        running = 0;
    }
}

int main() 
{
    int mem_fd;
    MemoryBlock *shared_area;
    
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
  
    mem_fd = shm_open(MEMORY_NAME, O_CREAT | O_RDWR, 0666);
    if (mem_fd == -1) HandleError("shm_open");
    
    ftruncate(mem_fd, sizeof(MemoryBlock));
    
    shared_area = mmap(NULL, sizeof(MemoryBlock), PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, 0);
    if(shared_area == MAP_FAILED) 
    {
        perror("mmap");
        close(mem_fd);
        exit(1);
    }
    
    signal(SIGINT, SignalHandler);
    
    child_id = fork();
    
    switch(child_id)
    {
        case -1:
            HandleError("fork");
        
        case 0:
            while (running) 
            {
                usleep(3);
                sem_wait(semaphore);
                ProcessData(shared_area);
                sem_post(semaphore);
            }
        
        default:
            srand(time(NULL));
            while (running)
            {
                sem_wait(semaphore);
                GenerateData(shared_area);
                sem_post(semaphore);

                sleep(2); 
                    
                sem_wait(semaphore);    
                if (shared_area->min_result != INT_MAX && shared_area->max_result != INT_MIN) 
                {
                    if (running) printf("Min value: %d\nMax value: %d\n", shared_area->min_result, shared_area->max_result);
                    processed_sets++;
                }
                sem_post(semaphore);
            }
    }
    
    if (child_id > 0) 
    {
        wait(NULL);
    }

    munmap(shared_area, sizeof(MemoryBlock));
    shm_unlink(MEMORY_NAME);
    sem_close(semaphore);
    sem_unlink(SEMAPHORE_NAME);

    return 0;
}