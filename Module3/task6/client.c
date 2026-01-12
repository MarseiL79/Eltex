#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define KEY_SYMBOL 'M'
#define BUFFER_SIZE 256

typedef struct Packet 
{
    long packet_type;
    char packet_data[BUFFER_SIZE];
} Packet;

int queue_handle = -1;

void FetchMessages(int user_id)
{
    Packet received_packet;
    while (1) 
    {
        if (msgrcv(queue_handle, &received_packet, sizeof(received_packet.packet_data), 0, 0) == -1) 
        {
            perror("msgrcv");
            exit(1);
        }
        
        if (received_packet.packet_type != user_id)
        {
            printf("Received from %ld: %s\n", received_packet.packet_type, received_packet.packet_data);
        }
    }
}

int main(int argc, char *argv[]) 
{
    if (argc != 2 || atoi(argv[1]) <= 10) 
    {
        fprintf(stderr, "Usage: %s <client id > 10>\n", argv[0]);
        exit(1);
    }

    int user_id = atoi(argv[1]);

    key_t queue_key = ftok(".", KEY_SYMBOL);
    if (queue_key == -1) 
    {
        perror("ftok");
        exit(1);
    }

    queue_handle = msgget(queue_key, 0666 | IPC_CREAT);
    if (queue_handle == -1) 
    {
        perror("msgget");
        exit(1);
    }

    printf("Client %d started. Enter messages:\n", user_id);

    pid_t process_id = fork();
    if (process_id == 0) 
    {
        FetchMessages(user_id);
        exit(0);
    }

    char user_input[BUFFER_SIZE];
    while (1) 
    {
        fgets(user_input, BUFFER_SIZE, stdin);
        user_input[strlen(user_input)-1] = '\0';

        Packet outgoing_packet;
        outgoing_packet.packet_type = user_id;
        strncpy(outgoing_packet.packet_data, user_input, BUFFER_SIZE);

        if (msgsnd(queue_handle, &outgoing_packet, strlen(outgoing_packet.packet_data)+1, 0) == -1) 
        {
            perror("msgsnd");
        }
        
        if (msgsnd(queue_handle, &outgoing_packet, strlen(outgoing_packet.packet_data)+1, 0) == -1) 
        {
            perror("msgsnd");
        }
        
        if (strcmp(user_input, "shutdown") == 0) 
        {
            break;
        }
    }

    kill(process_id, SIGTERM);
    wait(NULL);
    printf("Client %d finished\n", user_id);

    return 0;
}