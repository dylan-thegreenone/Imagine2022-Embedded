idf_component_register(SRCS "mesh.c"
                            "mesh_netif.c"
                    REQUIRES udp
                    INCLUDE_DIRS "." "include")
