/**
 * @file led_driver.hpp
 * @brief Definition of the LedDriver class.
 */

#ifndef LED_DRIVER_HPP
#define LED_DRIVER_HPP

#include <stdio.h>
#include <cstring>
#include <driver/gpio.h>
#include "max7312.hpp"

/**
 * @class LedDriver
 * @brief Class representing a LED driver using the Max7312 IO expander.
 */
class LedDriver
{
private:
    Max7312 ioExpander; /**< The Max7312 IO expander used for LED control. */
    uint16_t prevData; /**< Previous data sent to the IO expander. */

public:
    /**
     * @brief Constructor for the LedDriver class.
     */
    LedDriver();

    /**
     * @brief Initializes the LED driver and IO expander.
     */
    void init();

    /**
     * @brief Sets the LED level to a specified value.
     * @param level The desired LED level (0 up to 16).
     */
    void setLevel(uint8_t level);

    /**
     * @brief Turns on all LEDs.
     */
    void allOn();

    /**
     * @brief Turns off all LEDs.
     */
    void allOff();
};

#endif // LED_DRIVER_HPP