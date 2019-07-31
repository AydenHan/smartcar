#include "car.h"
#include "motor.h"

void MotorInit(void)
{
	//硬件芯片使能
	gpio_init(PTE9, GPO, 1);
	gpio_init(PTE10, GPO, 1);

	//初始化电机的PWM
	ftm_pwm_init(MOTOR_FTM, MOTORL_Z_PWM,MOTOR_FREQ,0);
	ftm_pwm_init(MOTOR_FTM, MOTORL_F_PWM,MOTOR_FREQ,0);
    ftm_pwm_init(MOTOR_FTM, MOTORR_Z_PWM,MOTOR_FREQ,0);
	ftm_pwm_init(MOTOR_FTM, MOTORR_F_PWM,MOTOR_FREQ,0);

        //先不动
	ftm_pwm_duty(MOTOR_FTM, MOTORL_Z_PWM,0);
	ftm_pwm_duty(MOTOR_FTM, MOTORR_Z_PWM,0);

	//DEBUG_TEXT("motor system init\r\n");
}

void MotorSpeed(int16 outL, int16 outR)
{
	//输出方向的确定
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


