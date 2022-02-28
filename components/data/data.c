#include <stdio.h>
#include "data.h"


int calc_len(csha_bt_packet* frame)
{
    return strlen(frame->name) + strlen(frame->mac) + (frame->rssi == 0 || frame->rssi == 1 ? 1 : ceil(log10(abs(frame->rssi)))) + (frame->rssi < 0 ? 1:0) + 5 + 18 + 18 + 10;
}
char* format_data(char* str, time_t timestamp, char* sourcemac, csha_bt_packet* frame)
{
    sprintf(str, "%s%s%d%s%s%s%s%s%d%s%s",
            sourcemac, SEPARATOR, 
            (int) timestamp, SEPARATOR,
            frame->name,SEPARATOR, 
            frame->mac,SEPARATOR, 
            frame->rssi,SEPARATOR,SEPARATOR
            );
    return str;
}
