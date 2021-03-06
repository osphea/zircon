#ifndef SYSROOT_NETINET_ETHER_H_
#define SYSROOT_NETINET_ETHER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <netinet/if_ether.h>

char* ether_ntoa(const struct ether_addr*);
struct ether_addr* ether_aton(const char*);
char* ether_ntoa_r(const struct ether_addr*, char*);
struct ether_addr* ether_aton_r(const char*, struct ether_addr*);
int ether_line(const char*, struct ether_addr*, char*);
int ether_ntohost(char*, const struct ether_addr*);
int ether_hostton(const char*, struct ether_addr*);

#ifdef __cplusplus
}
#endif

#endif  // SYSROOT_NETINET_ETHER_H_
