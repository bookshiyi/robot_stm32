/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： ADC.c
*	@Brief   	： 
*	@Author  	： admin@oarap.org
*	@Hardware	： STM32F103RCT6
*	@Date		： 2017-03
*	@Description    ： ADC模块
*	@History	： 
*
*	Rev1.0 
*		Date：2017
*		Author：Blade
*		Modification：
*
*--------------------Copyright (C) oarap.org 2016------------------------------
*----------------------All rights reserved-------------------------------------
*
********************          一名工匠的遗憾与骄傲          ********************/
#include "Sys_Conf.h"


u16 AD_Value[NUM_OF_ADC_CHANNEL];


u16 AD_to_Voltage()
{
  u16 voltage;//电池电压
  voltage = (int)(AD_Value[0]*9.641408);       //实际电压如果是24.753 ，输出结果为24753
  return voltage;
}
void Bat_Under_Voltage_Check_Out(u8 th_voltage)
{
  if((RTC_Second_Conuter==10) && (AD_to_Voltage()>18*1000))
  {
	
	if((AD_to_Voltage()<(th_voltage*1000)-500))
	{
	  RTC_Second_Conuter=0;
	  Beep(2,100);
	}
	else if((AD_to_Voltage()<th_voltage*1000))
	{
	  
	  RTC_Second_Conuter=0;
	  Beep(2,100);
	}
  }
}
/*******************************************************************************
	ADC引脚和DMA初始化
********************************************************************************/
void ADC1_DMA_Init(void)
{
  ADC_InitTypeDef ADC_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);//打开ADC1时钟，打开GPIOC时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;//| GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; //配置成模拟输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
  /*
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //配置成模拟输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);   
  */
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value;/////////////////DMA内存地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = NUM_OF_ADC_CHANNEL;//////////////
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址寄存器递增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  DMA_Cmd(DMA1_Channel1, ENABLE);
    
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//独立模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;//连续多通道模式
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//连续转换
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//模数转换有软件启动
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC数据右对齐
  ADC_InitStructure.ADC_NbrOfChannel = NUM_OF_ADC_CHANNEL;	;////////////////扫描通道数，从1到16
  ADC_Init(ADC1, &ADC_InitStructure);

  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
  /*ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_239Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_239Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_239Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_239Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 8, ADC_SampleTime_239Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 9, ADC_SampleTime_239Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9,10, ADC_SampleTime_239Cycles5);*/
  ADC_DMACmd(ADC1, ENABLE);
  ADC_Cmd(ADC1, ENABLE);  //使能ADC1 
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));  
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));     
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

