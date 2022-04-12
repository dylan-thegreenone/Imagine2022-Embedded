#include "bluetooth.h"

extern char wifi_mac_str[18];
extern bool wifi_ready;
extern bool sntp_ready;

char *bda2str(esp_bd_addr_t bda, char *str, size_t size)
{
    if (bda == NULL || str == NULL || size < 18) 
    {
        return NULL;
    }

    uint8_t *p = bda;
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            p[0], p[1], p[2], p[3], p[4], p[5]);
    return str;
}

char *uuid2str(esp_bt_uuid_t *uuid, char *str, size_t size)
{
    if (uuid == NULL || str == NULL) 
    {
        return NULL;
    }
    if (uuid->len == 2 && size >= 5) 
    {
        sprintf(str, "%04x", uuid->uuid.uuid16);
    }
    else if (uuid->len == 4 && size >= 9) 
    {
        sprintf(str, "%08x", uuid->uuid.uuid32);
    }
    else if (uuid->len == 16 && size >= 37) 
    {
        uint8_t *p = uuid->uuid.uuid128;
        sprintf(str, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                p[15], p[14], p[13], p[12], p[11], p[10], p[9], p[8],
                p[7], p[6], p[5], p[4], p[3], p[2], p[1], p[0]);
    }
    else 
    {
        return NULL;
    }

    return str;
}

bool get_name_from_eir(uint8_t *eir, uint8_t *bdname, uint8_t *bdname_len)
{
    uint8_t *rmt_bdname = NULL;
    uint8_t rmt_bdname_len = 0;

    if (!eir) 
    {
        return false;
    }

    rmt_bdname = esp_bt_gap_resolve_eir_data(eir, ESP_BT_EIR_TYPE_CMPL_LOCAL_NAME, &rmt_bdname_len);
    if (!rmt_bdname) 
    {
        rmt_bdname = esp_bt_gap_resolve_eir_data(eir, ESP_BT_EIR_TYPE_SHORT_LOCAL_NAME, &rmt_bdname_len);
    }

    if (rmt_bdname) 
    {
        if (rmt_bdname_len > ESP_BT_GAP_MAX_BDNAME_LEN) 
        {
            rmt_bdname_len = ESP_BT_GAP_MAX_BDNAME_LEN;
        }

        if (bdname) 
        {
            memcpy(bdname, rmt_bdname, rmt_bdname_len);
            bdname[rmt_bdname_len] = '\0';
        }
        if (bdname_len) 
        {
            *bdname_len = rmt_bdname_len;
        }
        return true;
    }

    return false;
}

#ifdef CONFIG_BT_CLASSIC_ENABLED

void update_device_info(esp_bt_gap_cb_param_t *param)
{
    char bda_str[18];
    uint32_t cod = 0;
    int32_t rssi = -129; /* invalid value */
    esp_bt_gap_dev_prop_t *p;

    // ESP_LOGI(CSHA_TAG, "Device found: %s", bda2str(param->disc_res.bda, bda_str, 18)); // You can see these even when the device isn't visible
    for (int i = 0; i < param->disc_res.num_prop; ++i) 
    {
        p = param->disc_res.prop + i;
        switch (p->type) 
        {
        case ESP_BT_GAP_DEV_PROP_COD:
            cod = *(uint32_t *)(p->val);
            //ESP_LOGI(CSHA_TAG, "--Class of Device: 0x%x", cod);
            break;
        case ESP_BT_GAP_DEV_PROP_RSSI:
            rssi = *(int8_t *)(p->val);
            //ESP_LOGI(CSHA_TAG, "--RSSI: %d", rssi);
            break;
        case ESP_BT_GAP_DEV_PROP_BDNAME:
        default:
            break;
        }
    }

    /* search for device with MAJOR service class as "rendering" in COD */
    bt_app_gap_cb_t *p_dev = &bt_m_dev_info;
    memcpy(p_dev->bda, param->disc_res.bda, ESP_BD_ADDR_LEN);

    p_dev->dev_found = true;
    for (int i = 0; i < param->disc_res.num_prop; ++i) 
    {
        p = param->disc_res.prop + i;
        switch (p->type) {
        case ESP_BT_GAP_DEV_PROP_COD:
            p_dev->cod = *(uint32_t *)(p->val);
            break;
        case ESP_BT_GAP_DEV_PROP_RSSI:
            p_dev->rssi = *(int8_t *)(p->val);
            break;
        case ESP_BT_GAP_DEV_PROP_BDNAME: 
            // uint8_t len = (p->len > ESP_BT_GAP_MAX_BDNAME_LEN) ? ESP_BT_GAP_MAX_BDNAME_LEN :
            //               (uint8_t)p->len;
            // memcpy(p_dev->bdname, (uint8_t *)(p->val), len);
            // p_dev->bdname[len] = '\0';
            // p_dev->bdname_len = len;
            break;
        case ESP_BT_GAP_DEV_PROP_EIR:
            memcpy(p_dev->eir, (uint8_t *)(p->val), p->len);
            p_dev->eir_len = p->len;
            break;
        default:
            break;
        }
    }

    get_name_from_eir(p_dev->eir, p_dev->bdname, &p_dev->bdname_len); 

    csha_bt_packet btpacket;

    
    //copy bda string to bt packet
    sprintf(btpacket.mac, "%s", bda_str);
    sprintf(btpacket.name, "%s",  p_dev->bdname);
    btpacket.rssi = rssi;

    time_t now;

    time(&now);

    ESP_LOGI(BT_TAG, "Found a target device, address %s, name %s, RSSI %d, TIMESTAMP %d", bda2str(param->disc_res.bda, bda_str, 18), p_dev->bdname, rssi, wifi_ready ? (int) now : -1);

    int data_str_len = calc_len(now, &btpacket);
    char data_str[data_str_len];

    if (wifi_ready && sntp_ready)
    {
        format_data(data_str, now, wifi_mac_str, &btpacket);
        ESP_LOGI(WIFI_TAG, "%d : %s", data_str_len, data_str);
        udp_send_str(data_str, MAX_SAFE_UDP_BLOCK_SIZE);
    }
}

