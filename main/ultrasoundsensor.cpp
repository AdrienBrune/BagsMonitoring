#include "ultrasoundsensor.hpp"

#include "esp_rom_sys.h"
#include "esp_timer.h"

uint16_t UltrasoundSensor::GetDistance()
{
    // send trigger
    gpio_set_level(m_pinTrigger, 0);
    esp_rom_delay_us(20);

    gpio_set_level(m_pinTrigger, 1);
    esp_rom_delay_us(50);
    gpio_set_level(m_pinTrigger, 0);

    // echo reception
    int64_t timeoutUs = 30000; // 5 meter maximum
    int64_t startTimeUs = esp_timer_get_time();
    while (gpio_get_level(m_pinEcho) == 0)
    {
        if ((esp_timer_get_time() - startTimeUs) > timeoutUs)
        {
            DebugLogger::GetInstance().print(DEBUG_GENERIC, DEBUG_WARNING, "timeout reached : first loop");
            return 0; // Timeout
        }
    }

    int64_t pulseStartUs = esp_timer_get_time();
    while (gpio_get_level(m_pinEcho) == 1)
    {
        if ((esp_timer_get_time() - pulseStartUs) > timeoutUs)
        {
            DebugLogger::GetInstance().print(DEBUG_GENERIC, DEBUG_WARNING, "timeout reached : second loop");
            return 0; // timeout
        }
    }

    // distance computing
    int64_t pulseEndUs = esp_timer_get_time();
    int64_t durationUs = pulseEndUs - pulseStartUs;
    uint16_t distanceCm = (uint16_t)(durationUs / 58); // ((Time * Speed) / 2) ~ 1/58

    DebugLogger::GetInstance().print(DEBUG_GENERIC, DEBUG_INFO, "distance measured: %d cm", distanceCm);

    return distanceCm;
}
