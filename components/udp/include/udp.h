#ifndef _UDP_H_
#define _UDP_H_

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

// maximum safe bytes per block for udp packets
#define MAX_SAFE_UDP_BLOCK_SIZE 508

/*
* send packet containg just a string over UDP
* can be chunked based on block_size.  No length of string sent will exceed block_size
* will automatically send full string in appropriately sized chunks
*/
int udp_send_str(char* msg_str, int block_size);

/**
* setup UDP socket on wifi interface
* sometimes works even without wifi, so not reliable indicator of connection status
* !!! MUST BE CALLED ONCE BEFORE ATTEMPTING TO SEND OVER UDP !!!
* returns true on success
*/
bool init_udp_socket(char* dest_addr, int dest_port);

/*
* get index of default Address Family
*/
int default_af();

/*
* get socket file descriptor
* automatically called in init_udp_socket
*/
int get_sockfd();

/* 
* checks whether socket is open and ready to send data
* socket can still open without wifi, so not a reliable connection status indicator
*/
bool socket_ready();

#endif
