#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/stat.h>

void HandleError(const char *message) 
{
    perror(message);
    exit(0);
}

void TransferFile(int connection_fd, const char* filename) 
{
    struct stat file_stat;
    if(stat(filename, &file_stat) != 0) HandleError("ERROR accessing file\n");

    size_t name_length = strlen(filename);
    write(connection_fd, &name_length, sizeof(size_t));

    write(connection_fd, filename, name_length);

    off_t file_size = file_stat.st_size;
    write(connection_fd, &file_size, sizeof(off_t));

    FILE *file_ptr = fopen(filename, "rb");
    if(file_ptr == NULL) HandleError("ERROR opening file\n");

    unsigned char data_buffer[1024];
    ssize_t bytes_read;
    while((bytes_read = fread(data_buffer, 1, sizeof(data_buffer), file_ptr)) > 0) 
    {
        write(connection_fd, data_buffer, bytes_read);
    }

    fclose(file_ptr);
}

int main(int argc, char *argv[])
{
    int socket_fd, port_number, bytes_read;
    struct sockaddr_in server_address;
    struct hostent *server_host;
    char data_buffer[1024];
    
    printf("TCP CLIENT\n");
    
    if (argc < 3) 
    {
        fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
        exit(0);
    }

    port_number = atoi(argv[2]);

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) HandleError("ERROR creating socket");

    server_host = gethostbyname(argv[1]);
    if (server_host == NULL) 
    {
        fprintf(stderr, "ERROR, host not found\n");
        exit(0);
    }
    
    bzero((char*) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy((char*)server_host->h_addr, (char *)&server_address.sin_addr.s_addr, server_host->h_length);
    
    server_address.sin_port = htons(port_number);

    if (connect(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
        HandleError("ERROR connecting");
    
    TransferFile(socket_fd, "datafile.txt");
    bytes_read = recv(socket_fd, &data_buffer[0], sizeof(data_buffer) - 1, 0);
    data_buffer[bytes_read] = 0;
    printf("Server: %s", data_buffer);
    
    while ((bytes_read = recv(socket_fd, &data_buffer[0], sizeof(data_buffer) - 1, 0)) > 0)
    {        
        data_buffer[bytes_read] = 0;
        
        printf("Server: %s", data_buffer);
        
        printf("Client: ");
        fgets(&data_buffer[0], sizeof(data_buffer) - 1, stdin);
        
        send(socket_fd, &data_buffer[0], strlen(&data_buffer[0]), 0);

        if (!strcmp(&data_buffer[0], "end\n")) 
        {
            printf("Terminating...\n");
            close(socket_fd);
            return 0;
        }
    }
    
    printf("Terminating...\n");
    close(socket_fd);
    return 0;
}