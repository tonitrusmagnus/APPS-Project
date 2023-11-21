#include "servo_driver.hpp"
#include "helper_functions/helper_functions.hpp"

Servo::Servo()
{

}

Servo::~Servo()
{
}

void Servo::init(int pin){
    servo_pin = pin;

    ledc_timer_config_t timer_conf;
    timer_conf.duty_resolution = LEDC_TIMER_15_BIT; // 12-bit resolution for more precise control
    timer_conf.freq_hz = 50; // 50Hz PWM signal for most servo motors
    timer_conf.speed_mode = LEDC_HS_MODE;
    timer_conf.timer_num = LEDC_HS_TIMER;
    timer_conf.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&timer_conf);

    ledc_channel_config_t ledc_conf;
    ledc_conf.channel = LEDC_CHANNEL_0;
    ledc_conf.duty = 0;
    ledc_conf.gpio_num = (int)pin;
    ledc_conf.intr_type = LEDC_INTR_DISABLE;
    ledc_conf.speed_mode = LEDC_HS_MODE;
    ledc_conf.timer_sel = LEDC_HS_TIMER;
    ledc_conf.hpoint = 0;
    ledc_channel_config(&ledc_conf);   
}

void Servo::setAngle(int angle) {
    angle = constrain(angle, SERVODRIVER_MIN_ANGLE, SERVODRIVER_MAX_ANGLE);
    //int duty = map(angle,SERVODRIVER_MIN_ANGLE,SERVODRIVER_MAX_ANGLE,500,2400);  //
    int duty = map(angle,SERVODRIVER_MIN_ANGLE,SERVODRIVER_MAX_ANGLE,SERVO_MIN_COUNT,SERVO_MAX_COUNT); //((SERVO_MIN_COUNT) + ((SERVO_MAX_COUNT - SERVO_MIN_COUNT) * angle) / (SERVO_MAX_DEGREE)); //(angle * (SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) / SERVO_MAX_DEGREE) + SERVO_MIN_PULSEWIDTH;
    ledc_set_duty(LEDC_HS_MODE, LEDC_CHANNEL_0, duty);
    ledc_update_duty(LEDC_HS_MODE, LEDC_CHANNEL_0);
}
