#ifndef DATA
#define DATA

#include <string.h>
#include <math.h>

#define SEPARATOR "|"

/*
* represents detection data of bluetooth device
*/
typedef struct {
    char name[100];
    char mac[18];
    int rssi;
} csha_bt_packet;

/*
* returns the length minimum length of a string that can hold the data formatted to send
*/
int calc_len(csha_bt_packet* frame);

/*
* formats detection data and stores it in str
* formatted as <esp-mac>|<timestamp>|<detected-name>|<detected-mac>|<rssi-value>||
* replace | with defined separator
*
* use calc_len to get minimum length of str
* 
* returns pointer to str
*/
char* format_data(char* str, time_t timestamp, char* sourcemac, csha_bt_packet* frame);



#endif
