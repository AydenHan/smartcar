#include "common.h"
#include "MK60_port.h"
#include "MK60_gpio.h"

GPIO_MemMapPtr GPIOX[PTX_MAX] = {PTA_BASE_PTR, PTB_BASE_PTR, PTC_BASE_PTR, PTD_BASE_PTR, PTE_BASE_PTR}; //定义五个指针数组保存 GPIOX 的地址

void gpio_init (PTXn_e ptxn, GPIO_CFG cfg, uint8 data)
{
    //复用管脚为GPIO功能
    port_init( ptxn, ALT1);

    //端口方向控制输入还是输出
    if(  cfg == GPI )
    {
        //设置端口方向为输入
        GPIO_PDDR_REG(GPIOX_BASE(ptxn)) &= ~(1 << PTn(ptxn));       // GPIO PDDR 管脚号 清0，即对应管脚配置为端口方向输入
    }
    else
    {
        //设置端口方向为输出
        GPIO_PDDR_REG(GPIOX_BASE(ptxn)) |= (1 << PTn(ptxn));        // GPIO PDDR 管脚号 置1，即对应管脚配置为端口方向输出

        //端口输出数据
        if(data == 0)
        {
            GPIO_PDOR_REG(GPIOX_BASE(ptxn)) &= ~(1 << PTn(ptxn));   // GPIO PDOR 管脚号 清0，即对应管脚配置为端口输出低电平
        }
        else
        {
            GPIO_PDOR_REG(GPIOX_BASE(ptxn))  |= (1 << PTn(ptxn));   // GPIO PDOR 管脚号 置1，即对应管脚配置为端口输出高电平
        }
    }
}

void    gpio_ddr   (PTXn_e ptxn, GPIO_CFG cfg)
{
    //端口方向控制输入还是输出
    if(  cfg == GPI )
    {
        //设置端口方向为输入
        GPIO_PDDR_REG(GPIOX_BASE(ptxn)) &= ~(1 << PTn(ptxn));           // GPIO PDDR 管脚号 清0，即对应管脚配置为端口方向输入
    }
    else
    {
        //设置端口方向为输出
        GPIO_PDDR_REG(GPIOX_BASE(ptxn)) |= (1 << PTn(ptxn));            // GPIO PDDR 管脚号 置1，即对应管脚配置为端口方向输出
    }
}

void gpio_set (PTXn_e ptxn, uint8 data)
{
    ASSERT( BIT_GET( GPIO_PDDR_REG(GPIOX_BASE(ptxn)) , PTn(ptxn)) == GPO ); // 断言，检测 输出方向是否为输出
                                                                            // 获取 GPIO PDDR 管脚号 ，比较是否为输出

    //端口输出数据
    if(data == 0)
    {
        GPIO_PDOR_REG(GPIOX_BASE(ptxn)) &= ~(1 << PTn(ptxn));   // GPIO PDOR 管脚号 清0，即对应管脚配置为端口输出低电平
    }
    else
    {
        GPIO_PDOR_REG(GPIOX_BASE(ptxn))  |= (1 << PTn(ptxn));   // GPIO PDOR 管脚号 置1，即对应管脚配置为端口输出高电平
    }
}

void gpio_turn (PTXn_e ptxn)
{
    ASSERT( BIT_GET( GPIO_PDDR_REG(GPIOX_BASE(ptxn)) , PTn(ptxn)) == GPO ); // 断言，检测 输出方向是否为输出
                                                                            // 获取 GPIO PDDR 管脚号 ，比较是否为输出

    GPIO_PTOR_REG( GPIOX_BASE(ptxn))  =  1 << (PTn(ptxn ));                 // GPIO PTOR ptxn 置1，其他清0 ，即对应管脚配置为端口输出反转，其他位不变
                                                                            // 此处不能用 BIT_SET 这个宏来置1 ，因为必须保证其他位 不变，其他位直接清0即可
}

uint8 gpio_get(PTXn_e ptxn)
{
    ASSERT( BIT_GET( GPIO_PDDR_REG(GPIOX_BASE(ptxn)) , PTn(ptxn)) == GPI ); // 断言，检测 输出方向是否为输入
                                                                            // 获取 GPIO PDDR 管脚号 ，比较是否为输入

    return ((GPIO_PDIR_REG(GPIOX_BASE(ptxn)) >> PTn(ptxn )) & 0x01);        // 获取 GPIO PDIR ptxn 状态，即读取管脚输入电平
}
