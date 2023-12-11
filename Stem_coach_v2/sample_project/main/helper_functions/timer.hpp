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

    esp_timer_handle_t timer1;
    int period;
    
    void timer_interupt(void);
    
public:
    Timer(int period_);

    bool flag;
    void reset();
    void init();
    void start();
    void stop();
};

#endif