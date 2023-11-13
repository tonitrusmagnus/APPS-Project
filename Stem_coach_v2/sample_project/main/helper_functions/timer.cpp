#include "timer.hpp"

bool sec = false;
bool skipbool = false;

void timer_init(){
    const esp_timer_create_args_t timer_config = {
        .callback = &timer_isr,
        .name = "my_timer"
    };

    esp_timer_handle_t timer1;
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_timer_create(&timer_config, &timer1));
    
    // Set the timer to trigger every 10 seconds (10,000,000 microseconds)
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_timer_start_periodic(timer1, 10000000));
    ESP_LOGI("timer","setup");
}

void timer_isr(void* arg) {
    if(!skipbool){
        sec = true;
    }
    skipbool = false;
    ESP_LOGI("timer","interupt");
}
