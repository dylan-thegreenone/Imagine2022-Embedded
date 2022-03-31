#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <string.h>
#include <stdint.h>
#include <sys/time.h>

#include "esp_log.h"
#include "esp_event.h"
#include "esp_system.h"
#include "esp_sntp.h"

#define WIFI_TAG "CSHA Wifi"
#define TIME_TAG "CSHA NTP"
#define NTP_SERVER "pool.ntp.org"
#define SNTP_MAX_RETRY_COUNT 5
#define WIFI_MAX_RETRY_ATTEMPTS 2

//static struct tm timeinfo = { 0 };

/*
* extracts ESP's default wifi MAC and stores it in str
* str length be at least 17
* formatted as 00:00:00:00:00:00
* returns pointer to str
*/
char* get_wifi_mac_str();

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
