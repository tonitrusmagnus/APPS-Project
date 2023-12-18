#ifndef HELPER_FUNCTIONS_HPP
#define HELPER_FUNCTIONS_HPP

#include <stdlib.h>

int constrain(int value, int min, int max);
int map(int value, int fromLow, int fromHigh, int toLow, int toHigh);
double mapDouble(double value, double fromLow, double fromHigh, double toLow, double toHigh);
int randomNumber(int min,int max);
unsigned long maxValue(size_t dataSize);
#endif