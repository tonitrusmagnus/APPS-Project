#ifndef LED_DRIVER_HPP
#define LED_DRIVER_HPP
#include <stdio.h>
#include <cstring>
#include <driver/gpio.h>
#include "leds.hpp"

#pragma once 

class Led_Driver
{
private:
    Leds* ledarray;
    int ledSize;
    /* data */
public:
    Led_Driver(gpio_num_t *number,int size);
    ~Led_Driver();
    void setLevel(int level);
};

#endif