#include "Handle.h"
#include "car.h"
//白色是0，黑色是1

extern uint8 image_edge[120][188];
uint8 Weight[120] = { 1,  1,  1,  1,  1,    1,  1,  1,  1,  2,
					  7,  9, 11, 13, 15,   21, 20, 19, 19, 18,
				     10, 12, 14, 16, 19,   10, 12, 14, 16, 19,
				     10, 12, 14, 16, 17,   17, 14, 12, 12, 10,
				     17, 14, 12, 12, 10,    8,  8,  8,  8,  6,
				      8,  8,  8,  8,  6,    8,  8,  8,  8,  6,
			  		  4,  3,  3,  2,  1,    1,  1,  1,  1,  1,
				  	  4,  3,  3,  2,  1,    1,  1,  1,  1,  1,
					  1,  1,  1,  1,  1,    1,  1,  1,  1,  1,
					  1,  1,  1,  1,  1,    1,  1,  1,  1,  1,
					  1,  1,  1,  1,  1,    1,  1,  1,  1,  1,
				      1,  1,  1,  1,  1,    1,  1,  1,  1,  1
};	//加权平均参数

uchar Left_Line[123], Mid_Line[123], Right_Line[123];//左中右边界线
uchar Left_Line_New[123],Right_Line_New[123];//边界补线的坐标

uchar Width[121], Width_Min;//赛道宽度，最小赛道宽度

uchar Left_Add[121], Right_Add[121];//左右边界补线标志位
uchar Left_Add_Start, Right_Add_Start;//记录补线的开始位置
uchar Left_Add_End, Right_Add_End;//记录补线的结束位置

uchar Left_y, Right_y;//左右边界极值的纵坐标
//uchar Point_x, Point_y;//目标点的横纵坐标

uchar Mid_Count = 0;
uchar gg = 0;
uchar outflag = 0;
uchar finishflag = 0;

uchar threshold_cmp = 100;

uchar Turn_Left = 0, Turn_Right = 0;
uchar secleftside[120] = { 0 }, secrightside[120] = {0};
float angle;
uchar Left_Add_Flag, Right_Add_Flag;//左右边界补线计数
uchar Repair_Flag;//补线标志位
uchar Hazard_Left_Flag, Hazard_Right_Flag;//障碍物标志位

uchar Left_Jam, Right_Jam;//左右干扰信号
uchar StartLine_Flag;//起跑线标志位

int32 Last_Angle = 0;
float Last_Slope = 0;
float Left_Last_Slope = 0;
float Right_Last_Slope = 0;

uint8 Point_Mid = 0;
uint8 Foresight = 0;//前瞻控制速度
uint8 Out_Side = 0;//出界计数

//限幅保护函数
int32 range_protect(int32 duty, int32 min, int32 max)//限幅保护
{
  if(duty >= max)
    return max;
  if(duty <= min)
    return min;
  else
    return duty;
}

void Traversal_Left_Line(uint8 i, uint8 *data, uint8 *Left_Line, uint8 *Right_Line)
//左遍历边界，记录左右边界
{
  char j, White_Flag = 1;
  for(j = 0; j < 186; j++)
  {
    if(White_Flag)//先查找左边界
    {
      if(data[j] && data[j + 1] && data[j + 2])//检测到连续的三个白点，起到滤波的效果
      {
        White_Flag = 0;//左边界已经找到，清楚标志位
        Left_Add[119] = 0;//左边界不需要补线，清除标志位
        *Left_Line = j;//记录当前j值为本行左边界
      }
    }
    else//找到左边界之后查找右边界
    {
      if(!data[j] && !data[j + 1] && !data[j + 2])
        //检测到连续三个黑点，起到滤波的效果
      {
        Right_Add[109] = 0;//右边界不需要补线，清楚标志位
        *Right_Line = j;//记录当前j值为本行右边界
        break;
      }
    }
  }
}

