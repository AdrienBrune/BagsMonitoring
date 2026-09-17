#pragma once

#include "debug.hpp"

class I_Sensor
{
public:
    virtual ~I_Sensor(){};

    virtual inline void Init() = 0;
    virtual uint16_t GetDistance() = 0;
};