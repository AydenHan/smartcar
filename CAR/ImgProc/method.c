#include "method.h"
#include "car.h"

float left_curvature;//曲率
float right_curvature;//曲率

void DrawCycleLine(void)
{
	uchar i,row,mid;

	if (roadinfo.cyclestate == NearCycle)
	{
		if (roadinfo.rodetype == LeftCycle)
		{
			for (i = 119; i > 40; i--)
			{
				Left_Line_New[i] = 188 - Right_Line[i];
			}
		}
		if (roadinfo.rodetype == RightCycle)
		{
			for (i = 119; i > 40; i--)
			{
			    Right_Line_New[i] = 188 - Left_Line[i];
			}
		}
	}

	if (roadinfo.cyclestate == EnterCycleDelay)
	{
		Ring_Off;
		DrawCurveLineIn();//进环岛补线
	}

	if (roadinfo.cyclestate == InCycle)
	{
		roadinfo.addlinestate = CommonDraw;
	}

	if (roadinfo.cyclestate == OutCycleDelay)
	{
		//Ring_On;
		roadinfo.addlinestate = CycleDraw;
		DrawCurveLineOut();
	}

	if (roadinfo.cyclestate == OutCycle)
	{
		roadinfo.addlinestate = CommonDraw;
	}

	if (roadinfo.cyclestate == FarCycle)
	{
		Ring_Off;
		if (roadinfo.rodetype == LeftCycle)
		{
			row = sidestate.FarCycleCornerR;
			mid = (Right_Line[row] + Right_Line[row + 2]) / 2;

			for (i = 119; i > row; i--)
			{
				Left_Line[i] = Right_Line[i] - mid;
			}
		}
		if (roadinfo.rodetype == RightCycle)
		{
			row = sidestate.FarCycleCornerL;
			mid = (Left_Line[row] + Left_Line[row + 2]) / 2;

			for (i = 119; i > row; i--)
			{
				Right_Line[i] = Left_Line[i] - mid;
			}
		}
	}
}

void DrawCurveLineIn(void)
{
	uchar i,row;
	float slope;

	if (roadinfo.rodetype == LeftCycle )
	{
		row = sidestate.FarCycleCornerR;
		slope = 1.0 * (Right_Line[row] - Right_Line[row - 5]) / 5;
		for (i = row + 1; i < 120; i++)
		{
			Right_Line[i] = (i - row)*slope + Right_Line[row];
		}
	}
	if (roadinfo.rodetype == RightCycle)
	{
		row = sidestate.FarCycleCornerL;
		slope = 1.0 * (Left_Line[row] - Left_Line[row - 5]) / 5;
		for (i = row + 1; i < 120; i++)
		{
			Left_Line[i] = (i - row)*slope + Left_Line[row];
		}
	}
}

void DrawCurveLineOut(void)
{
	uchar i,row;
	float slope;

	if (roadinfo.rodetype == LeftCycle)
	{
		row = sidestate.CloseCycleCornerR;//注意左环是右跳变
		slope = 1.0 * (Right_Line[row + 4] - Right_Line[row]) / 4;
		
		for (i = row - 1; i >40; i--)
		{
			Right_Line[i] = Right_Line[row] - (row - i)*slope;
		}
	}
	if (roadinfo.rodetype == RightCycle)
	{
		row = sidestate.CloseCycleCornerL;//注意右环是左跳变
		slope = 1.0 * (Left_Line[row + 4] - Left_Line[row]) / 4;

		for (i = row - 1; i >40; i--)
		{
			Left_Line[i] = Left_Line[row] - (row - i)*slope;
		}
	}
}

void Get_Curvature(void)
{
	//unsigned int left_ek[]
}
