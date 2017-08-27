/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： sw_iic.c
*	@Brief   	： 
*	@Author  	： admin@oarap.org
*	@Hardware	： STM32F103RCT6
*	@Date		： 2017-03
*	@Description    ： 软件模拟IIC的驱动程序
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


//extern u8 Obstacle_Weight_Buf[];
extern u8 Obstacle_Weight[];//先，障碍物方向权重
extern u16 Distance_Sum[];//后，障碍物距离

Ultrasonic_GPIO_TypeDef Ultrasonic_GPIO[NUM_OF_ULTRASONIC];

void Ultrasonic_GPIO_Struct_Init()
{
  Ultrasonic_GPIO[0].TRIG_PORT				= GPIOB;
  Ultrasonic_GPIO[0].TRIG_PIN					= GPIO_Pin_7;
  Ultrasonic_GPIO[0].ECHO_PORT				= GPIOB;
  Ultrasonic_GPIO[0].ECHO_PIN					= GPIO_Pin_8;
  Ultrasonic_GPIO[0].ECHO_GPIO_PortSourceGPIO	= GPIO_PortSourceGPIOB;
  Ultrasonic_GPIO[0].ECHO_GPIO_PinSource		= GPIO_PinSource8;
  Ultrasonic_GPIO[0].ECHO_EXTI_Line			= EXTI_Line8;
  Ultrasonic_GPIO[0].ECHO_EXTI_IRQn			= EXTI9_5_IRQn;
  
  Ultrasonic_GPIO[1].TRIG_PORT				= GPIOB;
  Ultrasonic_GPIO[1].TRIG_PIN					= GPIO_Pin_5;
  Ultrasonic_GPIO[1].ECHO_PORT				= GPIOB;
  Ultrasonic_GPIO[1].ECHO_PIN					= GPIO_Pin_6;
  Ultrasonic_GPIO[1].ECHO_GPIO_PortSourceGPIO	= GPIO_PortSourceGPIOB;
  Ultrasonic_GPIO[1].ECHO_GPIO_PinSource		= GPIO_PinSource6;
  Ultrasonic_GPIO[1].ECHO_EXTI_Line			= EXTI_Line6;
  Ultrasonic_GPIO[1].ECHO_EXTI_IRQn			= EXTI9_5_IRQn;
  
  Ultrasonic_GPIO[2].TRIG_PORT				= GPIOB;
  Ultrasonic_GPIO[2].TRIG_PIN					= GPIO_Pin_3;
  Ultrasonic_GPIO[2].ECHO_PORT				= GPIOB;
  Ultrasonic_GPIO[2].ECHO_PIN					= GPIO_Pin_4;
  Ultrasonic_GPIO[2].ECHO_GPIO_PortSourceGPIO	= GPIO_PortSourceGPIOB;
  Ultrasonic_GPIO[2].ECHO_GPIO_PinSource		= GPIO_PinSource4;
  Ultrasonic_GPIO[2].ECHO_EXTI_Line			= EXTI_Line4;
  Ultrasonic_GPIO[2].ECHO_EXTI_IRQn			= EXTI4_IRQn;
  
  Ultrasonic_GPIO[3].TRIG_PORT				= GPIOA;		        
  Ultrasonic_GPIO[3].TRIG_PIN					= GPIO_Pin_8;         
  Ultrasonic_GPIO[3].ECHO_PORT				= GPIOA;		        
  Ultrasonic_GPIO[3].ECHO_PIN					= GPIO_Pin_15;	        
  Ultrasonic_GPIO[3].ECHO_GPIO_PortSourceGPIO	= GPIO_PortSourceGPIOA;
  Ultrasonic_GPIO[3].ECHO_GPIO_PinSource		= GPIO_PinSource15;
  Ultrasonic_GPIO[3].ECHO_EXTI_Line			= EXTI_Line15;		
  Ultrasonic_GPIO[3].ECHO_EXTI_IRQn			= EXTI15_10_IRQn;
  
  Ultrasonic_GPIO[4].TRIG_PORT				= GPIOC;		        
  Ultrasonic_GPIO[4].TRIG_PIN					= GPIO_Pin_8;         
  Ultrasonic_GPIO[4].ECHO_PORT				= GPIOC;		        
  Ultrasonic_GPIO[4].ECHO_PIN					= GPIO_Pin_9;	        
  Ultrasonic_GPIO[4].ECHO_GPIO_PortSourceGPIO	= GPIO_PortSourceGPIOC;
  Ultrasonic_GPIO[4].ECHO_GPIO_PinSource		= GPIO_PinSource9;
  Ultrasonic_GPIO[4].ECHO_EXTI_Line			= EXTI_Line9;			
  Ultrasonic_GPIO[4].ECHO_EXTI_IRQn			= EXTI9_5_IRQn;
  
  Ultrasonic_GPIO[5].TRIG_PORT				= GPIOC;		        
  Ultrasonic_GPIO[5].TRIG_PIN					= GPIO_Pin_6;          
  Ultrasonic_GPIO[5].ECHO_PORT				= GPIOC;		        
  Ultrasonic_GPIO[5].ECHO_PIN					= GPIO_Pin_7;	        
  Ultrasonic_GPIO[5].ECHO_GPIO_PortSourceGPIO	= GPIO_PortSourceGPIOC;
  Ultrasonic_GPIO[5].ECHO_GPIO_PinSource		= GPIO_PinSource7;
  Ultrasonic_GPIO[5].ECHO_EXTI_Line			= EXTI_Line7;			
  Ultrasonic_GPIO[5].ECHO_EXTI_IRQn			= EXTI9_5_IRQn;
  
  Ultrasonic_GPIO[6].TRIG_PORT				= GPIOB;	        
  Ultrasonic_GPIO[6].TRIG_PIN					= GPIO_Pin_13;          
  Ultrasonic_GPIO[6].ECHO_PORT				= GPIOB;		        
  Ultrasonic_GPIO[6].ECHO_PIN					= GPIO_Pin_12;	        
  Ultrasonic_GPIO[6].ECHO_GPIO_PortSourceGPIO	= GPIO_PortSourceGPIOB;
  Ultrasonic_GPIO[6].ECHO_GPIO_PinSource		= GPIO_PinSource12;
  Ultrasonic_GPIO[6].ECHO_EXTI_Line			= EXTI_Line12; 		
  Ultrasonic_GPIO[6].ECHO_EXTI_IRQn			= EXTI15_10_IRQn;
  
  Ultrasonic_GPIO[7].TRIG_PORT				= GPIOB;		        
  Ultrasonic_GPIO[7].TRIG_PIN					= GPIO_Pin_15;          
  Ultrasonic_GPIO[7].ECHO_PORT				= GPIOB;		        
  Ultrasonic_GPIO[7].ECHO_PIN					= GPIO_Pin_14;	        
  Ultrasonic_GPIO[7].ECHO_GPIO_PortSourceGPIO	= GPIO_PortSourceGPIOB;
  Ultrasonic_GPIO[7].ECHO_GPIO_PinSource		= GPIO_PinSource14;
  Ultrasonic_GPIO[7].ECHO_EXTI_Line			= EXTI_Line14;			
  Ultrasonic_GPIO[7].ECHO_EXTI_IRQn			= EXTI15_10_IRQn;
  
}
void Ultrasonic_GPIO_Configuration(void)
{
  u8 i;
  GPIO_InitTypeDef GPIO_InitStructure;	
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  Ultrasonic_GPIO_Struct_Init();
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//关闭jtag(PB3.PB4)
  //1.1版本的硬件不再需要禁用SWD端口，因为没有使用SWD端口
  //通过AFIO_EXTICRx配置GPIO线上的外部中断/事件，必须先使能AFIO时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO , ENABLE);////////////////////
  for(i=0;i<NUM_OF_ULTRASONIC;i++)
  {
	//输出
	GPIO_InitStructure.GPIO_Pin = Ultrasonic_GPIO[i].TRIG_PIN;					 //TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(Ultrasonic_GPIO[i].TRIG_PORT, &GPIO_InitStructure);	                 //初始化外设GPIO 
	
	//输入
	GPIO_InitStructure.GPIO_Pin = Ultrasonic_GPIO[i].ECHO_PIN;				     //ECH0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		 //设为输入
	GPIO_Init(Ultrasonic_GPIO[i].ECHO_PORT,&GPIO_InitStructure);						 //初始化GPIO
	
	GPIO_EXTILineConfig(Ultrasonic_GPIO[i].ECHO_GPIO_PortSourceGPIO,Ultrasonic_GPIO[i].ECHO_GPIO_PinSource);
	
	EXTI_InitStructure.EXTI_Line = Ultrasonic_GPIO[i].ECHO_EXTI_Line;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;///////////////////双边跳变沿触发方式
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;////////////////////////////
	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器	
	
	NVIC_InitStructure.NVIC_IRQChannel = Ultrasonic_GPIO[i].ECHO_EXTI_IRQn;//EXTI9_5_IRQn;			//使能所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//抢占优先级2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//子优先级2 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
  }
  
}

