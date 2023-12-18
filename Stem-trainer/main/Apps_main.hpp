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

extern volatile int Mode;
void task_read_audio(void* params);
void task_process_audio(void *params);
void task_run_statemachine(void *params);

void queue_add(std::queue<int>& Queue, int value);
int queue_average(const std::queue<int>& Queue) ;

#endif