#include "Handle.h"
#include "car.h"
//��ɫ��0����ɫ��1

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
};	//��Ȩƽ������

uchar Left_Line[123], Mid_Line[123], Right_Line[123];//�����ұ߽���
uchar Left_Line_New[123],Right_Line_New[123];//�߽粹�ߵ�����

uchar Width[121], Width_Min;//������ȣ���С�������

uchar Left_Add[121], Right_Add[121];//���ұ߽粹�߱�־λ
uchar Left_Add_Start, Right_Add_Start;//��¼���ߵĿ�ʼλ��
uchar Left_Add_End, Right_Add_End;//��¼���ߵĽ���λ��

uchar Left_y, Right_y;//���ұ߽缫ֵ��������
//uchar Point_x, Point_y;//Ŀ���ĺ�������

uchar Mid_Count = 0;
uchar gg = 0;
uchar outflag = 0;
uchar finishflag = 0;

uchar threshold_cmp = 100;

uchar Turn_Left = 0, Turn_Right = 0;
uchar secleftside[120] = { 0 }, secrightside[120] = {0};
float angle;
uchar Left_Add_Flag, Right_Add_Flag;//���ұ߽粹�߼���
uchar Repair_Flag;//���߱�־λ
uchar Hazard_Left_Flag, Hazard_Right_Flag;//�ϰ����־λ

uchar Left_Jam, Right_Jam;//���Ҹ����ź�
uchar StartLine_Flag;//�����߱�־λ

int32 Last_Angle = 0;
float Last_Slope = 0;
float Left_Last_Slope = 0;
float Right_Last_Slope = 0;

uint8 Point_Mid = 0;
uint8 Foresight = 0;//ǰհ�����ٶ�
uint8 Out_Side = 0;//�������

//�޷���������
int32 range_protect(int32 duty, int32 min, int32 max)//�޷�����
{
  if(duty >= max)
    return max;
  if(duty <= min)
    return min;
  else
    return duty;
}

void Traversal_Left_Line(uint8 i, uint8 *data, uint8 *Left_Line, uint8 *Right_Line)
//������߽磬��¼���ұ߽�
{
  char j, White_Flag = 1;
  for(j = 0; j < 186; j++)
  {
    if(White_Flag)//�Ȳ�����߽�
    {
      if(data[j] && data[j + 1] && data[j + 2])//��⵽�����������׵㣬���˲���Ч��
      {
        White_Flag = 0;//��߽��Ѿ��ҵ��������־λ
        Left_Add[119] = 0;//��߽粻��Ҫ���ߣ������־λ
        *Left_Line = j;//��¼��ǰjֵΪ������߽�
      }
    }
    else//�ҵ���߽�֮������ұ߽�
    {
      if(!data[j] && !data[j + 1] && !data[j + 2])
        //��⵽���������ڵ㣬���˲���Ч��
      {
        Right_Add[109] = 0;//�ұ߽粻��Ҫ���ߣ������־λ
        *Right_Line = j;//��¼��ǰjֵΪ�����ұ߽�
        break;
      }
    }
  }
}

void Traversal_Right_Line(uint8 i, uint8 *data, uint8 *Left_Line, uint8 *Right_Line)
//�ұ����߽磬��¼���ұ߽�
{
  char j, White_Flag = 1;
  for(j = 187; j >= 2; j--)
  {
    if(White_Flag)//�Ȳ����ұ߽�
    {
      if(data[j] && data[j - 1] && data[j - 2])//��⵽�����������׵㣬���˲���Ч��
      {
        White_Flag = 0;//�ұ߽��Ѿ��ҵ��������־λ
        Right_Add[119] = 0;//�ұ߽粻��Ҫ���ߣ������־λ
        *Right_Line = j;//��¼��ǰjֵΪ�����ұ߽�
      }
    }
    else//�ҵ��ұ߽�֮�������߽�
    {
      if(!data[j] && !data[j - 1] && !data[j - 2])
      //��⵽���������ڵ㣬���˲���Ч��
      {
        Left_Add[109] = 0;//��߽粻��Ҫ���ߣ������־λ
        *Left_Line = j;//��¼��ǰjֵΪ������߽�
        break;
      }
    }
  }
}

