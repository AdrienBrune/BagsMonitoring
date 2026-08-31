#include "monitoring.hpp"
#include "zigbee.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_systick_etm.h"

#define LOOP_DELAY_SEC 10 //120

void Monitoring::Process()
{
    if (xSemaphoreTake(mSemaphore, portMAX_DELAY))
    {
        uint16_t bagNumber = 0;
        for (BagStackComputing* stack : m_bagStacks)
        {
            stack->ComputeBagNumber();
            bagNumber += stack->GetBagNumber();
        }
        m_bagNumber = bagNumber;

        // Quit now if not ready to report
        for (BagStackComputing* stack : m_bagStacks)
        {
            if (!stack->IsReadyToReport())
            {
                xSemaphoreGive(mSemaphore);

                vTaskDelay(pdMS_TO_TICKS(LOOP_DELAY_SEC * 1000));
                return;
            }
        }

        Memory &memory = Memory::GetMemory();
        memory.Set<uint16_t>(DATA_BAG_NUMBER, m_bagNumber);

        updateNumberOfBag(ZB_EP, m_bagNumber);
        
        xSemaphoreGive(mSemaphore);
    }

    vTaskDelay(pdMS_TO_TICKS(LOOP_DELAY_SEC * 1000));
}

void Monitoring::RequestReporting()
{
    if (xSemaphoreTake(mSemaphore, portMAX_DELAY))
    {
        // Quit now if not ready to report
        for (BagStackComputing* stack : m_bagStacks)
        {
            if (!stack->IsReadyToReport())
            {
                xSemaphoreGive(mSemaphore);

                vTaskDelay(pdMS_TO_TICKS(LOOP_DELAY_SEC * 1000));
                return;
            }
        }
        updateNumberOfBag(ZB_EP, m_bagNumber);
        
        xSemaphoreGive(mSemaphore);
    }
}