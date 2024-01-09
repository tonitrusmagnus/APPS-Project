// Standard libraries
#include <stdio.h>

// ESP-IDF library
#include "esp_err.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "Apps_main.hpp"
#include "helper_functions/defines.hpp"

enum {CORE_PROGRAM = 0, CORE_APP = 1, CORE_APP2 = 2};

extern "C"
{
    void app_main(void)
    {
        esp_err_t err = nvs_flash_init();
		if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
			ESP_ERROR_CHECK(nvs_flash_erase());
			err = nvs_flash_init();
		}
		ESP_ERROR_CHECK(err);
		ESP_LOGI("Start","running..");

		// Create tasks to run on 2 ESP cores
		xTaskCreatePinnedToCore(task_process_audio, 	"process audio", 	TASK_STACK_SIZE , NULL, 1, NULL, CORE_PROGRAM);
		xTaskCreatePinnedToCore(task_read_audio, 		"read audio", 		TASK_STACK_SIZE , NULL, 1, NULL, CORE_APP);
		xTaskCreate(task_run_statemachine, 	"statemachine",	TASK_STACK_SIZE , NULL, 1, NULL);
    }
	
}
