idf_component_register(
    SRCS "main.cpp" "zigbee.cpp" "ultrasoundsensor.cpp" "monitoring.cpp" "memory/pers_mem.cpp"
    INCLUDE_DIRS "."
    PRIV_REQUIRES nvs_flash esp_driver_uart esp_driver_gpio esp_timer ieee802154
)
