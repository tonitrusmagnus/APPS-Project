/**
 * @file helper_functions.hpp
 * @brief Collection of helper functions for common tasks.
 */

#ifndef HELPER_FUNCTIONS_HPP
#define HELPER_FUNCTIONS_HPP

#include <stdlib.h>

/**
 * @brief Constrains a value to be within a specified range.
 * @param value The value to be constrained.
 * @param min The minimum allowed value.
 * @param max The maximum allowed value.
 * @return The constrained value.
 */
int constrain(int value, int min, int max);

/**
 * @brief Maps a value from one range to another.
 * @param value The value to be mapped.
 * @param fromLow The lower bound of the original range.
 * @param fromHigh The upper bound of the original range.
 * @param toLow The lower bound of the target range.
 * @param toHigh The upper bound of the target range.
 * @return The mapped value.
 */
int map(int value, int fromLow, int fromHigh, int toLow, int toHigh);

/**
 * @brief Maps a double value from one range to another.
 * @param value The value to be mapped.
 * @param fromLow The lower bound of the original range.
 * @param fromHigh The upper bound of the original range.
 * @param toLow The lower bound of the target range.
 * @param toHigh The upper bound of the target range.
 * @return The mapped value.
 */
double mapDouble(double value, double fromLow, double fromHigh, double toLow, double toHigh);

/**
 * @brief Generates a random number within a specified range.
 * @param min The minimum value of the random number.
 * @param max The maximum value of the random number.
 * @return A random number within the specified range.
 */
int randomNumber(int min, int max);

/**
 * @brief Calculates the maximum representable value for a given data size.
 * @param dataSize The size of the data in bytes.
 * @return The maximum representable value for the given data size.
 */
unsigned long maxValue(size_t dataSize);

#endif // HELPER_FUNCTIONS_HPP
