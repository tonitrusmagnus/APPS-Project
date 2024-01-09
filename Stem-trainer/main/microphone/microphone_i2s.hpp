#ifndef MICROPHONE_I2S_HPP
#define MICROPHONE_I2S_HPP
/**
 * @file microphone_i2s.hpp
 * @author Jan Sevrin
 * @brief Microphone module used to interface with ESP32's I2S components.
 * @version 0.1
 * @date 2022-04-25
 * 
 * @copyright Fully open-source
 * 
 */

#pragma once

//Standard library
#include <vector>
#include <cstdint>

//ESP-IDF library
#include "driver/i2s.h"

//ESP-IDF library
#include "driver/i2s.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"

/**
 * @class Microphone
 * @brief An interface to ESP32's I2S specifically for microphone usage.
 * 
 */
class Microphone{
public:
	static constexpr size_t buffer_size = 256;
	static constexpr int sample_rate = 16000;
private:
	static constexpr char TAG[] = "Microphone";
	i2s_port_t port_num;
	int bck_io_num;
	int ws_io_num; 
	int data_in_num;
	uint8_t buffer32[buffer_size * 4] = {0};

	/**
	 * @brief Try to install an I2S driver via ESP32's i2s_driver_install(), restart the ESP if not succesful.
	 * 
	 * @return esp_err_t Output. Used for error handling.
	 */
	esp_err_t install_driver() const;
	/**
	 * @brief Try to set I2S pins via ESP32's i2s_set_pin(), restart the ESP if not succesful.
	 * 
	 * @return esp_err_t Output. Used for error handling.
	 */
	esp_err_t set_pins() const;
public:
	Microphone(
		const i2s_port_t port,
		const int bck,
		const int ws,
		const int data)
		: port_num(port), bck_io_num(bck), ws_io_num(ws), data_in_num(data)
	{ 
		esp_err_t err;

		err = install_driver();
		if(err != ESP_OK)
		{
			ESP_LOGE(TAG, "Installing driver failed.");
			esp_restart();
		}

		err = set_pins();
		if(err != ESP_OK)
		{
			ESP_LOGE(TAG, "Setting pins failed.");
			esp_restart();
		}
		ESP_LOGI(TAG, "Initialized.");
	}

	/**
	 * @brief Attempt to read audio.
	 * 
	 * @param vec Output.
	 * @return esp_err_t Output. Used for error handling.
	 */
	esp_err_t read_full(std::vector<float> &vec);
};
 
#endif //MICROPHONE_I2S_HPP