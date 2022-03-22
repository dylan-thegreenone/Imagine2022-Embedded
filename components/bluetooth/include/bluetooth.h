#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_bt.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_gap_ble_api.h"
#include "data.h"
#include "udp.h"
#include "network.h"

#define BT_TAG "CSHA Bluetooth"
#define BLE_TAG "CSHA BLE"


/*
* possible BLE app states or states 
*/
typedef enum {
    APP_GAP_STATE_IDLE = 0,
    APP_GAP_STATE_DEVICE_DISCOVERING,
    APP_GAP_STATE_DEVICE_DISCOVER_COMPLETE,
    APP_GAP_STATE_SERVICE_DISCOVERING,
    APP_GAP_STATE_SERVICE_DISCOVER_COMPLETE,
} app_gap_state_t;

/*
* all event information passed to BT app callback 
*/
typedef struct {
    bool dev_found;
    uint8_t bdname_len;
    uint8_t eir_len;
    uint8_t rssi;
    uint32_t cod;
    uint8_t eir[ESP_BT_GAP_EIR_DATA_LEN];
    uint8_t bdname[ESP_BT_GAP_MAX_BDNAME_LEN + 1];
    esp_bd_addr_t bda;
    app_gap_state_t state;
} bt_app_gap_cb_t;

bt_app_gap_cb_t bt_m_dev_info;

/*
* all event information passed to BLE app callback
*/
typedef struct {
    esp_gap_search_evt_t search_evt;
    esp_bd_addr_t bda;
    esp_bt_dev_type_t dev_type;
    esp_ble_addr_type_t ble_addr_type;
    esp_ble_evt_type_t ble_evt_type;
    int rssi;
    uint8_t ble_adv[ESP_BLE_ADV_DATA_LEN_MAX + ESP_BLE_SCAN_RSP_DATA_LEN_MAX];
    int flag;
    int num_resps;
    uint8_t adv_data_len;
    uint8_t scan_rsp_len;
    uint32_t num_dis;
} ble_app_scan_result_t;
ble_app_scan_result_t* ble_scan_dev_info;

/*
* format Bluetooth Device Address into string
* returns pointer to string
*/
char *bda2str(esp_bd_addr_t bda, char *str, size_t size);

/*
* format UUID to string
*/
char *uuid2str(esp_bt_uuid_t *uuid, char *str, size_t size);

/* 
* get string name from Extended Inquiry Response data
*/
bool get_name_from_eir(uint8_t *eir, uint8_t *bdname, uint8_t *bdname_len);

/*
* update detection information to most recent detection data
*/
void update_device_info(esp_bt_gap_cb_param_t *param);

/*
* setup bluetooth application
*/
void bt_app_gap_init(void);

/*
* callback on bluetooth app state change
*/
void bt_app_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param);

/*
* start bluetooth application
*/
void bt_app_gap_start_up(void);

/*
* callback on BLE app state change
*/
void ble_app_gap_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);

/*
* start BLE application
*/
void ble_app_gap_start_up(void);
#endif
