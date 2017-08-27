


#ifndef _BRUSHLESS_H_
#define _BRUSHLESS_H_

#include "stm32f10x_conf.h"
typedef struct
{/*
  	uint32_t BRK_RCC;
	GPIO_TypeDef * BRK_Port;
	uint16_t BRK_Pin;
	*/
	uint32_t FR_RCC;
	GPIO_TypeDef * FR_Port;
	uint16_t FR_Pin;
	
	uint32_t SV_RCC;
	GPIO_TypeDef * SV_Port;
	uint16_t SV_Pin;
}BLDC_GPIO_TypeDef;

extern BLDC_GPIO_TypeDef BLDC_GPIO[4];

void BLDC_GPIO_Config();
void PWM_Set(u8 ch,float Pulse);
void Motion_Control(u8 driection,float speed);

void autoMotion(float speed);
void backwardLeft(float speed);
void backwardRight(float speed);
void leftSpin(float speed);
void rightSpin(float speed);
void backWard(float speed);
void stopMove(float speed);
void forwardMove(float speed);
void forwardLeft(float speed);
void forwardRight(float speed);
#define forwardSpeedOffset 0       //只有fangfang3 是1.3 其他都为0
#define backwardSpeedOffset 0     //只有fangfang3 是1.5 其他都为0 
#endif
