#ifndef LED_DRIVER_HPP
#define LED_DRIVER_HPP
#include <stdio.h>
#include <cstring>
#include <driver/gpio.h>
#include "leds.hpp"
#include "max7312.hpp"

#pragma once 

class Led_Driver
{
private:
    Max7312 ioExpander;
    int ledAmount;

public:
    Led_Driver();
    void setLevel(int levelL,int levelR);
    void init();
};

#endif