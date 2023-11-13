#ifndef ADC_HPP
#define ADC_HPP
#include <stdio.h>
#include <queue>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "helper_functions/queue.hpp"


class adc
{
private:
    /* data */
    gpio_num_t adc_pin_;
    adc1_channel_t adc_channel_;
    int_queue adc_queue;



    static const int DEFAULT_VREF = 1100; // Default ADC reference voltage (in mV)
public:
    adc(gpio_num_t adc_pin);
    ~adc();
    float read();
};

adc1_channel_t gpiotoadc1(gpio_num_t number);

#endif