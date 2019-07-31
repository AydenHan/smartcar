#ifndef _MK60_CAN_H_
#define _MK60_CAN_H_

/*! CAN编号 */
typedef enum
{
    CAN0,
    CAN1,

} CANn_e;

/*! MB缓冲区编号 */
typedef enum
{
    MB_NUM_0,       //定义邮箱0
    MB_NUM_1,
    MB_NUM_2,
    MB_NUM_3,
    MB_NUM_4,
    MB_NUM_5,
    MB_NUM_6,
    MB_NUM_7,
    MB_NUM_8,
    MB_NUM_9,
    MB_NUM_10,
    MB_NUM_11,
    MB_NUM_12,
    MB_NUM_13,
    MB_NUM_14,
    MB_NUM_15,

    NUMBER_OF_MB,
    MB_NUM_MAX = NUMBER_OF_MB,

} mb_num_e;

/*! CAN工作模式 */
typedef enum
{
    CAN_NORMAL,         //正常模式
    CAN_LOOPBACK,       //环回模式
} CAN_mode_e;


/*! CAN波特率的寄存器配置结构体 */
typedef struct
{
    uint32_t band;          //波特率（Kb/s）
    uint16_t presdiv;       //分频系数
    uint8_t  prop_seg;      //传播时间段
    uint8_t  pseg1;         //相位缓冲段1
    uint8_t  pseg2;         //相位缓冲段2
    uint8_t  rjw;           //同步跳转宽度

    uint8_t  res[2];        //保留

} CAN_band_cfg_t;

/*! CAN波特率编号 */
typedef enum        //若修改此处的值，则还必须修改 can_band_cfg 数组
{
    CAN_BAUD_10K    ,
    CAN_BAUD_20K    ,
    CAN_BAUD_50K    ,
    CAN_BAUD_100K   ,
    CAN_BAUD_125K   ,
    CAN_BAUD_250K   ,
    CAN_BAUD_500K   ,
    CAN_BAUD_1M     ,

    CAN_BAUD_MAX,

} CAN_BAUD_e;

/*! CAN时钟源选择 */
typedef enum
{
    CAN_CLKSRC_BUS,   //BUS总线
    CAN_CLKSRC_OSC,   //外部晶振
} CAN_CLKSRC_e;


/*! 用户自定义 CAN ID */
typedef struct
{
    uint32  ID: 29;     //ID
    uint32  IDE: 1;     //IDE   // 1 : 扩展ID, 0: 标准ID
    uint32  RTR: 1;     //RTR   // 1: 远程帧, 0: 数据帧
} CAN_USR_ID_t;


extern void    can_init        (CANn_e cann, CAN_BAUD_e band, CAN_mode_e mode,CAN_CLKSRC_e clksrc);    //CAN 初始化
extern void    can_tx          (CANn_e cann, mb_num_e nMB, CAN_USR_ID_t id, uint8 len, void *buff);    //CAN 发送数据
extern void    can_rx          (CANn_e cann, mb_num_e nMB, CAN_USR_ID_t *id, uint8 *len, void *buff);  //CAN 接收数据

extern void    can_rxbuff_enble(CANn_e cann, mb_num_e nMB, CAN_USR_ID_t id);                           //使能 CAN 接收缓冲区
extern void    can_rxbuff_mask (CANn_e cann, mb_num_e nMB, uint32 mask,uint8 isIRMQ);                  //CAN 接收掩码配置，选择

extern void    can_irq_en      (CANn_e cann, mb_num_e nMB);                                            //使能 CAN 缓冲区接收和发送中断
extern void    can_irq_dis     (CANn_e cann, mb_num_e nMB);                                            //禁止 CAN 缓冲区接收和发送中断

extern void    can_clear_flag  (CANn_e cann, mb_num_e nMB);                                            //清 CAN 缓冲区中断标志
extern uint32  can_get_flag    (CANn_e cann, mb_num_e nMB);                                            //获得 CAN 缓冲区中断标志

extern void    can_setband     (CANn_e cann, CAN_BAUD_e band);                                         //设置 CAN 的波特率


#endif

