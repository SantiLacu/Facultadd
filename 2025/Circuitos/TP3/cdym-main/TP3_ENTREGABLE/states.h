#ifndef STATES_H_
#define STATES_H_
	
#include "main.h"

enum timerInterruptFlag{
	ENABLED, DISABLED
} FLAG_INT;

enum stateFlag{
	DEFAULT, WAIT, ON, OFF, SET_TIME, SET_ALARM
} FLAG;

void task_INIT();
void task_PRINT();
void task_WAIT();
void task_ON();
void task_OFF();
void task_SET_TIME();
void task_SET_ALARM();
void timer1_CONFIG();
void timer1_ENABLE();




#endif /* STATES_H_ */