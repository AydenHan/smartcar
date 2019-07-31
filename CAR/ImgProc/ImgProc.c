#include "ImgProc.h"
#include "car.h"

uchar image_threshold;  //图像阈值
int midline_aver;
int left_1 = 0;//记录第一行的左边界
int right_1 = 0;//记录第一行的右边界
int width_min = 25;//赛道宽度异常的下限
int left_Add_Start, right_Add_Start;
float left_Last_Slope = 0;
float right_Last_Slope = 0;

int edge_left[120] = { 0 };    //左边线
int edge_right[120] = { 0 };   //右边线
int MidLine[120] = { 0 };      //中线

int width[120] = { 0 };        //赛道宽度
uchar Draw_Left[120] = { 0 };   //左边线补线标志位
uchar Draw_Right[120] = { 0 };  //右边线补线标志位
int edge_left_New[120] = { 0 };    //左边线-新
int edge_right_New[120] = { 0 };   //右边线-新

int directions[5][2] = { {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1} };//这是搜寻左线用的五邻域
int directions1[4][2] = { {0, -1}, {-1, -1}, {-1, 0}, {-1, 1} };



int GetMidLine(int *edgel, int *edger)
{
	int i = 0, j = 0;
	int sum = 0,sum1 = 0,sum2 = 0,sum3 = 0;

	float r119_100 = 0.4;
	float r99_80 = 0.4;
	float r79_60 = 0.2;

	/*for (i = 119; i >= 0; i--)
	{
		MidLine[i] = (edgel[i] + edger[i]) / 2;
	}
	
	DrawSideLine();*/

	for (j = 119; j >= 100; j--)
	{
		sum1 += Mid_Line[j];
	}
	for (j = 99; j >= 80; j--)
	{
		sum2 += Mid_Line[j];
	}
	for (j = 79; j >= 60; j--)
	{
		sum3 += Mid_Line[j];
	}
	sum = ( sum1 * r119_100 + sum2 * r99_80 + sum3 * r79_60 ) / 20;

	return sum;
}


uint8 threshold(uint8 *image, uint16 col, uint16 row)   //注意计算阈值的一定要是原图像
{
#define GrayScale 256
	uint16 width = col;
	uint16 height = row;
	int pixelCount[GrayScale];
	float pixelPro[GrayScale];
	int i, j, pixelSum = width * height / 4;
	uint8 threshold = 0;
	uint8* data = image;  //指向像素数据的指针
	for (i = 0; i < GrayScale; i++)
	{
		pixelCount[i] = 0;
		pixelPro[i] = 0;
	}

	uint32 gray_sum = 0;
	//统计灰度级中每个像素在整幅图像中的个数
	for (i = 0; i < height; i += 2)
	{
		for (j = 0; j < width; j += 2)
		{
			pixelCount[(int)data[i * width + j]]++;  //将当前的点的像素值作为计数数组的下标
			gray_sum += (int)data[i * width + j];       //灰度值总和
		}
	}
	/**/
	//计算每个像素值的点在整幅图像中的比例
	for (i = 0; i < GrayScale; i++)
	{
		pixelPro[i] = (float)pixelCount[i] / pixelSum;

	}

	//遍历灰度级[0,255]
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;

	w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;

	/**/


	for (j = 0; j < GrayScale; j++)
	{

		w0 += pixelPro[j];  //背景部分每个灰度值的像素点所占比例之和   即背景部分的比例
		u0tmp += j * pixelPro[j];  //背景部分 每个灰度值的点的比例 *灰度值

		w1 = 1 - w0;
		u1tmp = gray_sum / pixelSum - u0tmp;

		u0 = u0tmp / w0;              //背景平均灰度
		u1 = u1tmp / w1;              //前景平均灰度
		u = u0tmp + u1tmp;            //全局平均灰度
		deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = j;
		}
		if (deltaTmp < deltaMax)
		{
			break;
		}

	}
	return threshold;
}

