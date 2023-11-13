// MP3 Driver 
// By Robin van Geemen
//
// Play sounds using MP3-TF-16P, communicating via UART1
// MP3 files should be in a folder 'MP3' and numbered
// (Example: 'MP3/0001.mp3')
// To play track, run playMP3('track number')

#include "mp3_driver.hpp"


static const int RX_BUF_SIZE = 1024;


//int num = 2;

MP3Driver::MP3Driver(){
    // Constructor here
}

MP3Driver::~MP3Driver() {
    // Clean up any resources if needed
}

bool MP3Driver::init(gpio_num_t TxPin, gpio_num_t RxPin, uart_port_t UartNum_) {
    UartNum = UartNum_;

    /* Configure parameters of an UART driver, communication pins and install the driver */
    const uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    uart_driver_install(UartNum, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UartNum, &uart_config);
    uart_set_pin(UartNum, TxPin, RxPin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    return true;
}

void MP3Driver::play(char folderNr, char trackNr) {
    
    sendData(MP3_PLAY_TRACK, folderNr, trackNr);
}

void MP3Driver::stopMP3() {
    sendData(MP3_STOP_PLAYBACK, 0, 0);
}

void MP3Driver::setVolume(unsigned int volume) {
    volume = (volume > 30) ? 30 : volume; // constrain to max 30
    sendData(MP3_SET_VOL, 0, volume);
}

void MP3Driver::repeatPlay(bool enable) {
    sendData(MP3_LOOP_CURRENT_TRACK, 0, !enable);
}

void MP3Driver::sendData(char command, char dataMSB, char dataLSB)
{
    char buffer[8];
    buffer[0] = MP3_UART_START_BYTE;
    buffer[1] = MP3_UART_VERSION;
    buffer[2] = MP3_UART_DATA_LEN;
    buffer[3] = command;
    buffer[4] = MP3_ACK_RETURN;
    buffer[5] = dataMSB;
    buffer[6] = dataLSB;
    buffer[7] = MP3_UART_END_BYTE;

    uart_write_bytes(UartNum, buffer, 8);

    vTaskDelay(50 / portTICK_PERIOD_MS);
}