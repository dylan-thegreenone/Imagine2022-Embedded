idf_component_register(SRCS "bluetooth.c"
                    REQUIRES bt freertos data udp network
                    INCLUDE_DIRS "include")
