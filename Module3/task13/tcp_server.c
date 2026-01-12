#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int active_connections = 0;

void ProcessClient(int);
void ShowError(const char *message)
{
    perror(message);
    exit(1);
}

int CalculateValues(int x, int y, char operation) 
{
    switch (operation)
    {
        case '+':
            return x + y;
        case '-':
            return x - y;
        case '*':
            return x * y;
        case '/':
            if (y != 0) return x / y;
            else return 2147483647;
        default:
            return 2147483647;
    }
}

void AcceptFile(int client_socket) 
{
    size_t file_name_length;
    read(client_socket, &file_name_length, sizeof(size_t));

    char file_name[file_name_length + 1];
    read(client_socket, file_name, file_name_length);
    file_name[file_name_length] = '\0';

    off_t file_size;
    read(client_socket, &file_size, sizeof(off_t));

    FILE *file_ptr = fopen(file_name, "wb");
    if(file_ptr == NULL) ShowError("ERROR opening file\n");

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
    snprintf(response, sizeof(response), "File '%s' received, size %ld bytes.\n", file_name, file_size);
    printf("%s", response);
    send(client_socket, response, strlen(response), 0);
}

void DisplayConnections()
{
    if (active_connections) 
    {
        printf("%d clients connected\n\n", active_connections);
    }
    else 
    {
        printf("No clients connected\n\n");
    }
}

int main(int argc, char *argv[])
{
    char buffer[1024];
    int main_socket, client_socket;
    int port_number;
    int process_id;
    socklen_t client_length;
    struct sockaddr_in server_address, client_address;
    struct addrinfo address_info, *result;
    char host_name[NI_MAXHOST];
    char service_name[NI_MAXSERV];

    printf("TCP SERVER APPLICATION\n");

    if (argc < 2) 
    {
        fprintf(stderr, "ERROR, port not specified\n");
        exit(1);
    }

    main_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (main_socket < 0) ShowError("ERROR creating socket");

    memset(&server_address, 0, sizeof(server_address));
    port_number = atoi(argv[1]);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port_number);

    if (bind(main_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
        ShowError("ERROR binding");

    listen(main_socket, 5);
    client_length = sizeof(client_address);
    
    while (1)
    {   
        client_socket = accept(main_socket, (struct sockaddr *) &client_address, &client_length);
        if(client_socket < 0) ShowError("ERROR accepting connection");
        AcceptFile(client_socket); 
        active_connections++;

        getnameinfo((struct sockaddr*)&client_address, client_length, host_name, NI_MAXHOST, 
            service_name, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

        printf("+%s [%s] connected!\n", host_name, inet_ntoa(client_address.sin_addr));
        DisplayConnections();

        process_id = fork();
        if (process_id < 0) ShowError("Error in fork");
        if (process_id == 0) 
        {
            close(main_socket);
            ProcessClient(client_socket);
            exit(0);
        }
        else close(client_socket);
    }

    close(main_socket);
    return 0;
}

void ProcessClient(int client_socket)
{
    int received_bytes;
    int value_a, value_b;
    char data_buffer[20*1024];
    #define msg1 "Select operation (+, -, *, /)\r\n"
    #define msg2 "Enter first number\r\n"
    #define msg3 "Enter second number\r\n"
    #define err_msg "ERROR in input\r\n"

    write(client_socket, msg1, sizeof(msg1)-1);

    received_bytes = recv(client_socket, data_buffer, sizeof(data_buffer), 0);
    if (received_bytes <= 0) ShowError("ERROR reading from socket");
    char operation = data_buffer[0];

    write(client_socket, msg2, sizeof(msg2)-1);

    received_bytes = recv(client_socket, data_buffer, sizeof(data_buffer), 0);
    if (received_bytes <= 0) ShowError("ERROR reading from socket");
    value_a = atoi(data_buffer);
    
    write(client_socket, msg3, sizeof(msg3)-1);

    received_bytes = recv(client_socket, data_buffer, sizeof(data_buffer), 0);
    if (received_bytes <= 0) ShowError("ERROR reading from socket");
    value_b = atoi(data_buffer);

    float result = CalculateValues(value_a, value_b, operation);
    if (result != 2147483647)
    {
        snprintf(data_buffer, sizeof(data_buffer), "%f\r\n", result);
        send(client_socket, data_buffer, strlen(data_buffer), 0);
    }
    else 
    {
        send(client_socket, err_msg, strlen(err_msg), 0);
    }

    active_connections--;
    printf("-disconnected\n");
    DisplayConnections();
    return;
}