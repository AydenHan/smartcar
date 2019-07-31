#include "common.h"
#include "include.h"
#include "car.h"

extern unsigned char image_threshold ;  //图像阈值
extern int midline_aver;        //中线平均值
int servo_chg;                  //舵机差值

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








