#include "ImgProc.h"
#include "car.h"

uchar image_threshold;  //ͼ����ֵ
int midline_aver;
int left_1 = 0;//��¼��һ�е���߽�
int right_1 = 0;//��¼��һ�е��ұ߽�
int width_min = 25;//��������쳣������
int left_Add_Start, right_Add_Start;
float left_Last_Slope = 0;
float right_Last_Slope = 0;

int edge_left[120] = { 0 };    //�����
int edge_right[120] = { 0 };   //�ұ���
int MidLine[120] = { 0 };      //����

int width[120] = { 0 };        //�������
uchar Draw_Left[120] = { 0 };   //����߲��߱�־λ
uchar Draw_Right[120] = { 0 };  //�ұ��߲��߱�־λ
int edge_left_New[120] = { 0 };    //�����-��
int edge_right_New[120] = { 0 };   //�ұ���-��

int directions[5][2] = { {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1} };//������Ѱ�����õ�������
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


uint8 threshold(uint8 *image, uint16 col, uint16 row)   //ע�������ֵ��һ��Ҫ��ԭͼ��
{
#define GrayScale 256
	uint16 width = col;
	uint16 height = row;
	int pixelCount[GrayScale];
	float pixelPro[GrayScale];
	int i, j, pixelSum = width * height / 4;
	uint8 threshold = 0;
	uint8* data = image;  //ָ���������ݵ�ָ��
	for (i = 0; i < GrayScale; i++)
	{
		pixelCount[i] = 0;
		pixelPro[i] = 0;
	}

	uint32 gray_sum = 0;
	//ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
	for (i = 0; i < height; i += 2)
	{
		for (j = 0; j < width; j += 2)
		{
			pixelCount[(int)data[i * width + j]]++;  //����ǰ�ĵ������ֵ��Ϊ����������±�
			gray_sum += (int)data[i * width + j];       //�Ҷ�ֵ�ܺ�
		}
	}
	/**/
	//����ÿ������ֵ�ĵ�������ͼ���еı���
	for (i = 0; i < GrayScale; i++)
	{
		pixelPro[i] = (float)pixelCount[i] / pixelSum;

	}

	//�����Ҷȼ�[0,255]
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;

	w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;

	/**/


	for (j = 0; j < GrayScale; j++)
	{

		w0 += pixelPro[j];  //��������ÿ���Ҷ�ֵ�����ص���ռ����֮��   ���������ֵı���
		u0tmp += j * pixelPro[j];  //�������� ÿ���Ҷ�ֵ�ĵ�ı��� *�Ҷ�ֵ

		w1 = 1 - w0;
		u1tmp = gray_sum / pixelSum - u0tmp;

		u0 = u0tmp / w0;              //����ƽ���Ҷ�
		u1 = u1tmp / w1;              //ǰ��ƽ���Ҷ�
		u = u0tmp + u1tmp;            //ȫ��ƽ���Ҷ�
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

void found_l_r(unsigned char *pIn6, int width6)//�ҵ���һ���������ߵ������
{
	//ÿ��������ռ�ֽ��������ͼ��������ͼ����ͬ
	int lineByte = (width6 + 3) / 4 * 4;
	//ѭ��������ͼ�������
	int i = 118;
	//�м����
	float x, y, t;
	// int m = 0;
	 //�ҵ���һ�к͵ڶ��е�����߽紦

	for (int j = 0; j < 94; j++)
	{
		x = *(pIn6 + (i - 1)*lineByte + j + 1) + 2 * *(pIn6 + i * lineByte + j + 1) + *(pIn6 + (i + 1)*lineByte + j + 1) - *(pIn6 + (i - 1)*lineByte + j - 1) - 2 * *(pIn6 + i * lineByte + j - 1) - *(pIn6 + (i + 1)*lineByte + j - 1);
		//y�����ݶ�
		y = *(pIn6 + (i - 1)*lineByte + j - 1) + 2 * *(pIn6 + (i - 1)*lineByte + j) + *(pIn6 + (i - 1)*lineByte + j + 1) - *(pIn6 + (i + 1)*lineByte + j - 1) - 2 * *(pIn6 + (i + 1)*lineByte + j) - *(pIn6 + (i + 1)*lineByte + j + 1);
		t = abs(x) + abs(y) + 0.5;

		if (t > image_threshold)
		{
			*(pIn6 + i * lineByte + j) = 255;//��ɫ�߽�
			left_1 = j;
			break;
		}
	}

	for (int j = 187; j > 94; j--)
	{
		x = *(pIn6 + (i - 1)*lineByte + j + 1) + 2 * *(pIn6 + i * lineByte + j + 1) + *(pIn6 + (i + 1)*lineByte + j + 1) - *(pIn6 + (i - 1)*lineByte + j - 1) - 2 * *(pIn6 + i * lineByte + j - 1) - *(pIn6 + (i + 1)*lineByte + j - 1);
		//y�����ݶ�
		y = *(pIn6 + (i - 1)*lineByte + j - 1) + 2 * *(pIn6 + (i - 1)*lineByte + j) + *(pIn6 + (i - 1)*lineByte + j + 1) - *(pIn6 + (i + 1)*lineByte + j - 1) - 2 * *(pIn6 + (i + 1)*lineByte + j) - *(pIn6 + (i + 1)*lineByte + j + 1);
		t = abs(x) + abs(y) + 0.5;

		if (t > image_threshold)
		{
			*(pIn6 + i * lineByte + j) = 255;//��ɫ�߽�
			right_1 = j;
			break;
		}
	}

}


void fourseek_sobel_2(unsigned char *pIn5, int width5, int height5, unsigned char *pOut5)
{
	float x, y, y1, t, t1;
	int lineByte = (width5 + 3) / 4 * 4;
	int start_x = 118, start_y = left_1, start_y1 = right_1;//������ʼ��ֱ��7
	int now_x = 118, now_y = left_1, now_y1 = right_1;//���嵱ǰ�����26,181
	int counts = 0, curr_d = 0;//��ʼ����ûȷ��
	//���������130

	if (*(pIn5 + now_x * lineByte + now_y) == 255)//�Ѿ�����ͼ������������˵�һ�е����
	{
		int tra_flag = 0;

		while (!tra_flag)
		{
			//ѭ���Ĵ�
			for (counts = 0; counts < 5; counts++)
			{
				//��ֹ��������

				//���µ�ǰ������
				now_x = start_x + directions[curr_d][0];//Ҫ�鿴0�����ֵ�ǲ���255��ɫ
				//����ǣ���һ����ʼ�о͵������Ȼ������һ��ѭ���Ĵ�
				//������ǣ���ô����һ�������жϣ�ͬ��
				//����ĸ����򶼲��ǣ��Ǿ�˵�������Ѿ�����GG��
				now_y = start_y + directions[curr_d][1];
				//���б߽��ж�
				if ((now_x > 0) && (now_x < width5 - 1) && (now_y > 0) && (now_y < height5 - 1))
				{
					//�����������ָ���������sobelȻ��ȥ�ı�ԭ���ĻҶ�����

					x = *(pIn5 + (now_x - 1)*lineByte + now_y + 1) + 2 * *(pIn5 + now_x * lineByte + now_y + 1) + *(pIn5 + (now_x + 1)*lineByte + now_y + 1) - *(pIn5 + (now_x - 1)*lineByte + now_y - 1) - 2 * *(pIn5 + now_x * lineByte + now_y - 1) - *(pIn5 + (now_x + 1)*lineByte + now_y - 1);
					//y�����ݶ�
					y = *(pIn5 + (now_x - 1)*lineByte + now_y - 1) + 2 * *(pIn5 + (now_x - 1)*lineByte + now_y) + *(pIn5 + (now_x - 1)*lineByte + now_y + 1) - *(pIn5 + (now_x + 1)*lineByte + now_y - 1) - 2 * *(pIn5 + (now_x + 1)*lineByte + now_y) - *(pIn5 + (now_x + 1)*lineByte + now_y + 1);
					t = abs(x) + abs(y) + 0.5;

					if (t > image_threshold)
					{
						*(pOut5 + now_x * lineByte + now_y) = 255;//��ɫ�߽�
					}
					else
					{
						*(pOut5 + now_x * lineByte + now_y) = 0;
					}

					//sobel->pOut4�ı���

					//������ڱ�Ե
					if (*(pOut5 + now_x * lineByte + now_y) == 255)
					{
						//���cur = 1����ôcur = 0
						//������ʼ������
						start_x = now_x;
						start_y = now_y;

						if ((curr_d >= 1) && (curr_d < 4))
							curr_d = 0;
						else if (curr_d >= 4)
						{
							curr_d = 1;
						}
						//�������0��ʱ���ҵ��ڵ㣬����ȥ֮��Ͳ�Ҫ0��
						break;//������ѭ��
					}
					else
					{
						//�����ʱ���Ѿ������ڵ��ˣ���ôӦ�õ�����һ���ˣ�
						//���ǲ����Ե����ĵ����ԽǴ�������Խ����Ļ��Ǻ� = 0����ô�����껹Ҫ��һ
						start_x = now_x - directions[curr_d][0];//��Ӧ�ò��䣬��������Ǹ������ѭ����
						//����Ҫ�ȼ�¼һ����һ�еı�Ե��
						start_y = now_y - directions[curr_d][1];//Ȼ����������򣬣���//��Ӧ���Ǽ�һ��Ӧ���ǰ�������������鷵�ظո��Ǹ��ط�
						*(pOut5 + start_x * lineByte + start_y) = 255;//��ɫ�߽�

					 //   Widget::edge_left[now_x] = now_y;
						edge_left[start_x] = start_y;
						if (now_x == 0) { tra_flag = 1; }//������ﶥ���˾Ͳ���Ҫ�ټ���׷����ȥ��
					}
				}
				curr_d = curr_d + 1;
			}

			//���ٵ�ֹͣ��������������򶼲����ڱ�Ե
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

	if (*(pIn5 + now_x * lineByte + now_y1) == 255)//�Ѿ�����ͼ������������˵�һ�е����
	{
		int tra_flag1 = 0;

		while (!tra_flag1)
		{
			//ѭ���Ĵ�
			for (counts = 0; counts < 4; counts++)
			{
				//��ֹ��������
				if (curr_d >= 4)
				{
					//curr_d -= 4;
					tra_flag1 = 1;
				}

				//���µ�ǰ������
				now_x = start_x + directions1[curr_d][0];//Ҫ�鿴0�����ֵ�ǲ���255��ɫ
				//����ǣ���һ����ʼ�о͵������Ȼ������һ��ѭ���Ĵ�
				//������ǣ���ô����һ�������жϣ�ͬ��
				//����ĸ����򶼲��ǣ��Ǿ�˵�������Ѿ�����GG��
				now_y1 = start_y1 + directions1[curr_d][1];

				//���б߽��ж�
				if ((now_x > 0) && (now_x < height5 - 1) && (now_y1 > 0) && (now_y1 < width5 - 1))
				{

					//�����������ָ���������sobelȻ��ȥ�ı�ԭ���ĻҶ�����

					x = *(pIn5 + (now_x - 1)*lineByte + now_y1 + 1) + 2 * *(pIn5 + now_x * lineByte + now_y1 + 1) + *(pIn5 + (now_x + 1)*lineByte + now_y1 + 1) - *(pIn5 + (now_x - 1)*lineByte + now_y1 - 1) - 2 * *(pIn5 + now_x * lineByte + now_y1 - 1) - *(pIn5 + (now_x + 1)*lineByte + now_y1 - 1);
					//y�����ݶ�
					y1 = *(pIn5 + (now_x - 1)*lineByte + now_y1 - 1) + 2 * *(pIn5 + (now_x - 1)*lineByte + now_y1) + *(pIn5 + (now_x - 1)*lineByte + now_y1 + 1) - *(pIn5 + (now_x + 1)*lineByte + now_y1 - 1) - 2 * *(pIn5 + (now_x + 1)*lineByte + now_y1) - *(pIn5 + (now_x + 1)*lineByte + now_y1 + 1);
					t1 = abs(x) + abs(y1) + 0.5;


					if (t1 > image_threshold)
					{
						*(pOut5 + now_x * lineByte + now_y1) = 255;//��ɫ�߽�
					}
					else
					{
						*(pOut5 + now_x * lineByte + now_y1) = 0;
					}

					//sobel->pOut54�ı���

					//������ڱ�Ե
					if (*(pOut5 + now_x * lineByte + now_y1) == 255)
					{
						//������ʼ������
						start_x = now_x;
						start_y1 = now_y1;
						if (curr_d >= 1)
							curr_d = 0;
						break;//������ѭ��
					}
					else
					{
						//�����ʱ���Ѿ������ڵ��ˣ���ôӦ�õ�����һ���ˣ�
						//���ǲ����Ե����ĵ����ԽǴ�������Խ����Ļ��Ǻ� = 0����ô�����껹Ҫ��һ
					   // if(curr_d == 3)
					//	{qDebug() << "the end" << endl;
					//		tra_flag1 = 1;
					//		break;
					//	}//����û��
						start_x = now_x - directions1[curr_d][0];
						start_y1 = now_y1 - directions1[curr_d][1];//Ȼ����������򣬣���

						*(pOut5 + start_x * lineByte + start_y1) = 255;//��ɫ�߽�

						edge_right[start_x] = start_y1;

						if (now_x == 0) { tra_flag1 = 1; }//������ﶥ���˾Ͳ���Ҫ�ټ���׷����ȥ��
						//���ᵽ����
					}
				}
				curr_d = curr_d + 1;
			}

			//���ٵ�ֹͣ��������������򶼲����ڱ�Ե
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

//��ֵ��
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
			temp = *(p + i * 188 + j);//��ȡ���ص�
			if (temp > threshold)
				*(q + i * 188 + j) = 255;//��ɫ�ĵ�������������Ϊ0
			else
				*(q + i * 188 + j) = 0;
		}
	}
}

void send_upper(void)
{
	seekfree_sendimg_032();  //���ͻҶ�ͼ��
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