#ifndef DEFINES_HPP
#define DEFINES_HPP

#include "driver/gpio.h"

#define DEBUGGING true

#define INMP441_WS GPIO_NUM_25
#define INMP441_BCK GPIO_NUM_26
#define INMP441_DATA GPIO_NUM_33

#define POT1_PIN GPIO_NUM_35

#define TXD_PIN GPIO_NUM_17
#define RXD_PIN GPIO_NUM_16

#define HZ_ON_LED_BUTTON_PIN GPIO_NUM_18
#define DB_ON_LED_BUTTON_PIN GPIO_NUM_19
#define CALIBRATE_BUTTON_PIN GPIO_NUM_39

#define LED_AMOUNT 8

#define SERVO_PIN GPIO_NUM_3
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 110

// LEDS or SERVO
#define VISUAL_FEEDBACK LEDS

#define HZ_THRES_MIN 50
#define HZ_THRES_MAX 200
#define DB_THRES_MIN 0
#define DB_THRES_MAX 80

#define DB_CALIBRATION_OFFSET 5 
#define HZ_CALIBRATION_OFFSET 5 
#define MIN_CALIBRATIONVALLUES_AMOUNT 5

#define UARTMP3 UART_NUM_2

#endif