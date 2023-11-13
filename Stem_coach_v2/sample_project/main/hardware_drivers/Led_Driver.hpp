#ifndef LED_DRIVER_HPP
#define LED_DRIVER_HPP
#include <stdio.h>
#include <cstring>
#include <driver/gpio.h>

#pragma once 

class Led_Driver
{
private:
    gpio_num_t Led_Pins[8];
    int Led_Size;
    /* data */
public:
    Led_Driver(gpio_num_t *number,int size);
    ~Led_Driver();
    void setLevel(int level);
};

#endif