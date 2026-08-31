#pragma once

#include "ha/esp_zigbee_ha_standard.h"
#include "zcl_utility.h"
#include "zcl/esp_zigbee_zcl_command.h"
#include "zcl/esp_zigbee_zcl_ias_zone.h"
#include "zcl/esp_zigbee_zcl_power_config.h"
#include "esp_zigbee_core.h"
#include "atomic"

#define ZB_EP                   1

#define CUSTOM_CLUSTER_ID        0xFF00 

#define ATTR_BAG_HEIGHT_ID       0x0000
#define ATTR_SENSOR_POS_ID       0x0001
#define ATTR_BAG_COUNT_ID        0x0002

extern std::atomic<bool> connected;

esp_err_t initZigbee();
esp_err_t initDevice();
void updateNumberOfBag(uint8_t endpoint, uint16_t numberOfBag);
