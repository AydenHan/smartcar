#include "common.h"
#include "MK60_uart.h"

UART_MemMapPtr UARTN[UART_MAX] = {UART0_BASE_PTR, UART1_BASE_PTR, UART2_BASE_PTR, UART3_BASE_PTR, UART4_BASE_PTR, UART5_BASE_PTR}; //�������ָ�����鱣�� UARTN �ĵ�ַ


void uart_init (UARTn_e uratn, uint32 baud)
{
    register uint16 sbr, brfa;
    uint8 temp;
    uint32 sysclk;     //ʱ��

    /* ���� UART���ܵ� ���ùܽ� */
    switch(uratn)
    {
    case UART0:
        SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;      //ʹ�� UART0 ʱ��

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
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
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
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
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
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
        }

        if((UART1_TX_PIN == PTC4) || (UART1_TX_PIN == PTE0))
        {
            port_init( UART1_TX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
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
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
        }


        if(UART2_RX_PIN == PTD2)
        {
            port_init( UART2_RX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
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
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
        }

        if((UART3_TX_PIN == PTB11) || (UART3_TX_PIN == PTC17) || (UART3_TX_PIN == PTE4) )
        {
            port_init( UART3_TX_PIN, ALT3);             //��PTB11��ʹ��UART3_RXD
        }
        else
        {
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
        }
        break;

    case UART4:
        SIM_SCGC1 |= SIM_SCGC1_UART4_MASK;

        if((UART4_RX_PIN == PTC14) || (UART4_RX_PIN == PTE25)  )
        {
            port_init( UART4_RX_PIN, ALT3);            //��PTC14��ʹ��UART4_RXD
        }
        else
        {
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
        }

        if((UART4_TX_PIN == PTC15) || (UART4_TX_PIN == PTE24)  )
        {
            port_init( UART4_TX_PIN, ALT3);
        }
        else
        {
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
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
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
        }

        if((UART5_TX_PIN == PTD9) ||(UART5_TX_PIN == PTE8))
        {
            port_init( UART5_TX_PIN, ALT3);             //��PTD9��ʹ��UART5_RXD
        }
        else
        {
            ASSERT(0);                           //���������������㣬ֱ�Ӷ���ʧ���ˣ����ùܽ�����
        }
        break;
    default:
        break;
    }

    //���õ�ʱ��Ӧ�ý�ֹ���ͽ���
    UART_C2_REG(UARTN[uratn]) &= ~(0
                                   | UART_C2_TE_MASK
                                   | UART_C2_RE_MASK
                                  );


    //���ó�8λ��У��ģʽ
    //���� UART ���ݸ�ʽ��У�鷽ʽ��ֹͣλλ����ͨ������ UART ģ����ƼĴ��� C1 ʵ�֣�
    UART_C1_REG(UARTN[uratn]) |= (0
                                  //| UART_C2_M_MASK                    //9 λ�� 8 λģʽѡ�� : 0 Ϊ 8λ ��1 Ϊ 9λ��ע���˱�ʾ0����8λ�� �������9λ��λ8��UARTx_C3�
                                  //| UART_C2_PE_MASK                   //��żУ��ʹ�ܣ�ע���˱�ʾ���ã�
                                  //| UART_C2_PT_MASK                   //У��λ���� : 0 Ϊ żУ�� ��1 Ϊ ��У��
                                 );

    //���㲨���ʣ�����0��1ʹ���ں�ʱ�ӣ���������ʹ��busʱ��
    if ((uratn == UART0) || (uratn == UART1))
    {
        sysclk = core_clk_khz * 1000;                                   //�ں�ʱ��
    }
    else
    {
        sysclk = bus_clk_khz * 1000;                                    //busʱ��
    }

    //UART ������ = UART ģ��ʱ�� / (16 �� (SBR[12:0] + BRFA))
    //������ BRFA ������£� SBR = UART ģ��ʱ�� / (16 * UART ������)
    sbr = (uint16)(sysclk / (baud * 16));
    if(sbr > 0x1FFF)sbr = 0x1FFF;                                       //SBR �� 13bit�����Ϊ 0x1FFF

    //��֪ SBR ���� BRFA =  = UART ģ��ʱ�� / UART ������ - 16 ��SBR[12:0]
    brfa = (sysclk / baud)  - (sbr * 16);
    ASSERT( brfa <= 0x1F);                  //���ԣ������ֵ�����������������õ�����������Ĵ���������
                                            //����ͨ����������������������

    //д SBR
    temp = UART_BDH_REG(UARTN[uratn]) & (~UART_BDH_SBR_MASK);           //���� ��� SBR �� UARTx_BDH��ֵ
    UART_BDH_REG(UARTN[uratn]) = temp |  UART_BDH_SBR(sbr >> 8);        //��д��SBR��λ
    UART_BDL_REG(UARTN[uratn]) = UART_BDL_SBR(sbr);                     //��д��SBR��λ

    //д BRFD
    temp = UART_C4_REG(UARTN[uratn]) & (~UART_C4_BRFA_MASK) ;           //���� ��� BRFA �� UARTx_C4 ��ֵ
    UART_C4_REG(UARTN[uratn]) = temp |  UART_C4_BRFA(brfa);             //д��BRFA



    //����FIFO(FIFO���������Ӳ�������ģ�������������)
    UART_PFIFO_REG(UARTN[uratn]) |= (0
                                     | UART_PFIFO_TXFE_MASK               //ʹ��TX FIFO(ע�ͱ�ʾ��ֹ)
                                     //| UART_PFIFO_TXFIFOSIZE(0)         //��ֻ����TX FIFO ��С��0Ϊ1�ֽڣ�1~6Ϊ 2^(n+1)�ֽ�
                                     | UART_PFIFO_RXFE_MASK               //ʹ��RX FIFO(ע�ͱ�ʾ��ֹ)
                                     //| UART_PFIFO_RXFIFOSIZE(0)         //��ֻ����RX FIFO ��С��0Ϊ1�ֽڣ�1~6Ϊ 2^(n+1)�ֽ�
                                    );

    /* �������ͺͽ��� */
    UART_C2_REG(UARTN[uratn]) |= (0
                                  | UART_C2_TE_MASK                     //����ʹ��
                                  | UART_C2_RE_MASK                     //����ʹ��
                                  //| UART_C2_TIE_MASK                  //�����жϻ�DMA��������ʹ�ܣ�ע���˱�ʾ���ã�
                                  //| UART_C2_TCIE_MASK                 //��������ж�ʹ�ܣ�ע���˱�ʾ���ã�
                                  //| UART_C2_RIE_MASK                  //�������жϻ�DMA��������ʹ�ܣ�ע���˱�ʾ���ã�
                                 );

    //�����Ƿ��������պͷ����жϡ�ͨ������ UART ģ��� C2 �Ĵ�����
    //RIE �� TIE λʵ�֡����ʹ���жϣ���������ʵ���жϷ������
}

void uart_getchar (UARTn_e uratn, char *ch)
{
    //�ȴ����յ����ݣ�ʹ����FIFO���������ַ��������У�����һ������˼��
    //while( !UART_RCFIFO_REG(UARTN[uratn]) );                                //�ȴ����յ�����������0
    //while( (UART_SFIFO_REG(UARTN[uratn]) & UART_SFIFO_RXEMPT_MASK)) ;     //�ȴ����ջ�����/FIFO �ǿյ�

    //���·����Ƿ�˼�����ٷ������ṩ�ķ���
    while (!(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK));       //�ȴ���������

    // ��ȡ���յ���8λ����
    *ch =  UART_D_REG(UARTN[uratn]);

    // ��ȡ 9λ���ݣ�Ӧ���ǣ���Ҫ�޸ĺ����ķ������ͣ���
    // *ch =   ((( UARTx_C3_REG(UARTN[uratn]) & UART_C3_R8_MASK ) >> UART_C3_R8_SHIFT ) << 8)   |   UART_D_REG(UARTN[uratn]);  //����9bit

}

char uart_querychar (UARTn_e uratn, char *ch)
{
    if( UART_RCFIFO_REG(UARTN[uratn]) )         //��ѯ�Ƿ���ܵ�����
    {
        *ch  =   UART_D_REG(UARTN[uratn]);      //���ܵ�8λ������
        return  1;                              //���� 1 ��ʾ���ճɹ�
    }

    *ch = 0;                                    //���ղ�����Ӧ������˽�����
    return 0;                                   //����0��ʾ����ʧ��
}

uint32 uart_querystr (UARTn_e uratn, char *str, uint32 max_len)
{
    uint32 i = 0,j;

    for(j=0;j<10000;j++)                 // 10000 ����������ʱ�������и�������޸�
    {
        while(uart_querychar(uratn, str + i)  )
        {
            j=0;
            if( *(str + i) == NULL )    //���յ��ַ���������
            {
                return i;
            }

            i++;
            if(i >= max_len)            //�����趨�����ֵ���˳�
            {
                *(str + i) = 0;     //ȷ���ַ�����β��0x00
                return i;
            }
        }
    }

    *(str + i) = 0;                     //ȷ���ַ�����β��0x00
    return i;
}

uint32 uart_querybuff (UARTn_e uratn, char *buff, uint32 max_len)
{
    uint32 i = 0,j;
    for(j=0;j<10000;j++)                 // 10000 ����������ʱ�������и�������޸�
    {
        while(uart_querychar(uratn, buff + i)  )
        {
            i++;
            if(i >= max_len)            //�����趨�����ֵ���˳�
            {
                return i;
            }
        }
    }

    return i;
}

void uart_putchar (UARTn_e uratn, char ch)
{
    //�ȴ����ͻ�������
    while(!(UART_S1_REG(UARTN[uratn]) & UART_S1_TDRE_MASK));

    //��������
    UART_D_REG(UARTN[uratn]) = (uint8)ch;
}

char uart_query (UARTn_e uratn)
{
    if(UART_RCFIFO_REG(UARTN[uratn]))                 //���յ�����������0
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
    UART_C2_REG(UARTN[uratn]) |= UART_C2_RIE_MASK;                          //ʹ��UART�����ж�
    enable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));                  //ʹ��IRQ�ж�
}

