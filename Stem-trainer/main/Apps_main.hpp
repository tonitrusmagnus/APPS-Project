#ifndef APPS_MAIN_HPP
#define APPS_MAIN_HPP
/**
 * @file Apps_main.hpp
 * @author Raymon Albricht, Tristan Bosveld
 * @brief
 * @version 0.1
 * @date 30-05-2023
 *
 * @copyright Fully open-source
 *
 */
#include <stdio.h>

//Standard library
#include <vector>
#include <deque>
#include <string>
#include <queue>

#define QUEUE_MAX_SIZE 6
#define MIN_PEAK_AMPLITUDE 0.13f
/**
 * @brief Reads audio data and updates the global variables accordingly.
 * @param params Unused parameter.
 */
void task_read_audio(void* params);

/**
 * @brief Processes audio data and performs necessary computations.
 * @param params Unused parameter.
 */
void task_process_audio(void *params);

/**
 * @brief Runs the main state machine logic.
 * @param params Unused parameter.
 */
void task_run_statemachine(void *params);

#endif // APPS_MAIN