#include "statemachine.hpp"

#include "esp_log.h"

#include "hardware_drivers/buttons.hpp"
#include "hardware_drivers/Led_Driver.hpp"
#include "hardware_drivers/servo_driver.cpp"
#include "software_drivers/mp3_driver.hpp"
#include "helper_functions/timer.hpp"
#include "helper_functions/queue.hpp"
#include "helper_functions/defines.hpp"
#include "helper_functions/audioDefines.hpp"
#include "helper_functions/helper_functions.hpp"

gpio_num_t LedPins[LED_AMOUNT] = {GPIO_NUM_4, GPIO_NUM_15, GPIO_NUM_2, GPIO_NUM_5, GPIO_NUM_13, GPIO_NUM_12, GPIO_NUM_14, GPIO_NUM_27};

int_queue volumeQueue(MEASURE_QUEUE_LENGTH);
int_queue frequencyQueue(MEASURE_QUEUE_LENGTH);
int_queue calibrateQueueHZ(CALIBRATE_QUEUE_LENGTH);
int_queue calibrateQueueDb(CALIBRATE_QUEUE_LENGTH);

#if CALIBRATE_BUTTON_ACTIVE == true
Button calibrateButton;
#endif
#if MUTE_BUTTON_ACTIVE == true
Button muteButton;
#endif
Button volumeOnLedButton;
Button frequencyOnLedButton;

Led_Driver ledDriver(LedPins, LED_AMOUNT);
Servo servoDriver(SERVO_PIN);

MP3Driver mp3Driver;

Timer tenSecondTimer;

int currentState = setup;
int nextState = setup;
int feedbackState = volumeOnLed;
int volume;
int frequency;
bool dataReady = 0;
bool feedbackStateChanged = 0;
bool muted = false;

// Thresholds
int thres_Hz = 90;
int thres_dB = 40;

void statemachine(bool *_dataReady, int _volume, int _frequency)
{
    dataReady = *_dataReady;
    //ESP_LOGW("ready 1?" ,"dataReady:%d, _dataReady:%d", dataReady, *_dataReady);

    volume = _volume;
    frequency = _frequency;

    if (DEBUGGING)
    {
        stateDebug();
    }

    switch (currentState)
    {
    case setup:
        // ESP_LOGI("Current State:", "setup");
        //  Initialisations here
        stateSetup();
        nextState = relax;
        break;
    case relax:
        // ESP_LOGI("Current State:", "relax");
        //  relax excercises
        stateRelax();

        nextState = process;
        break;
    case process:
        // ESP_LOGI("Current State:", "process");
        //  Process values
        stateProcess();
        dataReady = 0;

        nextState = feedback;
#if CALIBRATE_BUTTON_ACTIVE == true
        if (calibrateButton.flag == true)
        {
            vTaskDelay(50 / portTICK_PERIOD_MS); // Wait 50ms for debounce
            nextState = calibrate;
            calibrateButton.flag = false;
        }
#endif
        break;
    case feedback:
        // ESP_LOGI("Current State:", "feedback");

        // Give feedback
        stateFeedback();
        nextState = process;
        break;

    case calibrate:
#if CALIBRATE_BUTTON_ACTIVE == true

        // Calibrate voice trainer
        stateCalibrate();

        // If callibration button pressed, go back to process
        if ((calibrateButton.flag == true))
        {    
            vTaskDelay(50 / portTICK_PERIOD_MS); // Wait 50ms for debounce
            if(calibrateQueueDb.length() > MIN_CALIBRATIONVALUES_AMOUNT){
                thres_dB = calibrateQueueDb.highest() - DB_CALIBRATION_OFFSET;
                thres_Hz = calibrateQueueHZ.lowest() + HZ_CALIBRATION_OFFSET;
            }
            ESP_LOGI("Vol Thres", "%d", thres_dB);
            ESP_LOGI("Freq Thres", "%d", thres_Hz);
            calibrateQueueDb.clear();
            calibrateQueueHZ.clear();
            nextState = process;
            calibrateButton.flag = false;
        }

#else
        nextState = process;
#endif
        break;
    default:
        ESP_LOGE("State error", "Undefined state");
        nextState = process;
        break;
    }

    *_dataReady = dataReady;
    //ESP_LOGW("ready? 2" ,"dataReady:%d, _dataReady:%d", dataReady, *_dataReady);
    currentState = nextState;
}

void stateSetup(void)
{
    mp3Driver.init(TXD_PIN, RXD_PIN, UARTMP3);
    tenSecondTimer.init(10000);
    volumeOnLedButton.init(DB_ON_LED_BUTTON_PIN, rising);
    frequencyOnLedButton.init(HZ_ON_LED_BUTTON_PIN, rising);
#if CALIBRATE_BUTTON_ACTIVE == true
    calibrateButton.init(CALIBRATE_BUTTON_PIN, rising);
#endif
#if MUTE_BUTTON_ACTIVE == true
    muteButton.init(MUTE_BUTTON_PIN, rising);
#endif
}

void stateRelax(void)
{
}

