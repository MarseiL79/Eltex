#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <signal.h>

#define KEY_SYMBOL 'M'
#define BUFFER_SIZE 256
#define MAX_USERS 10
#define BASE_ID 10

typedef struct Packet 
{
    long packet_type;
    char packet_data[BUFFER_SIZE];
} Packet;

int queue_identifier = -1;
int users[MAX_USERS];
int users_total = 0;

int LocateUser(int user_id)
{
    for (int i = 0; i < users_total; i++) 
    {
        if (users[i] == user_id) 
        {
            return i;
        }
    }
    return -1;
}

void EraseUser(int user_id)
{
    int position = LocateUser(user_id);
    if (position >= 0) 
    {
        for (int i = position; i < users_total - 1; i++) 
        {
            users[i] = users[i + 1];
        }
        users_total--;
        printf("User %d disconnected\n", user_id);
    }
}

void RegisterUser(int user_id)
{
    if (LocateUser(user_id) < 0 && users_total < MAX_USERS) 
    {
        users[users_total++] = user_id;
        printf("User %d connected\n", user_id);
    }
}

void HandleExit(int sig)
{
    if (queue_identifier != -1) 
    {
        msgctl(queue_identifier, IPC_RMID, NULL);
    }
    printf("\nServer terminated\n");
    exit(0);
}

int main() 
{
    signal(SIGINT, HandleExit);

    key_t queue_key = ftok(".", KEY_SYMBOL);
    if (queue_key == -1) 
    {
        perror("ftok");
        exit(1);
    }

    queue_identifier = msgget(queue_key, 0666 | IPC_CREAT);
    if (queue_identifier == -1) 
    {
        perror("msgget");
        exit(1);
    }

    printf("Server ready for messages\n");

    Packet packet;

    while (1) 
    {
        if (msgrcv(queue_identifier, &packet, sizeof(packet.packet_data), 0, 0) == -1) 
        {
            perror("msgrcv");
            exit(0);
        }

        int sender_id = packet.packet_type;
        RegisterUser(sender_id);

        printf("Message from %d: %s\n", sender_id, packet.packet_data);

        if (strcmp(packet.packet_data, "shutdown") == 0)
        { 
            EraseUser(sender_id);
            continue;
        }

        for (int i = 0; i < users_total; i++) 
        {
            if (users[i] == sender_id) continue;
            
            Packet new_packet;
            new_packet.packet_type = sender_id;
            strncpy(new_packet.packet_data, packet.packet_data, BUFFER_SIZE);

            if (msgsnd(queue_identifier, &new_packet, strlen(new_packet.packet_data)+1, 0) == -1) 
            {
                perror("msgsnd");
            }
        }
    }

    HandleExit(0);
    return 0;
}