#include "led_driver.hpp"
#include "esp_log.h"
#include "helper_functions/defines.hpp"
#include "helper_functions/helper_functions.hpp"
#include <cmath>

// Macro to print a 16-bit integer in binary
#define INT16_TO_BINARY(byte) \
    "%c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c", \
    ((byte) & 0x8000 ? '1' : '0'), \
    ((byte) & 0x4000 ? '1' : '0'), \
    ((byte) & 0x2000 ? '1' : '0'), \
    ((byte) & 0x1000 ? '1' : '0'), \
    ((byte) & 0x800 ? '1' : '0'), \
    ((byte) & 0x400 ? '1' : '0'), \
    ((byte) & 0x200 ? '1' : '0'), \
    ((byte) & 0x100 ? '1' : '0'), \
    ((byte) & 0x80 ? '1' : '0'), \
    ((byte) & 0x40 ? '1' : '0'), \
    ((byte) & 0x20 ? '1' : '0'), \
    ((byte) & 0x10 ? '1' : '0'), \
    ((byte) & 0x8 ? '1' : '0'), \
    ((byte) & 0x4 ? '1' : '0'), \
    ((byte) & 0x2 ? '1' : '0'), \
    ((byte) & 0x1 ? '1' : '0') 

LedDriver::LedDriver() : ioExpander(LED_SDA_PIN, LED_SCL_PIN, LED_I2C_PORT) 
{
}

void LedDriver::init(){
    ioExpander.init();
    ioExpander.set(0); // All leds off after initialisation
}

void LedDriver::setLevel(uint8_t level)
{
    uint16_t data = 0; 
    int data_size = sizeof(data) * 8; // A byte is 8 bits
    
    data = (maxValue(sizeof(data)) >> (data_size - level));

    // Only change when led data is different
    if(prevData != data){
        ESP_LOGI("Led data", INT16_TO_BINARY(data));

        // Write led data to the i/o expander
        ioExpander.set(data);
    }
    prevData = data;
}

void LedDriver::allOn() {
    setLevel(16);
}

void LedDriver::allOff() {
    setLevel(0);
}