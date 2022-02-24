#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
//#include "esp_wps.h"
#include "esp_event.h"
//#include "nvs_flash.h"
#include <string.h>
#include <stdint.h>
#include "esp_system.h"

#define WIFI_TAG "CSHA Wifi"

static char wifi_mac_str[17];

void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data);
void got_ip_event_handler(void* arg, esp_event_base_t event_base,
                             int32_t event_id, void* event_data);
void start_wifi(char* wifi_ssid, char* wifi_pass);

/*
* converts 6 byte mac address to string representation and stores it in str
* str length must be at least 17
* formatted as 00:00:00:00:00:00
* returns pointer to str
*/
char* byte_mac_to_str(char* str, uint8_t* mac);

/*
* extracts ESP's default MAC and stores it in str
* str length be at least 17
* formatted as 00:00:00:00:00:00
* returns pointer to str
*/
char* get_mac_str(char* str);
