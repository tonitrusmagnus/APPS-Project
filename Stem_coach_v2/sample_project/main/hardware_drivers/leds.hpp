#include <driver/gpio.h>

class Leds
{
private:
    gpio_num_t LedPin;
    bool currentState = false;
public:
    Leds(gpio_num_t d);
    void init();
    void setState(bool State);
    void inverse();
};