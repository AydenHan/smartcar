#include "common.h"
#include "MK60_uart.h"

UART_MemMapPtr UARTN[UART_MAX] = {UART0_BASE_PTR, UART1_BASE_PTR, UART2_BASE_PTR, UART3_BASE_PTR, UART4_BASE_PTR, UART5_BASE_PTR}; //定义五个指针数组保存 UARTN 的地址


void uart_init (UARTn_e uratn, uint32 baud)
{
    register uint16 sbr, brfa;
    uint8 temp;
    uint32 sysclk;     //时钟

    /* 配置 UART功能的 复用管脚 */
    switch(uratn)
    {
    case UART0:
        SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;      //使能 UART0 时钟

        if(UART0_RX_PIN == PTA1)
        {
            port_init( UART0_RX_PIN, ALT2);
        }
        else if((UART0_RX_PIN == PTA15) || (UART0_RX_PIN == PTB16) || (UART0_RX_PIN == PTD6)  )
        {
            port_init( UART0_RX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }

        if(UART0_TX_PIN == PTA2)
        {
            port_init( UART0_TX_PIN, ALT2);
        }
        else if((UART0_TX_PIN == PTA14) || (UART0_TX_PIN == PTB17) || (UART0_TX_PIN == PTD7) )
        {
            port_init( UART0_TX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }

        break;

    case UART1:
        SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;

        if((UART1_RX_PIN == PTC3) || (UART1_RX_PIN == PTE1))
        {
            port_init( UART1_RX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }

        if((UART1_TX_PIN == PTC4) || (UART1_TX_PIN == PTE0))
        {
            port_init( UART1_TX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }

        break;

    case UART2:
        SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
        if(UART2_TX_PIN == PTD3)
        {
            port_init( UART2_TX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }


        if(UART2_RX_PIN == PTD2)
        {
            port_init( UART2_RX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }

        break;

    case UART3:
        SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;

        if((UART3_RX_PIN == PTB10)|| (UART3_RX_PIN == PTC16) || (UART3_RX_PIN == PTE5) )
        {
            port_init( UART3_RX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }

        if((UART3_TX_PIN == PTB11) || (UART3_TX_PIN == PTC17) || (UART3_TX_PIN == PTE4) )
        {
            port_init( UART3_TX_PIN, ALT3);             //在PTB11上使能UART3_RXD
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }
        break;

    case UART4:
        SIM_SCGC1 |= SIM_SCGC1_UART4_MASK;

        if((UART4_RX_PIN == PTC14) || (UART4_RX_PIN == PTE25)  )
        {
            port_init( UART4_RX_PIN, ALT3);            //在PTC14上使能UART4_RXD
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }

        if((UART4_TX_PIN == PTC15) || (UART4_TX_PIN == PTE24)  )
        {
            port_init( UART4_TX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }
        break;

    case UART5:
        SIM_SCGC1 |= SIM_SCGC1_UART5_MASK;

        if((UART5_RX_PIN == PTD8) || (UART5_RX_PIN == PTE9))
        {
            port_init( UART5_RX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }

        if((UART5_TX_PIN == PTD9) ||(UART5_TX_PIN == PTE8))
        {
            port_init( UART5_TX_PIN, ALT3);             //在PTD9上使能UART5_RXD
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }
        break;
    default:
        break;
    }

    //设置的时候，应该禁止发送接受
    UART_C2_REG(UARTN[uratn]) &= ~(0
                                   | UART_C2_TE_MASK
                                   | UART_C2_RE_MASK
                                  );


    //配置成8位无校验模式
    //设置 UART 数据格式、校验方式和停止位位数。通过设置 UART 模块控制寄存器 C1 实现；
    UART_C1_REG(UARTN[uratn]) |= (0
                                  //| UART_C2_M_MASK                    //9 位或 8 位模式选择 : 0 为 8位 ，1 为 9位（注释了表示0，即8位） （如果是9位，位8在UARTx_C3里）
                                  //| UART_C2_PE_MASK                   //奇偶校验使能（注释了表示禁用）
                                  //| UART_C2_PT_MASK                   //校验位类型 : 0 为 偶校验 ，1 为 奇校验
                                 );

    //计算波特率，串口0、1使用内核时钟，其它串口使用bus时钟
    if ((uratn == UART0) || (uratn == UART1))
    {
        sysclk = core_clk_khz * 1000;                                   //内核时钟
    }
    else
    {
        sysclk = bus_clk_khz * 1000;                                    //bus时钟
    }

    //UART 波特率 = UART 模块时钟 / (16 × (SBR[12:0] + BRFA))
    //不考虑 BRFA 的情况下， SBR = UART 模块时钟 / (16 * UART 波特率)
    sbr = (uint16)(sysclk / (baud * 16));
    if(sbr > 0x1FFF)sbr = 0x1FFF;                                       //SBR 是 13bit，最大为 0x1FFF

    //已知 SBR ，则 BRFA =  = UART 模块时钟 / UART 波特率 - 16 ×SBR[12:0]
    brfa = (sysclk / baud)  - (sbr * 16);
    ASSERT( brfa <= 0x1F);                  //断言，如果此值不符合条件，则设置的条件不满足寄存器的设置
                                            //可以通过增大波特率来解决这个问题

    //写 SBR
    temp = UART_BDH_REG(UARTN[uratn]) & (~UART_BDH_SBR_MASK);           //缓存 清空 SBR 的 UARTx_BDH的值
    UART_BDH_REG(UARTN[uratn]) = temp |  UART_BDH_SBR(sbr >> 8);        //先写入SBR高位
    UART_BDL_REG(UARTN[uratn]) = UART_BDL_SBR(sbr);                     //再写入SBR低位

    //写 BRFD
    temp = UART_C4_REG(UARTN[uratn]) & (~UART_C4_BRFA_MASK) ;           //缓存 清空 BRFA 的 UARTx_C4 的值
    UART_C4_REG(UARTN[uratn]) = temp |  UART_C4_BRFA(brfa);             //写入BRFA



    //设置FIFO(FIFO的深度是由硬件决定的，软件不能设置)
    UART_PFIFO_REG(UARTN[uratn]) |= (0
                                     | UART_PFIFO_TXFE_MASK               //使能TX FIFO(注释表示禁止)
                                     //| UART_PFIFO_TXFIFOSIZE(0)         //（只读）TX FIFO 大小，0为1字节，1~6为 2^(n+1)字节
                                     | UART_PFIFO_RXFE_MASK               //使能RX FIFO(注释表示禁止)
                                     //| UART_PFIFO_RXFIFOSIZE(0)         //（只读）RX FIFO 大小，0为1字节，1~6为 2^(n+1)字节
                                    );

    /* 允许发送和接收 */
    UART_C2_REG(UARTN[uratn]) |= (0
                                  | UART_C2_TE_MASK                     //发送使能
                                  | UART_C2_RE_MASK                     //接收使能
                                  //| UART_C2_TIE_MASK                  //发送中断或DMA传输请求使能（注释了表示禁用）
                                  //| UART_C2_TCIE_MASK                 //发送完成中断使能（注释了表示禁用）
                                  //| UART_C2_RIE_MASK                  //接收满中断或DMA传输请求使能（注释了表示禁用）
                                 );

    //设置是否允许接收和发送中断。通过设置 UART 模块的 C2 寄存器的
    //RIE 和 TIE 位实现。如果使能中断，必须首先实现中断服务程序；
}

void uart_getchar (UARTn_e uratn, char *ch)
{
    //等待接收到数据（使能了FIFO后，以下两种方法都可行，都是一样的意思）
    //while( !UART_RCFIFO_REG(UARTN[uratn]) );                                //等待接收到数据数量非0
    //while( (UART_SFIFO_REG(UARTN[uratn]) & UART_SFIFO_RXEMPT_MASK)) ;     //等待接收缓冲区/FIFO 非空的

    //如下方法是飞思卡尔官方例程提供的方法
    while (!(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK));       //等待接收满了

    // 获取接收到的8位数据
    *ch =  UART_D_REG(UARTN[uratn]);

    // 获取 9位数据，应该是（需要修改函数的返回类型）：
    // *ch =   ((( UARTx_C3_REG(UARTN[uratn]) & UART_C3_R8_MASK ) >> UART_C3_R8_SHIFT ) << 8)   |   UART_D_REG(UARTN[uratn]);  //返回9bit

}

char uart_querychar (UARTn_e uratn, char *ch)
{
    if( UART_RCFIFO_REG(UARTN[uratn]) )         //查询是否接受到数据
    {
        *ch  =   UART_D_REG(UARTN[uratn]);      //接受到8位的数据
        return  1;                              //返回 1 表示接收成功
    }

    *ch = 0;                                    //接收不到，应该清空了接收区
    return 0;                                   //返回0表示接收失败
}

uint32 uart_querystr (UARTn_e uratn, char *str, uint32 max_len)
{
    uint32 i = 0,j;

    for(j=0;j<10000;j++)                 // 10000 的作用是延时，可自行根据情况修改
    {
        while(uart_querychar(uratn, str + i)  )
        {
            j=0;
            if( *(str + i) == NULL )    //接收到字符串结束符
            {
                return i;
            }

            i++;
            if(i >= max_len)            //超过设定的最大值，退出
            {
                *(str + i) = 0;     //确保字符串结尾是0x00
                return i;
            }
        }
    }

    *(str + i) = 0;                     //确保字符串结尾是0x00
    return i;
}

uint32 uart_querybuff (UARTn_e uratn, char *buff, uint32 max_len)
{
    uint32 i = 0,j;
    for(j=0;j<10000;j++)                 // 10000 的作用是延时，可自行根据情况修改
    {
        while(uart_querychar(uratn, buff + i)  )
        {
            i++;
            if(i >= max_len)            //超过设定的最大值，退出
            {
                return i;
            }
        }
    }

    return i;
}

void uart_putchar (UARTn_e uratn, char ch)
{
    //等待发送缓冲区空
    while(!(UART_S1_REG(UARTN[uratn]) & UART_S1_TDRE_MASK));

    //发送数据
    UART_D_REG(UARTN[uratn]) = (uint8)ch;
}

char uart_query (UARTn_e uratn)
{
    if(UART_RCFIFO_REG(UARTN[uratn]))                 //接收到数据数量非0
        //if(!(UART_SFIFO_REG(UARTN[uratn]) & UART_SFIFO_RXEMPT_MASK))
        //if(UART_S1_REG(UARTN[uratn]) & UART_S1_TDRE_MASK)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    //return UART_RCFIFO_REG(UARTN[uratn]);
}

void uart_putbuff (UARTn_e uratn, uint8 *buff, uint32 len)
{
    while(len--)
    {
        uart_putchar(uratn, *buff);
        buff++;
    }
}

void uart_putstr (UARTn_e uratn, const uint8 *str)
{
    while(*str)
    {
        uart_putchar(uratn, *str++);
    }
}

void uart_rx_irq_en(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) |= UART_C2_RIE_MASK;                          //使能UART接收中断
    enable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));                  //使能IRQ中断
}

void uart_tx_irq_en(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) |= UART_C2_TIE_MASK;                          //使能UART发送中断
    enable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));                  //使能IRQ中断
}

void uart_txc_irq_en(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) |= UART_C2_TCIE_MASK;                         //使能UART发送中断
    enable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));                  //使能IRQ中断
}

