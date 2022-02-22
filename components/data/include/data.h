#ifndef DATA
#define DATA

#include <string.h>
#include <math.h>

#define SEPARATOR "|"

/*
* represents detection data of bluetooth device
*/
typedef struct {
    char* name;
    char* mac;
    int rssi;
} csha_bt_packet;

/*
* a simple malloc helper function 
* allocates memory for new detection packet and returns pointer to it
*/
csha_bt_packet* new_frame();

/*
* returns the length minimum length of a string that can hold the data formatted to send
*/
int calc_len(csha_bt_packet* frame);

/*
* formats detection data and stores it in str
* formatted as <esp-mac>|<detected-name>|<detected-mac>|<rssi-value>||
* replace | with defined separator
*
* use calc_len to get minimum length of str
* 
* returns pointer to str
*/
char* format_data(char* str, csha_bt_packet* frame);



#endif
