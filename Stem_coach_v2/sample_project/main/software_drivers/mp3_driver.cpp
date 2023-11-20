// MP3 Driver 
// By Robin van Geemen
//
// Play sounds using MP3-TF-16P, communicating via UART

#include "mp3_driver.hpp"
#include "helper_functions/helper_functions.hpp"


static const int RX_BUF_SIZE = 1024;


MP3Driver::MP3Driver(){
    // Constructor here
}

MP3Driver::~MP3Driver() {
    // Clean up any resources if needed
}

bool MP3Driver::init(gpio_num_t TxPin, gpio_num_t RxPin, uart_port_t UartNum_, int readTimeout_) {
    UartNum = UartNum_;
    readTimeout = readTimeout_;

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
    
    sendData(MP3_PLAY_FOLDER, folderNr, trackNr);
}

void MP3Driver::playRandom(char folderNr, char amount) {

    play(folderNr,randomNumber(1,amount));
}

bool MP3Driver::isPlaying() {
    if (getFeedback(MP3_GET_STATUS) == MP3_STATUS_PLAYING) return true;
    return false;
}

int MP3Driver::getFeedback(char command) {
    sendData(command, 0, 0); // No parameters needed when 

    char buffer[MP3_UART_FRAME_SIZE];

    uart_flush(UartNum);

    if (uart_read_bytes(UartNum,buffer,MP3_UART_FRAME_SIZE,readTimeout) < MP3_UART_FRAME_SIZE) return -1;
    
    if (buffer[3] == command) {
        int result = buffer[5] << 8 | buffer[6];
        return result;
    }

    return -1;
}

// bool MP3Driver::readFeedback(char *command, char * data){
//     char readData[25];
//     //uart_read_bytes(UARTMP3,readData,)
//     char startbyte;
//     int err;
//     size_t bufferlength;
//     uart_get_buffered_data_len(UartNum,&bufferlength);
//     if(bufferlength > 7){
//         do {
//             err = uart_read_bytes(UartNum,&readData[0],1,readTimeout);
//             if (err == -1) {
//                 return false;
//             }

//         } while (startbyte != MP3_UART_START_BYTE);
        
//         uart_read_bytes(UartNum,&readData[1],FEEDBACK_BYTE_AMOUNT-1,readTimeout);
//         if (readData[9] != MP3_UART_END_BYTE) return false;    
//         *command = readData[FEEDBACK_COMMAND_POS];
//         *data = readData[FEEDBACK_DATA_POS];
//         return true;
//     }
//     return false;
// }

void MP3Driver::stop() {
    sendData(MP3_STOP_PLAYBACK, 0, 0);
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

    uart_write_bytes(UartNum, buffer, 8);

    vTaskDelay(50 / portTICK_PERIOD_MS);
}