void stateProcess()
{
    if (dataReady == true)
    {
        volumeQueue.add(volume);
        frequencyQueue.add(frequency);
        dataReady = false;
    }

    // Buttons
    if ((volumeOnLedButton.flag == true) && (feedbackState == frequencyOnLed))
    {
        vTaskDelay(50 / portTICK_PERIOD_MS); // Wait 50ms for debounce
        frequencyOnLedButton.flag = false;
        volumeOnLedButton.flag = false;

        ESP_LOGI("Feedback state:", "Volume on Leds");
        feedbackState = volumeOnLed;
        feedbackStateChanged = true;
    }

    else if ((frequencyOnLedButton.flag == true) && (feedbackState == volumeOnLed))
    {
        vTaskDelay(50 / portTICK_PERIOD_MS); // Wait 50ms for debounce
        volumeOnLedButton.flag = false;
        frequencyOnLedButton.flag = false;

        ESP_LOGI("Feedback state:", "Frequency on Leds");
        feedbackState = frequencyOnLed;
        feedbackStateChanged = true;
    }

#if MUTE_BUTTON_ACTIVE == true
    if (muteButton.flag == true)
    {
        vTaskDelay(50 / portTICK_PERIOD_MS); // Wait 50ms for debounce
        muteButton.flag = false;
        muted = !muted;
        ESP_LOGI("mutebutton", "%d", muted);
    }
#endif
}

void stateFeedback()
{
    // display the visual feedback
    visualFeedback();
    // send the audio feedback
    audioFeedback();
}

void stateCalibrate()
{
    ESP_LOGI("In state", "Calibrate");

    if (dataReady == true)
    {
        calibrateQueueDb.add(volume);
        calibrateQueueHZ.add(frequency);
        ESP_LOGI("calibrate queue", "add %d db and %d HZ",calibrateQueueDb.latest(),calibrateQueueHZ.latest());
        dataReady = false;
    }
}

void stateDebug()
{
}

void visualFeedback()
{
    // Calculate the fraction depending on the feedback state
    float fraction = 0;
    if (feedbackState == volumeOnLed)
    {
        // Volume
        fraction = (float)(constrain(volumeQueue.latest(), DB_THRES_MIN, thres_dB) - DB_THRES_MIN) / (float)(thres_dB - DB_THRES_MIN);
    }
    else if (feedbackState == frequencyOnLed)
    {
        // Frequency
        fraction = (float)(HZ_THRES_MAX - (constrain(frequencyQueue.latest(), thres_Hz, HZ_THRES_MAX))) / (float)(HZ_THRES_MAX - thres_Hz);
    }

// Feedback can be on Leds or Servo depending on specific prototype
#if VISUAL_FEEDBACK == LEDS

    // Calculate the amount of leds to be turned on
    int ledLevel = constrain(fraction * LED_AMOUNT, 0, LED_AMOUNT);
    ledDriver.setLevel(ledLevel);

#elif VISUAL_FEEDBACK == SERVO

    // Calculate percentage
    int servoLevel = constrain(fraction * SERVO_MAX_ANGLE, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
    servoDriver.setAngle(servoLevel);

#endif
}

void audioFeedback()
{

    if (feedbackStateChanged == true)
    {
        feedbackStateChanged = false;
        tenSecondTimer.skip = true;
        if (feedbackState == volumeOnLed)
        {
            mp3Driver.play(FOLDER_VOLUME_ON_LED, randomNumber(1, VOLUME_ON_LED_FILES_AMOUNT));
            ESP_LOGI("feedbackstateaudio", "VOL ON LED");
        }
        else if (feedbackState == frequencyOnLed)
        {
            mp3Driver.play(FOLDER_FREQUENCY_ON_LED, randomNumber(1, FREQUENCY_ON_LED_FILES_AMOUNT));
            ESP_LOGI("feedbackstateaudio", "FREQ ON LED");
        }
    }
    else if (tenSecondTimer.flag == true && muted == false)
    {
        tenSecondTimer.flag = false;
        ESP_LOGI("tenseconds_interupt", "trigger");
        
        ESP_LOGI("vol queue avg", "%lf", volumeQueue.average());

        if (feedbackState == volumeOnLed && volumeQueue.average() >= thres_dB)
        {
            mp3Driver.play(FOLDER_VOLUME_GOOD, randomNumber(1, VOLUME_GOOD_FILES_AMOUNT));
            ESP_LOGI("FEEDBACK", "VOLUME_GOOD");
        }
        else if (feedbackState == volumeOnLed)
        {
            mp3Driver.play(FOLDER_VOLUME_HIGHER, randomNumber(1, VOLUME_HIGHER_FILES_AMOUNT));
            ESP_LOGI("FEEDBACK", "VOLUME_HIGHER");
        }
        else if (feedbackState == frequencyOnLed && frequencyQueue.average() <= thres_Hz)
        {
            mp3Driver.play(FOLDER_FREQUENCY_GOOD, randomNumber(1, FREQUENCY_GOOD_FILES_AMOUNT));
            ESP_LOGI("FEEDBACK", "FREQUENCY_GOOD");
        }
        else if (feedbackState == frequencyOnLed)
        {
            mp3Driver.play(FOLDER_FREQUENCY_LOWER, randomNumber(1, FREQUENCY_LOWER_FILES_AMOUNT));
            ESP_LOGI("FEEDBACK", "FREQUENCY_LOWER");
        }
    }
}