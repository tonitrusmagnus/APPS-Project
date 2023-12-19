// Standard libraries
#include <stdio.h>

// ESP-IDF library
#include "esp_err.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "Apps_main.hpp"
#include "software_drivers/uart.hpp"
#include "esp_timer.h"

enum {CORE_PROGRAM = 0, CORE_APP = 1, CORE_APP2 = 2};

extern "C"
{
	void debug_task(void *params);
    void app_main(void)
    {
        esp_err_t err = nvs_flash_init();
		if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
			ESP_ERROR_CHECK(nvs_flash_erase());
			err = nvs_flash_init();
		}
		ESP_ERROR_CHECK(err);
		//_uartInit();
		ESP_LOGI("Start","running..");

		xTaskCreatePinnedToCore(task_process_audio, 	"process audio", 	8192 , NULL, 1, NULL, CORE_PROGRAM);
		xTaskCreatePinnedToCore(task_read_audio, 		"read audio", 		8192 , NULL, 1, NULL, CORE_APP);
		xTaskCreate(task_run_statemachine, 	"statemachine",	8192 , NULL, 1, NULL);
		// xTaskCreatePinnedToCore(task_run_statemachine, 	"run statemachine",	2048, NULL, 1, NULL, CORE_APP2);
    }
	
}

