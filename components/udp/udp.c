#include "udp.h"

int send_str(int sock, struct sockaddr_in* destaddr, char* msg_str, int block_size)
{
    int last_end = 0;
    int len = strlen(msg_str);
    for (int i = 0; i < len; i++)
    {
        if (i - last_end >= block_size || i == len - 1)
        {
            sendto(sock, &msg_str[last_end],  i-last_end + (i == len - 1 ? 1 : 0), 0, (const struct sockaddr *)destaddr, sizeof(*destaddr));
            last_end = i;
        }
    }
    return 0;
}
struct sockaddr_in* construct_sockaddr_info(struct sockaddr_in* destaddr, char* addr, int port, int af)
{
    inet_pton(af, addr, &destaddr ->sin_addr);
    destaddr -> sin_port = htons(port);
    destaddr -> sin_family = af;
    return destaddr;
}
int get_sockfd() // if returns < 0, something goofed
{
    return socket(AF_INET, SOCK_DGRAM, 0);
}
int default_af()
{
    return AF_INET;
}
