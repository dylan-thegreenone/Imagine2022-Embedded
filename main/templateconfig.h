/*
* copy to config.h and add secrets
*/
#ifndef _CONFIG_H_
#define _CONFIG_H_

#define DESTINATION_ADDRESS "server_ip_here"
#define DESTINATION_PORT port_number_here

// to disable wifi connection, comment out defining a password
// without wifi, node will need to connect to BLE mesh.
#define WIFI_SSID "ssid_here"
#define WIFI_PASS "password_here"

#endif
