#include "servo_driver.hpp"
#include "esp_log.h"
#include "helper_functions/helper_functions.hpp"

Servo::Servo(int pin_, ledc_timer_t timer_, ledc_channel_t channel_)
{
    servo_pin = pin_;
    timer = timer_;
    channel = channel_;
}

void Servo::init(){
    ledc_timer_config_t timer_conf;
    timer_conf.duty_resolution = SERVO_DUTY_RES;
    timer_conf.freq_hz = SERVO_FREQUENCY;
    timer_conf.speed_mode = SERVO_SPEED_MODE;
    timer_conf.timer_num = timer;
    timer_conf.clk_cfg = LEDC_AUTO_CLK;
    ESP_ERROR_CHECK(ledc_timer_config(&timer_conf));

    ledc_channel_config_t ledc_conf;
    ledc_conf.channel = channel;
    ledc_conf.duty = 0;
    ledc_conf.gpio_num = (int)servo_pin;
    ledc_conf.intr_type = LEDC_INTR_DISABLE;
    ledc_conf.speed_mode = SERVO_SPEED_MODE;
    ledc_conf.timer_sel = timer;
    ledc_conf.hpoint = 0;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_conf));   
}

void Servo::setAngle(int angle) {
    // Angle should be between min and max angle values (0-180 degrees)
    angle = constrain(angle, SERVODRIVER_MIN_ANGLE, SERVODRIVER_MAX_ANGLE);

    // Only change when angle is different
    if (angle != prevAngle) {
        ESP_LOGI("Servo angle","%d",angle);

        // Calculate duty cycle
        int duty = map(angle, SERVODRIVER_MIN_ANGLE,SERVODRIVER_MAX_ANGLE, SERVO_MIN_COUNT,SERVO_MAX_COUNT); 
        
        // Set and update duty cycle
        ESP_ERROR_CHECK(ledc_set_duty(SERVO_SPEED_MODE, channel, duty));
        ESP_ERROR_CHECK(ledc_update_duty(SERVO_SPEED_MODE, channel));
    }
    prevAngle = angle; 
}