void Traversal_Right_Line(uint8 i, uint8 *data, uint8 *Left_Line, uint8 *Right_Line)
//右遍历边界，记录左右边界
{
  char j, White_Flag = 1;
  for(j = 187; j >= 2; j--)
  {
    if(White_Flag)//先查找右边界
    {
      if(data[j] && data[j - 1] && data[j - 2])//检测到连续的三个白点，起到滤波的效果
      {
        White_Flag = 0;//右边界已经找到，清楚标志位
        Right_Add[119] = 0;//右边界不需要补线，清除标志位
        *Right_Line = j;//记录当前j值为本行右边界
      }
    }
    else//找到右边界之后查找左边界
    {
      if(!data[j] && !data[j - 1] && !data[j - 2])
      //检测到连续三个黑点，起到滤波的效果
      {
        Left_Add[109] = 0;//左边界不需要补线，清楚标志位
        *Left_Line = j;//记录当前j值为本行左边界
        break;
      }
    }
  }
}

uint8 Traversal_Left(uint8 *data)//左遍历边界，返回左右边界的距离
{
  char j, White_Flag = 1;
  uint8 Left_Line = 0, Right_Line = 187;
  for(j = 0; j < 187; j++)
  {
    if(White_Flag)//先查找左边界
    {
      if(data[j] && data[j + 1] && data[j + 2])//检测到连续的三个白点，起到滤波的效果
      {
        Left_Line = j;//记录当前j值为本行左边界
      }
    }
    else
    {
      if(data[j] && data[j + 1] && data[j + 2])//检测到连续的三个白点，起到滤波的效果
      {
        Right_Line = j;//记录当前j值为本行右边界
        break;
      }
    }
  }
  return (Right_Line - Left_Line);
}

uint8 Traversal_Right(uint8 *data)	//右遍历边界, 返回左右边界的距离
{
    char j, White_Flag = 1;
    uint8 Left_Line = 0, Right_Line = 187;

    for (j = 187; j >= 2; j--)
    {
        if (White_Flag)	//先查找右边界
        {
            if (data[j] && data[j-1] && data[j-2])//检测到连续三个白点，起到滤波的效果
            {
                Right_Line = j;//记录当前j值为本行右边界
            }
        }
        else
        {
            if (!data[j] && !data[j-1] && !data[j-2])//检测到连续三个黑点，起到滤波的效果
            {
                Left_Line = j;	//记录当前j值为本行左边界

                break;
            }
        }
    }

    return (Right_Line - Left_Line);
}

void First_Line_Add(uint8 *data)
{
  uint8 i = 109;
  if(Turn_Left)
  {
    Traversal_Left_Line(i, &data[188 * i], &Left_Line[i], &Right_Line[i]);
    //使用左遍历获取边界
  }
  else if(Turn_Right)
  {
    Traversal_Right_Line(i, &data[188 * i], &Left_Line[i], &Right_Line[i]);
  }
  else
  {
    if(Traversal_Left(&data[188 * i]) >= Traversal_Right(&data[188 * i]))
      //左边间距大于右边间距
    {
      Traversal_Left_Line(i, &data[188 * i], &Left_Line[i], &Right_Line[i]);
    //使用左遍历获取边界
    }
    else
    {
      Traversal_Right_Line(i, &data[188 * i], &Left_Line[i], &Right_Line[i]);
    }
  }

  if(Left_Add[i])//如果左边需要补线
  {
    Left_Line[i] = 5;//以前一行的左边界作为本行左边界临时补线位置
  }
  if(Right_Add[i])//如果右边需要补线
  {
    Right_Line[i] = 183;//以前一行的右边界作为本行右边界临时补线位置
  }
}

void Traversal_Left_Side(uint8 i, uint8 *data)//从两边向中间找边界
//仅出现在干扰的情况下
{
  uint8 j;

  for (j = Left_Line[i+1]; j < 186; j++)
  {
//		if (data[80*i+j] && data[80*i+j-1])//检测到连续两个白点，起到滤波的效果
        if (data[188*i+j])//检测到白点
        {
            Left_Add[i] = 0;		 //左边界不需要补线，清除标志位
            Left_Line[i] = j;		 //记录当前j值为第i行左边界

            break;
        }
  }
}

void Traversal_Right_Side(uint8 i, uint8 *data)
{
    uint8 j;

    for (j = Right_Line[i+1]; j > 2; j--)
    {
//		if (data[80*i+j] && data[80*i+j-1])//检测到连续两个白点，起到滤波的效果
        if (data[188*i+j])//检测到白点
        {
            Right_Add[i] = 0;		 //右边界不需要补线，清除标志位
            Right_Line[i] = j;		 //记录当前j值为第i行右边界

            break;
        }
    }
}
/************************** 以下为重点 每行的扫描判定边界和路况 ******************************/

