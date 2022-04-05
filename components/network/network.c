#include "network.h"
#include <stdint.h>

extern char wifi_mac_str[18];
extern bool wifi_ready;
extern bool sntp_ready;

uint8_t blank_mac[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

char* get_wifi_mac_str()
{
    uint8_t mac[6];
    esp_base_mac_addr_get(mac);
    snprintf(wifi_mac_str, 18, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
    return wifi_mac_str;
}

void time_sync_notification_cb(struct timeval* tv)
{
    sntp_ready = true;
    ESP_LOGI(TIME_TAG, "Time synced");
    time_t now = 0;
    time(&now);

    ESP_LOGI(TIME_TAG, "now : %d", (int)now);
}

void init_sntp(void)
{
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, NTP_SERVER);
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    sntp_init();
}

bool sntp_update_time(void)
{
    init_sntp();
    int retry = 0;
    for (; retry < SNTP_MAX_RETRY_COUNT && sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET; ++retry)
    {
        vTaskDelay(2000/portTICK_PERIOD_MS);
    }
    return sntp_get_sync_status() == SNTP_SYNC_STATUS_COMPLETED;
}