void uart_rx_irq_dis(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) &= ~UART_C2_RIE_MASK;                         //禁止UART接收中断

    //如果发送中断还没有关，则不关闭IRQ
    if(!(UART_C2_REG(UARTN[uratn]) & (UART_C2_TIE_MASK | UART_C2_TCIE_MASK)) )
    {
        disable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));             //关IRQ中断
    }
}

void uart_txc_irq_dis(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) &= ~UART_C2_TCIE_MASK;                        //禁止UART发送完成中断

    //如果接收中断还没有关，则不关闭IRQ
    if(!(UART_C2_REG(UARTN[uratn]) & UART_C2_RIE_MASK) )
    {
        disable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));             //关IRQ中断
    }
}

void uart_tx_irq_dis(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) &= ~UART_C2_TIE_MASK;                         //禁止UART发送中断

    //如果接收中断还没有关，则不关闭IRQ
    if(!(UART_C2_REG(UARTN[uratn]) & UART_C2_RIE_MASK) )
    {
        disable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));             //关IRQ中断
    }
}

void uart3_test_handler(void)
{
    UARTn_e uratn = UART3;

    if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   //接收数据寄存器满
    {
    }

    if(UART_S1_REG(UARTN[uratn]) & UART_S1_TDRE_MASK )  //发送数据寄存器空
    {
    }
}