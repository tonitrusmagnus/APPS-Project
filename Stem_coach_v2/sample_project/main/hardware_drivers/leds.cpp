#include "leds.hpp"

Leds::Leds(gpio_num_t LedPin_)
{
    LedPin = LedPin_;
}

void Leds::init() {
    esp_rom_gpio_pad_select_gpio(LedPin);
    ESP_ERROR_CHECK(gpio_set_direction(LedPin,GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(LedPin,0));
}

void Leds::setState(bool State){
    currentState = State;
    ESP_ERROR_CHECK(gpio_set_level(LedPin,State));
}

void Leds::inverse() {
    currentState = !currentState;
    ESP_ERROR_CHECK(gpio_set_level(LedPin,currentState));
}