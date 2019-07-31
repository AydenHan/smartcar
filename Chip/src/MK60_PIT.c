#include "common.h"
#include  "MK60_PIT.h"     //�����жϼ�ʱ��

void pit_init(PITn_e pitn, uint32 cnt)
{
    //PIT �õ��� Bus Clock ����Ƶ��
    //������� = ����Ƶ�� * ʱ��

    ASSERT( cnt > 0 );                          //�ö��Լ�� ʱ����벻��Ϊ 0

    SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;          //ʹ��PITʱ��

    PIT_MCR         = (0
                       //| PIT_MCR_MDIS_MASK       //����PIT��ʱ��ʱ��ѡ��0��ʾʹ��PIT��1��ʾ��ֹPIT��
                       //| PIT_MCR_FRZ_MASK        //����ģʽ��ֹͣ���У�0��ʾ�������У�1��ʾֹͣ���У�
                      );

    PIT_LDVAL(pitn)  = cnt - 1 ;                    //��������ж�ʱ��

    PIT_Flag_Clear(pitn);                           //���жϱ�־λ

    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;        //��ֹPITn��ʱ����������ռ���ֵ��
    PIT_TCTRL(pitn)  = ( 0
                         | PIT_TCTRL_TEN_MASK        //ʹ�� PITn��ʱ��
                         | PIT_TCTRL_TIE_MASK        //��PITn�ж�
                       );

    //enable_irq((int)pitn + PIT0_IRQn);            //���ж�
}

void pit_delay(PITn_e pitn, uint32 cnt)
{
    //PIT �õ��� Bus Clock ����Ƶ��
    //������� = ����Ƶ�� * ʱ��

    ASSERT( cnt > 0 );              //�ö��Լ�� ʱ����벻��Ϊ 0

    SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;                          //ʹ��PITʱ��

    PIT_MCR         &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );    //ʹ��PIT��ʱ��ʱ�� ������ģʽ�¼�������

    PIT_TCTRL(pitn) &= ~( PIT_TCTRL_TEN_MASK );                     //����PIT ���Ա����ü���ֵ��Ч

    PIT_LDVAL(pitn)  = cnt - 1;                                     //��������ж�ʱ��

    PIT_Flag_Clear(pitn);                                           //���жϱ�־λ

    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                        //��ֹPITn��ʱ����������ռ���ֵ��
    PIT_TCTRL(pitn)  = ( 0
                         | PIT_TCTRL_TEN_MASK                        //ʹ�� PITn��ʱ��
                         //| PIT_TCTRL_TIE_MASK                      //��PITn�ж�
                       );

    while( !(PIT_TFLG(pitn)& PIT_TFLG_TIF_MASK));

    PIT_Flag_Clear(pitn);                                           //���жϱ�־λ
}

void pit_time_start(PITn_e pitn)
{
    //PIT �õ��� Bus Clock ����Ƶ��
    //������� = ����Ƶ�� * ʱ��

    SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;                          //ʹ��PITʱ��

    PIT_MCR         &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );    //ʹ��PIT��ʱ��ʱ�� ������ģʽ�¼�������

    PIT_TCTRL(pitn) &= ~( PIT_TCTRL_TEN_MASK );                     //����PIT ���Ա����ü���ֵ��Ч

    PIT_LDVAL(pitn)  = ~0;                                          //��������ж�ʱ��

    PIT_Flag_Clear(pitn);                                           //���жϱ�־λ

    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                        //��ֹPITn��ʱ����������ռ���ֵ��
    PIT_TCTRL(pitn)  = ( 0
                         | PIT_TCTRL_TEN_MASK                        //ʹ�� PITn��ʱ��
                         //| PIT_TCTRL_TIE_MASK                      //��PITn�ж�
                       );
}

uint32 pit_time_get(PITn_e pitn)
{
    uint32 val;

    val = (~0) - PIT_CVAL(pitn);

    if(PIT_TFLG(pitn)& PIT_TFLG_TIF_MASK)                           //�ж��Ƿ�ʱ�䳬ʱ
    {
        PIT_Flag_Clear(pitn);                                       //���жϱ�־λ
        PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                    //��ֹPITn��ʱ����������ռ���ֵ��
        return ~0;
    }

    if(val == (~0))
    {
        val--;              //ȷ�� ������ ~0
    }
    return val;
}

void pit_close(PITn_e pitn)
{
    PIT_Flag_Clear(pitn);                                       //���жϱ�־λ
    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                    //��ֹPITn��ʱ����������ռ���ֵ��
}