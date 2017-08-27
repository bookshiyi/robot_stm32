#ifndef __TIMER_H
#define __TIMER_H

#define TIM3_PERIOD 3599

extern u16 TIM_Count[];
extern u16 Ultrasonic_Distance[]; 

void Timer2_Init(u16 arr,u16 psc); //超声波计数
void Timer3_Init();     //PWM输出
void Timer4_Init(u16 arr,u16 psc);//超声波后台工作模式下的调度定时器
void Timer5_Init(u16 arr,u16 psc);//舵机信号定时输出
#endif
