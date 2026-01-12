#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/poll.h>

#define WAIT_TIMEOUT (-1)
#define MAX_CONNECTIONS 100

int active_clients = 0;

void ProcessClient(int);
void ReportError(const char *message)
{
    perror(message);
    exit(1);
}

int ComputeResult(int x_val, int y_val, char operation) 
{
    switch (operation)
    {
        case '+':
            return x_val + y_val;
        case '-':
            return x_val - y_val;
        case '*':
            return x_val * y_val;
        case '/':
            if (y_val != 0) return x_val / y_val;
            else return 2147483647;
        default:
            return 2147483647;
    }
}

void ReceiveFileData(int client_socket) 
{
    size_t filename_size;
    read(client_socket, &filename_size, sizeof(size_t));

    char filename[filename_size + 1];
    read(client_socket, filename, filename_size);
    filename[filename_size] = '\0';

    off_t file_size;
    read(client_socket, &file_size, sizeof(off_t));

    FILE *file_ptr = fopen(filename, "wb");
    if(file_ptr == NULL) ReportError("ERROR opening file\n");

    unsigned char buffer[1024];
    ssize_t total_bytes = 0;
    while(total_bytes < file_size) 
    {
        ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer));
        fwrite(buffer, 1, bytes_read, file_ptr);
        total_bytes += bytes_read;
    }

    fclose(file_ptr);
    char response[1024];
    snprintf(response, sizeof(response), "File '%s' received, %ld bytes.\n", filename, file_size);
    printf("%s", response);
    send(client_socket, response, strlen(response), 0);
}

void ShowConnections()
{
    if (active_clients) 
    {
        printf("%d clients connected\n\n", active_clients);
    }
    else 
    {
        printf("No active connections\n\n");
    }
}

int main(int argc, char *argv[])
{
    char buffer[1024];
    int main_socket, client_socket;
    int port_number;
    socklen_t addr_length;
    struct sockaddr_in server_address, client_address;
    struct addrinfo addr_info, *result;
    char host_name[NI_MAXHOST];
    char service_info[NI_MAXSERV];
    struct pollfd descriptors[MAX_CONNECTIONS + 1];
    int descriptor_count = 1;

    printf("TCP SERVER WITH MULTIPLEXING\n");

    if (argc < 2) 
    {
        fprintf(stderr, "ERROR: Port required\n");
        exit(1);
    }

    main_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (main_socket < 0) ReportError("ERROR creating socket");

    memset(&server_address, 0, sizeof(server_address));
    port_number = atoi(argv[1]);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port_number);

    if (bind(main_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
        ReportError("ERROR binding");

    listen(main_socket, 5);
    addr_length = sizeof(client_address);
    
    descriptors[0].fd = main_socket;
    descriptors[0].events = POLLIN; 

    while (1)
    {
        int poll_result = poll(descriptors, descriptor_count, WAIT_TIMEOUT);

        if (poll_result < 0) 
        {
            ReportError("Error in poll()");
        }
        else if (poll_result == 0)
        {
            continue;
        }
        else
        {
            for (int i = 0; i < descriptor_count; i++) 
            {
                if (descriptors[i].revents & POLLIN) 
                {
                    if (descriptors[i].fd == main_socket) 
                    {
                        client_socket = accept(main_socket, (struct sockaddr*)&client_address, &addr_length);
                        if (client_socket < 0) ReportError("Error accepting connection");
                        ReceiveFileData(client_socket);                         

                        if (active_clients >= MAX_CONNECTIONS) 
                        {
                            close(client_socket);
                            continue;
                        }
                        
                        getnameinfo((struct sockaddr*)&client_address, addr_length, host_name, NI_MAXHOST, 
                             service_info, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

                        descriptors[descriptor_count].fd = client_socket;
                        descriptors[descriptor_count].events = POLLIN;
                        descriptor_count++;
                        active_clients++;

                        printf("+%s [%s] connected!\n", host_name, inet_ntoa(client_address.sin_addr));
                        ShowConnections();
                    }
                    else
                    {
                        ProcessClient(descriptors[i].fd);
                        active_clients--;
                        printf("-client disconnected\n");
                        ShowConnections();
                        close(descriptors[i].fd);
                        descriptors[i] = descriptors[descriptor_count - 1];
                        descriptor_count--;
                    }
                }
            }
        }
    }

    close(main_socket);
    return 0;
}

void ProcessClient(int client_fd)
{
    int received_bytes;
    int first_val, second_val;
    char data_buffer[20*1024];
    #define msg1 "Select operation (+, -, *, /)\r\n"
    #define msg2 "Enter first number\r\n"
    #define msg3 "Enter second number\r\n"
    #define err_msg "ERROR in input\r\n"

    send(client_fd, msg1, strlen(msg1), 0);

    received_bytes = recv(client_fd, data_buffer, sizeof(data_buffer), 0);
    if (received_bytes <= 0) return;
    if (!strcmp(&data_buffer[0], "end\n")) return;
    char operation = data_buffer[0];

    send(client_fd, msg2, strlen(msg2), 0);

    received_bytes = recv(client_fd, data_buffer, sizeof(data_buffer), 0);
    if (received_bytes <= 0) return;
    if (!strcmp(&data_buffer[0], "end\n")) return;
    first_val = atoi(data_buffer);
    
    send(client_fd, msg3, strlen(msg3), 0);

    received_bytes = recv(client_fd, data_buffer, sizeof(data_buffer), 0);
    if (received_bytes <= 0) return;
    if (!strcmp(&data_buffer[0], "end\n")) return;
    second_val = atoi(data_buffer);

    float calculation_result = ComputeResult(first_val, second_val, operation);
    if (calculation_result != 2147483647)
    {
        snprintf(data_buffer, sizeof(data_buffer), "%f\r\n", calculation_result);
        send(client_fd, data_buffer, strlen(data_buffer), 0);
    }
    else 
    {
        send(client_fd, err_msg, strlen(err_msg), 0);
    }

    return;
}