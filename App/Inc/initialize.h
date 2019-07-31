#ifndef __INITIALIZE_H__
#define __INITIALIZE_H__
#include "common.h"
#include "include.h"
#include "car.h"

extern uint8 imgbuff[];

void Dev_Init();             //模块初始化
void Sys_Init();             //系统初始化（优先级，中断等）
void Sys_NVICInit();        //定时器
void Sys_InterruptInit();         //优先级
void Data_Init();

#endif