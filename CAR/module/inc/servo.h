#ifndef _CAR_SERVO_H
#define _CAR_SERVO_H

#include "ParaDef.h"

void ServoInit(void);

#if 1
void ServoReset(void);
void ServoChange(int16 out);
#else
#define ServoReset()  ftm_pwm_duty(SERVO_PORT, SERVO_CH,SERVO_MIDDLE)
#define ServoChange(out) ftm_pwm_duty(SERVO_PORT, SERVO_CH,out)
#endif


#endif