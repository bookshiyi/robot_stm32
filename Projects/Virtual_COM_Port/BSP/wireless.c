/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： wireless.c
*	@Brief   	： 
*	@Author  	： admin@oarap.org
*	@Hardware	： STM32F103RCT6
*	@Date		： 2017-03
*	@Description    ： 无线遥控模块
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

Wireless_GPIO_TypeDef Wireless_GPIO[NUM_OF_WIRELESS_GPIO];

void Wireless_EXTI_Init(void)
{
  u8 i;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  for(i=0;i<NUM_OF_WIRELESS_GPIO;i++)
  {
	GPIO_EXTILineConfig(Wireless_GPIO[i].GPIO_PortSourceGPIO,Wireless_GPIO[i].GPIO_PinSource);
	
	EXTI_InitStructure.EXTI_Line	= Wireless_GPIO[i].EXTI_Line;
	EXTI_InitStructure.EXTI_Mode	= EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//上升沿
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = Wireless_GPIO[i].EXTI_IRQn;			//使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//先占优先级4位,共16级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//先占优先级0位,从优先级4位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	
  }
}



void Wireless_GPIO_Struct_Init(void)
{
	Wireless_GPIO[0].RCC_APB				= RCC_APB2Periph_GPIOB;
	Wireless_GPIO[0].PORT					= GPIOB;
	Wireless_GPIO[0].PIN					= GPIO_Pin_2;
	Wireless_GPIO[0].GPIO_PortSourceGPIO	= GPIO_PortSourceGPIOB;
	Wireless_GPIO[0].GPIO_PinSource			= GPIO_PinSource2;
	Wireless_GPIO[0].EXTI_Line				= EXTI_Line2;
	Wireless_GPIO[0].EXTI_IRQn				= EXTI2_IRQn;
	

	Wireless_GPIO[1].RCC_APB				= RCC_APB2Periph_GPIOB;
	Wireless_GPIO[1].PORT					= GPIOB;
	Wireless_GPIO[1].PIN					= GPIO_Pin_0;
	Wireless_GPIO[1].GPIO_PortSourceGPIO	= GPIO_PortSourceGPIOB;
	Wireless_GPIO[1].GPIO_PinSource			= GPIO_PinSource0;
	Wireless_GPIO[1].EXTI_Line				= EXTI_Line0;
	Wireless_GPIO[1].EXTI_IRQn				= EXTI0_IRQn;
	/*
	Wireless_GPIO[2].RCC_APB				=	RCC_APB2Periph_GPIOC;
	Wireless_GPIO[2].PORT					=	GPIOC;
	Wireless_GPIO[2].PIN					=	GPIO_Pin_5;
			
	Wireless_GPIO[3].RCC_APB				=	RCC_APB2Periph_GPIOC;
	Wireless_GPIO[3].PORT					=	GPIOC;
	Wireless_GPIO[3].PIN					=	GPIO_Pin_4;*/
	

}
void Wireless_GPIO_Config(void)
{
  u8 i;
  GPIO_InitTypeDef  GPIO_InitStructure;	
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  Wireless_GPIO_Struct_Init();
  for(i=0;i<NUM_OF_WIRELESS_GPIO;i++)
  {
	RCC_APB2PeriphClockCmd(Wireless_GPIO[i].RCC_APB,ENABLE);//使能GPIO的外设时钟
	
	GPIO_InitStructure.GPIO_Pin = Wireless_GPIO[i].PIN;				     //ECH0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;				 //设为输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;					//设置引脚速度为50MHZ   
	GPIO_Init(Wireless_GPIO[i].PORT,&GPIO_InitStructure);						 //初始化GPIO
	
	GPIO_EXTILineConfig(Wireless_GPIO[i].GPIO_PortSourceGPIO,Wireless_GPIO[i].GPIO_PinSource);
	
	EXTI_InitStructure.EXTI_Line = Wireless_GPIO[i].EXTI_Line;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;///////////////////双边跳变沿触发方式
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;////////////////////////////
	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器	
	
	NVIC_InitStructure.NVIC_IRQChannel = Wireless_GPIO[i].EXTI_IRQn;			//使能所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//抢占优先级2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//子优先级2 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
  }
}
