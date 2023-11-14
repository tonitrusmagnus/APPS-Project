#ifndef TIMER_HPP
#define TIMER_HPP

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_timer.h"
#include <esp_system.h>
#include "esp_err.h"
#include "esp_log.h"

class Timer
{
private:
    static void IRAM_ATTR timer_isr(void* arg);
    void timer_interupt(void);
    
    /* data */
public:
    bool flag;
    bool skip;
    void init(int period);
    Timer();
    ~Timer();
};

#endif