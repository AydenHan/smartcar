#include "Mode.h"
#include "car.h"

ModeState modestate;
uchar modeflag = 0;

void ModeSelect(void)
{
	if (modeflag == 0)
	{
		modestate = Mode1;  //�µ������򷢳���ͣ����·�ϡ���������·���µ�����
	}
	else if (modeflag == 1)
	{
		modestate = Mode2;  //�µ������򷢳���ײ��·�ϡ���������·���µ�����
	}
	else if (modeflag == 2)
	{
		modestate = Mode3;  //�µ������򷢳����µ����١��ָ��е�š���������·��ͣ����·��
	}
	else if (modeflag == 3)
	{
		modestate = Mode4;  //�µ������򷢳����µ����١��ָ��е�š���������·��ײ��·��
	}
	else if (modeflag == 4)
	{
		modestate = Mode5;  //�µ������򷢳���ײ��·�ϡ���������·���µ�ʶ�𲻼��٣��ͣ�
	}
	else if (modeflag == 5)
	{
		modestate = Mode6;  //�µ������򷢳��������е�š���������·����·��
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