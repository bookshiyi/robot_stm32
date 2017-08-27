
#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"
/*-------------------------------STM32F103C8T6-------------------

ADC1_IN0        PA0
ADC1_IN1        PA1
ADC1_IN2        PA2
ADC1_IN3        PA3
ADC1_IN4        PA4
ADC1_IN5        PA5
ADC1_IN6        PA6
ADC1_IN7        PA7

ADC1_IN8        PB0
ADC1_IN9        PB1
-----------------------------------------------------------------*/
#define ADC1_DR_Address    ((u32)0x4001244C)
#define NUM_OF_ADC_CHANNEL 1

extern u16 AD_Value[NUM_OF_ADC_CHANNEL];

u16 AD_to_Voltage();
void Bat_Under_Voltage_Check_Out(u8 th_voltage);

//extern  u16 ADC_ConvertedValue;
void ADC1_DMA_Init(void);

#endif
