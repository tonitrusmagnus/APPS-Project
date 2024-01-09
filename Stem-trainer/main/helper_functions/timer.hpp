/**
 * @file timer.hpp
 * @brief Declaration of the Timer class for handling periodic timer events.
 */

#ifndef TIMER_HPP
#define TIMER_HPP

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_timer.h"
#include <esp_system.h>
#include "esp_err.h"
#include "esp_log.h"

/**
 * @class Timer
 * @brief Class for handling periodic timer events using ESP32 timers.
 */
class Timer
{
private:
    /**
     * @brief Timer ISR (Interrupt Service Routine) function.
     * @param arg Pointer to the Timer object.
     */
    static void IRAM_ATTR timer_isr(void* arg);

    esp_timer_handle_t timer1; /**< ESP32 timer handle. */
    int period; /**< Period of the timer in microseconds. */

    /**
     * @brief Function called on timer interrupt.
     */
    void timer_interrupt();

public:
    /**
     * @brief Constructor for the Timer class.
     * @param period_ The period of the timer in microseconds.
     */
    Timer(int period_);

    bool flag; /**< Flag indicating whether the timer has triggered. */

    /**
     * @brief Resets the timer flag.
     */
    void reset();

    /**
     * @brief Initializes the timer.
     */
    void init();

    /**
     * @brief Starts the timer.
     */
    void start();

    /**
     * @brief Stops the timer.
     */
    void stop();
};

#endif // TIMER_HPP
