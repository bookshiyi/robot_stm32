#ifndef __SYSTICK_H
#define __SYSTICK_H


#include "stm32f10x.h"
typedef struct
{
	u16 LED_Period;
	u16 LED_Nagative;
	u32 Counter;
}Systick_Data_TypeDef;
extern Systick_Data_TypeDef Systick_Data;


void Systick_Init(void);
void Delay_us(u16 nus);
void Delay_ms(u16 nms);
#endif
