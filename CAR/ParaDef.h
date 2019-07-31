#ifndef _CAR_PARADEF_
#define _CAR_PARADEF_

/*****************下面都是一些 传感器引脚 的定义*******************/

// SERVO 舵机
#define SERVO_PORT	FTM0
#define SERVO_CH    FTM_CH2
#define SERVO_FREQ	50    //S_D5 300
#define SERVO_MIDDLE	750   //中值（1.5ms）  S_D5 4500

//MOTOR 电机端口
#define MOTOR_FTM       FTM3
#define MOTORL_Z_PWM     FTM_CH0
#define MOTORL_F_PWM     FTM_CH1
#define MOTORR_Z_PWM     FTM_CH2
#define MOTORR_F_PWM     FTM_CH3
#define MOTOR_FREQ      (15*1000)    //频率15k
#define MOTOR_MAX       9900

//ENCODER 编码器
#define ENCODER_L_FTM	FTM1
#define ENCODER_R_FTM	FTM2

//HC-SR04 超声波
#define TRIG  PTD0
#define ECHO  PTD1

//RING 蜂鸣器
#define Ring_Off  (gpio_init(PTE26, GPO, 0))
#define Ring_On   (gpio_init(PTE26, GPO, 1))
#define Ring_Turn (gpio_turn(PTE26))

//LED 灯
#define LEDY_ON   (led (LED0, LED_ON))   //PTE0 黄
#define LEDY_OFF  (led (LED0, LED_OFF))  //PTE0 黄
#define LEDB_ON   (led (LED1, LED_ON))   //PTE2 蓝
#define LEDB_OFF  (led (LED1, LED_OFF))  //PTE2 蓝

/*****************下面都是一些 可能需要读取 的传感器数据*******************/

extern float Pwm_value;  //电磁控制的舵机中值

extern uint16 use_time;  //pit计时，不一定用到
extern float dis_data;  //避障距离

extern int servo_chg;  //摄像头控制的舵机中值
extern uchar image_threshold;  //图像阈值
extern int midline_aver;  //中线平均值
extern uchar Mid_Count;  //图像有效行
extern uchar Width[121];  //赛道宽度
extern uchar Left_Line[123], Mid_Line[123], Right_Line[123];  //左中右边界线
extern uchar Left_Line_New[123], Right_Line_New[123];  //边界补线
extern uchar Left_Add[121], Right_Add[121];  //左右边界补线标志位
extern float angle;  //环岛、斜入十字的尖角夹角大小，未使用


/*****************下面都是一些 可能需要修改的 参数、模式标志位*******************/


#define dis_detour 900  //开始避障的最大距离（毫米）

#define inbreakrow 80   //断路和路障用相应行数的宽度判断
#define outbreakrow 90
#define barrierrow 67

#define validwidthmin 32  //断路和路障临近突变行的有效的最小宽度

#define startbarrier_servo 80
#define endbarrier_servo 65
//#define startbarrier_time 300
//#define endbarrier_time 1000

extern uchar modeflag;
extern int count_s;
extern uchar threshold_cmp;//阈值比较
extern int count_delay;    //路障判断延时，起跑时不判断
extern int16 speedcmp;     //转速比较
extern int mid_cmp;        //中值比较值

extern uchar gg;            //撞路障标志位（只允许一次）
extern uchar StartLine_Flag;//起跑线标志位
extern uchar work;          //超声波工作与否

extern uchar breakL0R1;     //避障左右拐？
extern float dis_data;      //测距距离
extern uchar finishflag;

extern int16 speedlll, speedrrr;
extern int turnback, turnout, turnin, turnrepair;

extern uint16 Kp_e, Kd_e;  //电磁p、d
extern uchar Flag_Round1, Flag_Round2, Flag_Round3, Flag_Round4, Flag_Round5;
extern uint16 sensor[4], AD_sensor[4];
/*
car.KP  car.KD
car.speed
*/

/*****************下面都是一些 车状态 的结构体*******************/
/*****************这里面也有很多标志位和参数需要注意*******************/

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

typedef struct SideState    //边线状态
{
	uchar NoLoseSide;     //都在呢
	uchar LeftLoseSide;   //左边没了
	uchar RightLoseSide;  //右边没了
	uchar TwoLoseSide;    //两边全没了
	uchar LoseForesight;  //前面没了

	uchar CloseCycleCornerL; //环岛左近处交叉口
	uchar CloseCycleCornerR; //环岛右近处交叉口
	uchar FarCycleCornerL;   //环岛远角左
	uchar FarCycleCornerR;   //环岛远角右
}SideState;
extern SideState sidestate;

typedef enum CycleState   //环岛状态
{
	NearCycle,        //看到环岛了
	EnterCycleDelay,  //要进去了
	InCycle,          //进去啦
	OutCycleDelay,    //要出来了
	OutCycle,         //出来啦
	FarCycle          //溜了溜了
}CycleState;

typedef enum AddLineState  //补线状态
{
	CommonDraw,     //正常补线
	CycleDraw       //环岛补线
}AddLineState;

typedef enum CutElect  //在Handle.c
{
	InBreakRoad,    //要进断头路了，赶紧切电磁。。
	OutBreakRoad,   //出去了出去了，换回摄像头
	NoBreakRoad     //没看到断头路哇
}CutElect;

typedef enum RoadType    //道路类型
{
	None,
	StraightRoad,
	EnterCycle,
	LeftCycle,     //左右环，切电磁不同的数据？
	//InCycle,
	RightCycle,

	LeftCurve,
	RightCurve,
	CrossRoad,
	Barrier,       //判断是否该开超声波探路了，可不想撞墙。。在Handle.c
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
	uchar run;           //电机动不动
	uchar stop;          //判断电机该不该动
	uchar instraight;    //直道飙起来
	uchar incurve;     
	uchar incycle;
	uchar incyclestate;
	uchar inbreak;
	uchar inbarrier;     //避障 ？断头路 => 1 ? 0
	uchar instartline;   //0的时候不管起跑线的。。1看到起跑线才停车

	uchar inelec;        //电磁跑
	uchar incamera;      //摄像头跑
	uchar mode;

	float KP;            //摄像头p、d参数
	float KD;
	int speed;           //SXT车速（1800差不多有1m？上限9999）
	int elecspeed        //电磁车速
}CarInfoTypeDef; 
extern CarInfoTypeDef car;

typedef struct UI
{
	uchar index;       //页
	uchar row;         //行
	uchar cut;         //状态切换
	uchar data_index;  //数据修改页
}UI;
extern struct UI ui;



#endif