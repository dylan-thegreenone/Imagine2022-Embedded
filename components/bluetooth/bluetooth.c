#include "bluetooth.h"

static char identifier_mac[17];

char *bda2str(esp_bd_addr_t bda, char *str, size_t size)
{
    if (bda == NULL || str == NULL || size < 18) {
        return NULL;
    }

    uint8_t *p = bda;
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            p[0], p[1], p[2], p[3], p[4], p[5]);
    return str;
}

char *uuid2str(esp_bt_uuid_t *uuid, char *str, size_t size)
{
    if (uuid == NULL || str == NULL) {
        return NULL;
    }
    if (uuid->len == 2 && size >= 5) {
        sprintf(str, "%04x", uuid->uuid.uuid16);
    } else if (uuid->len == 4 && size >= 9) {
        sprintf(str, "%08x", uuid->uuid.uuid32);
    } else if (uuid->len == 16 && size >= 37) {
        uint8_t *p = uuid->uuid.uuid128;
        sprintf(str, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                p[15], p[14], p[13], p[12], p[11], p[10], p[9], p[8],
                p[7], p[6], p[5], p[4], p[3], p[2], p[1], p[0]);
    } else {
        return NULL;
    }

    return str;
}

bool get_name_from_eir(uint8_t *eir, uint8_t *bdname, uint8_t *bdname_len)
{
    uint8_t *rmt_bdname = NULL;
    uint8_t rmt_bdname_len = 0;

    if (!eir) {
        return false;
    }

    rmt_bdname = esp_bt_gap_resolve_eir_data(eir, ESP_BT_EIR_TYPE_CMPL_LOCAL_NAME, &rmt_bdname_len);
    if (!rmt_bdname) {
        rmt_bdname = esp_bt_gap_resolve_eir_data(eir, ESP_BT_EIR_TYPE_SHORT_LOCAL_NAME, &rmt_bdname_len);
    }

    if (rmt_bdname) {
        if (rmt_bdname_len > ESP_BT_GAP_MAX_BDNAME_LEN) {
            rmt_bdname_len = ESP_BT_GAP_MAX_BDNAME_LEN;
        }

        if (bdname) {
            memcpy(bdname, rmt_bdname, rmt_bdname_len);
            bdname[rmt_bdname_len] = '\0';
        }
        if (bdname_len) {
            *bdname_len = rmt_bdname_len;
        }
        return true;
    }

    return false;
}

