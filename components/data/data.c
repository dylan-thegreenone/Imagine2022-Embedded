#include <stdio.h>
#include "data.h"

csha_bt_packet* new_frame()
{
    return malloc(sizeof(csha_bt_packet));
}

int calc_len(csha_bt_packet* frame)
{
    return strlen(frame->name) + strlen(frame->mac) + (frame->rssi == 0 || frame->rssi == 1 ? 1 : ceil(log10(frame->rssi))) + 5 + 17;
}
char* format_data(char* str, csha_bt_packet* frame)
{
    sprintf(str, "%s%s%s%s%d%s%s", frame->name,SEPARATOR, frame->mac,SEPARATOR, frame->rssi,SEPARATOR,SEPARATOR);
    return str;
}
