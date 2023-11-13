#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

#define MEASURE_QUEUE_LENGTH 4
#define CALIBRATE_QUEUE_LENGTH 100

enum states{setup,relax,process,feedback,calibrate};
//enum feedbackstates{freqLow,freqGood,dBLow,dBGood};
enum feedbackstate{volumeOnLed,frequencyOnLed};

void statemachine(bool* dataReady, int volume, int frequency);
void stateSetup(void);
void stateRelax(void);
void stateProcess(bool _dataReady);
void stateFeedback();
void stateCalibrate();
void stateDebug();
#endif //STATEMACHINE_HPP