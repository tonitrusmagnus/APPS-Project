// MP3 Driver 
// By Robin van Geemen
//
// Play sounds using MP3-TF-16P, communicating via UART

#include "mp3_driver.hpp"

#include "helper_functions/helper_functions.hpp"

static const int RX_BUF_SIZE = 1024;

MP3Driver::MP3Driver(gpio_num_t TxPin_, gpio_num_t RxPin_, uart_port_t UartNum_, int readTimeout_) {
    TxPin = TxPin_;
    RxPin = RxPin_;
    UartNum = UartNum_;
    readTimeout = readTimeout_;
}

void MP3Driver::init() {
    // Configure parameters of an UART driver, communication pins and install the driver
    const uart_config_t uart_config = {
        .baud_rate = MP3_UART_BAUD,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    // Install driver for UART and set configuration
    ESP_ERROR_CHECK(uart_driver_install(UartNum, RX_BUF_SIZE * 2, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UartNum, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UartNum, TxPin, RxPin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
}

void MP3Driver::play(char folderNr, char trackNr) {
    // Send folder number and tracknumber to play to MP3 player
    sendData(MP3_PLAY_FOLDER, folderNr, trackNr);

    // Set current playing state to playing is true
    playing = true;
}

void MP3Driver::playRandom(char folderNr, char amount) {
    // Play a random track from specified folder
    play(folderNr,randomNumber(1,amount));
}

bool MP3Driver::isPlaying() {
    // Read status
    int status = getFeedback(MP3_GET_STATUS);

    if (status == -1) {   // If error reading feedback
        return playing;   // Return previous playing state
    }
    else if (status == MP3_STATUS_PLAYING) {
        playing = true;
        return true;
    }
    // If status not returned that it is playing
    // Set playing to false
    playing = false;
    return false;
}

void MP3Driver::stop() {
    sendData(MP3_STOP_PLAYBACK, 0, 0); // Stop playback has no parameters 

    // Set current playing state to playing is false
    playing = false;
}

void MP3Driver::setVolume(unsigned int volume) {
    // Constrain and then send volume to MP3 Player
    volume = constrain(volume, 0, MP3_MAX_VOLUME);
    sendData(MP3_SET_VOL, 0, volume); 
}

void MP3Driver::enableFeedback(bool feedbackEnabled_) {
    // Set feedback variable so it can be used when sending data
    feedbackEnabled = feedbackEnabled_; 
}

int MP3Driver::getFeedback(char command) {
    char buffer[MP3_UART_FRAME_SIZE];

    // Send query command
    sendData(command, 0, 0); // No parameters needed when sending query

    // Clear uart buffer before reading
    uart_flush(UartNum);
     
    // Read uart and check if recieved the right amount of bytes
    int res = uart_read_bytes(UartNum, buffer, MP3_UART_FRAME_SIZE, readTimeout);
    if (res < MP3_UART_FRAME_SIZE) {
        ESP_LOGW("UART", "Error");
        return -1;
    }

    // Check if the feedback has the same command as queried
    if (buffer[MP3_BUFFER_COMMAND] == command) {
        // Return the result of the answer
        int result = buffer[MP3_BUFFER_HIGH_DATA] << 8 | buffer[MP3_BUFFER_LOW_DATA];
        return result;
    }

    return -1;
}

void MP3Driver::sendData(char command, char dataMSB, char dataLSB)
{
    char buffer[8];
    buffer[0] = MP3_UART_START_BYTE;
    buffer[1] = MP3_UART_VERSION;
    buffer[2] = MP3_UART_DATA_LEN;
    buffer[3] = command;
    buffer[4] = (char)feedbackEnabled;
    buffer[5] = dataMSB;
    buffer[6] = dataLSB;
    buffer[7] = MP3_UART_END_BYTE;

    // Send bytes to MP3 player
    int out = uart_write_bytes(UartNum, buffer, sizeof(buffer));
    // uart_write_bytes returns -1 at a parameter error
    if (out == -1) ESP_LOGE("UART","Parameter error");

    vTaskDelay(50 / portTICK_PERIOD_MS);
}