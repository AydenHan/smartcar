#ifndef _CAR_HANDLE_H
#define _CAR_HANDLE_H

#include "car.h"

int32 range_protect(int32 duty, int32 min, int32 max);
void Traversal_Left_Line(uint8 i, uint8 *data, uint8 *Left_Line, uint8 *Right_Line);
void Traversal_Right_Line(uint8 i, uint8 *data, uint8 *Left_Line, uint8 *Right_Line);
uint8 Traversal_Left(uint8 *data);
uint8 Traversal_Right(uint8 *data);
void First_Line_Add(uint8 *data);
void Traversal_Left_Side(uint8 i, uint8 *data);
void Traversal_Right_Side(uint8 i, uint8 *data);
void Traversal_Mid_Line(uint8 i, uint8 *data);
void Left_Line_Repair(uint8 y, uint8 *data);
void Right_Line_Repair(uint8 y, uint8 *data);

int Point_Weight(void);
uint8 Point_Handle(void);
uint8 Image_GetLine(uint8 *data);
uchar FindCycle(void);

#endif