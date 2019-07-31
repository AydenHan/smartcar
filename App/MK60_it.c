#include    "MK60_it.h"


/*********************************中断服务函数********************************/

/*!
 *  @brief      UART4中断服务函数
 *  @since      v5.0

void uart4_handler(void)
{

}
 */

/*!
*  @brief      PORTA中断服务函数
*  @since      v5.0

void PORTA_IRQHandler()
{


}
*/

/*!
*  @brief      PORTE中断服务函数
*  @since      v5.0
*/
void PORTE_IRQHandler()
{

}


/*!
*  @brief      DMA0中断服务函数
*  @since      v5.0

void DMA0_IRQHandler()
{
  camera_dma();
}
*/

/*!
*  @brief      PIT0中断服务函数
*  @since      v5.0

void PIT0_IRQHandler()
{

}
*/

/*!
*  @brief      PIT1中断服务函数
*  @since      v5.0
*/
void PIT1_IRQHandler()
{

}


/*!
*  @brief      输入捕获中断服务函数
*  @since      v5.0
*/
void FTM2_INPUT_IRQHandler(void)
{

}

/******************************中断服务函数结束***********8*******************/