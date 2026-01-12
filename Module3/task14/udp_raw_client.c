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

int socket_desc;
socklen_t client_length;
struct sockaddr_in server_addr, client_addr;

void TerminateProgram()
{
    close(socket_desc);
    printf("Chat terminated\n");
    exit(0);
}

void* SendData(void* arg) 
{
    ssize_t bytes_transferred;
    char output_buffer[1024];
    
    while(1)
    {
        if (!fgets(output_buffer, sizeof(output_buffer), stdin)) 
        {
            close(socket_desc);
            break;
        }

        size_t data_length = strlen(output_buffer);
        if (sendto(socket_desc, output_buffer, data_length, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) != (ssize_t)data_length) 
        {
            perror("sendto");
            close(socket_desc);
            break;
        }
        
        if (strcmp(output_buffer, "quit\n") == 0) TerminateProgram();
    }
}

void* ReceiveData(void* arg) 
{
    ssize_t bytes_received;
    char input_buffer[1024];
    
    while(1)
    {
        client_length = sizeof(client_addr);     
        bytes_received = recvfrom(socket_desc, input_buffer, sizeof(input_buffer) - 1, 0, (struct sockaddr *)&client_addr, &client_length);
        while (bytes_received >= 0)
        {
            if (strcmp(input_buffer, "quit\n") == 0) TerminateProgram();
            input_buffer[bytes_received] = '\0';
            printf("Port %d: %s", ntohs(client_addr.sin_port), input_buffer);
            bytes_received = recvfrom(socket_desc, input_buffer, sizeof(input_buffer) - 1, 0, (struct sockaddr *)&client_addr, &client_length);
        }
    }
}

int main(int argc, char **argv)
{
    pthread_t sender, receiver;
    
    if (argc != 2) 
    {
        fprintf(stderr, "Usage: %s <server IP>\n", argv[0]);
        return 1;
    }

    if ((socket_desc = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("socket");
        return 1;
    }
    
    fcntl(socket_desc, F_SETFL, O_NONBLOCK);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(53000); 
    if (inet_aton(argv[1], &server_addr.sin_addr) == 0) 
    {
        fprintf(stderr, "Invalid IP address\n");
        close(socket_desc);
        return 1;
    }
    
    printf("Chat client started. Send messages.\n");
    
    pthread_create(&sender, NULL, SendData, NULL);
    pthread_create(&receiver, NULL, ReceiveData, NULL);
    
    pthread_join(sender, NULL); 
    pthread_join(receiver, NULL); 
    
    return 0;
}