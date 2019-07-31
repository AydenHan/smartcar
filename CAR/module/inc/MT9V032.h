#ifndef __MT9V032_H
#define __MT9V032_H

#include "include.h"
//#include "car.h"

#define COL   188 //ͼ����  ��Χ1-752  K60�ɼ���������188
#define ROW   120  //ͼ��߶�  ��Χ1-480
#define CAMERA_SIZE COL*ROW
//188*70=13160
//188/2 = 94
//70/2 = 35
#define L94   94
#define R35   35

#define OLEDV  94
#define OLEDH  35

//����ͷ����ö��
typedef enum
{
    INIT = 0,               //����ͷ��ʼ������
    AUTO_EXP,               //�Զ��ع�����
    EXP_TIME,               //�ع�ʱ������
    FPS,                    //����ͷ֡������
    SET_COL,                //ͼ��������
    SET_ROW,                //ͼ��������
    LR_OFFSET,              //ͼ������ƫ������
    UD_OFFSET,              //ͼ������ƫ������
    GAIN,                   //ͼ��ƫ������
    CONFIG_FINISH,          //������λ����Ҫ����ռλ����

    SET_EXP_TIME = 0XF0,    //���������ع�ʱ������
    GET_STATUS,             //��ȡ����ͷ��������
    GET_VERSION,            //�̼��汾������

    SET_ADDR = 0XFE,        //�Ĵ�����ַ����
    SET_DATA                //�Ĵ�����������
}CMD;

extern uint8 mt9v032_finish_flag;  //һ��ͼ��ɼ���ɱ�־λ
extern uint8 image[ROW][COL];      //�ڲ�ʹ��
extern uint8 image1[ROW][COL];      //�ڲ�ʹ��
extern uint8 image2[ROW*COL];      //�ڲ�ʹ��

extern uint8   Copy_Image[R35][L94];

//��������

/*
        MT9V032���߶��壺
        ------------------------------------
        ģ��ܽ�            ��Ƭ���ܽ�
        SDA(51��RX)         C17
        SCL(51��TX)         C16
        ���ж�              C6
        ���ж�				δʹ�ã���˲���
        �����ж�            C18
        ���ݿ�              C8-C15
        ------------------------------------

        Ĭ�Ϸֱ�����            188*120
        Ĭ��FPS                 50֡
*/

#define MT9V032_COF_UART    UART3  //��������ͷ��ʹ�õ��Ĵ���

//--------------------------------------------------------------------------------------------------
//        ����޸��˳��ж�������ôע���޸�PORTC_IRQn��PORTC_ISFR��
//        ����ʹ�õ���PTA�����������ô���������޸�ΪPORTA_IRQn��PORTA_ISFR����
//        ������ڶ�Ӧ���жϺ����е���VSYNC����
//--------------------------------------------------------------------------------------------------
#define MT9V032_PCLK                PTC18                            	//����ͷ����ʱ��

#define MT9V032_VSYNC               PTC6                            	//����ͷ��ͬ���ź�
#define MT9V032_VSYNC_CHANNEL       6                               //MT9V032_VSYNC��Ӧ���ŵ����ź� PTC6   -->     6
#define MT9V032_INTERRUPT_NUNBERS   PORTC_IRQn                      //�жϱ��
#define MT9V032_INTERRUPT_ISFR      PORTC_ISFR//�жϱ�־λ
//#define MT9V032_INTERRUPT_ISFR      PORTC->ISFR

//--------------------------------------------------------------------------------------------------
//        ����ͷ���ݽӿ�
//        C_IN_DATA(1)
//        ��һ����ĸC������ʹ��C�ڣ�����ѡ��A��B��C��D��E.�ֱ��ӦA_IN_DATA��B_IN_DATA��C_IN_DATA��D_IN_DATA��E_IN_DATA
//        �ڶ�����������Ĳ���1������ʹ�õ���C8 - C15��,
//		  ����ѡ��0��1��2��3���ֱ��Ӧ����	�ö˿ڵ�X0-X7��X8-X15��X16-X23��X24-X31(����x������ǵ�һ����ĸ���������ڵľ���C8-C15)
//--------------------------------------------------------------------------------------------------

#define MT9V032_DATAPORT 		PTC_B1_IN	                //DMA���ݿ�->D0-D7

void   mt9v032_cof_uart_interrupt(void);
void   VSYNC(void);
void   camera_init(void);
void   seekfree_sendimg_032(void);
void   row_finished(void);

void Copy_Arr(unsigned char aa[], unsigned char b[]);
void BinaryZation(unsigned char (*p)[L94]);//��ֵ��
void Image_Filter(void);//

#endif