void bt_app_gap_init(void)
{
    bt_app_gap_cb_t *p_dev = &bt_m_dev_info;
    memset(p_dev, 0, sizeof(bt_app_gap_cb_t));

    /* start to discover nearby Bluetooth devices */
    p_dev->state = APP_GAP_STATE_DEVICE_DISCOVERING;
}

void bt_app_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
    bt_app_gap_cb_t *p_dev = &bt_m_dev_info;
    char bda_str[18];
    char uuid_str[37];

    switch (event)
    {
        case ESP_BT_GAP_DISC_RES_EVT:
            update_device_info(param);
            break;
        case ESP_BT_GAP_DISC_STATE_CHANGED_EVT: 
            if (param->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STOPPED) 
            {
                ESP_LOGI(BT_TAG, "Device discovery stopped.");
            }
            else if (param->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STARTED) 
            {
                ESP_LOGI(BT_TAG, "Discovery started.");
            }
            break;
        case ESP_BT_GAP_RMT_SRVCS_EVT: 
            if (memcmp(param->rmt_srvcs.bda, p_dev->bda, ESP_BD_ADDR_LEN) == 0 &&
                    p_dev->state == APP_GAP_STATE_SERVICE_DISCOVERING) 
            {
                p_dev->state = APP_GAP_STATE_SERVICE_DISCOVER_COMPLETE;
                if (param->rmt_srvcs.stat == ESP_BT_STATUS_SUCCESS) 
                {
                    ESP_LOGI(BT_TAG, "Services for device %s found",  bda2str(p_dev->bda, bda_str, 18));
                    for (int i = 0; i < param->rmt_srvcs.num_uuids; i++) 
                    {
                        esp_bt_uuid_t *u = param->rmt_srvcs.uuid_list + i;
                        ESP_LOGI(BT_TAG, "--%s", uuid2str(u, uuid_str, 37));
                    }
                } 
                else 
                {
                    ESP_LOGI(BT_TAG, "Services for device %s not found",  bda2str(p_dev->bda, bda_str, 18));
                }
            }
            break;
        case ESP_BT_GAP_RMT_SRVC_REC_EVT:
        default:
            ESP_LOGI(BT_TAG, "event: %d", event);
            break;
    }
    return;
}
void bt_app_gap_start_up(void)
{
    //get_wifi_mac_str();
	char *dev_name = "ESP_GAP_INQUIRY";
	esp_bt_dev_set_device_name(dev_name);
	
	/* register GAP callback function */
	esp_bt_gap_register_callback(bt_app_gap_cb);
	/* set discoverable and connectable mode, wait to be connected */
	esp_bt_gap_set_scan_mode(ESP_BT_NON_CONNECTABLE, ESP_BT_NON_DISCOVERABLE);
	
	/* inititialize device information and status */
	bt_app_gap_init();
    
	    
    for (;;)
    {
	    ESP_LOGI(BT_TAG, "Begin inquiry");
	    esp_err_t discovery_result = esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);
        ESP_LOGI(BT_TAG, "discovery result = %d", discovery_result);
	    vTaskDelay(15000 / portTICK_PERIOD_MS);
    }
}
#endif

