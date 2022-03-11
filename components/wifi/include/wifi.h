#ifndef _WIFI_H_
#define _WIFI_H_

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
#include "esp_sntp.h"
#include <sys/time.h>

#define WIFI_TAG "CSHA Wifi"
#define TIME_TAG "CSHA NTP"
#define NTP_SERVER "pool.ntp.org"
#define SNTP_MAX_RETRY_COUNT 5
#define WIFI_MAX_RETRY_ATTEMPTS 2

static char wifi_mac_str[17];
static struct tm timeinfo = { 0 };
static bool sntp_setup = false;

/*
* callback on wifi state change
* automatically handles connections
*/
void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

/*
* callback on successfully acquiring ip from network
* retries connection WIFI_MAX_RETRY_ATTEMPTS times
*/
void got_ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

/*
* start wifi process and connect to network with given ssid and password
*/
void start_wifi(char* wifi_ssid, char* wifi_pass);

/*
* converts 6 byte mac address to string representation and stores it in str
* str length must be at least 17
* formatted as 00:00:00:00:00:00
* returns pointer to str
*/
char* byte_mac_to_str(char* str, uint8_t* mac);

/*
* extracts ESP's default wifi MAC and stores it in str
* str length be at least 17
* formatted as 00:00:00:00:00:00
* returns pointer to str
*/
void get_wifi_mac_str(char* str);

/*
* returns whether wifi is currently connected
*/
bool wifi_connected(void);

/*
* callback on successful SNTP time sync
*/
void time_sync_notification_cb(struct timeval* tv);

/*
* start SNTP time sync
* will fail without wifi
*/
void init_sntp(void);

/*
* returns true on success, false on failure
* probably failed because no wifi or sntp not setup yet
*/
bool sntp_update_time(void);

#endif
