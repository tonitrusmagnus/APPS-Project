#include "helper_functions.hpp"
#include "defines.hpp"
#include <time.h>
#include <cmath>

int constrain(int value, int min, int max){
    if(value < min){
        value = min;
    }
    else if(value > max){
        value = max;
    }
    return value;
}

int map(int value, int fromLow, int fromHigh, int toLow, int toHigh) {
    return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}

double mapDouble(double value, double fromLow, double fromHigh, double toLow, double toHigh) {
    return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}

int randomNumber(int min,int max){
    static bool first = true;
    if (first)
    {
        srand( time(NULL) ); // Only set seed for the first time
        first = false;
    }

    if (max >= min) {
        return (rand() % (max-min+1) ) + min ;
    }
    return -1;
}

unsigned long maxValue(size_t dataSize){
    return (pow(2,(dataSize*8))-1);
}