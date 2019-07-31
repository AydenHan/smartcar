#include "Mode.h"
#include "car.h"

ModeState modestate;
uchar modeflag = 0;

void ModeSelect(void)
{
	if (modeflag == 0)
	{
		modestate = Mode1;  //坡道反方向发车，停车绕路障、正常过断路、坡道加速
	}
	else if (modeflag == 1)
	{
		modestate = Mode2;  //坡道反方向发车，撞绕路障、正常过断路、坡道加速
	}
	else if (modeflag == 2)
	{
		modestate = Mode3;  //坡道正方向发车，坡道加速、恢复切电磁、正常出断路、停车绕路障
	}
	else if (modeflag == 3)
	{
		modestate = Mode4;  //坡道正方向发车，坡道加速、恢复切电磁、正常出断路、撞绕路障
	}
	else if (modeflag == 4)
	{
		modestate = Mode5;  //坡道反方向发车，撞绕路障、正常过断路、坡道识别不加速（低）
	}
	else if (modeflag == 5)
	{
		modestate = Mode6;  //坡道正方向发车，起跑切电磁、正常出断路、绕路障
	}
}

/*
  if (modestate == Mode1)
  {

  }
  else if (modestate == Mode2)
  {

  }
  else if (modestate == Mode3)
  {

  }
  else if (modestate == Mode4)
  {

  }
  else if (modestate == Mode5)
  {

  }
  else if (modestate == Mode6)
  {

  }
s
*/