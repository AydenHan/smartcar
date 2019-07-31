#include "common.h"
#include "MK60_SysTick.h"

void systick_delay(uint32 time)
{
    if(time == 0)
    {
        return;
    }

    ASSERT(time <= (SysTick_RVR_RELOAD_MASK >> SysTick_RVR_RELOAD_SHIFT));  //24位

    SYST_CSR = 0x00;                        //先关了 systick ,清标志位

    SYST_RVR = time;                        //设置延时时间

    SYST_CVR = 0x00;                        //清空计数器

    SYST_CSR = ( 0
                 | SysTick_CSR_ENABLE_MASK       //使能 systick
                 //| SysTick_CSR_TICKINT_MASK    //使能中断 (注释了表示关闭中断)
                 | SysTick_CSR_CLKSOURCE_MASK    //时钟源选择 ( core clk)
               );

    while( !(SYST_CSR & SysTick_CSR_COUNTFLAG_MASK));   //等待时间到
}

void systick_delay_ms(uint32 ms)
{
    while(ms--)
    {
        systick_delay(SYSTICK_CLK_KHZ);
    }
}

/*!
 *  @brief      SysTick定时函数
 *  @param      time          定时时间(0~65535)
 *  Sample usage:       systick_timing(32);     // systick 定时 32 个系统周期
 */
void systick_timing(uint32 time)
{

    ASSERT(time <= (SysTick_RVR_RELOAD_MASK >> SysTick_RVR_RELOAD_SHIFT));  //24位

    SYST_RVR = time;                        //设置延时时间

    //设置优先级
    NVIC_SetPriority (SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1); /* set Priority for Cortex-M4 System Interrupts */

    SYST_CVR = 0x00;                        //清空计数器

    SYST_CSR = ( 0
                 | SysTick_CSR_ENABLE_MASK       //使能 systick
                 | SysTick_CSR_TICKINT_MASK      //使能中断 (注释了表示关闭中断)
                 | SysTick_CSR_CLKSOURCE_MASK    //时钟源选择 ( core clk)
               );
}




