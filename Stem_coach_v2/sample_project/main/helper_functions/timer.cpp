#include "timer.hpp"

void IRAM_ATTR Timer::timer_isr(void* arg) {
    Timer* instance = reinterpret_cast<Timer*>(arg);
    if(instance){
        instance->timer_interupt();
    }
}

void Timer::timer_interupt(void){
    flag = true;
    ESP_LOGI("timer","interupt");
}

void Timer::reset(){

    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_timer_stop(timer1));
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_timer_start_periodic(timer1, (period*1000)));

}


//timer period in miliseconds
void Timer::init(int period_)
{
    period = period_;
    esp_timer_create_args_t timer_config = {
        .callback = &Timer::timer_isr,
        .arg = (void*) this,
        .name = "my_timer"
        
    };

    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_timer_create(&timer_config, &timer1));
    
    // Set the timer to trigger every n milliseconds
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_timer_start_periodic(timer1, (period*1000)));
    ESP_LOGI("timer","setup");
}

Timer::Timer()
{
}

Timer::~Timer()
{
}