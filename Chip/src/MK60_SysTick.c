#include "common.h"
#include "MK60_SysTick.h"

void systick_delay(uint32 time)
{
    if(time == 0)
    {
        return;
    }

    ASSERT(time <= (SysTick_RVR_RELOAD_MASK >> SysTick_RVR_RELOAD_SHIFT));  //24λ

    SYST_CSR = 0x00;                        //�ȹ��� systick ,���־λ

    SYST_RVR = time;                        //������ʱʱ��

    SYST_CVR = 0x00;                        //��ռ�����

    SYST_CSR = ( 0
                 | SysTick_CSR_ENABLE_MASK       //ʹ�� systick
                 //| SysTick_CSR_TICKINT_MASK    //ʹ���ж� (ע���˱�ʾ�ر��ж�)
                 | SysTick_CSR_CLKSOURCE_MASK    //ʱ��Դѡ�� ( core clk)
               );

    while( !(SYST_CSR & SysTick_CSR_COUNTFLAG_MASK));   //�ȴ�ʱ�䵽
}

void systick_delay_ms(uint32 ms)
{
    while(ms--)
    {
        systick_delay(SYSTICK_CLK_KHZ);
    }
}

/*!
 *  @brief      SysTick��ʱ����
 *  @param      time          ��ʱʱ��(0~65535)
 *  Sample usage:       systick_timing(32);     // systick ��ʱ 32 ��ϵͳ����
 */
void systick_timing(uint32 time)
{

    ASSERT(time <= (SysTick_RVR_RELOAD_MASK >> SysTick_RVR_RELOAD_SHIFT));  //24λ

    SYST_RVR = time;                        //������ʱʱ��

    //�������ȼ�
    NVIC_SetPriority (SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1); /* set Priority for Cortex-M4 System Interrupts */

    SYST_CVR = 0x00;                        //��ռ�����

    SYST_CSR = ( 0
                 | SysTick_CSR_ENABLE_MASK       //ʹ�� systick
                 | SysTick_CSR_TICKINT_MASK      //ʹ���ж� (ע���˱�ʾ�ر��ж�)
                 | SysTick_CSR_CLKSOURCE_MASK    //ʱ��Դѡ�� ( core clk)
               );
}