uint8 Traversal_Left(uint8 *data)//������߽磬�������ұ߽�ľ���
{
  char j, White_Flag = 1;
  uint8 Left_Line = 0, Right_Line = 187;
  for(j = 0; j < 187; j++)
  {
    if(White_Flag)//�Ȳ�����߽�
    {
      if(data[j] && data[j + 1] && data[j + 2])//��⵽�����������׵㣬���˲���Ч��
      {
        Left_Line = j;//��¼��ǰjֵΪ������߽�
      }
    }
    else
    {
      if(data[j] && data[j + 1] && data[j + 2])//��⵽�����������׵㣬���˲���Ч��
      {
        Right_Line = j;//��¼��ǰjֵΪ�����ұ߽�
        break;
      }
    }
  }
  return (Right_Line - Left_Line);
}

uint8 Traversal_Right(uint8 *data)	//�ұ����߽�, �������ұ߽�ľ���
{
    char j, White_Flag = 1;
    uint8 Left_Line = 0, Right_Line = 187;

    for (j = 187; j >= 2; j--)
    {
        if (White_Flag)	//�Ȳ����ұ߽�
        {
            if (data[j] && data[j-1] && data[j-2])//��⵽���������׵㣬���˲���Ч��
            {
                Right_Line = j;//��¼��ǰjֵΪ�����ұ߽�
            }
        }
        else
        {
            if (!data[j] && !data[j-1] && !data[j-2])//��⵽���������ڵ㣬���˲���Ч��
            {
                Left_Line = j;	//��¼��ǰjֵΪ������߽�

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
    //ʹ���������ȡ�߽�
  }
  else if(Turn_Right)
  {
    Traversal_Right_Line(i, &data[188 * i], &Left_Line[i], &Right_Line[i]);
  }
  else
  {
    if(Traversal_Left(&data[188 * i]) >= Traversal_Right(&data[188 * i]))
      //��߼������ұ߼��
    {
      Traversal_Left_Line(i, &data[188 * i], &Left_Line[i], &Right_Line[i]);
    //ʹ���������ȡ�߽�
    }
    else
    {
      Traversal_Right_Line(i, &data[188 * i], &Left_Line[i], &Right_Line[i]);
    }
  }

  if(Left_Add[i])//��������Ҫ����
  {
    Left_Line[i] = 5;//��ǰһ�е���߽���Ϊ������߽���ʱ����λ��
  }
  if(Right_Add[i])//����ұ���Ҫ����
  {
    Right_Line[i] = 183;//��ǰһ�е��ұ߽���Ϊ�����ұ߽���ʱ����λ��
  }
}

void Traversal_Left_Side(uint8 i, uint8 *data)//���������м��ұ߽�
//�������ڸ��ŵ������
{
  uint8 j;

  for (j = Left_Line[i+1]; j < 186; j++)
  {
//		if (data[80*i+j] && data[80*i+j-1])//��⵽���������׵㣬���˲���Ч��
        if (data[188*i+j])//��⵽�׵�
        {
            Left_Add[i] = 0;		 //��߽粻��Ҫ���ߣ������־λ
            Left_Line[i] = j;		 //��¼��ǰjֵΪ��i����߽�

            break;
        }
  }
}

void Traversal_Right_Side(uint8 i, uint8 *data)
{
    uint8 j;

    for (j = Right_Line[i+1]; j > 2; j--)
    {
//		if (data[80*i+j] && data[80*i+j-1])//��⵽���������׵㣬���˲���Ч��
        if (data[188*i+j])//��⵽�׵�
        {
            Right_Add[i] = 0;		 //�ұ߽粻��Ҫ���ߣ������־λ
            Right_Line[i] = j;		 //��¼��ǰjֵΪ��i���ұ߽�

            break;
        }
    }
}
/************************** ����Ϊ�ص� ÿ�е�ɨ���ж��߽��·�� ******************************/

void Traversal_Mid_Line(uint8 i, uint8 *data)	//���м�������ɨ��߽�
{
  uint8 j,p = 0,q = 0;  //p��q�Ƿ�ֹ�ڶ����ѵ��߽��������±߽�ֵ�ı�־λ
  uchar start_flag=0;
  
  for(j = Mid_Line[i + 1]; j >= 3; j--)  //��ǰһ���е�Ϊ���������ұ߽�
  {
      if(!data[188 * i + j] && !data[188 * i + j - 1])  //��⵽���������ڵ㣬���˲���Ч��
      {
		  if (!p)
		  {
			  Left_Add[i] = 0;  //��߽粻��Ҫ���ߣ������־λ
		      Left_Line[i] = j; //��¼��ǰjֵΪ��i����߽�
              p = 1;
		  }
      }

	  //�ѵ��߽��ˣ����������ѣ����п�����ʮ�ֻ��߻���
	  if (p && data[188 * i + j] && !data[188 * i + j - 1] && !data[188 * i + j - 2])  //�׺ں����䣬����ɫ�ذ�������ɫ��
	  {
		  secleftside[i] = j;
	  }

	  //�ڰ����䣬�����߼���ã����
	  if (!data[188 * i + j] && data[188 * i + j - 1])
	  {
		  start_flag++;
	  }

	  if (data[188 * i + j] && !data[188 * i + j - 1] && !data[188 * i + j - 2] && data[188 * i + j - 3])//�׺ںڰ�����㣬�����Ľ�
	  {
		  Left_Line[i] = j-1;
		  
	/*	  if (sidestate.CloseCycleCornerL == 1)//���������ļн���ʶ�𻷵��������б��ʮ��
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
  
  for(j = Mid_Line[i + 1]; j < 185; j++)  //��ǰһ���е�Ϊ������Ҳ��ұ߽�
  {
      if(!data[188 * i + j] && !data[188 * i + j + 1])  //��⵽���������ڵ㣬���˲���Ч��
      {
		  if(!q)
		  { 
			  Right_Add[i] = 0;  //��߽粻��Ҫ���ߣ������־λ
              Right_Line[i] = j; //��¼��ǰjֵΪ��i����߽�
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

	  if (data[188 * i + j] && !data[188 * i + j + 1] && !data[188 * i + j + 2]  && data[188 * i + j + 3])//�׺ںڰ�����㣬�����Ľ�
	  {
		  Right_Line[i] = j + 1;
		  
/*		  if(sidestate.CloseCycleCornerR == 1)//���������ļн���ʶ�𻷵��������б��ʮ��
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
  if (start_flag >= 7 && i > 100 && i < 110)   //������ͷ�ˣ��ڰ������кö��Ҳֻ���������ˡ���
  {
	  StartLine_Flag = 1;
  }

  if(Left_Add[i])//��һ�β���ֱ�Ӳ������ұ�Ե��
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
  float Slope,S_Slope;//б��
  for(i = 109; i > y; i--)
  {
    if(Left_Add[i])//��Ҫ����
    {
      if(i >= 100)//��109����Ҫ���ߣ��������²���
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
      else//�������ϲ���
      {
        Row = i + 1;//����Ҫ���ߵ�ǰһ�п�ʼ����
        Slope = 1.0 * (Left_Line[Row] - Left_Line[y]) / (Row - y);//����Ŀ����б��
        if(Slope > 0)//��߽粹��б�ʲ��ܴ���0
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
      break;//ֻ��Ҫ�ҵ����ߵ����Ϳ��ԣ�һ�β���
    }
  }
}

//�ұ߽�Ĳ�������
void Right_Line_Repair(uint8 y, uint8 *data)
{
  int32 temp;
  uint8 i, j, Row;
  float Slope, S_Slope;;

  for(i = 109; i > y; i--)
  {
    if(Right_Add[i])//��Ҫ����
    {
      if(i >= 100)//��109����Ҫ���ߣ��������²���
      {
        Row = y;
        //Slope = 1.0 * (Right_Line[y] - Right_Line[y - 5]) / 5;
		Slope = S_Slope;
        if(Slope < 0)//�ұ߽粹��б�ʲ�����С��0
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
      else//�������ϲ�
      {
        Row = i + 1;    //����Ҫ���ߵ�ǰһ�п�ʼ����
        Slope = 1.0 * (Right_Line[Row] - Right_Line[y]) / (Row - y);        //����Ŀ���б��
        if(Slope < 0)    //�ұ߽�Ĳ���б�ʲ�����С��0
        {
          Slope = 0;
        }
		S_Slope = Slope;
        for(j = Row; j > y; j--)
        {
          temp = (int32)((j - Row) * Slope + Right_Line[Row]);
          Right_Line_New[j] = range_protect(temp, 2, 186);
          data[188 * j + Right_Line_New[j] - 2] = 0;
          //������λ��ͼ������ʾ
        }
      }
      break;
    }
  }
}

//ʹ���޸��������ұ߽����¼����ص�
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

    temp = (Left + Right) / 2;//���ұ߽綼���޸����ģ�ֱ��ȡ��ֵ
    Mid_Line[i] = range_protect(temp, 2, 186);//�޷�����

    data[188 * i + Left + 2] = 0;//������λ��ʾ��߽�
    data[188 * i + Right - 2] = 0;//������λ��ʾ�ұ߽�
    data[188 * i + Mid_Line[i]] = 0;//��������ͼ������ʾ����


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

  Mid_Count = 120 - (i + 1);//��¼�ɹ��������е�ĸ���
}

/******************** �� �� �� �� *********************/

uchar FindCycle(void)
{
	uchar i;

	if (sidestate.CloseCycleCornerL)//�󻷵�λ��1
	{
		roadinfo.rodetype = LeftCycle;
		car.incycle = 1;
		return 1;
	}
		
	if (sidestate.CloseCycleCornerR)//�һ���λ��1
	{
		roadinfo.rodetype = RightCycle;
		car.incycle = 1;
		return 1;
	}
	return 0;
}

uint8 Image_GetLine(uint8 *data)//��ȡ�����б߽���
{
  uchar count_r = 0;
  uchar count_l = 0;
  uchar i = 119,h = 0,g = 0;//i��ʾ��
  uchar Line_Count, Left_Temp, Right_Temp;

  uint8 temp = 0;
  float Add_Slope;
  Left_Add_Flag = 0;//���߼���
  Right_Add_Flag = 0;

  Hazard_Left_Flag = 0;//�����ϰ����־λ����
  Hazard_Right_Flag = 0;
  Left_Jam = 0;//���Ҹ����ź�����
  Right_Jam = 0;
  StartLine_Flag = 0;//�����߱�־λ����

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
  Right_Line[112] = 187;//��Ϊȷ�����·�����ͼ��ı߽�

  sidestate.FarCycleCornerL = 0;
  sidestate.FarCycleCornerR = 0;
  for (h = 0; h < 120; h++)
  {
	  secleftside[h] = 0;
	  secrightside[h] = 0;
  }

  //ֻ�ɼ�ǰ70�е�ͼ������֮������Ϊ̫Զ��ͼ���ȶ����ο����岻��,������ʮ�У��ܿ����

  for(i = 109; i >= 40; i--)
  {
      Left_Add[i] = 1; //ɨ��ÿ��֮ǰ����Ϊ���ұ߽���Ҫ����
      Right_Add[i] = 1;                 
      
      Traversal_Mid_Line(i, data);	//��ǰһ���е�������Ѱ�ұ߽�
      if(i >= 100)
      {
        if(Right_Line[i] - Left_Line[i] < 94)//��һ�������쳣
        {
          Traversal_Left_Side(i, data);	//����ʹ�ñ߽�ɨ��
          Traversal_Right_Side(i, data);
        }
      }
   
    /*��һ�ֶ�ͼ����д�������Ҫ���ߵ�ʱ������б�����ϲ��ߣ�
    ��������ֻ��Ϊ�������ұ߽��ʱ�����׼ȷ����������б��ʮ��ʱ����
    ����֮��ı߽粻�ܹ�ֱ��ʹ�ã���ȻҪ�޸�֮��ſ���ʹ��*/

    Width[i] = Right_Line[i] - Left_Line[i];//���㱾��������ȣ��������������ȴ�����һ�е�������ȣ���Ϊ����ͻ��

    if((Width[i] >= Width[i + 1]) || (Width[i] >= Width_Min))
    {
      Repair_Flag += 1;
      if(Left_Add[i + 1])//ǰһ����߽粹����
      {
          if(Left_Line[i] < Left_Line_New[i + 1] - 1)//��ǰһ�е���߽粹�����Ƚ�
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
      else//���ǰһ���ұ߽�û�в���
      {
          if(Right_Line[i] > Right_Line[i + 1] + 1)
          {
              Right_Add[i] = 1;
			  Right_Add_Flag += 1;
          }
      }
    }
    else//�����������û�д�����һ���������
    {
	  /*/�ϰ����
      if((i != 119) && (i != 118) && (i > 60))//��һ�в��ܣ�̫Զ��Ҳ����
      {
        if((Right_Line[i] + 6 < Right_Line[i + 1]) && !Right_Add[i + 1] && !Right_Add[i + 2])   //���к�ǰһ���ұ߽��нϴ��ͻ��
        {
          if(!data[188 * (i - 1) + Right_Line[i]] && !data[188 * (i - 2) + Right_Line[i]])
          {
            if(!Hazard_Right_Flag)//�Ҳ��ϰ���־λ��û����λ
            {
              Hazard_Right_Flag = i;
            }
          }
          else
          {
            if(!Right_Jam)//����Ҳ�����źŻ�û����λ
            {
              Right_Jam = i;//����Ҳ���ָ��ţ���¼�����źŵ�λ��
            }
            Traversal_Right_Side(i, data);//����ʹ�ñ߽�Ѱ�߷�Ѱ�Ҹ����б߽�
          }
        }

        if(Left_Line[i] > (Left_Line[i + 1] + 6) && !Left_Add[i + 1] && !Left_Add[i + 2])   //���к�ǰһ�е���߽��нϴ��ͻ�� 
        {
          if(!data[188 * (i - 1) + Left_Line[i]] && !data[188 * (i - 2) + Left_Line[i]])
          {
            if(!Hazard_Left_Flag)//����ϰ���־λ��û����λ
            {
              Hazard_Left_Flag = i;
            }
          }
          else
          {
            if(!Left_Jam)//����������źŻ�û����λ
            {
              Left_Jam = i;//��������ָ��ţ���¼�����źŵ�λ��
            }
            Traversal_Left_Side(i, data);//����ʹ�ñ߽�Ѱ�߷�Ѱ�Ҹ����б߽�
          }
        }
      }*/
    }
    ///�ϰ�������

    if(Left_Add[i])//�����Ҫ����
    {
      if(!Left_Add_Start)//�����û�м�¼��ʼ���ߵ�λ��
      {
        Left_Add_Start = i;//��¼��߽粹�߿�ʼλ��
        count_l = 1;
      }
      if(i >= 100)//�ܹ�ʶ��������ǳ��٣����߱��ο���ֵ
      {
        Add_Slope = Left_Last_Slope;//ʹ����һ֡ͼ�����߽�б��
        if(Add_Slope > 0)  
        {
          Add_Slope = 0;
        }
        //temp = (uint8)((i - 119) * Add_Slope + Left_Line[119]);//ͨ��б�����㲹�ߵ�λ��
        temp = ((i - 108) * Add_Slope + Left_Line[108]);//ͨ��б�����㲹�ߵ�λ��
      }
      else//����ܹ�ʶ��������Ѿ��㹻���ˣ���ô�;߱��ο���ֵ
      {
          /*������һС�����Լ��ӵģ�����bug*/
          if((count_l == 1) && Left_Add_End)
          {
              Left_Add_Start = i;//��¼��߽粹�߿�ʼλ��
              count_l = 0;
          }
        if(Left_Add_Start >= 95)
        {
          Add_Slope = Left_Last_Slope;
        }
        else//ʹ��ǰ���е�б�ʽ��в���
        {
          Add_Slope = 1.0 * (Left_Line[Left_Add_Start + 6] - Left_Line[Left_Add_Start + 1]) / 5;
          //������ʶ���ǰ����ͼ���б��
        }
        if(Add_Slope > 0)//�޷� 
        {
          Add_Slope = 0;
        }
      //  temp = (uint8)((i - (Left_Add_Start + 1)) * Add_Slope + Left_Line[Left_Add_Start + 1]);
        temp = ((i - (Left_Add_Start + 1)) * Add_Slope + Left_Line[Left_Add_Start + 1]);
        //ͨ��б�����㲹�ߵ�λ��
        Left_Last_Slope = Add_Slope;//�����ϴ���߽��б��
      }
      Left_Line_New[i] = range_protect(temp, 2, 186);
      //��ֱ���޸ı߽磬ֻ�����ڲ�����������
    }

    if(Right_Add[i])//�ұ���Ҫ����
    {
  //    qDebug() <<  "Right need add" << endl;
      if(!Right_Add_Start)//�����û�м�¼��ʼ���ߵ�λ��
      {
        Right_Add_Start = i;//��¼�ұ߽粹�߿�ʼλ��
        count_r = 1;
      }

/*������һ��Bug����ǰ������Ҫ���ߺ���Ҳ��Ҫ��ʱ��ͻᵼ�º���Ĳ��ܲ���ȥ������˵ֻ�ܲ�һ����*/
      //���˸�count��־λ��bug�޺���

      if(i >= 95)//�ܹ�ʶ��������ǳ��٣����߱��ο���ֵ
      {
        Add_Slope = Right_Last_Slope;//ʹ����һ֡ͼ����ұ߽�б�ʽ��в���
        if(Add_Slope < 0)  
        {
          Add_Slope = 0;
        }
        temp = (i - 109) * Add_Slope + Right_Line[109];
        //ͨ��б����������ߵ�λ��
      }
      else
      {
          /*������һС�����Լ��ӵģ�����bug*/
          if((count_r == 1) && Right_Add_End)
          {
              Right_Add_Start = i;//��¼�ұ߽粹�߿�ʼλ��
              count_r = 0;
          }

        if(Right_Add_Start >= 95)
        {
          Add_Slope = Right_Last_Slope;
        }
        else
        {
          Add_Slope = 1.0 * (Right_Line[Right_Add_Start + 1] - Right_Line[Right_Add_Start + 6]) / 5;
          //������ʶ���ǰ���е�ͼ���б��
        }
        if(Add_Slope < 0)//�޷�    
        {
          Add_Slope = 0;
        }
        temp = ((i - (Right_Add_Start + 1)) * Add_Slope + Right_Line[Right_Add_Start + 1]);    //ͨ��б�����㲹�ߵ�λ��
        
        Right_Last_Slope = Add_Slope;    //������һ���ұ߽��б��
      }
      Right_Line_New[i] = range_protect(temp, 2, 186);    //��ֱ���޸ı߽磬ֻ�����ڲ�����������
    }

    //���߶���Ҫ����
    if(Left_Add[i] && Right_Add[i])
    {
      Width[i] = Right_Line_New[i] - Left_Line_New[i];    //���¼��㱾���������
	  Left_Temp = Left_Line_New[i];
	  Right_Temp = Right_Line_New[i];
	}
    else//����Ҫ���߻���ֻ��һ����Ҫ����
    {
      if(Left_Add[i])     //��
      {
        Width[i] = Right_Line[i] - Left_Line_New[i];  
		Left_Temp = Left_Line_New[i];
		Right_Temp = Right_Line[i];
      }
      else if(Right_Add[i])     //��
      {
        Width[i] = Right_Line_New[i] - Left_Line[i];
		Left_Temp = Left_Line[i];
		Right_Temp = Right_Line_New[i];
      }   
      else     //��
      {
        Width[i] = Right_Line[i] - Left_Line[i];   //���ܻ��и��ţ�Ҳ���¼���һ���������
		Left_Temp = Left_Line[i];
		Right_Temp = Right_Line[i];

      }

      if(Width[i] < Width_Min)
      {
        Width_Min = Width[i];    //������С�������
      }            

      if(!Left_Add[i] && Left_Add[i + 1] && Left_Add[i + 2] && !Left_Add_End)    //��߲���Ҫ������ǰһ����Ҫ���ߣ�������ߵ�һ�β���
      {
        Left_Add_End = 1;    //ֻ��¼��һ�β��ߵ�Ŀ������꣬���Ժ����
        Left_y = i;    //��¼��߽粹�߽�����λ��
      }

      if(!Right_Add[i] && Right_Add[i + 1] && Right_Add[i + 2] && !Right_Add_End)   //�ұ߲���Ҫ������ǰһ����Ҫ���ߣ������ұߵ�һ�β��� 
      {
        Right_Add_End = 1;    //ֻ��¼��һ�β��ߵ�Ŀ������꣬���Ժ����
        Right_y = i;    //��¼�ұ߽粹�߽�����λ��
      }
    }
    Mid_Line[i] = (Left_Temp + Right_Temp) / 2;    //������������е㣬ֻ�����ο�������ͼ��������ʾ
  }

  Line_Count = i;//����ɹ������ı߽������

  /************************ ���϶���һ�ֲ��� ***************************/
  

  /************************ ����Ϊ·��������̫�������ˡ�����************************/
   
  //·�Ϸ���  && (Width[109] < 10 || Width[109] > 140) && (Width[59] < 10 || Width[59] > 140)

  if ( car.incamera && image_threshold < threshold_cmp && !gg//��ֵͻ�䣬ײ��ȥ�ˣ�����ֻ����һ�Ρ���
	          && count_s == count_delay && speedlll < speedcmp ) //count_s��֤�����������жϣ�ײ��·��ת��Ϊ0��+-1����
  {
	  roadinfo.rodetype = Barrier;
	  gg = 1;
  }

  //��·���ڷ���
  if ( roadinfo.cutelect == InBreakRoad 
	      && Width[107] < 140 && Width[75] < 140 && Width[70] < 140
	          && Width[107] > validwidthmin && Width[75] > validwidthmin && Width[70] > validwidthmin)
  {
	  Ring_Off;
	  roadinfo.cutelect = OutBreakRoad;
  }

  //ֱ����·����
  if (  Width[inbreakrow - 5] < 10 && Width[inbreakrow] > validwidthmin  && Width[inbreakrow] < 140
	       && Width[inbreakrow + 1] > validwidthmin && Width[inbreakrow + 1] < 140 
	           && !StartLine_Flag && speedlll > speedcmp)
  {
	  HCSR04_START();
	  dis_measure();
  }  


  /****************** �ڶ���ͼ����
             ���ݱ���״̬����>�ж��������͡���>����***************///����ûʵ��

/**/     //HandleRoadType();  || FindCycle() 
  if (roadinfo.cutelect  == InBreakRoad || Flag_Round2 == 1)
  {
	  if(!car.inbarrier)  //����·�������е��
	  {  
		  LEDB_OFF; LEDY_ON; 
		  car.incamera = 0;
	      car.inelec = 1;
	  }
  }
  if (roadinfo.cutelect == OutBreakRoad || Flag_Round5 == 1)  //����·������ͷ�����Բ�����
  {
	  LEDB_ON; LEDY_OFF;
	  Flag_Round1 = 0; Flag_Round2 = 0;
	  Flag_Round3 = 0; Flag_Round4 = 0; 
	  Flag_Round5 = 0; 
	  car.incamera = 1;
	  car.inelec = 0;
	  roadinfo.cutelect = NoBreakRoad;
  } 
  if (roadinfo.addlinestate == CommonDraw)  //��ͨ���ֲ��ߣ���Ϊû������ͷ�����������Կ����������if
  { 
	  Left_Line_Repair(Left_y, data);
      Right_Line_Repair(Right_y, data);
  } 

  /*              ������ͼ����
    ʹ���޸��������ұ߽����¼����е㲢��ͼ������ʾ����*/

  Mid_Line_Repair(Line_Count, data);  //��ԭ�߽�Ͳ��߽߱����ϣ�������մ����������

  return Mid_Count;  //��Ч��
}

int Point_Weight(void)  //�����߽���Ȩ�ط��������õ����ʵĳ���λ��
{
	char i;
	int Point,sum = 0;
	static char Last_Point = 91;
	int32 Sum = 0, Weight_Count = 0;
	if (Out_Side)//�����������ͷ��ͼ���쳣
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
			Mid_Count = 60;//ֻ�õ���60�У���һ���޷�
		}
		//����60������ǰ40�еľ�ֵ
		for (i = 0; i < Mid_Count - 20; i++)
		{
			Sum += Mid_Line[109 - i];
		}
		Point = Sum / (Mid_Count - 20);//ǰ40�еľ�ֵ
	}
	else
	{
		Point = Mid_Line[110 - Mid_Count];
	}
}

