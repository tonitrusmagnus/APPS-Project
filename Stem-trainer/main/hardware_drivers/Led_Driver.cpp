#include "Led_Driver.hpp"
#include "esp_log.h"
#include "helper_functions/defines.hpp"
#include "helper_functions/helper_functions.hpp"
#include <cmath>

Led_Driver::Led_Driver() : ioExpander(LED_SDA_PIN, LED_SCL_PIN, LED_I2C_PORT) 
{
}

void Led_Driver::init(){
    ioExpander.init();
    ioExpander.set(0); // All leds off on initialisation
}

void Led_Driver::setLevel(int levelL,int levelR)
{
    uint16_t data = 0; 
    uint8_t dataL = 0;
    uint8_t dataR = 0;
    int data_sizeL = sizeof(dataL)*8; // Byte is 8 bits
    int data_sizeR = sizeof(dataR)*8;
    
    dataL = (maxValue(sizeof(dataL)) >> (data_sizeL - levelL));
    dataR = (maxValue(sizeof(dataR)) >> (data_sizeR - levelR));
    
    data = (dataR << 8) | (dataL);

    // Only change when led data is different
    if(prevData != data){
        ESP_LOGI("Led data","0x%04x",data);

        // Write led data to the i/o expander
        ioExpander.set(data);
    }
    prevData = data;
}