#include "max7312.hpp"

Max7312::Max7312(gpio_num_t sdaPin_, gpio_num_t sclPin_, i2c_port_t i2c_port_)
{
    i2c_port = i2c_port_;
    sdaPin = sdaPin_;
    sclPin = sclPin_;
}


void Max7312::init() {

    i2c_config_t conf_master;
    conf_master.mode = I2C_MODE_MASTER;
    conf_master.sda_io_num = sdaPin;
    conf_master.scl_io_num = sclPin;
    conf_master.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf_master.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf_master.master.clk_speed = I2C_CLK_FREQUENCY;
    conf_master.clk_flags = 0;
    
    ESP_ERROR_CHECK(i2c_param_config(i2c_port, &conf_master));
    ESP_ERROR_CHECK(i2c_driver_install(i2c_port, conf_master.mode, I2C_MASTER_RX_BUF_LEN, I2C_MASTER_TX_BUF_LEN, 0));

    uint8_t buffer[3] = {
        MAX7312_CMD_CONFIG_PORT2,
        0x00,
        0x00}; //set all ports to output
    ESP_ERROR_CHECK(i2c_master_write_to_device(i2c_port, MAX7312_ADDRESS, buffer, sizeof(buffer), I2C_TIMEOUT));

}

void Max7312::set(uint16_t data){

    uint8_t buffer[3];
    buffer[0] = MAX7312_CMD_OUTPUT_PORT2;
    buffer[1] = (uint8_t)(data>>8); // Highest data bits
    buffer[2] = (uint8_t)(data);    // Lowest data bits

    int err = 0;
    uint16_t timeout = MAX7312_SEND_TIMEOUT;
    do {
        // In case of timeout, break the loop
        if (timeout-- <= 0) {
            break;
        }

        err = i2c_master_write_to_device(i2c_port, MAX7312_ADDRESS, buffer, sizeof(buffer), I2C_TIMEOUT);
        ESP_ERROR_CHECK_WITHOUT_ABORT(err);

        vTaskDelay(100 / portTICK_PERIOD_MS);
    } while (err != 0);
    
}