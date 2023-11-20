#include "Led_Driver.hpp"
#include "esp_log.h"

Led_Driver::Led_Driver(gpio_num_t *LEDPin,int size)
{
    ledSize = size; 
    ledarray = (Leds*)malloc(sizeof(Leds)*size);
    for(int i = 0; i < ledSize; i++) {
        ledarray[i] = Leds(LEDPin[i]);
    }
}

Led_Driver::~Led_Driver()
{
}

void Led_Driver::setLevel(int level)
{
    for(int i = 0; i < ledSize; i++){
        ledarray[i].setState(false);
    }
    for(int i = 0; i < level; i++){
        ledarray[i].setState(true);
    }

}