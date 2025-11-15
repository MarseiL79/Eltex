#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ipv4.h"

uint32_t parseIp(const char* ipStr) {
    uint32_t a, b, c, d;
    if (sscanf(ipStr, "%u.%u.%u.%u", &a, &b, &c, &d) != 4) {
        printf("Error: invalid IP format: %s\n", ipStr);
        exit(1);
    }
    if (a > 255 || b > 255 || c > 255 || d > 255) {
        printf("Error: IP octet must be 0-255: %s\n", ipStr);
        exit(1);
    }
    return (a << 24) | (b << 16) | (c << 8) | d;
}

uint32_t parseMask(const char* maskStr) {
    if (maskStr[0] == '/') {
        int bits = atoi(maskStr + 1);
        if (bits < 0 || bits > 32) {
            printf("Error: CIDR mask must be 0-32: %s\n", maskStr);
            exit(1);
        }
        return (0xFFFFFFFF << (32 - bits)) & 0xFFFFFFFF;
    }
    else {
        return parseIp(maskStr);
    }
}

int isInSubnet(uint32_t ip, uint32_t gateway, uint32_t mask) {
    return (ip & mask) == (gateway & mask);
}

uint32_t genIp() {
    uint32_t address = 0;
    for (int i = 0; i < 4; i++) {
        uint8_t octet = rand() % 256;
        address = (address << 8) | octet;
    }
    return address;
}

void printIp(uint32_t ip) {
    printf("%u.%u.%u.%u",
           (ip >> 24) & 0xFF,
           (ip >> 16) & 0xFF,
           (ip >> 8) & 0xFF,
           ip & 0xFF);
}