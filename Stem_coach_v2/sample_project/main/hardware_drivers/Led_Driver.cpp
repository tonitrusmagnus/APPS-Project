#include "Led_Driver.hpp"
#include "esp_log.h"

Led_Driver::Led_Driver(gpio_num_t *LEDPin,int size)
{
    Led_Size = size;
    std::memcpy(Led_Pins,LEDPin,sizeof(Led_Pins)); 
    for(int i = 0; i < size; i++){
        esp_rom_gpio_pad_select_gpio(LEDPin[i]);
        ESP_ERROR_CHECK(gpio_set_direction(LEDPin[i],GPIO_MODE_OUTPUT));
        ESP_ERROR_CHECK(gpio_set_level(LEDPin[i],0));
    }
}

Led_Driver::~Led_Driver()
{
}

void Led_Driver::setLevel(int level)
{
    for(int i = 0; i < Led_Size; i++){
        ESP_ERROR_CHECK(gpio_set_level(Led_Pins[i],0));
    }
    for(int i = 0; i < level; i++){
        ESP_ERROR_CHECK(gpio_set_level(Led_Pins[i],1));
    }

}