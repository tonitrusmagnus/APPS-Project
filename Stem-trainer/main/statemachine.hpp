/**
 * @file statemachine.hpp
 * @brief Declaration of the Statemachine class for implementing a state machine control logic.
 */

#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "drivers/buttons.hpp"
#include "drivers/led_driver.hpp"
#include "drivers/mp3_driver.hpp"
#include "helper_functions/timer.hpp"
#include "helper_functions/queue.hpp"
#include "helper_functions/defines.hpp"
#include "helper_functions/audioDefines.hpp"
#include "helper_functions/helper_functions.hpp" 
#include "Apps_main.hpp"

/**
 * @class Statemachine
 * @brief Class for implementing a state machine control logic.
 */
class Statemachine
{
private:
    enum states { setup, relax, process, feedback, calibrate }; /**< Enum representing different states of the state machine. */
    enum feedbackstate { volumeOnLed, frequencyOnLed }; /**< Enum representing different feedback states. */

    IntQueue volumeQueue; /**< Queue for storing volume values. */
    IntQueue frequencyQueue; /**< Queue for storing frequency values. */
    IntQueue calibrateQueueHZ; /**< Queue for storing frequency values during calibration. */
    IntQueue calibrateQueueDb; /**< Queue for storing volume values during calibration. */
    Button volumeOnLedButton; /**< Button for triggering volume feedback to be on LED. */
    Button frequencyOnLedButton; /**< Button for triggering frequency feedback to be on LED. */
    Button calibrateButton; /**< Button for initiating calibration. */
    Button muteButton; /**< Button for muting audio. */
    LedDriver ledDriver; /**< LED driver for controlling the visual feedback. */
    Leds blinkingLed; /**< LED for feedback of running code. */
    MP3Driver mp3Driver; /**< MP3 driver for controlling the auditive feedback. */
    Timer tenSecondTimer; /**< Timer for 10-second intervals. */
    Timer secondTimer; /**< Timer for 1-second intervals. */

    bool mp3IsPlaying = false; /**< Flag indicating whether an MP3 is currently playing. */

    int currentState = setup; /**< Current state of the state machine. */
    int nextState = setup; /**< Next state of the state machine. */
    int feedbackState = volumeOnLed; /**< Current feedback state. */
    int volume; /**< Current volume value. */
    int frequency; /**< Current frequency value. */
    bool dataReady = 0; /**< Flag indicating whether data is ready for processing. */
    bool muted = false; /**< Flag indicating whether audio is muted. */

    // Thresholds
    int thres_Hz = FREQUENCY_STARTVALUE; /**< Threshold for frequency during calibration. */
    int thres_dB = VOLUME_STARTVALUE; /**< Threshold for volume during calibration. */

    /**
     * @brief Executes the setup state of the state machine.
     */
    void stateSetup(void);

    /**
     * @brief Executes the relax state of the state machine.
     */
    void stateRelax(void);

    /**
     * @brief Executes the process state of the state machine.
     */
    void stateProcess();

    /**
     * @brief Executes the feedback state of the state machine.
     */
    void stateFeedback();

    /**
     * @brief Executes the calibration state of the state machine.
     */
    void stateCalibrate();

    /**
     * @brief Provides visual feedback based on the feedback state.
     */
    void visualFeedback();

    /**
     * @brief Provides audio feedback based on the feedback state.
     */
    void audioFeedback();

public:
    /**
     * @brief Constructor for the Statemachine class.
     */
    Statemachine();

    /**
     * @brief Runs the state machine.
     */
    void run();

    /**
     * @brief Sets the frequency and volume values for processing.
     * @param frequency_ The frequency value.
     * @param volume_ The volume value.
     */
    void setValues(int frequency_, int volume_);
};

#endif // STATEMACHINE_HPP
