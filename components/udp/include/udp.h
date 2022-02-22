#ifndef _UDP_H_
#define _UDP_H_

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>


static struct sockaddr_in active_dest;
static int sockfd = -1;

int send_str(char* msg_str, int block_size);

/**
 * !!! MUST BE CALLED ONCE BEFORE ATTEMPTING TO SEND OVER UDP !!!
 * returns true on success
 */
bool init_udp_socket(char* dest_addr, int dest_port);
int default_af();
int get_sockfd();
bool socket_ready();

#endif
