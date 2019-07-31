#include "Variable.h"
#include "car.h"

int16 Right_speed;
int16 Left_speed;
int16 Left_goalspeed;
int16 Right_goalspeed;

uint8 set_speed_l = 0;
int set_speed_s = 0;
int Move_Z = 0;
//float KP = 2.6;
//float KI = 0;
//float KD = 1.2;
float KP_show = 0;
float KD_show = 0;
PID pid_left;
PID pid_right;




