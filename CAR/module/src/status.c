#include "car.h"
#include "status.h"

void Init_OK(void)
{
    led_init(LED_MAX);
	LEDY_ON;
	LEDB_ON;
	ui.index = 0;
	ui.row = 0;
}

static KEY_MSG_t keyMSG;
struct UI ui;

void Key(void)
{
	static KEY_STATUS_e  lastStatus = KEY_UP;

	if (get_key_msg(&keyMSG) == 0)
		return;

	//默认屏蔽掉抬起操作
	if (keyMSG.status == KEY_DOWN)
		return;

	//屏蔽HOLD后的UP动作
	if (keyMSG.status == KEY_UP && lastStatus == KEY_HOLD)
	{
		lastStatus = keyMSG.status;
		return;
	}

	lastStatus = keyMSG.status;

	switch (keyMSG.key)
	{
	case KEY_U:
	{
		if (ui.cut == 0)
		{
			ui.index += 1;
		}
		else if(ui.cut == 1)
		{
			ui.row += 1;
		}
		else
		{
			if (ui.index == ui.data_index)   //注意修改数据的页面页码
			{
				if (ui.row == 0)
					car.speed -= 100;
				else if (ui.row == 1)
					car.KP -= 0.2;
				else if (ui.row == 2)
					car.KD -= 0.1;
				else if (ui.row == 3)
					Kp_e -= 2;
				else if (ui.row == 4)
					Kd_e -= 2;
				else if (ui.row == 5)
					car.elecspeed -= 100;
				else if (ui.row == 6)
					breakL0R1 += 1;  //right
				else
					modeflag += 1;  
			}
			else if (ui.index == ui.data_index + 1)   //注意修改数据的页面页码
			{
				if (ui.row == 0)
					turnback -= 100;
				else if (ui.row == 1)
					turnout -= 100;
				else if (ui.row == 2)
					turnin -= 100;
				else if (ui.row == 3)
					turnrepair -= 100;
				else if (ui.row == 4)
					threshold_cmp -= 1;
				else if (ui.row == 5)
					count_delay -= 10;
				else if (ui.row == 6)
					speedcmp -= 1;
				else
					mid_cmp -= 1;
			}
		}
	} break;

	case KEY_D:
	{
		if (ui.cut == 0)
		{
			ui.index -= 1;
		}
		else if (ui.cut == 1)
		{
			ui.row -= 1;
		}
		else
		{
			if(ui.index == ui.data_index)    //注意修改数据的页面页码
			{
				if (ui.row == 0)
					car.speed += 100;
				else if (ui.row == 1)
					car.KP += 0.2;
				else if (ui.row == 2)
					car.KD += 0.1;
				else if (ui.row == 3)
					Kp_e += 2;
				else if (ui.row == 4)
					Kd_e += 2;
				else if (ui.row == 5)
					car.elecspeed += 100;
				else if (ui.row == 6)
					breakL0R1 -= 1;  //right
				else
					modeflag -= 1;
			}
			else if (ui.index == ui.data_index + 1)   //注意修改数据的页面页码
			{
				if (ui.row == 0)
					turnback += 100;
				else if (ui.row == 1)
					turnout += 100;
				else if (ui.row == 2)
					turnin += 100;
				else if (ui.row == 3)
					turnrepair += 100;
				else if (ui.row == 4)
					threshold_cmp += 1;
				else if (ui.row == 5)
					count_delay += 10;
				else if (ui.row == 6)
					speedcmp += 1;
				else
					mid_cmp += 1;
			}
		}
	} break;

	case KEY_S:
	{
		ui.cut += 1;
	} break;

	case KEY_C:
	{
		ui.cut -= 1;
	} break;

	case KEY_K:
	{
		if (!car.stop)
		{
			car.run = 1;
			car.stop = 1;
		    Ring_On;
		    DELAY_MS(1200);
		}
		else
		{
			car.run = 0;
			car.stop = 0;
			Ring_On;
			DELAY_MS(600);
		}
	} break;

	default:break;
	}
	Ring_Short();
}

void Key_Init(void)
{
	port_init(PTB16, ALT1 | IRQ_FALLING | PULLUP);
	port_init(PTB17, ALT1 | IRQ_FALLING | PULLUP);
	port_init(PTB18, ALT1 | IRQ_FALLING | PULLUP);
	port_init(PTB19, ALT1 | IRQ_FALLING | PULLUP);
	port_init(PTB20, ALT1 | IRQ_FALLING | PULLUP);
}

void Ring_Short(void)
{
	Ring_On;
	DELAY_MS(20);
	Ring_Off;
}
void Ring_Long(void)
{
	Ring_On;
	DELAY_MS(500);
	Ring_Off;
}



