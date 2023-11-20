#ifndef MP3_DRIVER_HPP
#define MP3_DRIVER_HPP


#include "driver/uart.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include "helper_functions/defines.hpp"

/* UART frame values */
#define MP3_UART_FRAME_SIZE      0x0A //total number of bytes in UART packet, same for cmd & feedback
#define MP3_UART_START_BYTE      0x7E //start byte
#define MP3_UART_VERSION         0xFF //protocol version
#define MP3_UART_DATA_LEN        0x06 //number of data bytes, except start byte, checksum & end byte
#define MP3_UART_END_BYTE        0xEF //end byte

/* command controls */
#define MP3_PLAY_NEXT            0x01 //play next uploaded file
#define MP3_PLAY_PREV            0x02 //play prev uploaded file
#define MP3_PLAY_TRACK           0x03 //play tracks in chronological order, by upload time
#define MP3_SET_VOL_UP           0x04 //increment volume by 1
#define MP3_SET_VOL_DOWN         0x05 //decrement volume by 1
#define MP3_SET_VOL              0x06 //set volume, range 0..30
#define MP3_SET_EQ               0x07 //0=Off, 1=Pop, 2=Rock, 3=Jazz, 4=Classic, 5=Bass (may not be supported by some modules)
#define MP3_LOOP_TRACK           0x08 //playing & looping track number 0001..9999
#define MP3_SET_PLAY_SRC         0x09 //1=USB-Disk, 2=TF-Card, 3=Aux, 4=sleep(YX5200)/NOR-Flash(GD3200), 5=NOR-Flash, 6=Sleep (3..6 may not be supported by some modules)
#define MP3_SET_STANDBY_MODE     0x0A //put player in standby mode, not the same as sleep mode
#define MP3_SET_NORMAL_MODE      0x0B //pull player out of standby mode (may not be supported by some modules)
#define MP3_RESET                0x0C //reset module, set all settings to factory default
#define MP3_RESUME_PLAYBACK      0x0D //resume playing current track
#define MP3_PAUSE                0x0E //pause playing current track
#define MP3_PLAY_FOLDER          0x0F //play track number 1..255 from folder number 1..99
#define MP3_SET_DAC_GAIN         0x10 //set DAC output gain/output voltage swing (may not be supported by some modules)
#define MP3_REPEAT_ALL           0x11 //repeat playback all files in chronological order
#define MP3_PLAY_MP3_FOLDER      0x12 //play track number 0001..9999 from "mp3" folder 
#define MP3_PLAY_ADVERT_FOLDER   0x13 //interrupt current track & play track number 0001..9999 from "advert" folder, than resume current track
#define MP3_PLAY_3000_FOLDER     0x14 //play track number 0001..3000 from folder that supports 3000 tracks (may not be supported by some modules)
#define MP3_STOP_ADVERT_FOLDER   0x15 //stop interrupting current track while playing track from "advert" folder
#define MP3_STOP_PLAYBACK        0x16 //stop playing current track
#define MP3_REPEAT_FOLDER        0x17 //repeat playback folder number 01..99
#define MP3_RANDOM_ALL_FILES     0x18 //play all tracks in random order
#define MP3_LOOP_CURRENT_TRACK   0x19 //loop currently played track
#define MP3_SET_DAC              0x1A //enable/disable DAC (mute/unmute), 0=enable, 1=disable
#define MP3_PLAY_ADVERT_FOLDER_N 0x25 //interrupt current track & play track number 001..255 from "advert1".."advert9" folder, than resume current track (may not be supported by some modules)

/* request command controls */
#define MP3_GET_STATUS           0x42 //get current stutus
#define MP3_GET_VOL              0x43 //get current volume, range 0..30
#define MP3_GET_EQ               0x44 //get current EQ, 0=Off, 1=Pop, 2=Rock, 3=Jazz, 4=Classic, 5=Bass (may not be supported by some modules)
#define MP3_GET_PLAY_MODE        0x45 //get current loop mode, 0=loop all, 1=loop folder, 2=loop track, 3=random, 4=disable (may not be supported by some modules)
#define MP3_GET_VERSION          0x46 //get software version
#define MP3_GET_QNT_USB_FILES    0x47 //get total number of tracks on USB-Disk
#define MP3_GET_QNT_TF_FILES     0x48 //get total number of tracks on TF-card
#define MP3_GET_QNT_FLASH_FILES  0x49 //get total number of tracks on NOR-Flash (may not be supported by some modules)
#define MP3_GET_USB_TRACK        0x4B //get currently playing track number on USB-Disk
#define MP3_GET_TF_TRACK         0x4C //get currently playing track number on TF-card
#define MP3_GET_FLASH_TRACK      0x4D //get currently playing track number on NOR-Flash (may not be supported by some modules)
#define MP3_GET_QNT_FOLDER_FILES 0x4E //get total number of tracks in folder
#define MP3_GET_QNT_FOLDERS      0x4F //get total number of folders in current source (may not be supported by some modules)

/* module returned codes at the end of any playback operation or if any command error, located in 4-th RX byte */
#define MP3_RETURN_CODE_OK_ACK   0x41 //OK, command is accepted (returned only if ACK/feedback byte is set to 0x01)
#define MP3_RETURN_ERROR         0x40 //error, module return this status automatically if command is not accepted (details located in 7-th RX byte)
#define MP3_RETURN_CODE_DONE     0x3D //track playback is is completed, module return this status automatically after the track has been played
#define MP3_RETURN_CODE_READY    0x3F //ready after boot or reset, module return this status automatically after boot or reset

// Feedback
#define MP3_FEEDBACK_ENABLED     0x01 // 
#define MP3_FEEDBACK_DISABLED    0x00 // 

// Status
#define MP3_STATUS_STOP             0x0200
#define MP3_STATUS_PLAYING          0x0201
#define MP3_STATUS_SLEEP_STANDBY    0x0202
#define MP3_STATUS_ERROR            0x0002

#define FEEDBACK_BYTE_AMOUNT     10
#define FEEDBACK_COMMAND_POS     4
#define FEEDBACK_DATA_POS        7

class MP3Driver {
public:
    MP3Driver();
    ~MP3Driver();

    // initialisers
    bool init(gpio_num_t TxPin, gpio_num_t RxPin, uart_port_t UartNum, int readTimeout_);
    void play(char folderNr, char trackNr);
    void playRandom(char folderNr, char amount);
    void stop();
    void setVolume(unsigned int volume); // 0 to 30
    void repeatPlay(bool enable);
    void enableFeedback(bool feedbackEnabled_);
    bool isPlaying();
private:
    uart_port_t UartNum;
    int readTimeout;
    bool feedbackEnabled = true;
    //bool readFeedback(char *command, char * data);
    int getFeedback(char command);
    void sendData(char command, char dataMSB, char dataLSB);
};

#endif //MP3_DRIVER_HPP