
/**
 * @file uart.cpp
 * @author Raymon Albricht & Tristan Bosveld
 * @brief Microphone module used to interface with ESP32's I2S components.
 * @version 0.1
 * @date 2022-04-25
 * 
 * @copyright Fully open-source
 * 
 */

//Standard library
#include <vector>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include "esp_rom_gpio.h"
#include "driver/gpio.h"

//ESP-IDF library
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"

#include "uart.hpp"
#include "driver/uart.h"

using namespace std;

static const int RX_BUF_SIZE = 1024;

#define TXD_PIN (GPIO_NUM_1)
#define RXD_PIN (GPIO_NUM_3)

#define UART UART_NUM_0
int num = 0;

void _uartInit(void)
{           
 
    /* Configure parameters of an UART driver, communication pins and install the driver */
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    uart_driver_install(UART, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART, &uart_config);
    uart_set_pin(UART, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}
void datatransfer::transferdata(int value, string valuetype) {
	char* Txdata = (char*) malloc(100);
    sprintf(Txdata,"%d %s\r\n", value, valuetype.c_str());
    uart_write_bytes(UART, Txdata, strlen(Txdata));
    // while (1) {
    // 	sprintf (Txdata, "Hello world index = %d\r\n", num++);
    //     uart_write_bytes(UART, Txdata, strlen(Txdata));
    //     vTaskDelay(2000 / portTICK_PERIOD_MS);
    // }
}

void datatransfer::transfercsv(int value_db, int value_hz, int value_db_av, int value_hz_av) {
    char* Txdata = (char*) malloc(100);
    sprintf(Txdata,"Db:%d,\taverage:%d\nHz:%d,\taverage:%d\r\n", value_db, value_db_av, value_hz, value_hz_av);
    uart_write_bytes(UART, Txdata, strlen(Txdata));
}

void datatransfer::uartTest(int value) {
    char* Txdata = (char*) malloc(100);
    sprintf(Txdata,"Testing value: %d\n", value);
    uart_write_bytes(UART, Txdata, strlen(Txdata));
}