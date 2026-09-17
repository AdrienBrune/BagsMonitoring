#pragma once

#include "debug.hpp"
#include "I_sensor.hpp"
#include "esp_zigbee_core.h"
#include "driver/gpio.h"

class UltrasoundSensor : public I_Sensor
{
public:
    UltrasoundSensor(gpio_num_t pinTrig, gpio_num_t pinEcho):
        m_pinTrigger(pinTrig),
        m_pinEcho(pinEcho)
    {}
    ~UltrasoundSensor(){}

public:
    inline void Init() override
    {
        gpio_set_direction((gpio_num_t)m_pinTrigger, GPIO_MODE_OUTPUT);
        gpio_set_direction((gpio_num_t)m_pinEcho, GPIO_MODE_INPUT);
    }
    uint16_t GetDistance() override;

private:
    gpio_num_t m_pinTrigger;
    gpio_num_t m_pinEcho;
};