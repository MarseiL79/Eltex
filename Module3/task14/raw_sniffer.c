#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>

#define MAX_PACKET_LENGTH 65535
#define TARGET_PORT 53000

int socket_handler;
socklen_t addr_size;
struct sockaddr_in client_addr;
uint16_t source_port_number;

typedef struct UdpHeader 
{
    uint16_t source_port;
    uint16_t dest_port;
    uint16_t packet_length;
    uint16_t checksum;
} UdpHeader;

void DisplayPacketData(const char *packet_buffer, size_t packet_size) 
{
    int column_counter = 0;
    for (size_t i = 0; i < packet_size; i++) 
    {
        if ((column_counter++ % 16) == 0) printf("\n%04zx: ", i);
        printf("%02hhx ", packet_buffer[i]);
    }
    printf("\n");
}

void ProcessPacket(const char *packet_buffer, size_t packet_size) 
{
    const UdpHeader *udp_header = (UdpHeader*)(packet_buffer + 20);
    const char *packet_data = (char*)(packet_buffer + 20 + sizeof(UdpHeader));
    size_t data_length = packet_size - (20 + sizeof(UdpHeader));
    
    source_port_number = ntohs(udp_header->source_port);

    printf("\nNew packet received!\n");
    printf("UDP Header:\n");
    printf("Source port: %hu\n", ntohs(udp_header->source_port));
    printf("Destination port: %hu\n", ntohs(udp_header->dest_port));
    printf("Packet length: %hu bytes\n", ntohs(udp_header->packet_length));
    printf("Checksum: %hu\n", udp_header->checksum);
    printf("Hex dump:");
    DisplayPacketData(packet_data, data_length);
    printf("Message content: ");
    fwrite(packet_data, data_length, 1, stdout);
    printf("\n");
}

void ShutdownProgram() 
{
    close(socket_handler);
    printf("Program terminated.\n");
    exit(0);
}

void ReportError(const char * error_message) 
{
    perror(error_message);
    close(socket_handler);
    exit(1);
}

void* TransmitPacket(void* arg) 
{
    char user_input[1024];
    char packet_data[MAX_PACKET_LENGTH];

    struct sockaddr_in destination_addr;
    memset(&destination_addr, 0, sizeof(destination_addr));
    destination_addr.sin_family = AF_INET;
    destination_addr.sin_port = htons(TARGET_PORT);

    struct udphdr *udp_hdr = (struct udphdr *)(packet_data + 20);

    while (1) 
    {
        if (!fgets(user_input, sizeof(user_input), stdin)) 
        {
            close(socket_handler);
            break;
        }

        size_t input_length = strlen(user_input);
        size_t total_packet_size = 20 + sizeof(struct udphdr) + input_length;

        udp_hdr->uh_sport = htons(TARGET_PORT);
        udp_hdr->uh_dport = htons(source_port_number);
        udp_hdr->uh_ulen = htons(input_length + sizeof(struct udphdr));
        udp_hdr->uh_sum = 0;

        memcpy(packet_data + 20 + sizeof(struct udphdr), user_input, input_length);

        if (sendto(socket_handler, packet_data, total_packet_size, 0, (struct sockaddr *)&destination_addr, sizeof(destination_addr)) != (ssize_t)total_packet_size) 
        {
            perror("sendto");
            close(socket_handler);
            break;
        }

        if (strcmp(user_input, "quit\n") == 0) ShutdownProgram();
    }
}

void* CapturePackets(void* arg) 
{
    char buffer[MAX_PACKET_LENGTH];
    while (1) 
    {
        ssize_t bytes_received = recvfrom(socket_handler, buffer, MAX_PACKET_LENGTH, 0, (struct sockaddr *)&client_addr, &addr_size);
        if (bytes_received > 0) 
        {
            ProcessPacket(buffer, bytes_received);
        }
    }
}

int main() 
{
    pthread_t packet_receiver, packet_sender;
    
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(TARGET_PORT);
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    socket_handler = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (socket_handler < 0) ReportError("socket");

    fcntl(socket_handler, F_SETFL, O_NONBLOCK);

    printf("Packet sniffer started. Waiting for packets...\n");

    pthread_create(&packet_receiver, NULL, CapturePackets, NULL);
    pthread_create(&packet_sender, NULL, TransmitPacket, NULL);

    pthread_join(packet_receiver, NULL);
    pthread_join(packet_sender, NULL);

    return 0;
}