#ifndef TIMER_HPP
#define TIMER_HPP

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_timer.h"
#include <esp_system.h>
#include "esp_err.h"
#include "esp_log.h"

void timer_init();
void timer_isr(void* arg);
extern bool sec;
extern bool skipbool;

#endif