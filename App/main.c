#include "common.h"
#include "include.h"
#include "car.h"

extern unsigned char image_threshold ;  //ͼ����ֵ
extern int midline_aver;        //����ƽ��ֵ
int servo_chg;                  //�����ֵ

extern int edge_left[120];
extern int edge_right[120];
extern int MidLine[120];

CarInfoTypeDef car;

void main(void)
{
      Sys_Init();
      Dev_Init();

	  while(1)
      {
		  Run();
      }
}