void update_device_info(esp_bt_gap_cb_param_t *param)
{
    char bda_str[18];
    uint32_t cod = 0;
    int32_t rssi = -129; /* invalid value */
    esp_bt_gap_dev_prop_t *p;

    // ESP_LOGI(CSHA_TAG, "Device found: %s", bda2str(param->disc_res.bda, bda_str, 18)); // You can see these even when the device isn't visible
    for (int i = 0; i < param->disc_res.num_prop; i++) {
        p = param->disc_res.prop + i;
        switch (p->type) {
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
    app_gap_cb_t *p_dev = &m_dev_info;
    memcpy(p_dev->bda, param->disc_res.bda, ESP_BD_ADDR_LEN);

    p_dev->dev_found = true;
    for (int i = 0; i < param->disc_res.num_prop; i++) {
        p = param->disc_res.prop + i;
        switch (p->type) {
        case ESP_BT_GAP_DEV_PROP_COD:
            p_dev->cod = *(uint32_t *)(p->val);
            break;
        case ESP_BT_GAP_DEV_PROP_RSSI:
            p_dev->rssi = *(int8_t *)(p->val);
            break;
        case ESP_BT_GAP_DEV_PROP_BDNAME: {
            uint8_t len = (p->len > ESP_BT_GAP_MAX_BDNAME_LEN) ? ESP_BT_GAP_MAX_BDNAME_LEN :
                          (uint8_t)p->len;
            memcpy(p_dev->bdname, (uint8_t *)(p->val), len);
            p_dev->bdname[len] = '\0';
            p_dev->bdname_len = len;
            break;
        }
        case ESP_BT_GAP_DEV_PROP_EIR: {
            memcpy(p_dev->eir, (uint8_t *)(p->val), p->len);
            p_dev->eir_len = p->len;
            break;
        }
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

    ESP_LOGI(CSHA_TAG, "Found a target device, address %s, name %s, RSSI %d, TIMESTAMP %d", bda2str(param->disc_res.bda, bda_str, 18), p_dev->bdname, rssi, wifi_connected() ? (int) now : -1);

    int data_str_len = calc_len(now, &btpacket);
    char data_str[data_str_len];


    if (wifi_connected())
    {
        format_data(data_str, now, identifier_mac, &btpacket);
        ESP_LOGI(WIFI_TAG, "%d : %s", data_str_len, data_str);
        udp_send_str(data_str, MAX_SAFE_UDP_BLOCK_SIZE);
    }
}

void bt_app_gap_init(void)
{
    app_gap_cb_t *p_dev = &m_dev_info;
    memset(p_dev, 0, sizeof(app_gap_cb_t));

    /* start to discover nearby Bluetooth devices */
    p_dev->state = APP_GAP_STATE_DEVICE_DISCOVERING;
}

void bt_app_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
    app_gap_cb_t *p_dev = &m_dev_info;
    char bda_str[18];
    char uuid_str[37];

    switch (event) {
        case ESP_BT_GAP_DISC_RES_EVT: {
            update_device_info(param);
            break;
        }
        case ESP_BT_GAP_DISC_STATE_CHANGED_EVT: {
            if (param->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STOPPED) {
                ESP_LOGI(CSHA_TAG, "Device discovery stopped.");
            } else if (param->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STARTED) {
                ESP_LOGI(CSHA_TAG, "Discovery started.");
            }
            break;
        }
        case ESP_BT_GAP_RMT_SRVCS_EVT: {
            if (memcmp(param->rmt_srvcs.bda, p_dev->bda, ESP_BD_ADDR_LEN) == 0 &&
                    p_dev->state == APP_GAP_STATE_SERVICE_DISCOVERING) {
                p_dev->state = APP_GAP_STATE_SERVICE_DISCOVER_COMPLETE;
                if (param->rmt_srvcs.stat == ESP_BT_STATUS_SUCCESS) {
                    ESP_LOGI(CSHA_TAG, "Services for device %s found",  bda2str(p_dev->bda, bda_str, 18));
                    for (int i = 0; i < param->rmt_srvcs.num_uuids; i++) {
                        esp_bt_uuid_t *u = param->rmt_srvcs.uuid_list + i;
                        ESP_LOGI(CSHA_TAG, "--%s", uuid2str(u, uuid_str, 37));
                    }
                } else {
                    ESP_LOGI(CSHA_TAG, "Services for device %s not found",  bda2str(p_dev->bda, bda_str, 18));
                }
            }
            break;
        }
        case ESP_BT_GAP_RMT_SRVC_REC_EVT:
        default: {
            ESP_LOGI(CSHA_TAG, "event: %d", event);
            break;
        }
    }
    return;
}

void bt_app_gap_start_up(void)
{
    get_wifi_mac_str(identifier_mac);
	char *dev_name = "ESP_GAP_INQUIRY";
	esp_bt_dev_set_device_name(dev_name);
	
	/* register GAP callback function */
	esp_bt_gap_register_callback(bt_app_gap_cb);
	/* set discoverable and connectable mode, wait to be connected */
	esp_bt_gap_set_scan_mode(ESP_BT_NON_CONNECTABLE, ESP_BT_NON_DISCOVERABLE);
	
	/* inititialize device information and status */
	bt_app_gap_init();
	    
    while (true) {
	    ESP_LOGI(CSHA_TAG, "Begin inquiry");
	    // esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);
	    esp_err_t discovery_result = esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);
        ESP_LOGI(CSHA_TAG, "discovery result = %d", discovery_result);
	    vTaskDelay(15000 / portTICK_PERIOD_MS);
    }
}
