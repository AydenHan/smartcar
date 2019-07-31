#include "car.h"
#include "motor.h"

void MotorInit(void)
{
	//Ӳ��оƬʹ��
	gpio_init(PTE9, GPO, 1);
	gpio_init(PTE10, GPO, 1);

	//��ʼ�������PWM
	ftm_pwm_init(MOTOR_FTM, MOTORL_Z_PWM,MOTOR_FREQ,0);
	ftm_pwm_init(MOTOR_FTM, MOTORL_F_PWM,MOTOR_FREQ,0);
    ftm_pwm_init(MOTOR_FTM, MOTORR_Z_PWM,MOTOR_FREQ,0);
	ftm_pwm_init(MOTOR_FTM, MOTORR_F_PWM,MOTOR_FREQ,0);

        //�Ȳ���
	ftm_pwm_duty(MOTOR_FTM, MOTORL_Z_PWM,0);
	ftm_pwm_duty(MOTOR_FTM, MOTORR_Z_PWM,0);

	//DEBUG_TEXT("motor system init\r\n");
}

void MotorSpeed(int16 outL, int16 outR)
{
	//��������ȷ��
	if (outL > 0)
	{
		ftm_pwm_duty(MOTOR_FTM, MOTORL_Z_PWM, outL);
		ftm_pwm_duty(MOTOR_FTM, MOTORL_F_PWM, 0);
	}
	else {
		ftm_pwm_duty(MOTOR_FTM, MOTORL_Z_PWM, 0);
		ftm_pwm_duty(MOTOR_FTM, MOTORL_F_PWM, -outL);
	}

        if (outR > 0)
	{
		ftm_pwm_duty(MOTOR_FTM, MOTORR_Z_PWM, outR);
		ftm_pwm_duty(MOTOR_FTM, MOTORR_F_PWM, 0);
	}
	else {
		ftm_pwm_duty(MOTOR_FTM, MOTORR_Z_PWM, 0);
		ftm_pwm_duty(MOTOR_FTM, MOTORR_F_PWM, -outR);
	}
}


