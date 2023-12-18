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

Button volumeOnLedButton(DB_ON_LED_BUTTON_PIN, GPIO_INTR_POSEDGE);
Button frequencyOnLedButton(HZ_ON_LED_BUTTON_PIN, GPIO_INTR_POSEDGE);
    
#if CALIBRATE_BUTTON_ACTIVE == true
Button calibrateButton(CALIBRATE_BUTTON_PIN, GPIO_INTR_POSEDGE);
#endif
#if MUTE_BUTTON_ACTIVE == true
Button muteButton(MUTE_BUTTON_PIN, GPIO_INTR_POSEDGE);
#endif


Led_Driver ledDriver;
Servo servoDriver(SERVO_PIN, LEDC_TIMER_0, LEDC_CHANNEL_0);
Leds blinkingLed(BLINKING_LED_PIN);

MP3Driver mp3Driver(MP3_TXD_PIN, MP3_RXD_PIN, UARTMP3,FEEDBACK_TIMEOUT_TICKS);
bool mp3IsPlaying = false;

Timer tenSecondTimer(10000);
Timer secondTimer(1000);

int currentState = setup;
int nextState = setup;
int feedbackState = volumeOnLed;
int volume;
int frequency;
bool dataReady = 0;
bool muted = false;

// Thresholds
int thres_Hz = 90;
int thres_dB = 45;

