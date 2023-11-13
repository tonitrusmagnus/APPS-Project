/**
 * @file microphone_i2s.cpp
 * @author Jan Sevrin
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

//ESP-IDF library
#include "driver/i2s.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"

//Main header
#include "microphone_i2s.hpp"

esp_err_t Microphone::install_driver() const
{
	i2s_config_t i2s_config = {
		.mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_RX),
		.sample_rate = sample_rate,
		.bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
		.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT, // Ground the L/R pin on the microphone.
		.communication_format = I2S_COMM_FORMAT_STAND_I2S,
		.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
		.dma_buf_count = 2,
		.dma_buf_len = buffer_size,
		.use_apll = false,
		.tx_desc_auto_clear = false,
		.fixed_mclk = 0,
	};
	return i2s_driver_install(port_num, &i2s_config, 0, NULL);
}

esp_err_t Microphone::set_pins() const
{
	i2s_pin_config_t pin_config = {
		.mck_io_num = -1,
		.bck_io_num = bck_io_num,
		.ws_io_num = ws_io_num,
		.data_out_num = -1,
		.data_in_num = data_in_num,
	};
	return i2s_set_pin(port_num, &pin_config);
}

esp_err_t Microphone::read_full(std::vector<float> &vec)
{
	const size_t N = vec.size();
	size_t vec_count = N;
	while(vec_count)
	{
		size_t bytesRead = 0;
		esp_err_t err = i2s_read(port_num, &buffer32, sizeof(buffer32), &bytesRead, 100);
		if(err != ESP_OK) return err;
		size_t samplesRead = bytesRead / 4;
		for (int i=0; i<samplesRead; ++i) {
			uint8_t useless = buffer32[i * 4 + 0]; //first byte isn't usable data.
			uint8_t lsb = buffer32[i * 4 + 1]; //second byte is too noisy.
			uint8_t mid = buffer32[i * 4 + 2];
			uint8_t msb = buffer32[i * 4 + 3];
			uint16_t raw = (((uint16_t)msb) << 8) + ((uint16_t)mid << 0);
			vec[i + N - vec_count] = static_cast<int16_t>(raw) * 1.0f; //Not sure if this double cast is ideal
		}

		vec_count -= samplesRead;
	}

	return ESP_OK;
}

