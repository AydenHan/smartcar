#include "common.h"
#include "MK60_port.h"

PORT_MemMapPtr PORTX[PTX_MAX] = {PORTA_BASE_PTR, PORTB_BASE_PTR, PORTC_BASE_PTR, PORTD_BASE_PTR, PORTE_BASE_PTR};


//Sample usage:       port_init (PTA8, IRQ_RISING | PF | ALT1 | PULLUP );    //��ʼ�� PTA8 �ܽţ������ش����жϣ�����Դ�˲��������ù���ΪGPIO ����������
void  port_init(PTXn_e ptxn, uint32 cfg )
{
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK << PTX(ptxn));                           //����PORTx�˿�

    PORT_ISFR_REG(PORTX_BASE(ptxn)) = (1<<PTn(ptxn));                           // ��ձ�־λ

    PORT_PCR_REG(PORTX_BASE(ptxn), PTn(ptxn)) = cfg;                            // ���ù��� , ȷ������ģʽ ,������������������
}

//Sample usage:       port_init_NoALT (PTA8, IRQ_RISING | PF | PULLUP );    //��ʼ�� PTA8 �ܽţ������ش����жϣ�����Դ�˲���������ԭ�ȸ��ù��ܣ���������
void  port_init_NoALT(PTXn_e ptxn, uint32 cfg)
{
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK << PTX(ptxn));                           //����PORTx�˿�

    PORT_ISFR_REG(PORTX_BASE(ptxn)) = (1<<PTn(ptxn));                           // ��ձ�־λ

    //���cfg���MUX�����ؼĴ������MUX
    cfg &= ~PORT_PCR_MUX_MASK;                      //����MUX �ֶΣ�������Ҫ����ALT������ԭ����ALT��
    cfg |=  (PORT_PCR_REG(PORTX_BASE(ptxn), PTn(ptxn)) & PORT_PCR_MUX_MASK);    //��ȡ�Ĵ��������õ� MUX

    PORT_PCR_REG(PORTX_BASE(ptxn), PTn(ptxn)) = cfg;            // ���ù��� , ȷ������ģʽ ,������������������
}

void porta_handler(void)
{
    uint8  n = 0;    //���ź�

    //PTA6
    n = 6;
    if(PORTA_ISFR & (1 << n))           //PTA6�����ж�
    {
        PORTA_ISFR  = (1 << n);        //д1���жϱ�־λ
    }
}