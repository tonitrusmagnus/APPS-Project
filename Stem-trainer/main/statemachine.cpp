#include "statemachine.hpp"

Statemachine::Statemachine() :
     volumeQueue(MEASURE_QUEUE_LENGTH),
     frequencyQueue(MEASURE_QUEUE_LENGTH),
     calibrateQueueHZ(CALIBRATE_QUEUE_LENGTH),
     calibrateQueueDb(CALIBRATE_QUEUE_LENGTH),
     volumeOnLedButton(DB_ON_LED_BUTTON_PIN, GPIO_INTR_POSEDGE),
     frequencyOnLedButton(HZ_ON_LED_BUTTON_PIN, GPIO_INTR_POSEDGE),
     calibrateButton(CALIBRATE_BUTTON_PIN, GPIO_INTR_POSEDGE),
     muteButton(MUTE_BUTTON_PIN, GPIO_INTR_POSEDGE),
     blinkingLed(BLINKING_LED_PIN),
     mp3Driver(MP3_TXD_PIN, MP3_RXD_PIN, UARTMP3,FEEDBACK_TIMEOUT_TICKS),
     tenSecondTimer(10000),
     secondTimer(1000)
{}

void Statemachine::run()
{
    switch (currentState)
    {
    case setup:
        ESP_LOGI("Current State:", "setup");
        //  Initialisations here
        stateSetup();

        // Always go to relax state after setup
        nextState = relax;
        break;

    case relax:
    
#if RELAX_ACTIVE == true
        // Run relax excercise
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
            
            // Start and reset ten second timer for auditive feedback
            tenSecondTimer.start();
            tenSecondTimer.reset();
            
            ESP_LOGI("Relaxation", "Completed");

            nextState = process;
        }
#else 
        // If relaxation excercise is turned off, go to process state
        nextState = process;
#endif
        break;

    case process:
        
        //  Process audio values and input from buttons
        stateProcess();

        // Next state is normally feedback state
        nextState = feedback;

        // But go to calibration state if calibrate button is pressed
        if (calibrateButton.getFlag() == true) {
            ESP_LOGW("Calibrate Button","Pressed, to state cal.");

            vTaskDelay(DEBOUNCE_DELAY / portTICK_PERIOD_MS); // Delay for debounce
            calibrateButton.resetFlag();
            
            nextState = calibrate;
            
            // Start the seconds timer for blinking leds
            secondTimer.start(); 
            // Run calibration audio file
            mp3Driver.play(FOLDER_CALIBRATE,CALIBRATE_FILE_STARTED); 
        }

        break;
    case feedback:

        // Give feedback
        stateFeedback();

        // Always go back to process state after feedback
        nextState = process;
        break;

    case calibrate:
        // Calibrate voice trainer
        stateCalibrate();

        // If calibration button pressed 
        // Set new threshold values and go back to process state
        if ((calibrateButton.getFlag() == true))
        {    
            vTaskDelay(DEBOUNCE_DELAY / portTICK_PERIOD_MS); // Delay for debounce
            calibrateButton.resetFlag();

            // Set new threshold values, if enough calibration samples collected
            if(calibrateQueueDb.length() > MIN_CALIBRATIONVALUES_AMOUNT){
                thres_dB = calibrateQueueDb.highest() - DB_CALIBRATION_OFFSET;
                thres_Hz = calibrateQueueHZ.lowest() + HZ_CALIBRATION_OFFSET;
            }
            ESP_LOGI("Volume cal result", "%d", thres_dB);
            ESP_LOGI("Frequency cal result", "%d", thres_Hz);

            // Clear queues for next calibration
            calibrateQueueDb.clear();
            calibrateQueueHZ.clear();

            // Run calibration end audio file
            mp3Driver.play(FOLDER_CALIBRATE,CALIBRATE_FILE_STOPPED);
            
            // Reset timer for audio feedback
            tenSecondTimer.reset(); 

            // Stop timer for blinking all leds
            secondTimer.stop();
            
            nextState = process;
        }

        break;
        
    default:
        ESP_LOGE("State error", "Undefined state");
        // Go back to process state when program ended up in an undefined state
        nextState = process;
        break;
    }

    blinkingLed.inverse(); // Inverse led to blink while active
    
    currentState = nextState;
}

