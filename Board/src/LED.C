#include "common.h"
#include "MK60_port.h"
#include "MK60_gpio.h"
#include "LED.H"

/*
 * ����LED ��Ŷ�Ӧ�Ĺܽ�
 */
PTXn_e LED_PTxn[LED_MAX] = {PTE0,PTE2};
//PTD15,PTE26,PTC0,PTA17

/*!
 *  @brief      ��ʼ��LED�˿�
 *  @param      LED_e    LED���
 *  @since      v5.0
 *  Sample usage:       led_init (LED0);    //��ʼ�� LED0
 */
void    led_init(LED_e ledn)
{
    if(ledn < LED_MAX)
    {
        gpio_init(LED_PTxn[ledn],GPO,LED_OFF);
    }
    else
    {
        ledn = LED_MAX;
        while(ledn--)
        {
            gpio_init(LED_PTxn[ledn],GPO,LED_OFF);
        }

    }
}



/*!
 *  @brief      ����LED������
 *  @param      LED_e           LED��ţ�LED0��LED1��LED2��LED3��
 *  @param      LED_status      LED����״̬��LED_ON��LED_OFF��
 *  @since      v5.2
 *  Sample usage:       led (LED0,LED_ON);    //���� LED0
 */
void    led(LED_e ledn,LED_status status)
{
    if(ledn < LED_MAX)
    {
        gpio_set(LED_PTxn[ledn],status);
    }
    else
    {
        ledn = LED_MAX;
        while(ledn--)
        {
            gpio_set(LED_PTxn[ledn],status);
        }
    }
}

/*!
 *  @brief      ����LED������ת
 *  @param      LED_e           LED��ţ�LED0��LED1��LED2��LED3��
 *  @since      v5.2
 *  Sample usage:       led_turn (LED0);    // LED0������ת
 */
void led_turn(LED_e ledn)
{
    if(ledn < LED_MAX)
    {
        gpio_turn(LED_PTxn[ledn]);
    }
    else
    {
        ledn = LED_MAX;
        while(ledn--)
        {
            gpio_turn(LED_PTxn[ledn]);
        }
    }
}
