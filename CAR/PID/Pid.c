#include"Pid.h"
#include"car.h"

extern PID pid_left;
extern PID pid_right;

extern int16 Left_speed;
extern int16 Right_speed;

extern float KP;
extern float KI;
extern float KD;

float P=13;
float I=0;
float D=45;

void Pid_Init(void)
{

	pid_left.kp = P;
	pid_left.ki = I;
	pid_left.kd = D;

	pid_right.kp = P;
	pid_right.ki = I;
	pid_right.kd = D;

	
	pid_left.err = 0;
	pid_left.last_err = 0;
	pid_left.last_last_err = 0;
	pid_left.result = 0;

	pid_right.err = 0;
	pid_right.last_err = 0;
	pid_right.last_last_err = 0;
	pid_right.result = 0;

}

void PID_Control(PID *pid, int16 actual, int16 set)
{
	float p = 0, i = 0, d = 0;
	pid->err = set - actual;
	p = pid->err - pid->last_err;
	i = pid->err;
	d = pid->err - 2 * pid->last_err + pid->last_last_err;
	pid->result += pid->kp * p + pid->ki * i + pid->kd * d;
	if (pid->result > 9500)
		pid->result = 9500;
	else if (pid->result < -9500)
		pid->result = -9500;
	pid->last_last_err = pid->last_err;
	pid->last_err = pid->err;
}

int Dir_Pid(int midpoint, int error_act)
{
	int16 Move_Z;

	static int midpoint_err = 0;
	static int lastpoint_err = 0;

	midpoint_err = midpoint - error_act;
	Move_Z = car.KP * midpoint_err + car.KD * (midpoint_err - lastpoint_err);
	lastpoint_err = midpoint_err;
	if (Move_Z >= 80)
		Move_Z = 80;
	else if (Move_Z <= -80)
		Move_Z = -80;

	if (abs(89 - Mid_Line[60]) >= 4 && abs(89 - Mid_Line[60]) <= 8)
	{
		Move_Z = Move_Z / 3;
	}
	else if (abs(89 - Mid_Line[60]) >= 9 && abs(89 - Mid_Line[60]) < 20)
	{
		Move_Z = Move_Z / 3 * 2;
	}
	else if (abs(89 - Mid_Line[60]) >= 20)
	{
	}

	return Move_Z + SERVO_MIDDLE;
}