void Traversal_Mid_Line(uint8 i, uint8 *data)	//从中间向两边扫描边界
{
  uint8 j,p = 0,q = 0;  //p、q是防止第二次搜到边界跳变后更新边界值的标志位
  uchar start_flag=0;
  
  for(j = Mid_Line[i + 1]; j >= 3; j--)  //以前一行中点为起点向左查找边界
  {
      if(!data[188 * i + j] && !data[188 * i + j - 1])  //检测到连续两个黑点，起到滤波的效果
      {
		  if (!p)
		  {
			  Left_Add[i] = 0;  //左边界不需要补线，清除标志位
		      Left_Line[i] = j; //记录当前j值为第i行左边界
              p = 1;
		  }
      }

	  //搜到边界了，再往外搜搜，还有可能是十字或者环岛
	  if (p && data[188 * i + j] && !data[188 * i + j - 1] && !data[188 * i + j - 2])  //白黑黑跳变，，蓝色地板好像算白色？
	  {
		  secleftside[i] = j;
	  }

	  //黑白跳变，起跑线检测用，多个
	  if (!data[188 * i + j] && data[188 * i + j - 1])
	  {
		  start_flag++;
	  }

	  if (data[188 * i + j] && !data[188 * i + j - 1] && !data[188 * i + j - 2] && data[188 * i + j - 3])//白黑黑白跳变点，环岛的角
	  {
		  Left_Line[i] = j-1;
		  
	/*	  if (sidestate.CloseCycleCornerL == 1)//计算跳变点的夹角以识别环岛、弯道和斜入十字
		  {
              angle = ( (secleftside[i + 2] - Left_Line[i]) * (Left_Line[i + 2] - Left_Line[i]) + 4)
			      / ( sqrt( pow( (secleftside[i + 2] - Left_Line[i]), 2) + 4) * sqrt( pow( (Left_Line[i + 2] - Left_Line[i]), 2) + 4));
		  }*/
		  
		  if (i > 92 && secleftside[i + 2])
		  {
			  sidestate.CloseCycleCornerL = i;
			  break;
		  }
		  else if (i > 70 && i < 80 && Left_Line[i - 3] < 4)
		  {
			  sidestate.FarCycleCornerL = i;
			  break;
		  }
	  }
  }
  
  for(j = Mid_Line[i + 1]; j < 185; j++)  //以前一行中点为起点向右查找边界
  {
      if(!data[188 * i + j] && !data[188 * i + j + 1])  //检测到连续两个黑点，起到滤波的效果
      {
		  if(!q)
		  { 
			  Right_Add[i] = 0;  //左边界不需要补线，清除标志位
              Right_Line[i] = j; //记录当前j值为第i行左边界
			  q = 1;
		  }  
      }

	  if (q && data[188 * i + j] && !data[188 * i + j + 1] && !data[188 * i + j + 2])
	  {
		  secrightside[i] = j;
	  }

	  if (!data[188 * i + j] && data[188 * i + j + 1])
	  {
		  start_flag++;
	  }

	  if (data[188 * i + j] && !data[188 * i + j + 1] && !data[188 * i + j + 2]  && data[188 * i + j + 3])//白黑黑白跳变点，环岛的角
	  {
		  Right_Line[i] = j + 1;
		  
/*		  if(sidestate.CloseCycleCornerR == 1)//计算跳变点的夹角以识别环岛、弯道和斜入十字
		  { 
			  angle = ((secrightside[i + 2] - Right_Line[i]) * (Right_Line[i + 2] - Right_Line[i]) + 4)
			      / (sqrt(pow((secrightside[i + 2] - Right_Line[i]), 2) + 4) * sqrt(pow((Right_Line[i + 2] - Right_Line[i]), 2) + 4));
		  }*/
		  
		  if (i > 92 && secrightside[i + 2])
		  {
			  sidestate.CloseCycleCornerR = i;
			  break;
		  }
		  else if (i > 70 && i < 80 && Right_Line[i - 3] > 184)
		  {
			  sidestate.FarCycleCornerR = i;
			  break;
		  }
	  }
  }
  if (start_flag >= 7 && i > 100 && i < 110)   //靠近车头了，黑白跳变有好多的也只有起跑线了。。
  {
	  StartLine_Flag = 1;
  }

  if(Left_Add[i])//第一次补线直接补到左右边缘处
  {
    Left_Line[i] = 2;
    Left_Line_New[i] = 2;
  }
  if(Right_Add[i])
  {
    Right_Line[i] = 186;
    Right_Line_New[i] = 186;
  }
}

