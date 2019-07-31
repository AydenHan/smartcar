#include "RoadType.h"
#include "car.h"

RoadInfoTypeDef roadinfo;
RoadType Roadtype = None;
SideState sidestate;

void HandleRoadType(void)
{
	Roadtype = MatchRodeType();
	/**/
	switch (Roadtype)
	{
	case EnterCycle://环岛补线操作
		if (!car.incycle)
		{
			//Ring_On;
			car.incycle = 1;
			roadinfo.cyclestate = NearCycle;
			roadinfo.addlinestate = CycleDraw;
		}
		else
		{
			//刷新数据
		}
		DrawCycleLine();
		//数据清零
		sidestate.CloseCycleCornerL = 0;
		sidestate.CloseCycleCornerR = 0;
		sidestate.LeftLoseSide = 0;
		sidestate.RightLoseSide = 0;
		break;

	case None:
	default:
		car.incycle = 0;//普通补线，暂时用直接跳过这个函数处理
		break;
	}
}

RoadType MatchRodeType(void)
{
	if (car.incycle)//近环了，开始一系列状况判断
	{
		switch (roadinfo.cyclestate)
		{
		case NearCycle://位置1
			car.incyclestate = 1;
			if (roadinfo.rodetype == LeftCycle)
			{
				if (Left_Add[110] && Left_Add[109] && Left_Add[108])
				{
					sidestate.LeftLoseSide = 1;
				    roadinfo.cyclestate = EnterCycleDelay;
				}
			}
			if (roadinfo.rodetype == RightCycle)
			{
				if (Right_Add[110] && Right_Add[109] && Right_Add[108])
				{
					sidestate.RightLoseSide = 1;
					roadinfo.cyclestate = EnterCycleDelay;
				}
			}
			break;

		case EnterCycleDelay://位置2
			car.incyclestate = 2;
			if (roadinfo.rodetype == LeftCycle)
			{
				if (!Left_Add[116] && !Left_Add[114] && !Left_Add[112])
				{
					roadinfo.cyclestate = InCycle;
				}
			}
			if (roadinfo.rodetype == RightCycle)
			{
				if (!Right_Add[116] && !Right_Add[114] && !Right_Add[112])
				{
					roadinfo.cyclestate = InCycle;
				}
			}
			break;

		case InCycle://位置3
			car.incyclestate = 3;
			if (roadinfo.rodetype == RightCycle)//注意右环是左跳变
			{
				if (sidestate.CloseCycleCornerL 
					    && Left_Line[sidestate.CloseCycleCornerL - 2] && Left_Line[sidestate.CloseCycleCornerL - 3])
				{
					sidestate.LeftLoseSide = 1;
					roadinfo.cyclestate = OutCycleDelay;
				}
			}
			if (roadinfo.rodetype == LeftCycle)//同上
			{
				if (sidestate.CloseCycleCornerR 
					    && Right_Line[sidestate.CloseCycleCornerR - 2] && Right_Line[sidestate.CloseCycleCornerR - 3])
				{
					sidestate.RightLoseSide = 1;
					roadinfo.cyclestate = OutCycleDelay;
				}
			}
			break;
		
		case OutCycleDelay://位置4
			car.incyclestate = 4;
			if (Right_Line[114] > 178 && Right_Line[117] > 178 && roadinfo.rodetype == LeftCycle)
			{
				roadinfo.cyclestate = OutCycle;
			}
			if (Left_Line[114] < 10 && Left_Line[117] < 10 && roadinfo.rodetype == RightCycle)
			{
				roadinfo.cyclestate = OutCycle;
			}
			break;

		case OutCycle://位置5
			car.incyclestate = 5;
			if (roadinfo.rodetype == LeftCycle)
			{
				if (sidestate.FarCycleCornerR && Right_Add[110] && Right_Add[109] && Right_Add[108])
				{
					sidestate.LeftLoseSide = 1;
					roadinfo.cyclestate = FarCycle;
				}
			}
			if (roadinfo.rodetype == RightCycle)
			{
				if (sidestate.FarCycleCornerL && Left_Add[110] && Left_Add[109] && Left_Add[108])
				{
					sidestate.RightLoseSide = 1;
					roadinfo.cyclestate = FarCycle;
				}
			}
			break;

		case FarCycle://位置6
			car.incyclestate = 6;
			if (roadinfo.rodetype == LeftCycle)
			{
				if (!Left_Add[116] && !Left_Add[114] && !Left_Add[112])
				{
					car.incycle = 0;
					return None;
				}
			}
			if (roadinfo.rodetype == RightCycle)
			{
				if (!Right_Add[116] && !Right_Add[114] && !Right_Add[112])
				{
					car.incycle = 0;
					return None;
				}
			}
			break;
		default:
			break;
		}
		return EnterCycle;
	}

	return (FindCycle())?EnterCycle:None;
}