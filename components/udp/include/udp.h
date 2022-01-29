#ifndef UDP
#define UDP

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

int send_str(int sock, struct sockaddr_in* destaddr, char* msg_str, int block_size);
struct sockaddr_in* construct_sockaddr_info(struct sockaddr_in* destaddr, char* addr, int port, int af);
int default_af();
int get_sockfd();

#endif
