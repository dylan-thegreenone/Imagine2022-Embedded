#include "wifi.h"
#include <stdint.h>

#define MAX_RETRY_ATTEMPTS     2

static wifi_config_t wps_ap_creds[MAX_WPS_AP_CRED];
static int s_ap_creds_num = 0;
static int s_retry_num = 0;
static bool connected = false;


void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    int ap_idx = 1;

    switch (event_id) {
        case WIFI_EVENT_STA_START:
            ESP_LOGI(WIFI_TAG, "WIFI_EVENT_STA_START");
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            ESP_LOGI(WIFI_TAG, "WIFI_EVENT_STA_DISCONNECTED");
            connected = false;
            if (s_retry_num < MAX_RETRY_ATTEMPTS) {
                esp_wifi_connect();
                s_retry_num++;
            } else if (ap_idx < s_ap_creds_num) {
                /* Try the next AP credential if first one fails */ if (ap_idx < s_ap_creds_num) {
                    ESP_LOGI(WIFI_TAG, "Connecting to SSID: %s, Passphrase: %s",
                             wps_ap_creds[ap_idx].sta.ssid, wps_ap_creds[ap_idx].sta.password);
                    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wps_ap_creds[ap_idx++]) );
                    esp_wifi_connect();
                }
                s_retry_num = 0;
            } else {
                ESP_LOGI(WIFI_TAG, "Failed to connect!");
            }
            break;

        default:
            break;
    }
}

void got_ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
    ESP_LOGI(WIFI_TAG, "got ip: " IPSTR, IP2STR(&event->ip_info.ip));
    connected = true;
}

/*init wifi as sta and start wps*/
void start_wifi(char* wifi_ssid, char* wifi_pass)
{

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &got_ip_event_handler, NULL));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    memcpy(wps_ap_creds[0].sta.ssid, wifi_ssid,
       strlen(wifi_ssid));
    memcpy(wps_ap_creds[0].sta.password, wifi_pass,
       strlen(wifi_pass));

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wps_ap_creds[0]) );
    esp_wifi_connect(); // Use the creds to connect to WiFi
}

char* byte_mac_to_str(char* str, uint8_t* mac)
{
    //17 char long
    sprintf(str, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
    return str;
}

char* get_wifi_mac_str(char* str)
{
    uint8_t mac[6];
    esp_efuse_mac_get_default(mac);
    return byte_mac_to_str(str, mac);
}

bool wifi_connected(void)
{
    return connected;
}
