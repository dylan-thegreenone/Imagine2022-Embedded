// ┌┬┐┬ ┬┌─┐┌─┐┬─┐┌─┐┌─┐┌┐┌┌─┐┌┐┌┌─┐
//  │ ├─┤├┤ │ ┬├┬┘├┤ ├┤ ││││ ││││├┤ 
//  ┴ ┴ ┴└─┘└─┘┴└─└─┘└─┘┘└┘└─┘┘└┘└─┘

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_log.h"

#include "bluetooth.h"
#include "udp.h"
#include "mesh.h"
#include "config.h"
#include "network.h"

char wifi_mac_str[18] = {};
bool wifi_ready = false;
bool sntp_ready = false;

void app_main(void)
{
    /* Initialize NVS — it is used to store PHY calibration data */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK) {
        ESP_LOGE(BT_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bt_controller_enable(ESP_BT_MODE_BLE)) != ESP_OK) 
    {
        ESP_LOGE(BLE_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bluedroid_init()) != ESP_OK) 
    {
        ESP_LOGE(BT_TAG, "%s initialize bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bluedroid_enable()) != ESP_OK) 
    {
        ESP_LOGE(BT_TAG, "%s enable bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    start_mesh();
    init_udp_socket(CONFIG_UDP_DEST_ADDRESS, CONFIG_UDP_DEST_PORT);
    sntp_update_time(); // Wait. How the fuck is this functioning?

    if (!socket_ready())
	    ESP_LOGE(WIFI_TAG, "Could not start UDP socket");
    
    get_wifi_mac_str();
    ESP_LOGI(WIFI_TAG,"ESP MAC Address: %s",  wifi_mac_str);

    // time will only be accurate if SNTP sync was successful (requires mesh for now)
    time_t now = 0;
    time(&now);

    ESP_LOGI(TIME_TAG, "now : %d", (int)now);

    ble_app_gap_start_up();
    ESP_LOGI("CSHacked", "Done!");
}

