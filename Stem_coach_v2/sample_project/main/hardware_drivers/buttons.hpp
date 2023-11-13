#ifndef BUTTONS_HPP
#define BUTTONS_HPP

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
enum edge{falling,rising};

class Button
{
private:
    /* data */
    static void IRAM_ATTR gpio_isr_handler(void* arg);

    static bool isrServiceInstalled;

    void onButtonPress();

public:
    volatile bool flag;
    Button();
    ~Button();
    void init(gpio_num_t buttonpin, edge interruptedge);
};

#endif //BUTTONS_HPP