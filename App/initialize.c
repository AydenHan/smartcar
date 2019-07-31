#include "initialize.h"
#include "car.h"

void UART3_RX_TX_IRQHandler(void);
void UART1_RX_TX_IRQHandler(void);

//void PORTD_IRQHandler(void);
void PORTC_IRQHandler(void);

void KeyScan_Handle(void);

void Sys_Init()
{
      Sys_NVICInit();
	  Sys_InterruptInit();
}

void Dev_Init()
{
      camera_init();
      ServoInit();     //FTM0 CH2
      MotorInit();     //FTM3 CH0-3
      EncoderInit();   //FTM1,FTM2
	  Key_Init();
	  sensor_init();
	  HCSR04_INIT(TRIG,ECHO);

	  UI_Init();
	  Data_Init();
	  Init_OK();       //D15状态灯亮
}

void Sys_NVICInit()
{
	  NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);

      NVIC_SetPriority(UART3_RX_TX_IRQn,15);//配置中断
      NVIC_SetPriority(PORTC_IRQn,0);//场中断
      NVIC_SetPriority(DMA0_IRQn,1);//DMA采集

	  NVIC_SetPriority(PIT1_IRQn,9);//按键每隔10ms扫描一次
      //NVIC_SetPriority(PORTA_IRQn, 2);

      //NVIC_SetPriority(UART5_RX_TX_IRQn, 4);
}

void Sys_InterruptInit()
{
      //pit_init_us(PIT0, 5);
      //set_vector_handler(PIT0_VECTORn, PIT0_IRQHandler);

      //pit_init_ms(PIT1, 10);
      //set_vector_handler(PIT1_VECTORn, KeyScan_Handle);
	  //enable_irq(PIT0_IRQn);

      set_vector_handler(UART3_RX_TX_VECTORn, UART3_RX_TX_IRQHandler);
      set_vector_handler(PORTC_VECTORn, PORTC_IRQHandler);
	  //set_vector_handler(PORTD_VECTORn, PORTD_IRQHandler);
	  set_vector_handler(DMA0_VECTORn, DMA0_IRQHandler);

	  uart_init(UART1, 115200);
	  //set_vector_handler(UART1_RX_TX_VECTORn, UART1_RX_TX_IRQHandler);
      //uart_rx_irq_en(UART1);
	  //set_vector_handler(PORTA_VECTORn, PORTA_IRQHandler);enable_irq(PORTA_IRQn);
}

void Data_Init(void)
{
	car.speed = 2500;
	car.KP = 2.6;
	car.KD = 1.2;
	car.elecspeed = 2300;

	car.stop = 0;
	car.instartline = 0;
	car.incycle = 0;
	//car.incyclestate = 0;
	car.incamera = 1;
	car.inelec = 0;
	car.mode = 0;  //正常模式
	car.inbarrier = 0;

	sidestate.CloseCycleCornerL = 0;
	sidestate.CloseCycleCornerR = 0;
	sidestate.FarCycleCornerL = 0;
	sidestate.FarCycleCornerR = 0;

	roadinfo.cutelect = NoBreakRoad;
	roadinfo.addlinestate = CommonDraw;
	
}

void DMA0_IRQHandler(void)
{
    DMA_IRQ_CLEAN(DMA_CH0);
    row_finished();
    //DMA_EN(DMA_CH0);                        //使能通道CHn 硬件请求
    //enable_irq(DMA0_DMA16_IRQn);
}

void UART3_RX_TX_IRQHandler(void)
{
    UARTn_e uratn = UART3;

    if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   //接收数据寄存器满
    {
        //用户需要处理接收数据
       mt9v032_cof_uart_interrupt();
    }

    if(UART_S1_REG(UARTN[uratn]) & UART_S1_TDRE_MASK )  //发送数据寄存器空
    {
        //用户需要处理发送数据
    }
}
/**/
void UART1_RX_TX_IRQHandler(void)
{
	/*
	char str[100];

	if (uart_querystr(UART4, str, sizeof(str) - 1 ) != 0)
	{
		KP = (str[0]-48) * 10 + str[1]-48;// * 10 + str[2];
		uart_putstr(UART4, str);
	}
	uart_rx_irq_dis(UART4);
	char ch;

		ch = 89 - midline_aver;   
		uart_putchar(UART4,ch);*/

}
/*
void PORTD_IRQHandler(void)
{
	PORTD_ISFR = 1 << 1;
	dis_measure();
}*/

void PORTC_IRQHandler(void)
{
    PORTC_ISFR = 0xffffffff;
    VSYNC();
}

void KeyScan_Handle(void)
{
	PIT_Flag_Clear(PIT1);
	//key_scan();
}

void PIT0_IRQHandler()
{
   PIT_Flag_Clear(PIT0);
}

