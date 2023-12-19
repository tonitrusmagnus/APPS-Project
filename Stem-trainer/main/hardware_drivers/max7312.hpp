#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_err.h"

#define I2C_CLK_FREQUENCY               400000
#define I2C_MASTER_RX_BUF_LEN           512
#define I2C_MASTER_TX_BUF_LEN           512
#define I2C_TIMEOUT                     20000

#define MAX7312_SEND_TIMEOUT 3 // Amount of times to send data on error before timeout

#define MAX7312_ADDRESS 0x20 // When AD2, AD1 and AD0 are connected to GND (Check datasheet for different addresses)

#define MAX7312_CMD_INPUT_PORT1         0x00
#define MAX7312_CMD_INPUT_PORT2         0x01
#define MAX7312_CMD_OUTPUT_PORT1        0x02
#define MAX7312_CMD_OUTPUT_PORT2        0x03
#define MAX7312_CMD_POLARITY_INV_PORT1  0x04
#define MAX7312_CMD_POLARITY_INV_PORT2  0x05
#define MAX7312_CMD_CONFIG_PORT1        0x06
#define MAX7312_CMD_CONFIG_PORT2        0x07
#define MAX7312_CMD_TIMEOUT_REG         0x08

class Max7312
{
private:

    i2c_port_t i2c_port;
    gpio_num_t sdaPin;
    gpio_num_t sclPin;

public:
    Max7312(gpio_num_t sdaPin_, gpio_num_t sclPin_, i2c_port_t i2c_port_);

    void init();
    void set(uint16_t data);
};
