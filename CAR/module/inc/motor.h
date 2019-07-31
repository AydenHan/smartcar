#ifndef _CAR_MOTOR_
#define _CAR_MOTOR_

#include "ParaDef.h"

void MotorInit(void);
void MotorSpeed(int16 outL, int16 outR);

#define MOTOR_STOP  MotorOut(0)

#endif