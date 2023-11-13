#include "statemachine.hpp"

#include "esp_log.h"

#include "hardware_drivers/buttons.hpp"
#include "hardware_drivers/Led_Driver.hpp"
#include "hardware_drivers/servo_driver.cpp"
#include "software_drivers/mp3_driver.hpp"
#include "helper_functions/queue.hpp"
#include "helper_functions/defines.hpp"
#include "helper_functions/helper_functions.hpp"

gpio_num_t LedPins[LED_AMOUNT] = {GPIO_NUM_4,GPIO_NUM_15,GPIO_NUM_2,GPIO_NUM_5,GPIO_NUM_13,GPIO_NUM_12,GPIO_NUM_14,GPIO_NUM_27};

int_queue volumeQueue(MEASURE_QUEUE_LENGTH);
int_queue frequencyQueue(MEASURE_QUEUE_LENGTH);
int_queue calibrateQueueHZ(CALIBRATE_QUEUE_LENGTH);
int_queue calibrateQueueDb(CALIBRATE_QUEUE_LENGTH);

Button calibrateButton;
Button volumeOnLedButton;
Button frequencyOnLedButton;

Led_Driver ledDriver(LedPins, LED_AMOUNT);
Servo servoDriver(SERVO_PIN, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);

MP3Driver mp3Driver;

int currentState = setup;
int nextState = setup;
int feedbackState = volumeOnLed;
int volume;
int frequency;
bool dataReady;

// Thresholds
int thres_Hz = 90;
int thres_dB = 40;

void statemachine (bool* dataReady , int _volume, int _frequency){
    volume = _volume;
    frequency = _frequency;

    ESP_LOGI("Running","statemachine");

    if (DEBUGGING) {
        stateDebug();
    }

    switch(currentState){
        case setup :
            //ESP_LOGI("Current State:", "setup");

            // Initialisations here
            stateSetup();

            nextState = relax;
            break;
        case relax :
            //ESP_LOGI("Current State:", "relax");

            // relax excercises
            stateRelax();

            nextState = process;
            break;
        case process : 
            //ESP_LOGI("Current State:", "process");

            // Process values
            stateProcess(dataReady);
            dataReady = 0;
                
            nextState = feedback;
           if (calibrateButton.flag == true) { 
                nextState = calibrate;
                calibrateButton.flag = false;
            }
            break;
        case feedback :
            //ESP_LOGI("Current State:", "feedback");

            // Give feedback 
            stateFeedback();
            nextState = process;
        break;

        case calibrate :
            //ESP_LOGI("Current State:", "calibrate");

            // Calibrate voice trainer
            stateCalibrate();

            // If callibration button pressed, go back to process
            if(calibrateButton.flag == true){
                nextState = process;
                calibrateButton.flag = false;
            }
        break;
        default :
            //ESP_LOGE("State error", "Undefined state");
        break;
    }
    
    currentState = nextState;
}

void stateSetup(void){
    mp3Driver.init(TXD_PIN, RXD_PIN, UARTMP3);
    calibrateButton.init(CALIBRATE_BUTTON_PIN, rising);
    volumeOnLedButton.init(DB_ON_LED_BUTTON_PIN, rising);
    frequencyOnLedButton.init(HZ_ON_LED_BUTTON_PIN, rising);

}

void stateRelax(void){
    
}

void stateProcess(bool dataReady){
    if (dataReady == true) {
        volumeQueue.add(volume);
        frequencyQueue.add(frequency);
    }
    // knopjes
    if((volumeOnLedButton.flag == true) && ( feedbackState == frequencyOnLed)){
        ESP_LOGI("Feedback state:", "Volume on Leds");
        feedbackState = volumeOnLed;
        volumeOnLedButton.flag = false;
    }

    else if((frequencyOnLedButton.flag == true) && ( feedbackState == volumeOnLed)){
        ESP_LOGI("Feedback state:", "Frequency on Leds");
        feedbackState = frequencyOnLed;
        frequencyOnLedButton.flag = false;
    }
    
}

void stateFeedback(){
    
    // Calculate the fraction depending on the feedback state
    float fraction = 0;
    if (feedbackState == volumeOnLed) {
        // Volume
        fraction = (float)(constrain(volumeQueue.latest(),DB_THRES_MIN,thres_dB) - DB_THRES_MIN) / (float)(thres_dB - DB_THRES_MIN);
    } else {
        // Frequency
        fraction = (float)(HZ_THRES_MAX - (constrain(frequency,thres_Hz,HZ_THRES_MAX) )) / (float)(HZ_THRES_MAX - thres_Hz);
    }

// Feedback can be on Leds or Servo depending on specific prototype
#if VISUAL_FEEDBACK == LEDS
    
    // Calculate the amount of leds to be turned on
    int ledLevel = constrain( fraction * LED_AMOUNT, 0, LED_AMOUNT);
    ledDriver.setLevel(ledLevel);
    
#elif VISUAL_FEEDBACK == SERVO
    
    // Calculate percentage
    int servoLevel = constrain( fraction * 100, 0, 100);
    servoDriver.set_servo(servoLevel);
    
#endif

}

void stateCalibrate(){
    if (dataReady) {
        calibrateQueueDb.add(volume);
        calibrateQueueHZ.add(frequency);
    }
    if ((calibrateButton.flag == true) && (calibrateQueueDb.length() > MIN_CALIBRATIONVALLUES_AMOUNT)){
        thres_dB = calibrateQueueDb.highest() - DB_CALIBRATION_OFFSET;
        thres_Hz = calibrateQueueHZ.lowest() + HZ_CALIBRATION_OFFSET;
        calibrateButton.flag = false;
    }
    
}

void stateDebug(){
    
}