#ifdef CONFIG_BT_BLE_ENABLED
static esp_ble_scan_params_t ble_scan_params = {
    .scan_type              = BLE_SCAN_TYPE_ACTIVE,
    .own_addr_type          = BLE_ADDR_TYPE_PUBLIC,
    .scan_filter_policy     = BLE_SCAN_FILTER_ALLOW_ALL, //BLE_SCAN_FILTER_ALLOW_ONLY_WLST,
    .scan_interval          = 0x30, // 0x320 * 0.625ms = 500ms  
    .scan_window            = 0x20,
    .scan_duplicate         = BLE_SCAN_DUPLICATE_ENABLE
};

void ble_app_gap_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    esp_err_t err;
    uint8_t* adv_name = NULL;
    uint8_t adv_name_len = 0;
    time_t now;
    time(&now);
    switch (event)
    {
        case ESP_GAP_BLE_SCAN_TIMEOUT_EVT:
        case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT:
            esp_ble_gap_start_scanning(0);
            break;
        case ESP_GAP_BLE_SCAN_START_COMPLETE_EVT: 
            if((err = param->scan_start_cmpl.status) != ESP_BT_STATUS_SUCCESS) 
            {
                ESP_LOGE(BLE_TAG,"Scan start failed: %s", esp_err_to_name(err));
            }
            else 
            {
                ESP_LOGI(BLE_TAG,"Start scanning...");
            }
            break;
        case ESP_GAP_BLE_SCAN_RESULT_EVT:
            switch(param->scan_rst.ble_evt_type)
            {
            case ESP_BLE_EVT_NON_CONN_ADV:
            case ESP_BLE_EVT_SCAN_RSP:
            case ESP_BLE_EVT_CONN_ADV:
            case ESP_BLE_EVT_CONN_DIR_ADV:
            case ESP_BLE_EVT_DISC_ADV: {
                adv_name = esp_ble_resolve_adv_data(param->scan_rst.ble_adv, ESP_BLE_AD_TYPE_NAME_CMPL, &adv_name_len);
                if (adv_name == NULL) 
                {
                    //ESP_LOGE(BLE_TAG, "Could not resolve name");
                    adv_name = (uint8_t*) "";
                    adv_name_len = 0;
                    return;
                }
                char bda_str[18];
                bda2str(param->scan_rst.bda, bda_str, 18);
                csha_bt_packet btpacket;
                if (!sntp_ready || !wifi_ready) return;
                
                sprintf(btpacket.mac, "%s", bda_str);
                // sprintf(btpacket.name, "%s",  adv_name);
                btpacket.rssi = param->scan_rst.rssi;

                int data_str_len = calc_len(now, &btpacket);
                char data_str[data_str_len];
            
                format_data(data_str, now, wifi_mac_str, &btpacket);
                ESP_LOGI(BLE_TAG, "%d : %s", data_str_len, data_str);
                udp_send_str(data_str, MAX_SAFE_UDP_BLOCK_SIZE);
                // ESP_LOGI(BLE_TAG, "Name: %s BDA : %s RSSI: %d Event: %d",
                //     adv_name, 
                //     bda_str,
                //     param->scan_rst.rssi,
                //     param->scan_rst.ble_evt_type
                //     );
                break;
            }
            default:
                break;
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        default:
            ESP_LOGI(BLE_TAG, "Event: %d", event);
            break;
    }
}
void ble_app_gap_start_up(void)
{
    esp_err_t err;
    get_wifi_mac_str(wifi_mac_str);
	char *dev_name = "ESP_GAP_INQUIRY";
	esp_bt_dev_set_device_name(dev_name);
	
    esp_ble_gap_set_scan_params(&ble_scan_params);
    
    if ((err = esp_ble_gap_register_callback(ble_app_gap_cb)) != ESP_OK)
    {
        ESP_LOGE(BLE_TAG, "%s Gap callback register error %s\n", __func__, esp_err_to_name(err));
        return;
    }
    for (;;) 
    {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
#endif
