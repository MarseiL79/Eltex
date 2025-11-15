#include <stdint.h>

uint32_t parseIp(const char* ipStr);
uint32_t parseMask(const char* maskStr);
int isInSubnet(uint32_t ip, uint32_t gateway, uint32_t mask);
uint32_t genIp(void);
void printIp(uint32_t ip);
