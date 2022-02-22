idf_component_register(SRCS "bluetooth.c"
                    REQUIRES bt freertos data udp wifi
                    INCLUDE_DIRS "include")
