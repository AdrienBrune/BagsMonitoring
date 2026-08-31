#pragma once

#include "ultrasoundsensor.hpp"
#include "memory/pers_mem.hpp"
#include <vector>
#include <algorithm>

class BagStackComputing
{
public:
    BagStackComputing(UltrasoundSensor &sensor, std::string id):
        m_sensor(nullptr),
        m_bagSizeCm(0),
        m_sensorPositionCm(0),
        m_previousBagNumber(0),
        m_validationCounter(0),
        m_bagNumber(0),
        m_readyToReport(false),
        m_id(id)
    {
        m_sensor = &sensor;
    }
    ~BagStackComputing(){}

public:
    inline uint16_t& GetBagSize(){ return m_bagSizeCm; }
    inline void SetBagSize(uint16_t size){ m_bagSizeCm = size; }
    inline uint16_t& GetSensorPosition(){ return m_sensorPositionCm; }
    inline void SetSensorPosition(uint16_t size){ m_sensorPositionCm = size; }
    inline uint16_t& GetBagNumber(){ return m_bagNumber; }
    inline bool& IsReadyToReport(){ return m_readyToReport; }

    inline void ComputeBagNumber()
    {
        DebugLogger::GetInstance().print(DEBUG_SENSOR, DEBUG_INFO, "-- %s computing started --", m_id.c_str());

        uint16_t measurementDeepthCm = m_sensor->GetDistance();
        if (measurementDeepthCm == 0)
        {
            DebugLogger::GetInstance().print(DEBUG_SENSOR, DEBUG_WARNING, "measurement error");
            DebugLogger::GetInstance().print(DEBUG_SENSOR, DEBUG_WARNING, "bags count not updated: %d", m_bagNumber);
            return;
        }

        uint16_t newBagNumber = (uint16_t)((float)(m_sensorPositionCm - measurementDeepthCm) / (float)m_bagSizeCm + 0.5f);

        m_validationCounter = (newBagNumber == m_previousBagNumber) ? m_validationCounter + 1 : 0;
        m_previousBagNumber = newBagNumber;

        DebugLogger::GetInstance().print(DEBUG_SENSOR, DEBUG_INFO, "bags found: %d", newBagNumber);
        DebugLogger::GetInstance().print(DEBUG_SENSOR, DEBUG_INFO, "validation counters: %d", m_validationCounter);

        if (m_validationCounter == 5)
        {
            DebugLogger::GetInstance().print(DEBUG_SENSOR, DEBUG_INFO, "number of bags updated: %d", m_bagNumber);
            m_bagNumber = newBagNumber;
            m_readyToReport = true;
        }
    }

private:
    UltrasoundSensor* m_sensor;

    uint16_t m_bagSizeCm;
    uint16_t m_sensorPositionCm;

    uint16_t m_previousBagNumber;
    uint32_t m_validationCounter;
    uint16_t m_bagNumber;
    bool m_readyToReport;

    std::string m_id;
};

class Monitoring
{
public:
    inline static Monitoring& GetInstance()
    {
        static Monitoring instance;
        return instance;
    }
private:
    Monitoring():
        m_bagSizeCm(0),
        m_sensorPositionCm(0),
        m_bagNumber(0)
    {
        mSemaphore = xSemaphoreCreateMutex();
    }
    ~Monitoring()
    {
        if (mSemaphore)
        {
            vSemaphoreDelete(mSemaphore);
            mSemaphore = nullptr;
        }
    }

    Monitoring(const Monitoring&) = delete;
    Monitoring& operator=(const Monitoring&) = delete;

    SemaphoreHandle_t mSemaphore;

public:
    inline void Init()
    {
        Memory &memory = Memory::GetMemory();

        if (xSemaphoreTake(mSemaphore, portMAX_DELAY))
        {
            m_bagNumber = memory.Get<uint16_t>(DATA_BAG_NUMBER);
            m_bagSizeCm = memory.Get<uint16_t>(DATA_BAG_HEIGHT);
            m_sensorPositionCm = memory.Get<uint16_t>(DATA_SENSOR_POSITION);

            xSemaphoreGive(mSemaphore);
        }
    }
    inline void RegisterStack(BagStackComputing &stack)
    {
        auto it = std::find(m_bagStacks.begin(), m_bagStacks.end(), &stack);
        if (it == m_bagStacks.end())
        {
            m_bagStacks.push_back(&stack);

            if (xSemaphoreTake(mSemaphore, portMAX_DELAY))
            {
                stack.SetBagSize(m_bagSizeCm);
                stack.SetSensorPosition(m_sensorPositionCm);

                xSemaphoreGive(mSemaphore);
            }
        }
    }
    inline void UnregisterStack(BagStackComputing &stack)
    {
        auto it = std::find(m_bagStacks.begin(), m_bagStacks.end(), &stack);
        if (it != m_bagStacks.end())
        {
            m_bagStacks.erase(it);
        }
    }
    inline void UpdateBagSize(uint16_t bagSizeCm)
    {
        if (xSemaphoreTake(mSemaphore, portMAX_DELAY))
        {
            m_bagSizeCm = bagSizeCm;
            for (BagStackComputing* stack : m_bagStacks)
            {
                stack->SetBagSize(bagSizeCm);
                DebugLogger::GetInstance().print(DEBUG_ZIGBEE, DEBUG_INFO, "update bag height for a stack: %d", bagSizeCm);
            }
            
            xSemaphoreGive(mSemaphore);
        }

        Memory &memory = Memory::GetMemory();
        memory.Set<uint16_t>(DATA_BAG_HEIGHT, bagSizeCm);
    }
    inline void UpdateSensorPosition(uint16_t sensorPositionCm)
    {
        if (xSemaphoreTake(mSemaphore, portMAX_DELAY))
        {
            m_sensorPositionCm = sensorPositionCm;
            for (BagStackComputing* stack : m_bagStacks)
            {
                stack->SetSensorPosition(sensorPositionCm);
                DebugLogger::GetInstance().print(DEBUG_ZIGBEE, DEBUG_INFO, "update sensor position for a stack: %d", sensorPositionCm);
            }
            
            xSemaphoreGive(mSemaphore);
        }

        Memory &memory = Memory::GetMemory();
        memory.Set<uint16_t>(DATA_SENSOR_POSITION, sensorPositionCm);
    }
    void Process();
    void RequestReporting();

private:
    std::vector<BagStackComputing*> m_bagStacks;

    uint16_t m_bagSizeCm;
    uint16_t m_sensorPositionCm;
    uint16_t m_bagNumber;
};