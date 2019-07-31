
//下面两个参数都与 flexbus_8080_init 函数相关，修改后可能需要修改 flexbus_8080_init 函数的内容
#define FB_BA       0x6000      //基地址     = FB_BA * 0x10000 ，用于区分 不同的片选信号
                                //参考 System memory map 的说明， External Memory 的范围为 0x6000_0000 ~ 0x9FFF_FFFF
                                //即 FB_BA 的取值范围为 0x6000 ~ 0x9FFF

#define FB_BAM      0x0800      //基地址掩膜 = (FB_BA + FB_BAM)* 0x10000 ，用于区分 同一个CS控制的两个不连续的块的地址

#define FB_8080_CMD     (*(volatile uint16 *)(FB_BA * 0x10000))
#define FB_8080_DATA    (*(volatile uint16 *)((FB_BA + FB_BAM )* 0x10000))



extern void flexbus_8080_init();


