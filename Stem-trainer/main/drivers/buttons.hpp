/*! @file buttons.hpp
    @brief button class.
*/


#ifndef BUTTONS_HPP
#define BUTTONS_HPP

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
/*!
*@class Button
*@brief Button class with interrupt
*/
class Button

{
private:
    /**
    *@brief Pin connected to button 
    */
    gpio_num_t buttonpin;

    /**
    @brief Selected edge for the interrupt to trigger
    */
    gpio_int_type_t interruptedge;

    /**
    *@brief Interrupt handler
    */
    static void IRAM_ATTR gpio_isr_handler(void* arg);
    
    /**
    *@brief Funtion called by interrupt on button press
    */
    void onButtonPress();

    /**
    *@brief Button flag set by onButtonPress() or reset by resetFlag()
    */
    bool flag;

public:
    /**
     * @brief Constructor for Button class
     * @param buttonpin The GPIO pin number to which the button is connected (should be GPIO_NUM_xx).
     * @param interruptedge The interrupt edge type for the button (choose from GPIO_INTR_NEGEDGE or GPIO_INTR_POSEDGE).
    */
    Button(gpio_num_t buttonpin, gpio_int_type_t interruptedge);
    
    /**
     *@brief Initializes the GPIO and interrupt handler for the button
    */
    void init();
    /**
     *@brief Get the flag status
     *@return flag status
    */
    bool getFlag();
    /**
     *@brief Resets button flag to 0
    */
    void resetFlag();
    
};

#endif //BUTTONS_HPP