void found_l_r(unsigned char *pIn6, int width6)//找到第一行左右两边的跳变点
{
	//每行像素所占字节数，输出图像与输入图像相同
	int lineByte = (width6 + 3) / 4 * 4;
	//循环变量，图像的坐标
	int i = 118;
	//中间变量
	float x, y, t;
	// int m = 0;
	 //找到第一行和第二行的跳变边界处

	for (int j = 0; j < 94; j++)
	{
		x = *(pIn6 + (i - 1)*lineByte + j + 1) + 2 * *(pIn6 + i * lineByte + j + 1) + *(pIn6 + (i + 1)*lineByte + j + 1) - *(pIn6 + (i - 1)*lineByte + j - 1) - 2 * *(pIn6 + i * lineByte + j - 1) - *(pIn6 + (i + 1)*lineByte + j - 1);
		//y方向梯度
		y = *(pIn6 + (i - 1)*lineByte + j - 1) + 2 * *(pIn6 + (i - 1)*lineByte + j) + *(pIn6 + (i - 1)*lineByte + j + 1) - *(pIn6 + (i + 1)*lineByte + j - 1) - 2 * *(pIn6 + (i + 1)*lineByte + j) - *(pIn6 + (i + 1)*lineByte + j + 1);
		t = abs(x) + abs(y) + 0.5;

		if (t > image_threshold)
		{
			*(pIn6 + i * lineByte + j) = 255;//白色边界
			left_1 = j;
			break;
		}
	}

	for (int j = 187; j > 94; j--)
	{
		x = *(pIn6 + (i - 1)*lineByte + j + 1) + 2 * *(pIn6 + i * lineByte + j + 1) + *(pIn6 + (i + 1)*lineByte + j + 1) - *(pIn6 + (i - 1)*lineByte + j - 1) - 2 * *(pIn6 + i * lineByte + j - 1) - *(pIn6 + (i + 1)*lineByte + j - 1);
		//y方向梯度
		y = *(pIn6 + (i - 1)*lineByte + j - 1) + 2 * *(pIn6 + (i - 1)*lineByte + j) + *(pIn6 + (i - 1)*lineByte + j + 1) - *(pIn6 + (i + 1)*lineByte + j - 1) - 2 * *(pIn6 + (i + 1)*lineByte + j) - *(pIn6 + (i + 1)*lineByte + j + 1);
		t = abs(x) + abs(y) + 0.5;

		if (t > image_threshold)
		{
			*(pIn6 + i * lineByte + j) = 255;//白色边界
			right_1 = j;
			break;
		}
	}

}