void statemachine(bool *_dataReady, int _volume, int _frequency)
{
    dataReady = *_dataReady;
    //ESP_LOGW("ready 1?" ,"dataReady:%d, _dataReady:%d", dataReady, *_dataReady);

    volume = _volume;
    frequency = _frequency;

    switch (currentState)
    {
    case setup:
        ESP_LOGI("Current State:", "setup");
        //  Initialisations here
        stateSetup();
        nextState = relax;
        break;
    case relax:
        // ESP_LOGI("Current State:", "relax");
        //  relax excercises
#if RELAX_ACTIVE == true
        stateRelax();

        // End relax state when audio is finished or when calibrate button is pressed to stop manually
        if(mp3Driver.isPlaying() == false || calibrateButton.getFlag() == true){ 
            vTaskDelay(DEBOUNCE_DELAY / portTICK_PERIOD_MS); // Delay for debounce
            mp3Driver.stop();

            // Reset all button flags
            calibrateButton.resetFlag();
            muteButton.resetFlag();
            frequencyOnLedButton.resetFlag();
            volumeOnLedButton.resetFlag();
            
            tenSecondTimer.start();
            tenSecondTimer.reset();
            
            ESP_LOGI("Relaxation", "Completed");
            nextState = process;
        }
#else 
        nextState = process;
#endif
        break;
    case process:
        
        //  Process values
        stateProcess();
        dataReady = 0;

        nextState = feedback;

#if CALIBRATE_BUTTON_ACTIVE == true
        if (calibrateButton.getFlag() == true)
        {
            ESP_LOGW("Calibrate Button","Pressed, to state cal.");

            vTaskDelay(DEBOUNCE_DELAY / portTICK_PERIOD_MS); // Delay for debounce
            calibrateButton.resetFlag();
            
            nextState = calibrate;
            secondTimer.start();
            mp3Driver.play(FOLDER_CALLIBRATE,CALLIBRATE_FILE_STARTED);
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
        if ((calibrateButton.getFlag() == true))
        {    
            ESP_LOGW("Calibrate Button","Pressed, to state process");

            vTaskDelay(DEBOUNCE_DELAY / portTICK_PERIOD_MS); // Delay for debounce
            calibrateButton.resetFlag();

            if(calibrateQueueDb.length() > MIN_CALIBRATIONVALUES_AMOUNT){
                thres_dB = calibrateQueueDb.highest() - DB_CALIBRATION_OFFSET;
                thres_Hz = calibrateQueueHZ.lowest() + HZ_CALIBRATION_OFFSET;
            }
            mp3Driver.play(FOLDER_CALLIBRATE,CALLIBRATE_FILE_STOPPED);
            tenSecondTimer.reset(); // Reset timer for audio feedback

            secondTimer.stop();

            ESP_LOGI("Volume result", "%d", thres_dB);
            ESP_LOGI("Frequency result", "%d", thres_Hz);
            calibrateQueueDb.clear();
            calibrateQueueHZ.clear();

            nextState = process;
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

    blinkingLed.inverse(); // Inverse led to blink while active
    *_dataReady = dataReady;
    //ESP_LOGW("ready? 2" ,"dataReady:%d, _dataReady:%d", dataReady, *_dataReady);
    currentState = nextState;
}

void stateSetup(void)
{
    blinkingLed.init();
    blinkingLed.setState(false); // Blinking led off on startup

    servoDriver.init();
    servoDriver.setAngle(0); // Set initial angle to 0

    ledDriver.init();
    blinkingLed.init();

    mp3Driver.init();
    mp3Driver.enableFeedback(false);
    mp3Driver.setVolume(30); // half volume

    tenSecondTimer.init();
    tenSecondTimer.stop();
    secondTimer.init();
    secondTimer.stop();

    volumeOnLedButton.init();
    frequencyOnLedButton.init();
    
#if CALIBRATE_BUTTON_ACTIVE == true
    calibrateButton.init();
#endif
#if MUTE_BUTTON_ACTIVE == true
    muteButton.init();
#endif

    vTaskDelay(500 / portTICK_PERIOD_MS); // Delay wait for devices to be set up
    
    ESP_LOGI("Setup", "Completed");
}

void stateRelax(void)
{
    ESP_LOGI("Relaxation", "playing");

    static bool isplaying = false;
    if(isplaying == false){
        isplaying = true;
        mp3Driver.playRandom(FOLDER_RELAX,RELAX_FILES_AMOUNT);
        
        ESP_LOGI("Relaxation", "Audio file started");
    }
    
    vTaskDelay(500 / portTICK_PERIOD_MS); // Delay to prevent reading the mp3 player too fast
}

void stateProcess()
{   
    if (dataReady == true)
    {
        bool isplaying = mp3Driver.isPlaying();
        ESP_LOGI("Audio is playing", "%s", (isplaying ? "true" : "false"));
        // Dont add readings from speaker
        if (isplaying == false) {
            ESP_LOGI("Volume","%d",volume);
            volumeQueue.add(volume);
            ESP_LOGI("Frequency","%d",frequency);
            frequencyQueue.add(frequency);
        }
        dataReady = false;
    }

    // Buttons
    if ((volumeOnLedButton.getFlag() == true) && (feedbackState == frequencyOnLed))
    {
        vTaskDelay(DEBOUNCE_DELAY / portTICK_PERIOD_MS); // Delay for debounce
        frequencyOnLedButton.resetFlag();
        volumeOnLedButton.resetFlag();

        ESP_LOGI("Feedback state:", "Volume on Leds");
        feedbackState = volumeOnLed;
        mp3Driver.playRandom(FOLDER_VOLUME_ON_LED,VOLUME_ON_LED_FILES_AMOUNT);

        tenSecondTimer.reset(); // Reset timer for audio feedback
    }

    else if ((frequencyOnLedButton.getFlag() == true) && (feedbackState == volumeOnLed))
    {
        vTaskDelay(DEBOUNCE_DELAY / portTICK_PERIOD_MS); // Delay for debounce
        volumeOnLedButton.resetFlag();
        frequencyOnLedButton.resetFlag();

        ESP_LOGI("Feedback state:", "Frequency on Leds");
        feedbackState = frequencyOnLed;
        mp3Driver.playRandom(FOLDER_FREQUENCY_ON_LED, FREQUENCY_ON_LED_FILES_AMOUNT);

        tenSecondTimer.reset(); // Reset timer for audio feedback
    }

#if MUTE_BUTTON_ACTIVE == true
    if (muteButton.getFlag() == true)
    {
        vTaskDelay(DEBOUNCE_DELAY / portTICK_PERIOD_MS); // Delay for debounce
        muteButton.resetFlag();
        muted = !muted;
        ESP_LOGI("Mutebutton", "%d", muted);

        if (muted) {
            mp3Driver.play(FOLDER_MUTE, MUTE_FILE_MUTED);
        }
        else {
            mp3Driver.play(FOLDER_MUTE, MUTE_FILE_UNMUTED);
        }
        tenSecondTimer.reset();
    }
#endif
}

void stateFeedback()
{
    // display the visual feedback
    visualFeedback();
    
#if PROTOTYPE_3 == false

    // send the audio feedback
    audioFeedback();

#endif 

}

void stateCalibrate()
{
    
    static bool flash = 0;
    if(secondTimer.flag == true){
        int level = flash*LED_AMOUNT;
        ledDriver.setLevel(level,level);
        secondTimer.flag = false;
        flash = !flash;
    }
    if (dataReady == true)
    {
        bool isplaying = mp3Driver.isPlaying();
        if(isplaying == false){
            calibrateQueueDb.add(volume);
            calibrateQueueHZ.add(frequency);
            ESP_LOGI("Calibrate queue", "add %d dB and %d Hz",calibrateQueueDb.latest(),calibrateQueueHZ.latest());
        }
        dataReady = false;
    }
}

void visualFeedback()
{
    // Calculate the frequency and volume fractions
    float fractionFrequency = (float)(HZ_THRES_MAX - (constrain(frequencyQueue.latest(), thres_Hz, HZ_THRES_MAX))) / (float)(HZ_THRES_MAX - thres_Hz);
    float fractionVolume = (float)(constrain(volumeQueue.latest(), DB_THRES_MIN, thres_dB) - DB_THRES_MIN) / (float)(thres_dB - DB_THRES_MIN);

#if PROTOTYPE_3 == false
    int ledLevel = 0;
    int servoLevel = 0;

    //ESP_LOGW("fractions:","V: %lf\tF: %lf", fractionVolume, fractionFrequency);

    // Set led and servo levels depending on the feedback state
    if (feedbackState == volumeOnLed)
    {
        // Calculate the amount of leds to be turned on
        ledLevel = constrain(fractionVolume * LED_AMOUNT, 0, LED_AMOUNT);

        // Calculate angle for the servo. invert fraction to make servo move to the right
        servoLevel = mapDouble(1-fractionVolume, 0, 1, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
    }
    else if (feedbackState == frequencyOnLed)
    {
        // Calculate the amount of leds to be turned on
        ledLevel = constrain(fractionFrequency * LED_AMOUNT, 0, LED_AMOUNT);
        
        // Calculate angle for the servo. invert fraction to make servo move to the right
        servoLevel = mapDouble(1-fractionFrequency, 0, 1, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);

    }
    
    ledDriver.setLevel(ledLevel,0);
    servoDriver.setAngle(servoLevel);
#else
    int ledLevelL = 0;
    int ledLevelR = 0;

    if (feedbackState == volumeOnLed)
    {
        ledLevelR = constrain(fractionVolume * LED_AMOUNT, 0, LED_AMOUNT);
    }
    else if (feedbackState == frequencyOnLed)
    {
        ledLevelL = constrain(fractionFrequency * LED_AMOUNT, 0, LED_AMOUNT);
    }


    ledDriver.setLevel(ledLevelL,ledLevelR);
#endif

}

void audioFeedback()
{

    if (tenSecondTimer.flag == true && muted == false)
    {
        tenSecondTimer.flag = false;
        
        ESP_LOGI("Average Volume", "%lf", volumeQueue.average());
        ESP_LOGI("Average Frequency", "%lf", volumeQueue.average());

        if (feedbackState == frequencyOnLed && volumeQueue.average() >= thres_dB)
        {
            mp3Driver.playRandom(FOLDER_VOLUME_GOOD, VOLUME_GOOD_FILES_AMOUNT);
            ESP_LOGI("FEEDBACK", "VOLUME_GOOD");
        }
        else if (feedbackState == frequencyOnLed)
        {
            mp3Driver.playRandom(FOLDER_VOLUME_HIGHER, VOLUME_HIGHER_FILES_AMOUNT);
            ESP_LOGI("FEEDBACK", "VOLUME_HIGHER");
        }
        else if (feedbackState == volumeOnLed && frequencyQueue.average() <= thres_Hz)
        {
            mp3Driver.playRandom(FOLDER_FREQUENCY_GOOD, FREQUENCY_GOOD_FILES_AMOUNT);
            ESP_LOGI("FEEDBACK", "FREQUENCY_GOOD");
        }
        else if (feedbackState == volumeOnLed)
        {
            mp3Driver.playRandom(FOLDER_FREQUENCY_LOWER, FREQUENCY_LOWER_FILES_AMOUNT);
            ESP_LOGI("FEEDBACK", "FREQUENCY_LOWER");
        }
    }
}