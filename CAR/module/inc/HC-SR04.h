#ifndef _HC_SR04_H_
#define _HC_SR04_H_


extern uint16 distance;                                                                //距离存储
extern uint32 back_time;                                                               //测距时间
extern uint8  success_flag;                                                            //测距成功标志位

void HCSR04_INIT(PTXn_e TRIG_PIN,PTXn_e ECHO_PIN);
void HCSR04_START(void);
float HCSR04_DATA_HANDLE(float back_time);
void dis_measure(void);


#endif