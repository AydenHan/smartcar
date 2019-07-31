#ifndef     __MK60_I2C_H__
#define     __MK60_I2C_H__
/**
 *  @brief I2C模块编号
 */
typedef enum
{
    I2C0  = 0,
    I2C1  = 1
} I2Cn_e;

/**
 *  @brief 主机读写模式选择
 */
typedef enum MSmode
{
    MWSR =   0x00,  /* 主机写模式  */
    MRSW =   0x01   /* 主机读模式  */
} MSmode;


//目前代码仅支持 I2C主机模式
extern uint32  i2c_init(I2Cn_e i2cn, uint32 baud);                               //初始化I2C
extern void    i2c_write_reg(I2Cn_e, uint8 SlaveID, uint8 reg, uint8 Data);      //写入数据到寄存器
extern uint8   i2c_read_reg (I2Cn_e, uint8 SlaveID, uint8 reg);                  //读取寄存器的数据

#endif  //__MK60_I2C_H__
