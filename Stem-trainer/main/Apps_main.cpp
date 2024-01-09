/**
 * @file Apps_main.cpp
 * @author Raymon Albricht, Tristan Bosveld, Robin van Geemen, Mark de Gast
 * @brief
 * @version 0.1
 * @date 30-05-2023
 *
 * @copyright Fully open-source
 *
 */
#include "Apps_main.hpp"

// Standard library
#include <vector>
#include <deque>
#include <string>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ESP-IDF includes
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"

#include "microphone/microphone_i2s.hpp"
#include "microphone/signalprocessing.hpp"
#include "statemachine.hpp"
#include "helper_functions/defines.hpp"

// Global values
int volumeValue = 0;
int frequencyValue = 0;
bool dataReady = false;


static constexpr bool is_powerof2(size_t size)
{
	return size && ((size & (size - 1)) == 0);
}

enum class AudioStatus
{
	WAIT,
	VEC1_READY,
	VEC2_READY
};
AudioStatus audio_status = AudioStatus::WAIT;

namespace dsp
{
	void remove_dc(std::vector<float> &vec);
	void hamming_windows(std::vector<float> &vec);
	void custom_logarithm_abs(std::vector<float> &vec);
	void fouriertransf(std::vector<float> &vec);
	void inversefouriertransf(std::vector<float> &vec);
}

// read_audio() globals
Microphone inmp441(I2S_NUM_0, INMP441_BCK, INMP441_WS, INMP441_DATA);
constexpr size_t SAMPLES_LENGTH = Microphone::buffer_size * 4;
std::vector<float> audio_vec1(SAMPLES_LENGTH);
std::vector<float> audio_vec2(SAMPLES_LENGTH);
static_assert(SAMPLES_LENGTH, "SAMPLES_LENGTH, which determines vector size, must be a power of 2!");
std::vector<float> audiodata;
Statemachine statemachine;

void process_audio(std::vector<float> &audio)
{
	static constexpr size_t FREQ_PEAKS_MAX_LENGTH = 1.0f * Microphone::sample_rate / SAMPLES_LENGTH * 2; //2 seconds
	static constexpr unsigned int min_fundamental_frequency = 70;
	static constexpr unsigned int max_fundamental_frequency = 200; //frequency range of a male voice is between 70 and 200 hz
	static std::deque<float> frequency_peaks;
	static dsp::signal_point peak;
	
	dsp::remove_dc(audio);
	dsp::hamming_windows(audio);

	const size_t min_pos = Microphone::sample_rate / max_fundamental_frequency;
    const size_t max_pos = Microphone::sample_rate / min_fundamental_frequency;
    peak = dsp::get_peak(audio, min_pos, max_pos + 1, Microphone::sample_rate);

	int Decibel = 20 * log10(peak.amplitude / 0.8);

	dsp::fouriertransf(audio);
	dsp::custom_logarithm_abs(audio);
	dsp::inversefouriertransf(audio);

    peak = dsp::get_peak(audio, min_pos, max_pos + 1, Microphone::sample_rate);

	if(peak.amplitude > MIN_PEAK_AMPLITUDE){
		volumeValue = Decibel;			 // Final usable volume value
		frequencyValue = peak.frequency; // Final usable frequency value

		// Send data to statemachine to be used
		statemachine.setValues(frequencyValue, volumeValue);
	}
}


void task_read_audio(void *params)
{
	while (true) {
		// Read 2 audio vectors 
		if (inmp441.read_full(audio_vec1) == ESP_OK) {
			audio_status = AudioStatus::VEC1_READY;
		}
		if (inmp441.read_full(audio_vec2) == ESP_OK) {
			audio_status = AudioStatus::VEC2_READY;
		}
	}
}

void task_process_audio(void *params) {
	
	while (true) 	{
		switch (audio_status) {
		case AudioStatus::VEC1_READY:
			audio_status = AudioStatus::WAIT;
			process_audio(audio_vec1);
			break;
		case AudioStatus::VEC2_READY:
			audio_status = AudioStatus::WAIT;
			process_audio(audio_vec2);
			break;
		default:
			vTaskDelay(10 / portTICK_PERIOD_MS); // Wait 10ms until audio is ready for processing.
			break;
		}
	}
}

void task_run_statemachine(void *params) {
	while (true) {
		statemachine.run();	
		
    	vTaskDelay(10 / portTICK_PERIOD_MS); // Wait 10ms to enable the FreeRTOS idle background tasks to run
	}
}
