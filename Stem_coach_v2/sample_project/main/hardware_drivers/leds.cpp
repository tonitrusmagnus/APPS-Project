#include "leds.hpp"

Leds::Leds(gpio_num_t LedPin_)
{
    LedPin = LedPin_;
    esp_rom_gpio_pad_select_gpio(LedPin);
    ESP_ERROR_CHECK(gpio_set_direction(LedPin,GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(LedPin,0));
}

Leds::~Leds()
{

}

void Leds::setState(bool State){
    ESP_ERROR_CHECK(gpio_set_level(LedPin,State));
}