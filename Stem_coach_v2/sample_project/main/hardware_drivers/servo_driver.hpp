#ifndef SERVO_DRIVER_HPP
#define SERVO_DRIVER_HPP

#include "driver/ledc.h"

// #define LEDC_HS_TIMER          LEDC_TIMER_0
// #define LEDC_HS_MODE           LEDC_LOW_SPEED_MODE
// #define LEDC_HS_CH0_CHANNEL    LEDC_CHANNEL_0
#define SERVO_FREQUENCY     50                  // Servo's generally work on 50 Hz
#define SERVO_DUTY_RES      LEDC_TIMER_12_BIT   // 12-bit resolution for more precise control
#define SERVO_SPEED_MODE    LEDC_LOW_SPEED_MODE // Speed mode available for every timer and esp type

#define SERVO_MIN_PULSEWIDTH   .5  // Minimum pulse width in ms
#define SERVO_MAX_PULSEWIDTH   2.5 // Maximum pulse width in ms
#define SERVO_COUNT_PER_MS     205 // (2^12bits-1)/20ms = 204.75
#define SERVO_MIN_COUNT        SERVO_COUNT_PER_MS*SERVO_MIN_PULSEWIDTH // SERVO_MIN_PULSEWIDTH*SERVO_COUNT_PER_MS
#define SERVO_MAX_COUNT        SERVO_COUNT_PER_MS*SERVO_MAX_PULSEWIDTH // SERVO_MAX_PULSEWIDTH*SERVO_COUNT_PER_MS
#define SERVO_MAX_DEGREE       180  // Maximum angle in degrees

#define SERVODRIVER_MIN_ANGLE 0
#define SERVODRIVER_MAX_ANGLE 180

class Servo
{
private:
    int servo_pin;
    ledc_timer_t timer;
    ledc_channel_t channel;


public:
    Servo(int pin_, ledc_timer_t timer_, ledc_channel_t channel_);
    void init();
    void setAngle(int angle);
};

#endif //SERVO_DRIVER_HPP