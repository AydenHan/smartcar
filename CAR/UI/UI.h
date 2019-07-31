#ifndef _CAR_UI_H
#define _CAR_UI_H

#include "car.h"

void UI_Init(void);
void Display(void);

void Binary_Display(uint8 *p);
void Edge_Display(int *edgel, int *edger);
void Midline_Display(int *midline);
void Data_Display(float num1, float num2, int num3, uchar num4);

#endif