void Left_Line_Repair(uint8 y, uint8 *data)
{
  int32 temp;
  uint8 i, j, Row;
  float Slope,S_Slope;//斜率
  for(i = 109; i > y; i--)
  {
    if(Left_Add[i])//需要补线
    {
      if(i >= 100)//第109行需要补线，从上往下补线
      {
        Row = y;
        //Slope = 1.0 * (Left_Line[y] - Left_Line[y - 5]) / 5;
		Slope = S_Slope;
        if(Slope > 0)
        {
          Slope = 0;
        }
        for(j = Row + 1; j <= 109; j++)
        {
          temp = (int32)((j - Row) * Slope + Left_Line[Row]);
          Left_Line_New[j] = range_protect(temp, 2, 186);
          data[188 * j + Left_Line_New[j] + 2] = 0;
        }
      }
      else//从下往上补线
      {
        Row = i + 1;//从需要补线的前一行开始补线
        Slope = 1.0 * (Left_Line[Row] - Left_Line[y]) / (Row - y);//计算目标点的斜率
        if(Slope > 0)//左边界补线斜率不能大于0
        {
          Slope = 0;
        }
		S_Slope = Slope;
        for(j = Row; j > y; j--)
        {
          temp = (int32)((j - Row) * Slope + Left_Line[Row]);
          Left_Line_New[j] = range_protect(temp, 2, 186);
          data[188 * j + Left_Line_New[j] + 2] = 0;
        }
      }
      break;//只需要找到补线的起点就可以，一次补完
    }
  }
}

//右边界的补线修正
void Right_Line_Repair(uint8 y, uint8 *data)
{
  int32 temp;
  uint8 i, j, Row;
  float Slope, S_Slope;;

  for(i = 109; i > y; i--)
  {
    if(Right_Add[i])//需要补线
    {
      if(i >= 100)//第109行需要补线，从上往下补线
      {
        Row = y;
        //Slope = 1.0 * (Right_Line[y] - Right_Line[y - 5]) / 5;
		Slope = S_Slope;
        if(Slope < 0)//右边界补线斜率不可以小于0
        {
          Slope = 0;
        }
        for(j = Row + 1; j <= 109; j++)
        {
          temp = (int32)((j - Row) * Slope + Right_Line[Row]);
          Right_Line_New[j] = range_protect(temp, 2, 186);
          data[188 * j + Right_Line_New[j] - 2] = 0;
        }
      }
      else//从下向上补
      {
        Row = i + 1;    //从需要补线的前一行开始补线
        Slope = 1.0 * (Right_Line[Row] - Right_Line[y]) / (Row - y);        //计算目标点斜率
        if(Slope < 0)    //右边界的补线斜率不可以小于0
        {
          Slope = 0;
        }
		S_Slope = Slope;
        for(j = Row; j > y; j--)
        {
          temp = (int32)((j - Row) * Slope + Right_Line[Row]);
          Right_Line_New[j] = range_protect(temp, 2, 186);
          data[188 * j + Right_Line_New[j] - 2] = 0;
          //左移两位在图像上显示
        }
      }
      break;
    }
  }
}

