#ifndef SERVO_DRIVER_HPP
#define SERVO_DRIVER_HPP

#include "driver/ledc.h"

#define LEDC_HS_TIMER          LEDC_TIMER_0
#define LEDC_HS_MODE           LEDC_HIGH_SPEED_MODE
#define LEDC_HS_CH0_CHANNEL    LEDC_CHANNEL_0
//#define LEDC_HS_CH0_GPIO       27   // GPIO where your servo signal pin is connected

#define SERVO_MIN_PULSEWIDTH   .5  // Minimum pulse width in ms
#define SERVO_MAX_PULSEWIDTH   2.5 // Maximum pulse width in ms
#define SERVO_COUNT_PER_MS     205 // 4095/20ms = 204.75 counts per ms
#define SERVO_MIN_COUNT        SERVO_COUNT_PER_MS*SERVO_MIN_PULSEWIDTH // SERVO_MIN_PULSEWIDTH*SERVO_COUNT_PER_MS
#define SERVO_MAX_COUNT        SERVO_COUNT_PER_MS*SERVO_MAX_PULSEWIDTH // SERVO_MAX_PULSEWIDTH*SERVO_COUNT_PER_MS
#define SERVO_MAX_DEGREE       180  // Maximum angle in degrees

#define SERVODRIVER_MIN_ANGLE 0
#define SERVODRIVER_MAX_ANGLE 180

class Servo
{
private:
    int servo_pin;

public:
    Servo(int pin);
    ~Servo();

    void setAngle(int angle);
};

#endif //SERVO_DRIVER_HPP