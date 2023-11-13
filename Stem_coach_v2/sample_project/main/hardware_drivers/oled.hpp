#ifndef OLED_HPP
#define OLED_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>

#include "ssd1306.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define tag "SSD1306"

using namespace std;

#pragma once 

class oled
{
private:
    /* data */
    SSD1306_t dev;
    int init;
public:
    oled(/* args */);
    ~oled();
    void settext(char* text,int row);   
    void oled_init();
};



#endif