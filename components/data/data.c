#include <stdio.h>
#include "data.h"


int calc_len(csha_bt_packet* frame)
{
    return strlen(frame->name) + strlen(frame->mac) + (frame->rssi == 0 || frame->rssi == 1 ? 1 : ceil(log10(abs(frame->rssi)))) + (frame->rssi < 0 ? 1:0) + 5 + 18;
}
char* format_data(char* str, csha_bt_packet* frame)
{
    sprintf(str, "%s%s%s%s%d%s%s", frame->name,SEPARATOR, frame->mac,SEPARATOR, frame->rssi,SEPARATOR,SEPARATOR);
    return str;
}
