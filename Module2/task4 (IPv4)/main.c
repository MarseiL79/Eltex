#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "ipv4.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: %s <gateway_ip> <subnet_mask> <packet_count>\n", argv[0]);
        printf("Example: %s 192.168.1.1 255.255.255.0 1000\n", argv[0]);
        printf("Or:      %s 192.168.1.1 /24 1000\n", argv[0]);
        return 1;
    }

    const char* gatewayStr = argv[1];
    const char* maskStr = argv[2];
    int packetCount = atoi(argv[3]);

    if (packetCount <= 0) {
        printf("Error: packet count must be positive\n");
        return 1;
    }

    uint32_t gateway = parseIp(gatewayStr);
    uint32_t mask = parseMask(maskStr);
    uint32_t subnet = gateway & mask;

    srand(time(NULL));

    int sameSubnetCount = 0;
    int otherSubnetCount = 0;

    printf("=== PACKET SIMULATION ===\n");
    printf("Gateway: ");
    printIp(gateway);
    printf("\nMask: ");
    printIp(mask);
    printf("\nSubnet: ");
    printIp(subnet);
    printf("\nPackets to send: %d\n\n", packetCount);

    for (int i = 0; i < packetCount; i++) {
        uint32_t randomIp = genIp();
        
        if (isInSubnet(randomIp, gateway, mask)) {
            sameSubnetCount++;
        } else {
            otherSubnetCount++;
        }
    }

    printf("=== STATISTICS ===\n");
    printf("Total packets: %d\n", packetCount);
    printf("Same subnet: %d (%.2f%%)\n", 
           sameSubnetCount, (float)sameSubnetCount / packetCount * 100);
    printf("Other subnets: %d (%.2f%%)\n", 
           otherSubnetCount, (float)otherSubnetCount / packetCount * 100);

    return 0;
}