void uart_tx_irq_en(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) |= UART_C2_TIE_MASK;                          //ʹ��UART�����ж�
    enable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));                  //ʹ��IRQ�ж�
}

void uart_txc_irq_en(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) |= UART_C2_TCIE_MASK;                         //ʹ��UART�����ж�
    enable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));                  //ʹ��IRQ�ж�
}

void uart_rx_irq_dis(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) &= ~UART_C2_RIE_MASK;                         //��ֹUART�����ж�

    //��������жϻ�û�йأ��򲻹ر�IRQ
    if(!(UART_C2_REG(UARTN[uratn]) & (UART_C2_TIE_MASK | UART_C2_TCIE_MASK)) )
    {
        disable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));             //��IRQ�ж�
    }
}

void uart_txc_irq_dis(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) &= ~UART_C2_TCIE_MASK;                        //��ֹUART��������ж�

    //��������жϻ�û�йأ��򲻹ر�IRQ
    if(!(UART_C2_REG(UARTN[uratn]) & UART_C2_RIE_MASK) )
    {
        disable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));             //��IRQ�ж�
    }
}

void uart_tx_irq_dis(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) &= ~UART_C2_TIE_MASK;                         //��ֹUART�����ж�

    //��������жϻ�û�йأ��򲻹ر�IRQ
    if(!(UART_C2_REG(UARTN[uratn]) & UART_C2_RIE_MASK) )
    {
        disable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));             //��IRQ�ж�
    }
}

void uart3_test_handler(void)
{
    UARTn_e uratn = UART3;

    if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   //�������ݼĴ�����
    {
    }

    if(UART_S1_REG(UARTN[uratn]) & UART_S1_TDRE_MASK )  //�������ݼĴ�����
    {
    }
}