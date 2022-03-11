idf_component_register(SRCS "wifi.c"
                    REQUIRES esp_wifi freertos esp_event lwip
                    INCLUDE_DIRS "include")
