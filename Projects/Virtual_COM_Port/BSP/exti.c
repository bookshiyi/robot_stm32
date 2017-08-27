/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： exti.c
*	@Brief   	： 
*	@Author  	： admin@oarap.org
*	@Hardware	： STM32F103RCT6
*	@Date		： 2017-03
*	@Description    ： 外部中断模块
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

  

/*------------------------		无线遥控外部中断		--------------------------*/	
void EXTI0_IRQHandler(void)
{
  u8 Channelx = 1;
	if(EXTI_GetITStatus(Wireless_GPIO[Channelx].EXTI_Line) != RESET)
	{
		if((GPIO_ReadInputDataBit(Wireless_GPIO[Channelx].PORT,Wireless_GPIO[Channelx].PIN)))//上升沿
		{
		  	Beep(1,20);
		}
		else
		{			       
			
		}
		EXTI_ClearITPendingBit(Wireless_GPIO[Channelx].EXTI_Line);  //清除EXTIx线路挂起位
	}
}
void EXTI2_IRQHandler(void)
{
  u8 Channelx = 0;
	if(EXTI_GetITStatus(Wireless_GPIO[Channelx].EXTI_Line) != RESET)
	{
		if((GPIO_ReadInputDataBit(Wireless_GPIO[Channelx].PORT,Wireless_GPIO[Channelx].PIN)))//上升沿
		{
		  	Beep(1,20);
		}
		else
		{			       
			
		}
		EXTI_ClearITPendingBit(Wireless_GPIO[Channelx].EXTI_Line);  //清除EXTIx线路挂起位
	}
}

/*------------------------		超声波ECHO外部中断		--------------------------*/	
void EXTI4_IRQHandler(void)
{
  	u8 Channelx = 2;//通道2对应的外部中断处理程序
	if(EXTI_GetITStatus(Ultrasonic_GPIO[Channelx].ECHO_EXTI_Line) != RESET)
	{
		if((GPIO_ReadInputDataBit(Ultrasonic_GPIO[Channelx].ECHO_PORT,Ultrasonic_GPIO[Channelx].ECHO_PIN)))//上升沿
		{
		  	TIM_Cmd(TIM2,ENABLE);  
		}
		else
		{			       
			TIM_Count[Channelx]=TIM_GetCounter(TIM2);
		}
		EXTI_ClearITPendingBit(Ultrasonic_GPIO[Channelx].ECHO_EXTI_Line);  //清除EXTIx线路挂起位
	}
}
void EXTI9_5_IRQHandler(void)
{
  u8 Channel[4]={0,1,4,5};//通道0,1,4,5对应的外部中断处理程序
  u8 x;
	for(x=0;x<4;x++)
	{
	  if(EXTI_GetITStatus(Ultrasonic_GPIO[Channel[x]].ECHO_EXTI_Line) != RESET)
	  {
		  if((GPIO_ReadInputDataBit(Ultrasonic_GPIO[Channel[x]].ECHO_PORT,Ultrasonic_GPIO[Channel[x]].ECHO_PIN)))//上升沿
		  {
			  TIM_Cmd(TIM2,ENABLE); 
		  }
		  else
		  {			       
			  TIM_Count[Channel[x]]=TIM_GetCounter(TIM2);
		  }
		  EXTI_ClearITPendingBit(Ultrasonic_GPIO[Channel[x]].ECHO_EXTI_Line);  //清除EXTIx线路挂起位
	  }
	}
}
void EXTI15_10_IRQHandler(void)
{
  u8 Channel[3]={3,6,7};//通道367对应的外部中断处理程序
  u8 x;
	for(x=0;x<3;x++)
	{
	  if(EXTI_GetITStatus(Ultrasonic_GPIO[Channel[x]].ECHO_EXTI_Line) != RESET)
	  {
		  if((GPIO_ReadInputDataBit(Ultrasonic_GPIO[Channel[x]].ECHO_PORT,Ultrasonic_GPIO[Channel[x]].ECHO_PIN)))//上升沿
		  {
			  TIM_Cmd(TIM2,ENABLE);   
		  }
		  else
		  {			       
			  TIM_Count[Channel[x]]=TIM_GetCounter(TIM2);
		  }
		  EXTI_ClearITPendingBit(Ultrasonic_GPIO[Channel[x]].ECHO_EXTI_Line);  //清除EXTIx线路挂起位
	  }
	}
}








//外部中断初始化程序
//初始化PA0,PA13,PA15为中断输入.
void EXTIX_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//关闭jtag
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);
	
	//init GPIOC.7	  下拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	//GPIOC.7
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource7);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//先占优先级4位,共16级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//先占优先级0位,从优先级4位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	
}