#include <driver/gpio.h>

class Leds
{
private:
    gpio_num_t LedPin;
public:
    Leds(gpio_num_t d);
    ~Leds();
    void setState(bool State);
};