/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： Blade.c
*	@Brief   	： Blade库核心代码
*	@Author  	： Blade@oarap.org
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

u16 BIT[16]={BIT0,BIT1,BIT2,BIT3,BIT4,BIT5,BIT6,BIT7,BIT8,BIT9,BIT10,BIT11,BIT12,BIT13,BIT14,BIT15};

s32 abs_s32(s32 i)
{    
  return (i < 0 ? -i : i);
}

void System_Init()
{
  Delay_ms(2000);
  LED_GPIO_Config();
  Wireless_GPIO_Config();
  Wireless_EXTI_Init();		//无线遥控的EXTI初始化
  Buzz_GPIO_Config();
  
  RTC_Init();
  ADC1_DMA_Init();
    
  //USART1_Config();//初始化上位机通讯端口
  
  USART2_Config();//初始化IMU通讯端口
  AHRS_Init();   //惯性单元初始化
  
  USART3_Config();//初始化RMDS
  RMDS_Init(RMDS_VEL_MODE);//速度模式
  Motion_Data_Init();
  
  UART4_Config();//初始化蓝牙通讯端口
  Bluetooth_Init();
  
  UART5_Config();//冗余串口
  
  USART_NVIC_Configuration();   
  USART_Data_Struct_Init();
  
  Timer2_Init(49999,71);//超声波模块计时使用定时器
  Ultrasonic_GPIO_Configuration();               //对超声波模块初始化
  
  //Timer3_Init();//无刷电机PWM输出定时器初始化
  //BLDC_GPIO_Config();
  
  Timer4_Init(3000,719);//每通道占用30ms//超声波后台工作模式下的调度定时器
  
  Timer5_Init(2000,719);//周期提交传感器数据到ROS（2500，719）-25ms，40hz,Motion_Data.Control.Update_Flag
  
  //开机成功
  Beep(1,200);
  //Systick_Init();
}

void Main_Process()
{     
	  
  ROS_Data_Process(USART_Data.BT);
  ROS_Data_Process(USART_Data.USB);
  Motion_Control_Data_Send();
  Motion_Status_Data_Read();
  Motor_Ang_Vel_Average_Calculate();
  Bat_Under_Voltage_Check_Out(24);
  
	debug_jy_901_to_mini_imu();
  
  if(Motion_Data.Status.Feedback_Flag == TRUE)
  {
	static u8 i=0;	
	LED_Toggle();
	
	  //debug_serial_to_ROS();///////////////  
	Distance_Data_to_ROS(3);		Delay_us(10);
	AHRS_Data_to_ROS();			Delay_us(10);
	Resolving_Data_to_ROS();		Delay_us(10);
	Motion_Data_Status_to_ROS();	Delay_us(10);	
	
	if(i++>=40)
	{
	  Bat_to_ROS();	 				Delay_us(10);
	  i=0;
	}
	/*if(Motion_Data.Control.Update_Flag == TRUE)
	{
	
	Motion_Data.Status.Update_Flag = FALSE;
  }*/
	
	Motion_Data.Status.Feedback_Flag = FALSE;
  }
}








/*             
  u8 str[]="AT+NAMERobotDriver";//18
  //u8 str2[]="AT+BAUD6";//8	6-38400	7-57600
  USART_Send_Assign(UART4,str,18);
  Delay_ms(500);
*/