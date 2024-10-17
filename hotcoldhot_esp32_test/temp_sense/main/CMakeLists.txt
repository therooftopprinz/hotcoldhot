set(COMPONENT_ADD_INCLUDEDIRS ".")
set(COMPONENT_SRCS
    "app_main.c"
    "network_manager.c")
set(COMPONENT_REQUIRES
    "esp32-ds18b20"
    "esp32-owb")

register_component()
