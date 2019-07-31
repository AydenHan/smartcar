#include "include.h"
#include "HC-SR04.h"

uint16 use_time;
float dis_data;
uchar work = 0;

/*!��飺  HCSR04��ʼ������
 * ������  TRIG_PIN��������ģ��Ĵ�������
 *         ECHO_PIN��������ģ���������������
 *
 */
void HCSR04_INIT(PTXn_e TRIG_PIN,PTXn_e ECHO_PIN)
{
    gpio_init (TRIG_PIN, GPO, 0);                                                   //�������ų�ʼ�� ������A26
	//gpio_init (ECHO_PIN, GPI, 0);
    port_init (ECHO_PIN, IRQ_FALLING | PF | ALT1 | PULLUP );                     
}

void HCSR04_START(void)
{
    gpio_set(TRIG,1);
    pit_delay_us(PIT0,10);
    gpio_set(TRIG,0);

	while (!gpio_get(ECHO));
	pit_time_start(PIT1);
	work = 1;
}

float HCSR04_DATA_HANDLE(float back_time)
{
	float distance;                                                   //����洢
    distance = back_time*(331.4+0.607*10)/2000;                       //�¶Ȳ���

	return distance;
}

void dis_measure(void)
{
	while (gpio_get(ECHO))
	{
		if (pit_time_get_us(PIT1) > 4000)
		{
			pit_close(PIT1);
			work = 0;
			Ring_On;
			roadinfo.cutelect = InBreakRoad;
			break;
		}
	}
	if (work)
	{
		use_time = pit_time_get_us(PIT1);
		pit_close(PIT1);
		dis_data = HCSR04_DATA_HANDLE(1.0 * use_time);//����
		work = 0;
		if (dis_data < 380)
		{
			roadinfo.rodetype = Barrier;
		}
		else
		{
			Ring_On;
			roadinfo.cutelect = InBreakRoad;
		}
	}
}