//使用修复过的左右边界重新计算重点
void Mid_Line_Repair(uint8 count, uint8 *data)
{
  uint8 i, temp;
  uint8 Left, Right;

  for(i = 109; i > count; i--)
  {
    if(Left_Add[i] && (Right_Add_Start || Hazard_Left_Flag))
    {
      Left = Left_Line_New[i];
    }
    else
    {
      Left = Left_Line[i];
    }
    if(Right_Add[i] && (Left_Add_Start || Hazard_Right_Flag))
    {
      Right = Right_Line_New[i];
    }
    else
    {
      Right = Right_Line[i];
    }

    temp = (Left + Right) / 2;//左右边界都是修复过的，直接取中值
    Mid_Line[i] = range_protect(temp, 2, 186);//限幅保护

    data[188 * i + Left + 2] = 0;//右移两位显示左边界
    data[188 * i + Right - 2] = 0;//左移两位显示右边界
    data[188 * i + Mid_Line[i]] = 0;//将中线在图像上显示出来


    if(Mid_Line[i] <= 2 || Mid_Line[i] >= 186)
    {
      i--;
      break;
    }
  }

  /*
  for (uint8 k = 13; k < 15; k++)
  {
    for (uint8 j = 0; j < 188; j++)//Left_Line[k]Right_Line[k]
    {
  	  if (data[188 * k + j] != data[188 * k + j ])
	  {
		  StartLine++;
	  }
    }
  }*/

  Mid_Count = 120 - (i + 1);//记录成功分析的中点的个数
}

/******************** 搜 环 大 法 *********************/

uchar FindCycle(void)
{
	uchar i;

	if (sidestate.CloseCycleCornerL)//左环岛位置1
	{
		roadinfo.rodetype = LeftCycle;
		car.incycle = 1;
		return 1;
	}
		
	if (sidestate.CloseCycleCornerR)//右环岛位置1
	{
		roadinfo.rodetype = RightCycle;
		car.incycle = 1;
		return 1;
	}
	return 0;
}

