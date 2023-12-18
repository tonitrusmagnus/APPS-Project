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

bool MP3Driver::init() {
    // Configure parameters of an UART driver, communication pins and install the driver
    const uart_config_t uart_config = {
        .baud_rate = MP3_UART_BAUD,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    const uart_intr_config_t uart_intr_conf = {
        .intr_enable_mask = UART_INTR_RXFIFO_FULL,
        .rxfifo_full_thresh = MP3_UART_FRAME_SIZE,
        
    };

    ESP_ERROR_CHECK(uart_intr_config(UartNum, &uart_intr_conf));

    ESP_ERROR_CHECK(uart_driver_install(UartNum, RX_BUF_SIZE * 2, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UartNum, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UartNum, TxPin, RxPin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

	// enable RX interrupt
	ESP_ERROR_CHECK(uart_enable_rx_intr(UartNum));

    return true;
}

void MP3Driver::play(char folderNr, char trackNr) {
    
    sendData(MP3_PLAY_FOLDER, folderNr, trackNr);
    playing = true;
}

void MP3Driver::playRandom(char folderNr, char amount) {

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

    playing = false;
    return false;
}

int MP3Driver::getFeedback(char command) {
    sendData(command, 0, 0); // No parameters needed when 

    char buffer[MP3_UART_FRAME_SIZE];

    uart_flush(UartNum);

    if (uart_read_bytes(UartNum,buffer,MP3_UART_FRAME_SIZE,readTimeout) < MP3_UART_FRAME_SIZE) {
        ESP_LOGW("UART", "Timeout");
        return -1;
    }

    if (buffer[3] == command) {
        int result = buffer[5] << 8 | buffer[6];
        return result;
    }

    return -1;
}

void MP3Driver::stop() {
    sendData(MP3_STOP_PLAYBACK, 0, 0);
    playing = false;
}

void MP3Driver::setVolume(unsigned int volume) {
    volume = constrain(volume, 0, 30);
    sendData(MP3_SET_VOL, 0, volume);
}

void MP3Driver::repeatPlay(bool enable) {
    sendData(MP3_LOOP_CURRENT_TRACK, 0, !enable);
}

void MP3Driver::enableFeedback(bool feedbackEnabled_) {
    feedbackEnabled = feedbackEnabled_;
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

    int out = uart_write_bytes(UartNum, buffer, sizeof(buffer));
    if (out == -1) ESP_LOGE("UART","Parameter error");

    vTaskDelay(50 / portTICK_PERIOD_MS);
}