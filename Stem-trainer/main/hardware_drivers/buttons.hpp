#ifndef BUTTONS_HPP
#define BUTTONS_HPP

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

class Button
{
private:

    gpio_num_t buttonpin;

    gpio_int_type_t interruptedge;

    static void IRAM_ATTR gpio_isr_handler(void* arg);
    void onButtonPress();
    bool flag;

public:
    Button(gpio_num_t buttonpin, gpio_int_type_t interruptedge);

    void init();
    bool getFlag();
    void resetFlag();
    
};

#endif //BUTTONS_HPP