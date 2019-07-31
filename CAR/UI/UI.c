#include "UI.h"
#include "car.h"

void UI_Init(void)
{
	LCD_Init();
	LCD_P6x8Str(30,3,"initialized");
	DELAY_MS(800);
	Display();
}

void Display(void)
{
	uint8 a[10];
	uint8 b[10];
	uint8 c[10];
	uint8 d[10];//
	uint8 e[10];

	uchar flag,count,i;

	LCD_CLS();
	ui.data_index = 1;   //************注意这是需要修改的数据页的页面

	if (ui.index == 0)
	{
		Data_Display(car.KP, car.KD, car.speed, image_threshold);
	    Binary_Display(image1[0]);

		sprintf((char*)d, "%d", ui.cut);
		LCD_P6x8Str(120, 0, d);
	}
	else if (ui.index == ui.data_index)
	{
		sprintf((char*)a, "%d", Kp_e);
		sprintf((char*)b, "%d", Kd_e);
		sprintf((char*)c, "%d", car.elecspeed);
		LCD_P6x8Str(0, 3, "E-P:");LCD_P6x8Str(27, 3, a);
		LCD_P6x8Str(0, 4, "E-D:");LCD_P6x8Str(27, 4, b);
		LCD_P6x8Str(0, 5, "E-S:");LCD_P6x8Str(27, 5, c);

		sprintf((char*)a, "%d", car.speed);
		sprintf((char*)b, "%.1f", car.KP);
		sprintf((char*)c, "%.1f", car.KD);
		LCD_P6x8Str(0, 0, "C-S:"); LCD_P6x8Str(27, 0, a);
		LCD_P6x8Str(0, 1, "C-P:"); LCD_P6x8Str(27, 1, b);
		LCD_P6x8Str(0, 2, "C-D:"); LCD_P6x8Str(27, 2, c);

		LCD_P6x8Str(0, 6, "turn:");
		if (breakL0R1 == 0)  //左拐
			LCD_P6x8Str(36, 6, "left");
		else  //右拐
			LCD_P6x8Str(36, 6, "right");

		LCD_P6x8Str(0, 7, "mode:");
		if (modeflag == 0) 
			LCD_P6x8Str(36, 7, "mode1");
		else if(modeflag == 1)
			LCD_P6x8Str(36, 7, "mode2");
		else if (modeflag == 2)
			LCD_P6x8Str(36, 7, "mode3");
		else if (modeflag == 3)
			LCD_P6x8Str(36, 7, "mode4");
		else if (modeflag == 4)
			LCD_P6x8Str(36, 7, "mode5");
		else
			LCD_P6x8Str(36, 7, "mode6");
		
		if (ui.row == 0)
			LCD_P6x8Str(90, ui.row, "<=");
		else if(ui.row == 1)
			LCD_P6x8Str(90, ui.row, "<=");
		else if(ui.row == 2)
			LCD_P6x8Str(90, ui.row, "<=");
		else if (ui.row == 3)
			LCD_P6x8Str(90, ui.row, "<=");
		else if (ui.row == 4)
			LCD_P6x8Str(90, ui.row, "<=");
		else if (ui.row == 5)
			LCD_P6x8Str(90, ui.row, "<=");
		else if (ui.row == 6)
			LCD_P6x8Str(90, ui.row, "<=");
		else
			LCD_P6x8Str(90, ui.row, "<=");


		sprintf((char*)d, "%d", ui.cut);
		LCD_P6x8Str(114, 0, d);
	}
	else if (ui.index == ui.data_index + 1)
	{
		
		sprintf((char*)a, "%d", turnback);
		LCD_P6x8Str(0, 0, "T-BACK:"); LCD_P6x8Str(45, 0, a);
		sprintf((char*)b, "%d", turnout);
		LCD_P6x8Str(0, 1, "T-OUT:");  LCD_P6x8Str(39, 1, b);
		sprintf((char*)c, "%d", turnin);
		LCD_P6x8Str(0, 2, "T-IN:");   LCD_P6x8Str(33, 2, c);
		sprintf((char*)e, "%d", turnrepair);
		LCD_P6x8Str(0, 3, "T-REPR:"); LCD_P6x8Str(45, 3, e);

        sprintf((char*)a, "%d", threshold_cmp);
		LCD_P6x8Str(0, 4, "YZ-CMP:"); LCD_P6x8Str(45, 4, a);
		sprintf((char*)b, "%d", count_delay);
		LCD_P6x8Str(0, 5, "DELAY:");  LCD_P6x8Str(39, 5, b);
		sprintf((char*)c, "%d", speedcmp);
		LCD_P6x8Str(0, 6, "S-CMP:");  LCD_P6x8Str(39, 6, c);
		sprintf((char*)e, "%d", mid_cmp);
		LCD_P6x8Str(0, 7, "M-CMP:");  LCD_P6x8Str(39, 7, e);
		
		if (ui.row == 0)
			LCD_P6x8Str(90, ui.row, "<=");
		else if (ui.row == 1)
			LCD_P6x8Str(90, ui.row, "<=");
		else if (ui.row == 2)
			LCD_P6x8Str(90, ui.row, "<=");
		else if (ui.row == 3)
			LCD_P6x8Str(90, ui.row, "<=");
		else if (ui.row == 4)
			LCD_P6x8Str(90, ui.row, "<=");
		else if (ui.row == 5)
			LCD_P6x8Str(90, ui.row, "<=");
		else if (ui.row == 6)
			LCD_P6x8Str(90, ui.row, "<=");
		else
			LCD_P6x8Str(90, ui.row, "<=");


		sprintf((char*)d, "%d", ui.cut);
		LCD_P6x8Str(114, 0, d);
	}

	else if (ui.index == 6)
	{
		for (flag = 1; flag <= 8; flag++)
		{
			for (count = 1; count <= 5; count++)
			{
				sprintf((char*)e, "%d", Right_Line[flag * 5 + count + 64]);
				LCD_P6x8Str((count - 1) * 24, flag - 1, e);
			}
		}
	}
	else if (ui.index == 5)
	{
		for (flag = 1; flag <= 8; flag++)
		{
			for (count = 1; count <= 5; count++)
			{
				sprintf((char*)e, "%d", Width[flag * 5 + count + 49]);//95
				LCD_P6x8Str((count - 1) * 24, flag - 1, e);
			}
		}
	}
	else if (ui.index == 3)
	{ 
		LCD_P6x8Str(0, 0, "SXT:");  LCD_P6x8Str(60, 0, "MID:");
		LCD_P6x8Str(0, 1, "ELE:");  LCD_P6x8Str(60, 1, "Y-Z:");
		LCD_P6x8Str(0, 2, "CNT:");  LCD_P6x8Str(60, 2, "");
		LCD_P6x8Str(0, 3, "");  LCD_P6x8Str(60, 3, "");
		LCD_P6x8Str(0, 4, "");  LCD_P6x8Str(60, 4, "");
		LCD_P6x8Str(0, 5, "");  LCD_P6x8Str(60, 5, "");
		LCD_P6x8Str(0, 6, "SPL");  LCD_P6x8Str(60, 6, "");
		LCD_P6x8Str(0, 7, "SPR");  LCD_P6x8Str(60, 7, "");
		
		sprintf((char*)a, "%d", car.incamera); LCD_P6x8Str(27, 0, a);
		sprintf((char*)b, "%d", car.inelec);   LCD_P6x8Str(27, 1, b);
		sprintf((char*)c, "%d", count_s);      LCD_P6x8Str(27, 2, c);
		//sprintf((char*)e, "%.1f", dis_data);   LCD_P6x8Str(27, 3, e);

		//sprintf((char*)a, "%d", );  LCD_P6x8Str(33, 4, a);
		//sprintf((char*)b, "%d", );  LCD_P6x8Str(33, 5, b);
		sprintf((char*)c, "%d", speedlll);     LCD_P6x8Str(33, 6, c);
		sprintf((char*)e, "%d", speedrrr);     LCD_P6x8Str(33, 7, e);

		sprintf((char*)a, "%d", midline_aver);    LCD_P6x8Str(87, 0, a);
		sprintf((char*)b, "%d", image_threshold); LCD_P6x8Str(87, 1, b); 
		//sprintf((char*)c, "%d", barrierfinish);    LCD_P6x8Str(87, 2, c);
		//sprintf((char*)e, "%d", breakfinish);    LCD_P6x8Str(87, 3, e);
	}
	else if (ui.index == 4)
	{
	LCD_P6x8Str(0, 0, "STO1:");  LCD_P6x8Str(60, 0, "ADC1:");
	LCD_P6x8Str(0, 1, "STO2:");  LCD_P6x8Str(60, 1, "ADC2:");
	LCD_P6x8Str(0, 2, "STO3:");  LCD_P6x8Str(60, 2, "ADC3:");
	LCD_P6x8Str(0, 3, "STO4:");  LCD_P6x8Str(60, 3, "ADC4:");
	LCD_P6x8Str(0, 4, "EMID:");  LCD_P6x8Str(60, 4, "");
	LCD_P6x8Str(0, 5, "CMID");  LCD_P6x8Str(60, 5, "");
	LCD_P6x8Str(0, 6, "");  LCD_P6x8Str(60, 6, "");
	LCD_P6x8Str(0, 7, "");  LCD_P6x8Str(60, 7, "");

	sprintf((char*)a, "%d", sensor[0]);  LCD_P6x8Str(33, 0, a);
	sprintf((char*)b, "%d", sensor[1]);  LCD_P6x8Str(33, 1, b);
	sprintf((char*)c, "%d", sensor[2]);  LCD_P6x8Str(33, 2, c);
	sprintf((char*)e, "%d", sensor[3]);  LCD_P6x8Str(33, 3, e);

	sprintf((char*)a, "%.1f", Pwm_value);  LCD_P6x8Str(33, 4, a);
	sprintf((char*)b, "%d", servo_chg);    LCD_P6x8Str(33, 5, b);

	sprintf((char*)a, "%d", AD_sensor[0]);  LCD_P6x8Str(93, 0, a);
	sprintf((char*)b, "%d", AD_sensor[1]);  LCD_P6x8Str(93, 1, b);
	sprintf((char*)c, "%d", AD_sensor[2]);  LCD_P6x8Str(93, 2, c);
	sprintf((char*)e, "%d", AD_sensor[3]);  LCD_P6x8Str(93, 3, e);
	}
}

