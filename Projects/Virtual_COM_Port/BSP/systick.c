/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： systick.c
*	@Brief   	： 
*	@Author  	： admin@oarap.org
*	@Hardware	： STM32F103RCT6
*	@Date		： 2016-05
*	@Description： 
*	@History	： 
*
*	Rev1.0 
*		Date：2016-05-16
*		Author：Blade
*		Modification：
*
*--------------------Copyright (C) oarap.org 2016------------------------------
*----------------------All rights reserved-------------------------------------
*
********************          一名工匠的遗憾与骄傲          ********************/

#include "Sys_Conf.h"

Systick_Data_TypeDef Systick_Data;
/*******************************************************************************
	Systick的中断处理程序
********************************************************************************/
void SysTick_Handler(void)
{	
  Systick_Data.Counter++;
  if(Systick_Data.Counter > Systick_Data.LED_Period)	
  {
	Systick_Data.Counter = 0;
  }
  LED_Flash_Handler(); 
}
//=============================================================================
//函数名称: SYSTICK_Init
//功能概要:初始化SYSTICK，1us中断1次
//参数名称:无
//函数返回:无
//注意    :
//SystemCoreClock/ 1000000：1us中断1次 
//SystemCoreClock/ 100000:	10us中断一次
//SystemCoreClock/ 10000:	100us中断一次
//SystemCoreClock/ 1000：	1ms中断一次   
//SystemCoreClock/ 100：	10ms中断一次  
//=============================================================================
void Systick_Init(void)
{		
  while (SysTick_Config(SystemCoreClock/30));           //33.3333MS反馈一次
  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  //NVIC_SetPriority(SysTick_IRQn, 0X0F);  
  /*n=0x00~0x03 设置Systick为抢占优先级0  
	n=0x04~0x07 设置Systick为抢占优先级1  
	n=0x08~0x0B 设置Systick为抢占优先级2  
	n=0x0C~0x0F 设置Systick为抢占优先级3    */
  Systick_Data.LED_Period = 50;  
  Systick_Data.LED_Nagative = 2;
}
//=============================================================================
//函数名称: delay_us
//功能概要:每次定时1us
//参数名称:无
//函数返回:无
//注意   :无
//=============================================================================
void Delay_us(u16 nus)
{
  u8 i;	
  while(nus--)
  {
  	i=5;
	while(i--);
  }
}
//=============================================================================
//函数名称: delay_ms
//功能概要:每次定时1ms
//参数名称:无
//函数返回:无
//注意   :无
//=============================================================================
void Delay_ms( uint16_t time_ms )
{
  uint16_t i,j;
  for( i=0;i<time_ms;i++ )
  {
    for( j=0;j<10309;j++ );//大约1ms
  }
}



