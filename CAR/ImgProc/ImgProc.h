#ifndef _CAR_IMGPROC_H
#define _CAR_IMGPROC_H

#include "car.h"

void send_upper(void);   //���Ҷ�ͼ����λ��
uint8 threshold(uint8 *image, uint16 col, uint16 row);  //��򷨶�ֵ��
int GetMidLine(int *edgel, int *edger);
void found_l_r(unsigned char *pIn6, int width6);
void fourseek_sobel_2(unsigned char *pIn5, int width5, int height5, unsigned char *pOut5);
void DrawSideLine(void); //����

float range_limit(float duty, float min, float max);//�޷�
void int_array_clr(int *p, int a);//�������
void uchar_array_clr(uchar *p, int a);
void binary(uint8 *q, uint8 *p, uint8 threshold);//��ֵ��

#endif
