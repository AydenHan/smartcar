#ifndef __INITIALIZE_H__
#define __INITIALIZE_H__
#include "common.h"
#include "include.h"
#include "car.h"

extern uint8 imgbuff[];

void Dev_Init();             //ģ���ʼ��
void Sys_Init();             //ϵͳ��ʼ�������ȼ����жϵȣ�
void Sys_NVICInit();        //��ʱ��
void Sys_InterruptInit();         //���ȼ�
void Data_Init();

#endif