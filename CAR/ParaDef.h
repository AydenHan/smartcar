#ifndef _CAR_PARADEF_
#define _CAR_PARADEF_

/*****************���涼��һЩ ���������� �Ķ���*******************/

// SERVO ���
#define SERVO_PORT	FTM0
#define SERVO_CH    FTM_CH2
#define SERVO_FREQ	50    //S_D5 300
#define SERVO_MIDDLE	750   //��ֵ��1.5ms��  S_D5 4500

//MOTOR ����˿�
#define MOTOR_FTM       FTM3
#define MOTORL_Z_PWM     FTM_CH0
#define MOTORL_F_PWM     FTM_CH1
#define MOTORR_Z_PWM     FTM_CH2
#define MOTORR_F_PWM     FTM_CH3
#define MOTOR_FREQ      (15*1000)    //Ƶ��15k
#define MOTOR_MAX       9900

//ENCODER ������
#define ENCODER_L_FTM	FTM1
#define ENCODER_R_FTM	FTM2

//HC-SR04 ������
#define TRIG  PTD0
#define ECHO  PTD1

//RING ������
#define Ring_Off  (gpio_init(PTE26, GPO, 0))
#define Ring_On   (gpio_init(PTE26, GPO, 1))
#define Ring_Turn (gpio_turn(PTE26))

//LED ��
#define LEDY_ON   (led (LED0, LED_ON))   //PTE0 ��
#define LEDY_OFF  (led (LED0, LED_OFF))  //PTE0 ��
#define LEDB_ON   (led (LED1, LED_ON))   //PTE2 ��
#define LEDB_OFF  (led (LED1, LED_OFF))  //PTE2 ��

/*****************���涼��һЩ ������Ҫ��ȡ �Ĵ���������*******************/

extern float Pwm_value;  //��ſ��ƵĶ����ֵ

extern uint16 use_time;  //pit��ʱ����һ���õ�
extern float dis_data;  //���Ͼ���

extern int servo_chg;  //����ͷ���ƵĶ����ֵ
extern uchar image_threshold;  //ͼ����ֵ
extern int midline_aver;  //����ƽ��ֵ
extern uchar Mid_Count;  //ͼ����Ч��
extern uchar Width[121];  //�������
extern uchar Left_Line[123], Mid_Line[123], Right_Line[123];  //�����ұ߽���
extern uchar Left_Line_New[123], Right_Line_New[123];  //�߽粹��
extern uchar Left_Add[121], Right_Add[121];  //���ұ߽粹�߱�־λ
extern float angle;  //������б��ʮ�ֵļ�ǼнǴ�С��δʹ��


/*****************���涼��һЩ ������Ҫ�޸ĵ� ������ģʽ��־λ*******************/


#define dis_detour 900  //��ʼ���ϵ������루���ף�

#define inbreakrow 80   //��·��·������Ӧ�����Ŀ���ж�
#define outbreakrow 90
#define barrierrow 67

#define validwidthmin 32  //��·��·���ٽ�ͻ���е���Ч����С���

#define startbarrier_servo 80
#define endbarrier_servo 65
//#define startbarrier_time 300
//#define endbarrier_time 1000

extern uchar modeflag;
extern int count_s;
extern uchar threshold_cmp;//��ֵ�Ƚ�
extern int count_delay;    //·���ж���ʱ������ʱ���ж�
extern int16 speedcmp;     //ת�ٱȽ�
extern int mid_cmp;        //��ֵ�Ƚ�ֵ

extern uchar gg;            //ײ·�ϱ�־λ��ֻ����һ�Σ�
extern uchar StartLine_Flag;//�����߱�־λ
extern uchar work;          //�������������

extern uchar breakL0R1;     //�������ҹգ�
extern float dis_data;      //������
extern uchar finishflag;

extern int16 speedlll, speedrrr;
extern int turnback, turnout, turnin, turnrepair;

extern uint16 Kp_e, Kd_e;  //���p��d
extern uchar Flag_Round1, Flag_Round2, Flag_Round3, Flag_Round4, Flag_Round5;
extern uint16 sensor[4], AD_sensor[4];
/*
car.KP  car.KD
car.speed
*/

/*****************���涼��һЩ ��״̬ �Ľṹ��*******************/
/*****************������Ҳ�кܶ��־λ�Ͳ�����Ҫע��*******************/

typedef enum ModeState
{
	Mode1,
	Mode2,
	Mode3,
	Mode4,
	Mode5,
	Mode6
}ModeState;
extern ModeState modestate;

typedef struct SideState    //����״̬
{
	uchar NoLoseSide;     //������
	uchar LeftLoseSide;   //���û��
	uchar RightLoseSide;  //�ұ�û��
	uchar TwoLoseSide;    //����ȫû��
	uchar LoseForesight;  //ǰ��û��

	uchar CloseCycleCornerL; //��������������
	uchar CloseCycleCornerR; //�����ҽ��������
	uchar FarCycleCornerL;   //����Զ����
	uchar FarCycleCornerR;   //����Զ����
}SideState;
extern SideState sidestate;

typedef enum CycleState   //����״̬
{
	NearCycle,        //����������
	EnterCycleDelay,  //Ҫ��ȥ��
	InCycle,          //��ȥ��
	OutCycleDelay,    //Ҫ������
	OutCycle,         //������
	FarCycle          //��������
}CycleState;

typedef enum AddLineState  //����״̬
{
	CommonDraw,     //��������
	CycleDraw       //��������
}AddLineState;

typedef enum CutElect  //��Handle.c
{
	InBreakRoad,    //Ҫ����ͷ·�ˣ��Ͻ��е�š���
	OutBreakRoad,   //��ȥ�˳�ȥ�ˣ���������ͷ
	NoBreakRoad     //û������ͷ·��
}CutElect;

typedef enum RoadType    //��·����
{
	None,
	StraightRoad,
	EnterCycle,
	LeftCycle,     //���һ����е�Ų�ͬ�����ݣ�
	//InCycle,
	RightCycle,

	LeftCurve,
	RightCurve,
	CrossRoad,
	Barrier,       //�ж��Ƿ�ÿ�������̽·�ˣ��ɲ���ײǽ������Handle.c
	NearBarrier,
	Ramp,
	EndLine
}RoadType;

typedef struct ROAD_INFO
{
	RoadType rodetype;
	CycleState cyclestate;
	AddLineState addlinestate;
	CutElect cutelect;
}RoadInfoTypeDef;
extern RoadInfoTypeDef roadinfo;

typedef struct
{
	uchar run;           //���������
	uchar stop;          //�жϵ���ò��ö�
	uchar instraight;    //ֱ�������
	uchar incurve;     
	uchar incycle;
	uchar incyclestate;
	uchar inbreak;
	uchar inbarrier;     //���� ����ͷ· => 1 ? 0
	uchar instartline;   //0��ʱ�򲻹������ߵġ���1���������߲�ͣ��

	uchar inelec;        //�����
	uchar incamera;      //����ͷ��
	uchar mode;

	float KP;            //����ͷp��d����
	float KD;
	int speed;           //SXT���٣�1800�����1m������9999��
	int elecspeed        //��ų���
}CarInfoTypeDef; 
extern CarInfoTypeDef car;

typedef struct UI
{
	uchar index;       //ҳ
	uchar row;         //��
	uchar cut;         //״̬�л�
	uchar data_index;  //�����޸�ҳ
}UI;
extern struct UI ui;



#endif