#pragma once

#include "driver/gpio.h"

class Led
{
public:
    inline static Led& GetInstance()
    {
        static Led instance;
        return instance;
    }
private:
    Led(){}
    Led(const Led&) = delete;
    Led& operator=(const Led&) = delete;
    ~Led(){}
public:
    inline void Init(gpio_num_t pin)
    {
        m_pin = pin;
        gpio_set_direction(m_pin, GPIO_MODE_OUTPUT);
        Set(0);
    }
    inline bool Get()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return gpio_get_level(m_pin) ? true : false;
    }
    inline void Set(bool state)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        gpio_set_level(m_pin, state ? 1 : 0);
    }
    inline void Pulse()
    {
        Set(0);
        Set(1);
        vTaskDelay(pdMS_TO_TICKS(50));
        Set(0);
    }
private:
    gpio_num_t m_pin;
    std::mutex m_mutex;
};