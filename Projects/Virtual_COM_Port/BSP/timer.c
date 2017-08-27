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

u16 TIM_Count[NUM_OF_ULTRASONIC];//时钟计数器，用来记录每个超声波返回的脚高电平持续的时钟个数
u16 Ultrasonic_Distance[NUM_OF_ULTRASONIC];      //计算出的距离 

//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3
/*********************        TIM2        *******************************/
void Timer2_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//TIM_OCInitTypeDef        TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  ////////////////TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM2, //TIM2
		//TIM_IT_Update |   //TIM 中断源
		//TIM_IT_CC1 | //CCR1中断使能
		//TIM_IT_CC2 |
		TIM_IT_Trigger ,  //TIM 触发中断源 
		ENABLE  //使能
		);
	//TIM_ARRPreloadConfig(TIM2, ENABLE);               	//使能TIMx在ARR上的预装载寄存器                     
		  
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器							 
	 TIM_Cmd(TIM2,ENABLE);
}
/*********************        TIM3       *******************************/
void Timer3_Init(void)
{	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef        TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	//定时器定时时间T计算公式：T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK=(3600*10/72M)s=0.0005s，即2K频率
	TIM_TimeBaseStructure.TIM_Period = TIM3_PERIOD;//自动重装载值，取值必须在0x0000~0xFFFF之间
	TIM_TimeBaseStructure.TIM_Prescaler =10-1;//预分频值，+1为分频系数，取值必须在0x0000~0xFFFF之间							 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 				//时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数模式	 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 
	
	TIM_OCInitStructure.TIM_Pulse =0;//开机关闭占空比输出
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
	//通道1
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);          	//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//使能TIMx在CCR1上的预装载寄存器
	//通道2
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);          	//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//使能TIMx在CCR2上的预装载寄存器
	//通道3 
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);          	//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//使能TIMx在CCR3上的预装载寄存器
	//通道4 
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);          	//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//使能TIMx在CCR4上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM3, ENABLE);               	//使能TIMx在ARR上的预装载寄存器                     
	TIM_Cmd(TIM3, DISABLE);								//禁止TIM3使能
	TIM_Cmd(TIM3, ENABLE);								//使能定时器	
}

void Timer4_Init(u16 arr,u16 psc)
{	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//TIM_OCInitTypeDef        TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	//定时器定时时间T计算公式：T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK=(3600*10/72M)s=0.0005s，即2K频率
	TIM_TimeBaseStructure.TIM_Period = arr;//自动重装载值，取值必须在0x0000~0xFFFF之间
	TIM_TimeBaseStructure.TIM_Prescaler =psc;//预分频值，+1为分频系数，取值必须在0x0000~0xFFFF之间							 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 				//时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数模式	 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 
/*	
	TIM_OCInitStructure.TIM_Pulse =4600;//开机关闭占空比输出
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;             //TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
	//通道1
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);          	//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); 	//使能TIMx在CCR1上的预装载寄存器
	
	//通道2
	TIM_OCInitStructure.TIM_Pulse =4605;//开机关闭占空比输出
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);          	//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); 	//使能TIMx在CCR2上的预装载寄存器
*/
		TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM4, 
		TIM_IT_Update |   //TIM 中断源
		//TIM_IT_CC1 | //CCR1中断使能
		//TIM_IT_CC2 |
		TIM_IT_Trigger ,  //TIM 触发中断源 
		ENABLE  //使能
		);
		
	TIM_ARRPreloadConfig(TIM4, ENABLE);               	//使能TIMx在ARR上的预装载寄存器   
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器							 
	TIM_Cmd(TIM4, ENABLE);								//使能定时器	
}

void Timer5_Init(u16 arr,u16 psc)
{	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);////////////////////////////
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能
	//定时器定时时间T计算公式：T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK=(3600*10/72M)s=0.0005s，即2K频率
	TIM_TimeBaseStructure.TIM_Period = arr;//自动重装载值，取值必须在0x0000~0xFFFF之间
	TIM_TimeBaseStructure.TIM_Prescaler =psc;//预分频值，+1为分频系数，取值必须在0x0000~0xFFFF之间							 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 				//时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数模式	 
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 
        TIM_ITConfig(  //使能或者失能指定的TIM中断
                     TIM5, 
                     TIM_IT_Update,// |   //TIM 中断源
                       //TIM_IT_Trigger ,  //TIM 触发中断源 
                       ENABLE  //使能
                         );
        
	TIM_ARRPreloadConfig(TIM5, ENABLE);               	//使能TIMx在ARR上的预装载寄存器   
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器							 
	TIM_Cmd(TIM5, ENABLE);								//使能定时器	
}


void TIM2_IRQHandler(void)   //TIM2中断
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
  {
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 //延时20US
  }
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 //延时20US
}

void TIM4_IRQHandler(void)   //TIM4中断
{
    static u8 Current_Sensor_Sequence = 0;			//当前正在处理的传感器
    EXTI_InitTypeDef EXTI_InitStructure;
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
    {	
        //关过去
	EXTI_InitStructure.EXTI_Line = Ultrasonic_GPIO[Current_Sensor_Sequence].ECHO_EXTI_Line;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器	
	
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
	
        //在中断中处理时间转换成距离的数据，取代之前的在函数中计算。
        Ultrasonic_Distance[Current_Sensor_Sequence] = (TIM_Count[Current_Sensor_Sequence]*17/1000);//////////
        
	Current_Sensor_Sequence++;////////////////
	if(Current_Sensor_Sequence==NUM_OF_ULTRASONIC)Current_Sensor_Sequence=0;
	
	TIM_Count[Current_Sensor_Sequence]=29999;//29999防止传感器从连接到断开后尽管不会进入外部中断但TIM_Count中会缓存最后的数值的情况的发生
	
	GPIO_SetBits(Ultrasonic_GPIO[Current_Sensor_Sequence].TRIG_PORT,Ultrasonic_GPIO[Current_Sensor_Sequence].TRIG_PIN); 		  //送>10US的高电平RIG_PORT,TRIG_PIN这两个在define中有?
	Delay_us(20);		                      //延时20US
	GPIO_ResetBits(Ultrasonic_GPIO[Current_Sensor_Sequence].TRIG_PORT,Ultrasonic_GPIO[Current_Sensor_Sequence].TRIG_PIN);
	
	//开当前
	EXTI_InitStructure.EXTI_Line = Ultrasonic_GPIO[Current_Sensor_Sequence].ECHO_EXTI_Line;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器	
	
	TIM_SetCounter(TIM2,0);	
	TIM_Cmd(TIM2,DISABLE);
  }
  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
}
void TIM5_IRQHandler(void)   //TIM5中断
{ 	 
  if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
  {
	Motion_Data.Status.Feedback_Flag = TRUE;
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 //延时20US
  }
  TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
}

