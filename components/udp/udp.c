#include "udp.h"


int send_str(char* msg_str, int block_size)
{
    int last_end = 0;
    int len = strlen(msg_str);
    for (int i = 0; i < len; i++)
    {
        if (i - last_end >= block_size || i == len - 1)
        {
            sendto(sockfd, &msg_str[last_end],  i-last_end + (i == len - 1 ? 1 : 0), 0, (const struct sockaddr*)&active_dest, sizeof(active_dest));
            last_end = i;
        }
    }
    return 0;
}
bool init_udp_socket(char* dest_addr, int dest_port)
{
    int af = default_af();
    inet_pton(af, dest_addr, &active_dest.sin_addr);
    active_dest.sin_port = htons(dest_port);
    active_dest.sin_family = af;
    sockfd = get_sockfd();
    return sockfd >= 0;
}
int get_sockfd() // if returns < 0, something goofed
{
    return socket(AF_INET, SOCK_DGRAM, 0);
}
int default_af()
{
    return AF_INET;
}
bool socket_ready()
{
    return sockfd >= 0;
}
