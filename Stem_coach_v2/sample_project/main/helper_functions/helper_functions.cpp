#include "helper_functions.hpp"
#include <time.h>
#include <stdlib.h>

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

int randomNumber(int min,int max){
    if (max!=min) {
        srand(time(NULL));
        return (rand()%(max-min))+ min ;
    } 
    else {
        return min;
    }
}