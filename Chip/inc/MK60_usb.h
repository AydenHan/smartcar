#ifndef _MK60_USB_H_
#define _MK60_USB_H_
// USB  模块时钟必须 配置 为 48M

// USB 模块时钟源 选择
#define USB_CLOCK_PLL       //PLL　作为时钟源
//#define USB_CLOCK_FLL     //FLL  作为时钟源
//#define USB_CLOCK_CLKIN   //USB_CLKIN (即 PTE26) 作为时钟源

//USB clk = 时钟源频率 * (1 + USB_CLK_FRAC) / (USB_CLK_DIV + 1)
//USB_CLK_FRAC = 0 或 1
//USB_CLK_DIV 0 ~ 7
#if defined(MK60DZ10)
//PLL  = 96M
#define USB_CLK_DIV         1       // 时钟源分频因子 。 
#define USB_CLK_FRAC        0

#elif defined(MK60F15)
//PLL  = 120M
#define USB_CLK_DIV         4       // 时钟源分频因子 。 
#define USB_CLK_FRAC        1


#endif




void    usb_init(void);                         // USB 模块初始化

void    usb_com_init(void);                     // USB 虚拟串口 初始化
void    usb_enum_wait(void);                    // USB 等待枚举
uint8   usb_com_rx(uint8 *rx_buf);              // USB 虚拟串口 接收
void    usb_com_tx(uint8 *tx_buf, uint8 len);   // USB 虚拟串口 发送


#endif  //_MK60_USB_H_
