/* Mesh Internal Communication Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#ifndef _MESH_H_
#define _MESH_H_

#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_mesh.h"
//#include "nvs_flash.h"
#include "driver/gpio.h"
#include "freertos/semphr.h"

#include "mesh_netif.h"
#include "udp.h"

/*******************************************************
 *                Macros
 *******************************************************/
#define EXAMPLE_BUTTON_GPIO     0

// commands for internal mesh communication:
// <CMD> <PAYLOAD>, where CMD is one character, payload is variable dep. on command
#define CMD_KEYPRESSED 0x55
// CMD_KEYPRESSED: payload is always 6 bytes identifying address of node sending keypress event
#define CMD_ROUTE_TABLE 0x56
// CMD_KEYPRESSED: payload is a multiple of 6 listing addresses in a routing table

/*******************************************************
 *                Function Definitions
 *******************************************************/

void static recv_cb(mesh_addr_t *from, mesh_data_t *data);

void mesh_event_handler(void *arg, esp_event_base_t event_base,
                        int32_t event_id, void *event_data);

void ip_event_handler(void *arg, esp_event_base_t event_base,
                      int32_t event_id, void *event_data);

void start_mesh(void);
#endif