void fourseek_sobel_2(unsigned char *pIn5, int width5, int height5, unsigned char *pOut5)
{
	float x, y, y1, t, t1;
	int lineByte = (width5 + 3) / 4 * 4;
	int start_x = 118, start_y = left_1, start_y1 = right_1;//定义起始点直道7
	int now_x = 118, now_y = left_1, now_y1 = right_1;//定义当前点弯道26,181
	int counts = 0, curr_d = 0;//初始方向还没确定
	//测试右弯道130

	if (*(pIn5 + now_x * lineByte + now_y) == 255)//已经利用图像最近处定好了第一行的起点
	{
		int tra_flag = 0;

		while (!tra_flag)
		{
			//循环四次
			for (counts = 0; counts < 5; counts++)
			{
				//防止索引出街

				//更新当前点坐标
				now_x = start_x + directions[curr_d][0];//要查看0邻域的值是不是255白色
				//如果是，下一个起始行就调到这里，然后再来一波循环四次
				//如果不是，那么就下一个邻域判断，同理
				//如果四个邻域都不是，那就说明车子已经出界GG了
				now_y = start_y + directions[curr_d][1];
				//进行边界判断
				if ((now_x > 0) && (now_x < width5 - 1) && (now_y > 0) && (now_y < height5 - 1))
				{
					//这里计算现在指向的这个点的sobel然后去改变原来的灰度数组

					x = *(pIn5 + (now_x - 1)*lineByte + now_y + 1) + 2 * *(pIn5 + now_x * lineByte + now_y + 1) + *(pIn5 + (now_x + 1)*lineByte + now_y + 1) - *(pIn5 + (now_x - 1)*lineByte + now_y - 1) - 2 * *(pIn5 + now_x * lineByte + now_y - 1) - *(pIn5 + (now_x + 1)*lineByte + now_y - 1);
					//y方向梯度
					y = *(pIn5 + (now_x - 1)*lineByte + now_y - 1) + 2 * *(pIn5 + (now_x - 1)*lineByte + now_y) + *(pIn5 + (now_x - 1)*lineByte + now_y + 1) - *(pIn5 + (now_x + 1)*lineByte + now_y - 1) - 2 * *(pIn5 + (now_x + 1)*lineByte + now_y) - *(pIn5 + (now_x + 1)*lineByte + now_y + 1);
					t = abs(x) + abs(y) + 0.5;

					if (t > image_threshold)
					{
						*(pOut5 + now_x * lineByte + now_y) = 255;//白色边界
					}
					else
					{
						*(pOut5 + now_x * lineByte + now_y) = 0;
					}

					//sobel->pOut4改变了

					//如果存在边缘
					if (*(pOut5 + now_x * lineByte + now_y) == 255)
					{
						//如果cur = 1了那么cur = 0
						//更新起始点坐标
						start_x = now_x;
						start_y = now_y;

						if ((curr_d >= 1) && (curr_d < 4))
							curr_d = 0;
						else if (curr_d >= 4)
						{
							curr_d = 1;
						}
						//如果是在0的时候找到黑点，跳回去之后就不要0了
						break;//跳出内循环
					}
					else
					{
						//如果这时候已经遇到黑点了，那么应该调到下一行了！
						//但是不可以单纯的调到对角处，如果对交出的还是黑 = 0，那么纵坐标还要减一
						start_x = now_x - directions[curr_d][0];//行应该不变，跳回左边那个点继续循搜索
						//但是要先记录一下这一行的边缘点
						start_y = now_y - directions[curr_d][1];//然后继续四邻域，，，//不应该是减一，应该是按照四邻域的数组返回刚刚那个地方
						*(pOut5 + start_x * lineByte + start_y) = 255;//白色边界

					 //   Widget::edge_left[now_x] = now_y;
						edge_left[start_x] = start_y;
						if (now_x == 0) { tra_flag = 1; }//如果到达顶行了就不需要再继续追踪下去了
					}
				}
				curr_d = curr_d + 1;
			}

			//跟踪的停止条件，如果四邻域都不存在边缘
			if (counts == 5)
			{
				curr_d = 0;
				tra_flag = 1;
				break;

			}
		}
	}
	else
	{
		//  qDebug() << "zailai1" << endl;
	}

	start_x = 118;
	now_x = 118;
	curr_d = 0;

	if (*(pIn5 + now_x * lineByte + now_y1) == 255)//已经利用图像最近处定好了第一行的起点
	{
		int tra_flag1 = 0;

		while (!tra_flag1)
		{
			//循环四次
			for (counts = 0; counts < 4; counts++)
			{
				//防止索引出街
				if (curr_d >= 4)
				{
					//curr_d -= 4;
					tra_flag1 = 1;
				}

				//更新当前点坐标
				now_x = start_x + directions1[curr_d][0];//要查看0邻域的值是不是255白色
				//如果是，下一个起始行就调到这里，然后再来一波循环四次
				//如果不是，那么就下一个邻域判断，同理
				//如果四个邻域都不是，那就说明车子已经出界GG了
				now_y1 = start_y1 + directions1[curr_d][1];

				//进行边界判断
				if ((now_x > 0) && (now_x < height5 - 1) && (now_y1 > 0) && (now_y1 < width5 - 1))
				{

					//这里计算现在指向的这个点的sobel然后去改变原来的灰度数组

					x = *(pIn5 + (now_x - 1)*lineByte + now_y1 + 1) + 2 * *(pIn5 + now_x * lineByte + now_y1 + 1) + *(pIn5 + (now_x + 1)*lineByte + now_y1 + 1) - *(pIn5 + (now_x - 1)*lineByte + now_y1 - 1) - 2 * *(pIn5 + now_x * lineByte + now_y1 - 1) - *(pIn5 + (now_x + 1)*lineByte + now_y1 - 1);
					//y方向梯度
					y1 = *(pIn5 + (now_x - 1)*lineByte + now_y1 - 1) + 2 * *(pIn5 + (now_x - 1)*lineByte + now_y1) + *(pIn5 + (now_x - 1)*lineByte + now_y1 + 1) - *(pIn5 + (now_x + 1)*lineByte + now_y1 - 1) - 2 * *(pIn5 + (now_x + 1)*lineByte + now_y1) - *(pIn5 + (now_x + 1)*lineByte + now_y1 + 1);
					t1 = abs(x) + abs(y1) + 0.5;


					if (t1 > image_threshold)
					{
						*(pOut5 + now_x * lineByte + now_y1) = 255;//白色边界
					}
					else
					{
						*(pOut5 + now_x * lineByte + now_y1) = 0;
					}

					//sobel->pOut54改变了

					//如果存在边缘
					if (*(pOut5 + now_x * lineByte + now_y1) == 255)
					{
						//更新起始点坐标
						start_x = now_x;
						start_y1 = now_y1;
						if (curr_d >= 1)
							curr_d = 0;
						break;//跳出内循环
					}
					else
					{
						//如果这时候已经遇到黑点了，那么应该调到下一行了！
						//但是不可以单纯的调到对角处，如果对交出的还是黑 = 0，那么纵坐标还要减一
					   // if(curr_d == 3)
					//	{qDebug() << "the end" << endl;
					//		tra_flag1 = 1;
					//		break;
					//	}//可以没有
						start_x = now_x - directions1[curr_d][0];
						start_y1 = now_y1 - directions1[curr_d][1];//然后继续四邻域，，，

						*(pOut5 + start_x * lineByte + start_y1) = 255;//白色边界

						edge_right[start_x] = start_y1;

						if (now_x == 0) { tra_flag1 = 1; }//如果到达顶行了就不需要再继续追踪下去了
						//不会到这里
					}
				}
				curr_d = curr_d + 1;
			}

			//跟踪的停止条件，如果四邻域都不存在边缘
			if (counts == 4)
			{
				curr_d = 0;
				tra_flag1 = 1;
				break;
			}

		}
	}
	else
	{
	}
}

//二值化
void binary(uint8 *q,uint8 *p, uint8 threshold)
{
	int i, j;
	uint16 temp = 0;

	for (i = 0; i < 120; i++)
	{
		for (j = 0; j < 188; j++)
		{
			*(q + i * 188 + j) = 255;
		}
	}

	for (i = 0; i < 120; i++)
	{
		for (j = 0; j < 188; j++)
		{
			temp = *(p + i * 188 + j);//读取像素点
			if (temp > threshold)
				*(q + i * 188 + j) = 255;//白色的点是赛道，设置为0
			else
				*(q + i * 188 + j) = 0;
		}
	}
}

void send_upper(void)
{
	seekfree_sendimg_032();  //发送灰度图像
}

float range_limit(float duty, float min, float max)
{
	if (duty >= max)
	{
		return max;
	}
	if (duty <= min)
	{
		return min;
	}
	else
	{
		return duty;
	}
}

void int_array_clr(int *p,int a)
{
	int i = 0;

	for (i = 0; i < a; i++)
	{
		p[i] = 0;
	}
}

void uchar_array_clr(uchar *p, int a)
{
	int i = 0;

	for (i = 0; i < a; i++)
	{
		p[i] = 0;
	}
}