void Binary_Display(uint8 *p)
{
	int i, j, k;
	uint16 temp = 0;

	for (i = 63; i >= 9; i--)
	{
		for (j = 0; j < 188; j++)
		{
			temp = *(p + (i + 45)* 188 + j);//读取像素点

			if (temp == 0)
				k = (j / (float)188) * 128;
				LCD_PutPixel(k, i);
		}
	}
}

void Edge_Display(int *edgel, int *edger)
{
	int i;
	uchar a, b;   //a左，b右

	for (i = 63; i >= 10; i--)
	{
		a = (edgel[i + 56] / (float)186) * 128;
		b = (edger[i + 56] / (float)186) * 128;

		LCD_PutPixel(a, i);
		LCD_PutPixel(b, i);
	}

}

void Midline_Display(int *midline)
{
	int i;
	uchar c;   //a左，b右

	for (i = 63; i >= 10; i--)
	{
		c = (midline[i + 46] / (float)186) * 128;

		LCD_PutPixel(c, i);
	}
}

void Data_Display(float num1, float num2,int num3, uchar num4)
{
	uchar a[10];
	uchar b[10];
	uchar c[10];
	uchar e[10];

	sprintf((char*)a ,"%.1f" , num1);
	sprintf((char*)b, "%.1f", num2);
	sprintf((char*)c, "%d", num3);
	sprintf((char*)e, "%d", num4);
	LCD_P6x8Str(0, 0, a);
	LCD_P6x8Str(24, 0, b);
	LCD_P6x8Str(48, 0, c);
	LCD_P6x8Str(90, 0, e);
}


