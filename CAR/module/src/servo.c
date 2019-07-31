#include "car.h"
#include "servo.h"

void ServoInit(void)
{
      //��ʼ�������PWM
      ftm_pwm_init(SERVO_PORT, SERVO_CH,SERVO_FREQ, SERVO_MIDDLE);
      //DEBUG_TEXT("steer init\r\n");
}

void ServoReset(void)
{
      ftm_pwm_duty(SERVO_PORT, SERVO_CH,SERVO_MIDDLE);
}

void ServoChange(int16 out)
{
      ftm_pwm_duty(SERVO_PORT, SERVO_CH,out);
}
 