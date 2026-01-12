#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>

int socket_descriptor;
socklen_t client_length;
struct sockaddr_in server_addr, client_addr;

void TerminateProgram()
{
    close(socket_descriptor);
    printf("Chat terminated\n");
    exit(0);
}

void* TransmitData(void* arg) 
{
    ssize_t bytes_sent;
    char outgoing_buffer[1024];
    while (1)
    {        
        if (!fgets(outgoing_buffer, sizeof(outgoing_buffer), stdin)) 
        {
            close(socket_descriptor);
            break;
        }

        size_t message_length = strlen(outgoing_buffer);
        if (sendto(socket_descriptor, outgoing_buffer, message_length, 0, (struct sockaddr *)&client_addr, sizeof(client_addr)) != (ssize_t)message_length) 
        {
            perror("sendto");
            close(socket_descriptor);
            break;
        }
        
        if (strcmp(outgoing_buffer, "quit\n") == 0) TerminateProgram();
    }
}

void* ReceiveData(void* arg) 
{
    ssize_t bytes_received;
    char incoming_buffer[1024];
    while (1)
    {      
        client_length = sizeof(client_addr);     
        bytes_received = recvfrom(socket_descriptor, incoming_buffer, sizeof(incoming_buffer) - 1, 0, (struct sockaddr *)&client_addr, &client_length);
        while (bytes_received >= 0)
        {
            if (strcmp(incoming_buffer, "quit\n") == 0) TerminateProgram();
            incoming_buffer[bytes_received] = '\0';
            printf("Port %d: %s", ntohs(client_addr.sin_port), incoming_buffer);
            bytes_received = recvfrom(socket_descriptor, incoming_buffer, sizeof(incoming_buffer) - 1, 0, (struct sockaddr *)&client_addr, &client_length);
        }
    }
}

int main()
{    
    pthread_t transmitter, receiver;
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(52000);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("socket");
        return 1;
    }
    
    fcntl(socket_descriptor, F_SETFL, O_NONBLOCK);

    if (bind(socket_descriptor, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {
        perror("bind");
        close(socket_descriptor);
        return 1;
    }

    printf("Chat started. Send messages.\n");
    
    pthread_create(&transmitter, NULL, TransmitData, NULL);
    pthread_create(&receiver, NULL, ReceiveData, NULL);
    
    pthread_join(transmitter, NULL); 
    pthread_join(receiver, NULL); 
    
    return 0;
}