/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： gpio.c
*	@Brief   	： 
*	@Author  	： admin@oarap.org
*	@Hardware	： STM32F103RCT6
*	@Date		： 2017-03
*	@Description    ： gpio配置及控制
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

/*******************************************************************************
	板载LED初始化
********************************************************************************/
void LED_GPIO_Config(void)
{	
    //定义一个GPIO_InitTypeDef 类型的结构体
    GPIO_InitTypeDef  GPIO_InitStructure;	
    //RCC_APB2PeriphClockCmd(LED1_GPIO_RCC|LED2_GPIO_RCC|LED3_GPIO_RCC,ENABLE);//使能GPIO的外设时钟
	RCC_APB2PeriphClockCmd(LED1_GPIO_RCC,ENABLE);//使能GPIO的外设时钟

    GPIO_InitStructure.GPIO_Pin =LED1_GPIO_PIN;//选择要用的GPIO引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置引脚模式为推免输出模式						 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置引脚速度为50MHZ         
    GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);//调用库函数，初始化GPIO
	
}
/*******************************************************************************
	LED电平变化函数
********************************************************************************/
void LED_Flash_Handler()
{
  if(Systick_Data.Counter<Systick_Data.LED_Nagative)
  {LED1_ONOFF(Bit_RESET);}
  else 
  {
    if((Systick_Data.Counter >= Systick_Data.LED_Nagative) && (Systick_Data.Counter <= Systick_Data.LED_Period))
	{LED1_ONOFF(Bit_SET);}
    else //超出上限
	{Systick_Data.Counter=0;}
  }
}
/*******************************************************************************
	LED电平翻转函数（每执行一次翻转一次电平）
********************************************************************************/
void LED_Toggle()
{
  static u8 mode=0;
  mode++;	
  if(mode%2==0)
   {LED1_ONOFF(Bit_SET);}
  else
	LED1_ONOFF(Bit_RESET);
}
	  
/*******************************************************************************
	蜂鸣器引脚初始化
********************************************************************************/
void Buzz_GPIO_Config()
{
	    //定义一个GPIO_InitTypeDef 类型的结构体
    GPIO_InitTypeDef  GPIO_InitStructure;	
    //RCC_APB2PeriphClockCmd(LED1_GPIO_RCC|LED2_GPIO_RCC|LED3_GPIO_RCC,ENABLE);//使能GPIO的外设时钟
	RCC_APB2PeriphClockCmd(BUZZ_GPIO_RCC,ENABLE);//使能GPIO的外设时钟

    GPIO_InitStructure.GPIO_Pin =BUZZ_GPIO_PIN;//选择要用的GPIO引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置引脚模式为推免输出模式						 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置引脚速度为50MHZ         
    GPIO_Init(BUZZ_GPIO_PORT, &GPIO_InitStructure);//调用库函数，初始化GPIO
	
	BUZZ_ONOFF(Bit_RESET);
}
/*******************************************************************************
	蜂鸣器发声函数
********************************************************************************/
void Beep(u8 num,u16 time)
{
  u8 i;
	for(i=0;i<num;i++)
	{
		BUZZ_ONOFF(Bit_SET);
	  	Delay_ms(time);
	  	BUZZ_ONOFF(Bit_RESET);
		Delay_ms(time);
	}
}
void Beep_Obj_Handler(u8 *param)
{
	Beep(1,param[0]);
}