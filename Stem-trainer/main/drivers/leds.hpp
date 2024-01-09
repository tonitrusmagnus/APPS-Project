/**
 * @file leds.hpp
 * @brief Declaration of the Leds class.
 */

#ifndef LEDS_HPP
#define LEDS_HPP

#include <driver/gpio.h>

/**
 * @class Leds
 * @brief Class representing an LED and its control.
 */
class Leds
{
private:
    gpio_num_t LedPin;    /**< The GPIO pin connected to the LED. */
    bool currentState;     /**< The current state (on/off) of the LED. */

public:
    /**
     * @brief Constructor for the Leds class.
     * @param d The GPIO pin for the LED.
     */
    Leds(gpio_num_t d);

    /**
     * @brief Initializes the LED.
     */
    void init();

    /**
     * @brief Sets the state of the LED.
     * @param State The desired state of the LED (true for on, false for off).
     */
    void setState(bool State);

    /**
     * @brief Inverts the current state of the LED.
     * If the LED is on, it will be turned off, and vice versa.
     */
    void inverse();
};

#endif // LEDS_HPP
