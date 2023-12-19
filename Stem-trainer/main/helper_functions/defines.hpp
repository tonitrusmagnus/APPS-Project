#include "driver/gpio.h"

#define DEBUGGING true

// Queue length for measurement queues
#define MEASURE_QUEUE_LENGTH 4

// Starting values for thresholds on reset
#define FREQUENCY_STARTVALUE 90
#define VOLUME_STARTVALUE 50

// Microphone pins
#define INMP441_WS GPIO_NUM_15
#define INMP441_BCK GPIO_NUM_7
#define INMP441_DATA GPIO_NUM_6

// Button pins
#define HZ_ON_LED_BUTTON_PIN GPIO_NUM_18
#define DB_ON_LED_BUTTON_PIN GPIO_NUM_19
#define CALIBRATE_BUTTON_PIN GPIO_NUM_10
#define MUTE_BUTTON_PIN GPIO_NUM_14
#define DEBOUNCE_DELAY 200 // High debounce time because of bad buttons

// Optional functions
// #define CALIBRATE_BUTTON_ACTIVE true
// #define MUTE_BUTTON_ACTIVE true
#define RELAX_ACTIVE true

// Led defines
#define LED_AMOUNT 7
#define LED_SDA_PIN GPIO_NUM_21
#define LED_SCL_PIN GPIO_NUM_11
#define LED_I2C_PORT I2C_NUM_0

// Servo defines
#define SERVO_PIN GPIO_NUM_3
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 110

// Threshold values
#define HZ_THRES_MIN 50
#define HZ_THRES_MAX 200
#define DB_THRES_MIN 0
#define DB_THRES_MAX 80

// Calibration values
#define DB_CALIBRATION_OFFSET 10
#define HZ_CALIBRATION_OFFSET 5 
#define CALIBRATE_QUEUE_LENGTH 100
#define MIN_CALIBRATIONVALUES_AMOUNT 5 // Length of calibration queue

// MP3 player defines
#define MP3_TXD_PIN GPIO_NUM_17
#define MP3_RXD_PIN GPIO_NUM_16
#define UARTMP3 UART_NUM_2
#define FEEDBACK_TIMEOUT_TICKS 200

// Blinking led
#define BLINKING_LED_PIN GPIO_NUM_2