void Obstacle_Scan()//障碍物检测
{//新增窗口测距，低于下限高于上限，改版状态
  static u8 Obstacle_Weight_Buf[2];
  if(Ultrasonic_Distance[0] > WINDOW_UPPER_VALUE - 15)	  	Obstacle_Weight_Buf[LEFT]  &= ~BIT[0];	//权重1 
  if(Ultrasonic_Distance[1] > WINDOW_UPPER_VALUE)	  		Obstacle_Weight_Buf[LEFT]  &= ~BIT[1];	//权重2
  if(Ultrasonic_Distance[2] > WINDOW_UPPER_VALUE)	  		Obstacle_Weight_Buf[LEFT]  &= ~BIT[2];	//权重3
  if(Ultrasonic_Distance[3] > WINDOW_UPPER_VALUE)	  		Obstacle_Weight_Buf[LEFT]  &= ~BIT[3];	//权重4
  if(Ultrasonic_Distance[4] > WINDOW_UPPER_VALUE)	  		Obstacle_Weight_Buf[RIGHT] &= ~BIT[3];	//权重4
  if(Ultrasonic_Distance[5] > WINDOW_UPPER_VALUE)	  		Obstacle_Weight_Buf[RIGHT] &= ~BIT[2];	//权重3
  if(Ultrasonic_Distance[6] > WINDOW_UPPER_VALUE)	  		Obstacle_Weight_Buf[RIGHT] &= ~BIT[1];	//权重2
  if(Ultrasonic_Distance[7] > WINDOW_UPPER_VALUE - 15)	  	Obstacle_Weight_Buf[RIGHT] &= ~BIT[0];	//权重1
  
  if(Ultrasonic_Distance[0] < WINDOW_FLOOR_VALUE - 15)	 	Obstacle_Weight_Buf[LEFT]  |=  BIT[0];	 
  if(Ultrasonic_Distance[1] < WINDOW_FLOOR_VALUE)	  		Obstacle_Weight_Buf[LEFT]  |=  BIT[1];
  if(Ultrasonic_Distance[2] < WINDOW_FLOOR_VALUE)	  		Obstacle_Weight_Buf[LEFT]  |=  BIT[2];
  if(Ultrasonic_Distance[3] < WINDOW_FLOOR_VALUE)	  		Obstacle_Weight_Buf[LEFT]  |=  BIT[3];
  if(Ultrasonic_Distance[4] < WINDOW_FLOOR_VALUE)	  		Obstacle_Weight_Buf[RIGHT] |=  BIT[3];
  if(Ultrasonic_Distance[5] < WINDOW_FLOOR_VALUE)	  		Obstacle_Weight_Buf[RIGHT] |=  BIT[2];
  if(Ultrasonic_Distance[6] < WINDOW_FLOOR_VALUE)	  		Obstacle_Weight_Buf[RIGHT] |=  BIT[1];
  if(Ultrasonic_Distance[7] < WINDOW_FLOOR_VALUE - 15)	  	Obstacle_Weight_Buf[RIGHT] |=  BIT[0];
  
  u8 Buf = 0;
  u8 i;
  for(i=0;i<4;i++)
  {
	if(Obstacle_Weight_Buf[LEFT]&BIT[i]) 
	{
	  Buf += 7; 
	}
  }
  Obstacle_Weight[LEFT] = Buf;//更新数据形式，不含权重的形式，每个传感器视为同样的权重
  Buf = 0;
  for(i=0;i<4;i++)
  {
	if(Obstacle_Weight_Buf[RIGHT]&BIT[i]) 
	{
	  Buf +=7; 
	}
	else
	{
	}
  }
  Obstacle_Weight[RIGHT] = Buf;//更新数据形式，不含权重的形式，每个传感器视为同样的权重
  Buf = 0;
  
  //计算两侧障碍物的距离和
  Distance_Sum[LEFT]=0;
  Distance_Sum[RIGHT]=0;
  for(i=0;i<4;i++)
  {
	Distance_Sum[LEFT]+=Ultrasonic_Distance[i];
  }
  for(i=4;i<8;i++)
  {
	Distance_Sum[RIGHT]+=Ultrasonic_Distance[i];
  }
}

