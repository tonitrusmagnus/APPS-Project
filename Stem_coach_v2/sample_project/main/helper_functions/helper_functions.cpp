#include "helper_functions.hpp"

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