void Statemachine::stateSetup(void)
{ 
    // Delay wait for devices to start up before initialising
    vTaskDelay(100 / portTICK_PERIOD_MS);

    // Init blinking led and turn it off on startup
    blinkingLed.init();
    blinkingLed.setState(false);

    // Init led driver and turn all feedback leds off on startup
    ledDriver.init();
    ledDriver.allOff();

    // Init MP3 driver, disable feedback(acknowledge) and set it to full volume
    mp3Driver.init();
    mp3Driver.enableFeedback(false);
    mp3Driver.setVolume(MP3_MAX_VOLUME); // Full volume

    // Init ten second timer used for audio feedback every 10 seconds
    tenSecondTimer.init();
    tenSecondTimer.stop(); // Timer is off on startup

    // Init one second timer used for blinking feedback leds in calibration mode
    secondTimer.init();
    secondTimer.stop(); // Timer is off on startup

    // Init buttons to change feedback mode
    volumeOnLedButton.init();
    frequencyOnLedButton.init();
    
    // Init the two other buttons for calibration and mute
    calibrateButton.init();
    muteButton.init();
    
    // Delay wait for devices to be initialised
    vTaskDelay(500 / portTICK_PERIOD_MS); 
    
    ESP_LOGI("Setup", "Completed");
}

void Statemachine::stateRelax(void)
{
    ESP_LOGI("Relaxation", "playing");

    // Only start audio file playing one time
    static bool exerciseStarted = false;
    if(exerciseStarted == false){
        exerciseStarted = true;

        // Play relaxation exercise audio file
        mp3Driver.playRandom(FOLDER_RELAX,RELAX_FILES_AMOUNT);
        
        ESP_LOGI("Relaxation", "Audio file started");
    }
    
    vTaskDelay(500 / portTICK_PERIOD_MS); // Delay to prevent reading the mp3 player too fast
}

void Statemachine::stateProcess()
{   
    // Check volume and frequency values if the data from microphone is ready
    if (dataReady == true)
    {
        // Dont add readings from speaker
        bool isplaying = mp3Driver.isPlaying();
        if (isplaying == false) {

            ESP_LOGI("Volume","%d",volume);
            // Add volume to volume queue, so average can be determined
            volumeQueue.add(volume);

            ESP_LOGI("Frequency","%d",frequency);
            // Add frequency to frequency queue, so average can be determined
            frequencyQueue.add(frequency);
        }
        dataReady = false;
    }

    // Volume-on-Leds button
    if ((volumeOnLedButton.getFlag() == true) && (feedbackState == frequencyOnLed))
    {
        vTaskDelay(DEBOUNCE_DELAY / portTICK_PERIOD_MS); // Delay for debounce
        frequencyOnLedButton.resetFlag();
        volumeOnLedButton.resetFlag();

        ESP_LOGI("Feedback state:", "Volume on Leds");
        // Feedback state is changed to volume on Leds
        feedbackState = volumeOnLed;

        // Play audiofile to indicate volume is now on Leds
        mp3Driver.playRandom(FOLDER_VOLUME_ON_LED,VOLUME_ON_LED_FILES_AMOUNT);

        // Reset timer for audio feedback
        tenSecondTimer.reset(); 
    }

    // Frequency-on-Leds button
    else if ((frequencyOnLedButton.getFlag() == true) && (feedbackState == volumeOnLed))
    {
        vTaskDelay(DEBOUNCE_DELAY / portTICK_PERIOD_MS); // Delay for debounce
        volumeOnLedButton.resetFlag();
        frequencyOnLedButton.resetFlag();

        ESP_LOGI("Feedback state:", "Frequency on Leds");
        // Feedback state is changed to frequency on Leds
        feedbackState = frequencyOnLed;

        // Play audiofile to indicate frequency is now on Leds
        mp3Driver.playRandom(FOLDER_FREQUENCY_ON_LED, FREQUENCY_ON_LED_FILES_AMOUNT);

        // Reset timer for audio feedback
        tenSecondTimer.reset(); 
    }


    // Mute button
    if (muteButton.getFlag() == true)
    {
        vTaskDelay(DEBOUNCE_DELAY / portTICK_PERIOD_MS); // Delay for debounce
        muteButton.resetFlag();

        // Invert muted state
        muted = !muted;
        ESP_LOGI("Mutebutton", "%d", muted);

        // Play the audio file for mute or unmuted
        if (muted) {
            mp3Driver.play(FOLDER_MUTE, MUTE_FILE_MUTED);
        }
        else {
            mp3Driver.play(FOLDER_MUTE, MUTE_FILE_UNMUTED);
        }

        // Reset timer for audio feedback
        tenSecondTimer.reset();
    }
}

