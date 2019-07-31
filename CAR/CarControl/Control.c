#include "Control.h"
#include "car.h"

uchar breakL0R1 = 0;
int count_s;
int16 speedlll = 0, speedrrr = 0;
int count_delay = 55;
int16 speedcmp = 5;
int turnback = 600, turnout = 900, turnin = 800, turnrepair = 400;
int mid_cmp = 94;

void Run(void)  //while循环的部分
{
	ModeSelect();  //模式设置

	Speed_Drive(); //转速检测

	Image_Drive(); //图像驱动

	Elec_Drive();  //电磁驱动

	if( roadinfo.rodetype == Barrier && roadinfo.cutelect != InBreakRoad)  //避障驱动
	{ 
		car.inbarrier = 1;
	}

	Mode_Elect();  //舵机控制模式切换

	Ui_Drive();  //显示驱动

	if(car.run)
	{ 
		if (count_s < count_delay)
		{
			count_s++;
		}
	}
	
	//Send_Upper();
/*	
	use_time = pit_time_get_us(PIT1);
	pit_close(PIT1);*/
}

void Elec_Drive(void)
{
	Get_ADC();
	DirectionCtrl();
	PID_elec();
}

void Speed_Drive(void)
{
	speedlll = ftm_quad_get(ENCODER_L_FTM);
	speedrrr = ftm_quad_get(ENCODER_R_FTM);
	EncoderClean();
}

void Mode_Elect(void)
{
	if (car.inbarrier)  //避障写死（真不智能。。）
	{
		if(breakL0R1 == 0)  //左拐
		{ 
			ServoChange(SERVO_MIDDLE);
			MotorSpeed(-2000, -2000);
			Ring_Long();
			DELAY_MS(turnback);

			ServoChange(SERVO_MIDDLE + startbarrier_servo);
			MotorSpeed(car.speed, car.speed);
			DELAY_MS(turnout);

			ServoChange(SERVO_MIDDLE - endbarrier_servo);
			DELAY_MS(turnin);
			ServoChange(SERVO_MIDDLE + endbarrier_servo);
			DELAY_MS(turnrepair);
		}
	    else  //右拐
	    {
			ServoChange(SERVO_MIDDLE);
		    MotorSpeed(-2000, -2000);
		    Ring_Long();
		    DELAY_MS(turnback);

            ServoChange(SERVO_MIDDLE - startbarrier_servo);
		    MotorSpeed(car.speed, car.speed);
		    DELAY_MS(turnout);

		    ServoChange(SERVO_MIDDLE + endbarrier_servo);
		    DELAY_MS(turnin);
			ServoChange(SERVO_MIDDLE - endbarrier_servo);
			DELAY_MS(turnrepair);
	    }
        car.inbarrier = 0;    //不再进避障
		car.instartline = 1;  //看到起跑线停车
		roadinfo.rodetype = StraightRoad;
	}
	else if(car.incamera)  //摄像头跑大部分路段（if阳光 电磁=_=）
	{
		if (car.run)
			MotorSpeed(car.speed, car.speed);
		else
			MotorSpeed(0, 0);  //car.run控制停车。。

		ServoChange(servo_chg); 
	}
	else if (car.inelec)  //断头路，进出环岛（保命模式）
	{
		if (car.run)
			MotorSpeed(car.elecspeed, car.elecspeed);
		else
			MotorSpeed(0, 0);

		ServoChange(Pwm_value);
	}

	if (StartLine_Flag == 1 && car.instartline == 1) //看到起跑线了，而且不是起跑，那么停车
	{
		car.run = 0;
		MotorSpeed(-1700, -1700);
		//ServoChange(SERVO_MIDDLE);
		Ring_Long();  //叫一叫表示完赛了，奈斯
	}
}

void Ui_Drive(void)
{
	key_scan();
	Key();
	Display();
}

void Image_Drive(void)
{
	if (mt9v032_finish_flag)
	{
		mt9v032_finish_flag = 0;
	/*	pit_time_start(PIT1);
		found_l_r(image[0], COL);
		fourseek_sobel_2 (image[0], COL, ROW, image1[0]);
		use_time = pit_time_get_us(PIT1);pit_close(PIT1);*/

	    image_threshold = threshold(image[0], COL, ROW);
	    binary(image1[0], image[0], image_threshold);
	    Mid_Count = Image_GetLine(image1[0]);

	    midline_aver = Point_Weight();
	    servo_chg = Dir_Pid(mid_cmp, midline_aver);
	    
		//send_upper();   //上位机
	}
}
