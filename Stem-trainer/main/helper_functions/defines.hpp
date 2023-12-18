// #ifndef DEFINES_HPP
// #define DEFINES_HPP

#include "driver/gpio.h"

#define DEBUGGING true
// #define DEBUG_LOGGING_ON true
// #if DEBUG_LOGGING_ON == true
//     #define DEBUG_LOG(x) ESP_LOGD(x)
// #else
//     #define DEBUG_LOG(x) 0
// #endif

//mic pins
#define INMP441_WS GPIO_NUM_15
#define INMP441_BCK GPIO_NUM_7
#define INMP441_DATA GPIO_NUM_6

// Button pins
#define HZ_ON_LED_BUTTON_PIN GPIO_NUM_18
#define DB_ON_LED_BUTTON_PIN GPIO_NUM_19
#define CALIBRATE_BUTTON_PIN GPIO_NUM_14
#define MUTE_BUTTON_PIN GPIO_NUM_10
#define DEBOUNCE_DELAY 150 // High debounce time because of bad buttons

// optional functions
#define CALIBRATE_BUTTON_ACTIVE true
#define MUTE_BUTTON_ACTIVE true
#define RELAX_ACTIVE true

// led defines
#define LED_AMOUNT 8
#define LED_SDA_PIN GPIO_NUM_21
#define LED_SCL_PIN GPIO_NUM_11
#define LED_I2C_PORT I2C_NUM_0

// servo defines
#define SERVO_PIN GPIO_NUM_3
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 110

#define HZ_THRES_MIN 50
#define HZ_THRES_MAX 200
#define DB_THRES_MIN 0
#define DB_THRES_MAX 80

#define DB_CALIBRATION_OFFSET 10
#define HZ_CALIBRATION_OFFSET 5 
#define MIN_CALIBRATIONVALUES_AMOUNT 5

#define MP3_TXD_PIN GPIO_NUM_17
#define MP3_RXD_PIN GPIO_NUM_16
#define UARTMP3 UART_NUM_2
#define FEEDBACK_TIMEOUT_TICKS 200

#define BLINKING_LED_PIN GPIO_NUM_2

#define PROTOTYPE_3 false

