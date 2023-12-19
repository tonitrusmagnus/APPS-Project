#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "hardware_drivers/buttons.hpp"
#include "hardware_drivers/Led_Driver.hpp"
#include "hardware_drivers/servo_driver.hpp"
#include "software_drivers/mp3_driver.hpp"
#include "helper_functions/timer.hpp"
#include "helper_functions/queue.hpp"
#include "helper_functions/defines.hpp"
#include "helper_functions/audioDefines.hpp"
#include "helper_functions/helper_functions.hpp" 
#include "Apps_main.hpp"

class Statemachine
{
private:
    enum states{setup,relax,process,feedback,calibrate};
    enum feedbackstate{volumeOnLed,frequencyOnLed};

    int_queue volumeQueue;
    int_queue frequencyQueue;
    int_queue calibrateQueueHZ;
    int_queue calibrateQueueDb;
    Button volumeOnLedButton;
    Button frequencyOnLedButton;
    Button calibrateButton;
    Button muteButton;
    Led_Driver ledDriver;
    Servo servoDriver;
    Leds blinkingLed;
    MP3Driver mp3Driver;
    Timer tenSecondTimer;
    Timer secondTimer;

    bool mp3IsPlaying = false;

    int currentState = setup;
    int nextState = setup;
    int feedbackState = volumeOnLed;
    int volume;
    int frequency;
    bool dataReady = 0;
    bool muted = false;

    // Thresholds
    int thres_Hz = FREQUENCY_STARTVALUE;
    int thres_dB = VOLUME_STARTVALUE;

    void stateSetup(void);
    void stateRelax(void);
    void stateProcess();
    void stateFeedback();
    void stateCalibrate();

    void visualFeedback();
    void audioFeedback();
    
public:
    Statemachine();
    void run();
    void setValues(int frequency_, int volume_);
};