void Statemachine::stateFeedback()
{
    // Display the visual feedback
    visualFeedback();

    // Play the audio feedback if not muted and ten seconds have passed
    if (tenSecondTimer.flag == true && muted == false)
    {
        tenSecondTimer.flag = false;
        audioFeedback();
    }
}

void Statemachine::stateCalibrate()
{
    // Invert all feedback leds every second
    // To indicate calibration is on
    static bool flash = 0;
    if(secondTimer.flag == true){
        secondTimer.flag = false;

        // Turn all leds on or off depending on flash boolean
        if (flash)  ledDriver.allOn();
        else        ledDriver.allOff();
        
        // Invert flash boolean
        flash = !flash;
    }  

    // Add values to calibration queues if the data from microphone is ready
    if (dataReady == true)
    {
        // Dont add readings from speaker
        bool isplaying = mp3Driver.isPlaying();
        if(isplaying == false){

            // Add the values to the queues
            calibrateQueueDb.add(volume);
            calibrateQueueHZ.add(frequency);
            
            ESP_LOGI("Calibrate queue", "add %d dB and %d Hz",calibrateQueueDb.latest(),calibrateQueueHZ.latest());
        }
        dataReady = false;
    }
}

void Statemachine::visualFeedback()
{
    uint8_t ledLevel = 0;

    // Calculate Led level depending on the feedback state
    if (feedbackState == volumeOnLed)
    {
        // Calculate the amount of Leds to be turned on for volume
        float fractionVolume = (float)(constrain(volumeQueue.latest(), DB_THRES_MIN, thres_dB) - DB_THRES_MIN) / (float)(thres_dB - DB_THRES_MIN);
        ledLevel = constrain(fractionVolume * LED_AMOUNT, 0, LED_AMOUNT);
    }
    else if (feedbackState == frequencyOnLed)
    {
        // Calculate the amount of Leds to be turned on for frequency
        float fractionFrequency = (float)(HZ_THRES_MAX - (constrain(frequencyQueue.latest(), thres_Hz, HZ_THRES_MAX))) / (float)(HZ_THRES_MAX - thres_Hz);
        ledLevel = constrain(fractionFrequency * LED_AMOUNT, 0, LED_AMOUNT);
    }
    // Set level on Leds
    ledDriver.setLevel(ledLevel+1); // +1 to always have at least one led on

}

void Statemachine::audioFeedback()
{
    ESP_LOGI("Average Volume", "%lf", volumeQueue.average());
    ESP_LOGI("Average Frequency", "%lf", frequencyQueue.average());

    switch(feedbackState){
    // Play Volume feedback when frequency is on Leds
    case frequencyOnLed:
        // Feedback is positive when average volume is on threshold or higher
        if (volumeQueue.average() >= thres_dB) {
            mp3Driver.playRandom(FOLDER_VOLUME_GOOD, VOLUME_GOOD_FILES_AMOUNT);
            ESP_LOGI("FEEDBACK", "VOLUME_GOOD");
        } 
        else {
            mp3Driver.playRandom(FOLDER_VOLUME_HIGHER, VOLUME_HIGHER_FILES_AMOUNT);
            ESP_LOGI("FEEDBACK", "VOLUME_HIGHER");
        }
        break;
        
    // Play Frequency feedback when volume is on Leds
    case volumeOnLed:
        // Feedback is positive when average frequency is on threshold or lower
        if (frequencyQueue.average() <= thres_Hz) {
            mp3Driver.playRandom(FOLDER_FREQUENCY_GOOD, FREQUENCY_GOOD_FILES_AMOUNT);
            ESP_LOGI("FEEDBACK", "FREQUENCY_GOOD");
        } 
        else {
            mp3Driver.playRandom(FOLDER_FREQUENCY_LOWER, FREQUENCY_LOWER_FILES_AMOUNT);
            ESP_LOGI("FEEDBACK", "FREQUENCY_LOWER");
        }
        break;
    }
}

void Statemachine::setValues(int frequency_, int volume_) {
    // Copy values
    frequency = frequency_;
    volume = volume_;
    
    // Data is ready to be used
    dataReady = true;
}