uint8 Image_GetLine(uint8 *data)//获取左右中边界线
{
  uchar count_r = 0;
  uchar count_l = 0;
  uchar i = 119,h = 0,g = 0;//i表示行
  uchar Line_Count, Left_Temp, Right_Temp;

  uint8 temp = 0;
  float Add_Slope;
  Left_Add_Flag = 0;//补线计数
  Right_Add_Flag = 0;

  Hazard_Left_Flag = 0;//左右障碍物标志位清零
  Hazard_Right_Flag = 0;
  Left_Jam = 0;//左右干扰信号清零
  Right_Jam = 0;
  StartLine_Flag = 0;//起跑线标志位清零

  Right_Last_Slope = 0;
  Left_Last_Slope = 0;

  Mid_Count = 0;
  Left_y = 119;
  Right_y = 119;
  Width_Min = 188;
  Repair_Flag = 0; 
  Left_Add_Start = 0;
  Right_Add_Start = 0;
  Left_Add_End = 0;
  Right_Add_End = 0;

  Width[110] = 186;

  Left_Add[110] = 0;
  Right_Add[110] = 0;

  Left_Line[110] = 2;
  Left_Line[111] = 2;
  Left_Line[112] = 1;

  Mid_Line[110] = 94;
  Mid_Line[111] = 94;
  Mid_Line[112] = 94;

  Right_Line[110] = 186;
  Right_Line[111] = 186;
  Right_Line[112] = 187;//人为确定最下方两行图像的边界

  sidestate.FarCycleCornerL = 0;
  sidestate.FarCycleCornerR = 0;
  for (h = 0; h < 120; h++)
  {
	  secleftside[h] = 0;
	  secrightside[h] = 0;
  }

  //只采集前70行的图像，三分之二，因为太远的图像不稳定，参考意义不大,向上提十行，避开电磁

  for(i = 109; i >= 40; i--)
  {
      Left_Add[i] = 1; //扫描每行之前都认为左右边界需要补线
      Right_Add[i] = 1;                 
      
      Traversal_Mid_Line(i, data);	//从前一行中点向两边寻找边界
      if(i >= 100)
      {
        if(Right_Line[i] - Left_Line[i] < 94)//第一行数据异常
        {
          Traversal_Left_Side(i, data);	//重新使用边界扫描
          Traversal_Right_Side(i, data);
        }
      }
   
    /*第一轮对图像进行处理，在需要补线的时候利用斜率向上补线，
    这样补线只是为了向上找边界的时候更加准确，不容易在斜入十字时误判
    补线之后的边界不能够直接使用，仍然要修复之后才可以使用*/

    Width[i] = Right_Line[i] - Left_Line[i];//计算本行赛道宽度，如果本行赛道宽度大于上一行的赛道宽度，认为遇到突变

    if((Width[i] >= Width[i + 1]) || (Width[i] >= Width_Min))
    {
      Repair_Flag += 1;
      if(Left_Add[i + 1])//前一行左边界补线了
      {
          if(Left_Line[i] < Left_Line_New[i + 1] - 1)//和前一行的左边界补线作比较
          {
              Left_Add[i] = 1;
			  Left_Add_Flag += 1;
          }
      }
      else
      {
          if(Left_Line[i] < Left_Line[i + 1] - 1)
          {
              Left_Add[i] = 1;
			  Left_Add_Flag += 1;
          }
      }

      if(Right_Add[i + 1])
      {
          if(Right_Line[i] > Right_Line_New[i + 1] + 1)
          {
              Right_Add[i] = 1;
			  Right_Add_Flag += 1;
          }
      }
      else//如果前一行右边界没有补线
      {
          if(Right_Line[i] > Right_Line[i + 1] + 1)
          {
              Right_Add[i] = 1;
			  Right_Add_Flag += 1;
          }
      }
    }
    else//本行赛道宽度没有大于上一行赛道宽度
    {
	  /*/障碍检测
      if((i != 119) && (i != 118) && (i > 60))//第一行不管，太远的也不管
      {
        if((Right_Line[i] + 6 < Right_Line[i + 1]) && !Right_Add[i + 1] && !Right_Add[i + 2])   //本行和前一行右边界有较大的突变
        {
          if(!data[188 * (i - 1) + Right_Line[i]] && !data[188 * (i - 2) + Right_Line[i]])
          {
            if(!Hazard_Right_Flag)//右侧障碍标志位还没有置位
            {
              Hazard_Right_Flag = i;
            }
          }
          else
          {
            if(!Right_Jam)//如果右侧干扰信号还没有置位
            {
              Right_Jam = i;//如果右侧出现干扰，记录干扰信号的位置
            }
            Traversal_Right_Side(i, data);//重新使用边界寻线法寻找干扰行边界
          }
        }

        if(Left_Line[i] > (Left_Line[i + 1] + 6) && !Left_Add[i + 1] && !Left_Add[i + 2])   //本行和前一行的左边界有较大的突变 
        {
          if(!data[188 * (i - 1) + Left_Line[i]] && !data[188 * (i - 2) + Left_Line[i]])
          {
            if(!Hazard_Left_Flag)//左侧障碍标志位还没有置位
            {
              Hazard_Left_Flag = i;
            }
          }
          else
          {
            if(!Left_Jam)//如果左侧干扰信号还没有置位
            {
              Left_Jam = i;//如果左侧出现干扰，记录干扰信号的位置
            }
            Traversal_Left_Side(i, data);//重新使用边界寻线法寻找干扰行边界
          }
        }
      }*/
    }
    ///障碍检测结束

    if(Left_Add[i])//左边需要补线
    {
      if(!Left_Add_Start)//如果还没有记录开始补线的位置
      {
        Left_Add_Start = i;//记录左边界补线开始位置
        count_l = 1;
      }
      if(i >= 100)//能够识别的行数非常少，不具备参考价值
      {
        Add_Slope = Left_Last_Slope;//使用上一帧图像的左边界斜率
        if(Add_Slope > 0)  
        {
          Add_Slope = 0;
        }
        //temp = (uint8)((i - 119) * Add_Slope + Left_Line[119]);//通过斜率推算补线的位置
        temp = ((i - 108) * Add_Slope + Left_Line[108]);//通过斜率推算补线的位置
      }
      else//如果能够识别的行数已经足够多了，那么就具备参考价值
      {
          /*下面这一小段是自己加的，，修bug*/
          if((count_l == 1) && Left_Add_End)
          {
              Left_Add_Start = i;//记录左边界补线开始位置
              count_l = 0;
          }
        if(Left_Add_Start >= 95)
        {
          Add_Slope = Left_Last_Slope;
        }
        else//使用前几行的斜率进行补线
        {
          Add_Slope = 1.0 * (Left_Line[Left_Add_Start + 6] - Left_Line[Left_Add_Start + 1]) / 5;
          //计算能识别的前几行图像的斜率
        }
        if(Add_Slope > 0)//限幅 
        {
          Add_Slope = 0;
        }
      //  temp = (uint8)((i - (Left_Add_Start + 1)) * Add_Slope + Left_Line[Left_Add_Start + 1]);
        temp = ((i - (Left_Add_Start + 1)) * Add_Slope + Left_Line[Left_Add_Start + 1]);
        //通过斜率推算补线的位置
        Left_Last_Slope = Add_Slope;//更新上次左边界的斜率
      }
      Left_Line_New[i] = range_protect(temp, 2, 186);
      //不直接修改边界，只保存在补线数组里面
    }

    if(Right_Add[i])//右边需要补线
    {
  //    qDebug() <<  "Right need add" << endl;
      if(!Right_Add_Start)//如果还没有记录开始补线的位置
      {
        Right_Add_Start = i;//记录右边界补线开始位置
        count_r = 1;
      }

/*这里有一个Bug就是前面行需要补线后面也需要的时候就会导致后面的不能补下去，就是说只能补一次线*/
      //加了个count标志位，bug修好了

      if(i >= 95)//能够识别的行数非常少，不具备参考价值
      {
        Add_Slope = Right_Last_Slope;//使用上一帧图像的右边界斜率进行补线
        if(Add_Slope < 0)  
        {
          Add_Slope = 0;
        }
        temp = (i - 109) * Add_Slope + Right_Line[109];
        //通过斜率推算出补线的位置
      }
      else
      {
          /*下面这一小段是自己加的，，修bug*/
          if((count_r == 1) && Right_Add_End)
          {
              Right_Add_Start = i;//记录右边界补线开始位置
              count_r = 0;
          }

        if(Right_Add_Start >= 95)
        {
          Add_Slope = Right_Last_Slope;
        }
        else
        {
          Add_Slope = 1.0 * (Right_Line[Right_Add_Start + 1] - Right_Line[Right_Add_Start + 6]) / 5;
          //计算能识别的前几行的图像的斜率
        }
        if(Add_Slope < 0)//限幅    
        {
          Add_Slope = 0;
        }
        temp = ((i - (Right_Add_Start + 1)) * Add_Slope + Right_Line[Right_Add_Start + 1]);    //通过斜率推算补线的位置
        
        Right_Last_Slope = Add_Slope;    //更新上一次右边界的斜率
      }
      Right_Line_New[i] = range_protect(temp, 2, 186);    //不直接修改边界，只保存在补线数组里面
    }

    //两边都需要补线
    if(Left_Add[i] && Right_Add[i])
    {
      Width[i] = Right_Line_New[i] - Left_Line_New[i];    //重新计算本行赛道宽度
	  Left_Temp = Left_Line_New[i];
	  Right_Temp = Right_Line_New[i];
	}
    else//不需要补线或者只有一行需要补线
    {
      if(Left_Add[i])     //左
      {
        Width[i] = Right_Line[i] - Left_Line_New[i];  
		Left_Temp = Left_Line_New[i];
		Right_Temp = Right_Line[i];
      }
      else if(Right_Add[i])     //右
      {
        Width[i] = Right_Line_New[i] - Left_Line[i];
		Left_Temp = Left_Line[i];
		Right_Temp = Right_Line_New[i];
      }   
      else     //无
      {
        Width[i] = Right_Line[i] - Left_Line[i];   //可能会有干扰，也重新计算一次赛道宽度
		Left_Temp = Left_Line[i];
		Right_Temp = Right_Line[i];

      }

      if(Width[i] < Width_Min)
      {
        Width_Min = Width[i];    //更新最小赛道宽度
      }            

      if(!Left_Add[i] && Left_Add[i + 1] && Left_Add[i + 2] && !Left_Add_End)    //左边不需要补线且前一行需要补线，且是左边第一段补线
      {
        Left_Add_End = 1;    //只记录第一段补线的目标点坐标，忽略后面的
        Left_y = i;    //记录左边界补线结束的位置
      }

      if(!Right_Add[i] && Right_Add[i + 1] && Right_Add[i + 2] && !Right_Add_End)   //右边不需要补线且前一行需要补线，且是右边第一段补线 
      {
        Right_Add_End = 1;    //只记录第一段补线的目标点坐标，忽略后面的
        Right_y = i;    //记录右边界补线结束的位置
      }
    }
    Mid_Line[i] = (Left_Temp + Right_Temp) / 2;    //求出本行赛道中点，只用作参考，不在图像上面显示
  }

  Line_Count = i;//保存成功分析的边界的行数

  /************************ 以上都是一轮补线 ***************************/
  

  /************************ 以下为路况分析（太不智能了、、）************************/
   
  //路障分析  && (Width[109] < 10 || Width[109] > 140) && (Width[59] < 10 || Width[59] > 140)

  if ( car.incamera && image_threshold < threshold_cmp && !gg//阈值突变，撞上去了，，但只允许一次。。
	          && count_s == count_delay && speedlll < speedcmp ) //count_s保证发车不进入判断，撞上路障转速为0（+-1？）
  {
	  roadinfo.rodetype = Barrier;
	  gg = 1;
  }

  //断路出口分析
  if ( roadinfo.cutelect == InBreakRoad 
	      && Width[107] < 140 && Width[75] < 140 && Width[70] < 140
	          && Width[107] > validwidthmin && Width[75] > validwidthmin && Width[70] > validwidthmin)
  {
	  Ring_Off;
	  roadinfo.cutelect = OutBreakRoad;
  }

  //直道断路进口
  if (  Width[inbreakrow - 5] < 10 && Width[inbreakrow] > validwidthmin  && Width[inbreakrow] < 140
	       && Width[inbreakrow + 1] > validwidthmin && Width[inbreakrow + 1] < 140 
	           && !StartLine_Flag && speedlll > speedcmp)
  {
	  HCSR04_START();
	  dis_measure();
  }  


  /****************** 第二轮图像处理
             根据边线状态——>判断赛道类型——>补线***************///好像没实现

/**/     //HandleRoadType();  || FindCycle() 
  if (roadinfo.cutelect  == InBreakRoad || Flag_Round2 == 1)
  {
	  if(!car.inbarrier)  //进断路、环岛切电磁
	  {  
		  LEDB_OFF; LEDY_ON; 
		  car.incamera = 0;
	      car.inelec = 1;
	  }
  }
  if (roadinfo.cutelect == OutBreakRoad || Flag_Round5 == 1)  //出断路回摄像头，可以不叫了
  {
	  LEDB_ON; LEDY_OFF;
	  Flag_Round1 = 0; Flag_Round2 = 0;
	  Flag_Round3 = 0; Flag_Round4 = 0; 
	  Flag_Round5 = 0; 
	  car.incamera = 1;
	  car.inelec = 0;
	  roadinfo.cutelect = NoBreakRoad;
  } 
  if (roadinfo.addlinestate == CommonDraw)  //普通二轮补线，因为没用摄像头进环岛，所以可以无视这个if
  { 
	  Left_Line_Repair(Left_y, data);
      Right_Line_Repair(Right_y, data);
  } 

  /*              第三轮图像处理
    使用修复过的左右边界重新计算中点并在图像中显示出来*/

  Mid_Line_Repair(Line_Count, data);  //将原边界和补线边界整合，获得最终处理的三条线

  return Mid_Count;  //有效行
}

int Point_Weight(void)  //对中线进行权重分析处理，得到合适的车的位置
{
	char i;
	int Point,sum = 0;
	static char Last_Point = 91;
	int32 Sum = 0, Weight_Count = 0;
	if (Out_Side)//出界或者摄像头的图像异常
	{
		Point = Last_Point;
		Point_Mid = 2;
	}
	else
	{
		for (i = 109; i > (110 - Mid_Count); i--)
		{
			Sum += Mid_Line[i] * Weight[109 - i];
			Weight_Count += Weight[109 - i];
		}
		Point = range_protect(Sum / Weight_Count, 2, 186);
	}
	return Point;
}

uint8 Point_Handle(void)
{
	uint8 i;
	uint8 Point = 94;
	int32 Sum = 0;

	if (Mid_Count > 20)
	{
		if (Mid_Count > 60)
		{
			Mid_Count = 60;//只用到了60行，做一下限幅
		}
		//计算60行里面前40行的均值
		for (i = 0; i < Mid_Count - 20; i++)
		{
			Sum += Mid_Line[109 - i];
		}
		Point = Sum / (Mid_Count - 20);//前40行的均值
	}
	else
	{
		Point = Mid_Line[110 - Mid_Count];
	}
}

