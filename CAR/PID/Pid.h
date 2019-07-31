#ifndef __CAR_PID_H
#define __CAR_PID_H

#include "car.h"

typedef struct 
{
	float err;
	float last_err;
	float last_last_err;
	float kp, ki, kd;
	float result;
}PID;

void Pid_Init(void);
int Dir_Pid(int midpoint, int error_act);
void PID_Control(PID *pid, int16 actual, int16 set);

#endif

