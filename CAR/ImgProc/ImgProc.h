#ifndef _CAR_IMGPROC_H
#define _CAR_IMGPROC_H

#include "car.h"

void send_upper(void);   //发灰度图像到上位机
uint8 threshold(uint8 *image, uint16 col, uint16 row);  //大津法二值化
int GetMidLine(int *edgel, int *edger);
void found_l_r(unsigned char *pIn6, int width6);
void fourseek_sobel_2(unsigned char *pIn5, int width5, int height5, unsigned char *pOut5);
void DrawSideLine(void); //补线

float range_limit(float duty, float min, float max);//限幅
void int_array_clr(int *p, int a);//清空数组
void uchar_array_clr(uchar *p, int a);
void binary(uint8 *q, uint8 *p, uint8 threshold);//二值化

#endif
