#ifndef _CAR_STATUS_H
#define _CAR_STATUS_H

#include "car.h"

void Init_OK(void);
void Key_Init(void);

void Key(void);
void key_handler(void);

void Ring_Short(void);
void Ring_Long(void);

#endif
