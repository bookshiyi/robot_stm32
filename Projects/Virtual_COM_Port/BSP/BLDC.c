/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： Brushless.c
*	@Brief   	： 
*	@Author  	： Blade@oarap.org
*	@Hardware	： STM32F103RCT6
*	@Date		： 2016-05
*	@Description    ： 无刷电机控制模块
*	@History	： 
*
*	Rev1.0 
*		Date：2016
*		Author：Blade
*		Modification：
*
*--------------------Copyright (C) oarap.org 2016------------------------------
*----------------------All rights reserved-------------------------------------
*
********************          一名工匠的遗憾与骄傲          ********************/

//#include "Sys_Conf.h"

u8 dance_rise_Flag = 0; // 跳舞手臂动作启动标志位
u8 second = 0; 
BLDC_GPIO_TypeDef BLDC_GPIO[4];
void BLDC_GPIO_Struct_Init()
{/*
	BLDC_GPIO[0].BRK_RCC  = RCC_APB2Periph_GPIOB;
	BLDC_GPIO[0].BRK_Port = GPIOB;
	BLDC_GPIO[0].BRK_Pin  = GPIO_Pin_12;*/
	BLDC_GPIO[0].FR_RCC   = RCC_APB2Periph_GPIOA;
	BLDC_GPIO[0].FR_Port  = GPIOA;
	BLDC_GPIO[0].FR_Pin   = GPIO_Pin_4;
	BLDC_GPIO[0].SV_RCC   = RCC_APB2Periph_GPIOA;
	BLDC_GPIO[0].SV_Port  = GPIOA;
	BLDC_GPIO[0].SV_Pin   = GPIO_Pin_6;
	
	/*
	BLDC_GPIO[1].BRK_RCC  = RCC_APB2Periph_GPIOB;
	BLDC_GPIO[1].BRK_Port = GPIOB;
	BLDC_GPIO[1].BRK_Pin  = GPIO_Pin_13;*/
	BLDC_GPIO[1].FR_RCC   = RCC_APB2Periph_GPIOA;
	BLDC_GPIO[1].FR_Port  = GPIOA;
	BLDC_GPIO[1].FR_Pin   = GPIO_Pin_5;
	BLDC_GPIO[1].SV_RCC   = RCC_APB2Periph_GPIOA;
	BLDC_GPIO[1].SV_Port  = GPIOA;
	BLDC_GPIO[1].SV_Pin   = GPIO_Pin_7;
	
	/*
	BLDC_GPIO[2].BRK_RCC  = RCC_APB2Periph_GPIOB;
	BLDC_GPIO[2].BRK_Port = GPIOB;
	BLDC_GPIO[2].BRK_Pin  = GPIO_Pin_14;*/
	BLDC_GPIO[2].FR_RCC   = RCC_APB2Periph_GPIOC;
	BLDC_GPIO[2].FR_Port  = GPIOC;
	BLDC_GPIO[2].FR_Pin   = GPIO_Pin_4;
	BLDC_GPIO[2].SV_RCC   = RCC_APB2Periph_GPIOB;
	BLDC_GPIO[2].SV_Port  = GPIOB;
	BLDC_GPIO[2].SV_Pin   = GPIO_Pin_0;
	
	/*
	BLDC_GPIO[3].BRK_RCC  = RCC_APB2Periph_GPIOB;
	BLDC_GPIO[3].BRK_Port = GPIOB;
	BLDC_GPIO[3].BRK_Pin  = GPIO_Pin_15;*/
	BLDC_GPIO[3].FR_RCC   = RCC_APB2Periph_GPIOC;
	BLDC_GPIO[3].FR_Port  = GPIOC;
	BLDC_GPIO[3].FR_Pin   = GPIO_Pin_5;
	BLDC_GPIO[3].SV_RCC   = RCC_APB2Periph_GPIOB;
	BLDC_GPIO[3].SV_Port  = GPIOB;
	BLDC_GPIO[3].SV_Pin   = GPIO_Pin_1;
}
void BLDC_GPIO_Config()
{
	BLDC_GPIO_Struct_Init();
	u8 i;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//TIM3用于PWM输出
	
	for(i=0;i<4;i++) 
	{
		//BRK
		/*RCC_APB2PeriphClockCmd(Brushless_GPIO[i].BRK_RCC,ENABLE);
		GPIO_InitStructure.GPIO_Pin = Brushless_GPIO[i].BRK_Pin; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(Brushless_GPIO[i].BRK_Port , &GPIO_InitStructure);
		
		GPIO_SetBits(Brushless_GPIO[i].BRK_Port,Brushless_GPIO[i].BRK_Pin);
		*/  
		
		//FR
		RCC_APB2PeriphClockCmd(BLDC_GPIO[i].FR_RCC,ENABLE);
		GPIO_InitStructure.GPIO_Pin = BLDC_GPIO[i].FR_Pin; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(BLDC_GPIO[i].FR_Port , &GPIO_InitStructure);
		
		//SV
		RCC_APB2PeriphClockCmd(BLDC_GPIO[i].SV_RCC,ENABLE);
		GPIO_InitStructure.GPIO_Pin = BLDC_GPIO[i].SV_Pin; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	//普通推挽输出不可用	 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(BLDC_GPIO[i].SV_Port , &GPIO_InitStructure);
	}
}
//百分位控制，脉冲宽度从0到100
void  PWM_Set(u8 ch,float Pulse)
{
  	TIM_OCInitTypeDef        TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_Pulse = (u16)Pulse*36;////////////////
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
  	switch(ch)
  	{
		case 0:
	  {
	  		TIM_OC1Init(TIM3, &TIM_OCInitStructure);          	//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	  		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//使能TIMx在CCR2上的预装载寄存器
			break;
	  }
  		case 1:
	  {
	  		TIM_OC2Init(TIM3, &TIM_OCInitStructure);          	//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	  		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//使能TIMx在CCR2上的预装载寄存器
			break;
	  }case 2:
	  {
	  		TIM_OC3Init(TIM3, &TIM_OCInitStructure);          	//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	  		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//使能TIMx在CCR2上的预装载寄存器
			break;
	  }case 3:
	  {
	  		TIM_OC4Init(TIM3, &TIM_OCInitStructure);          	//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	  		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//使能TIMx在CCR2上的预装载寄存器
			break;
	  }
  	}
}


void Motion_Control(u8 driection,float speed)
{//PWM通道0是右侧
  
  if((driection==MOTION_AUTO)                           || 
       (driection==MOTION_LEFT_BACKWARD)                || 
         (driection==MOTION_RIGHT_BACKWARD)             || 
           (driection==MOTION_BACKWARD)                 || 
             (driection==MOTION_STOP)                   || 
               (driection==MOTION_LEFT_SPIN)            || 
                 (driection==MOTION_RIGHT_SPIN)         ||
                   (driection==MOTION_DANCE_START)      ||
                     (driection==MOTION_DANCE_STOP))
  {
    switch (driection)
    {
    case MOTION_AUTO:
      {
         autoMotion(speed);
      }
      break;
    case MOTION_LEFT_BACKWARD://左后
      {
          backwardLeft(speed);
      }
      break;
    case MOTION_RIGHT_BACKWARD://右后
      {
          backwardRight(speed);
      }break; 
    case MOTION_LEFT_SPIN://左旋
      {
          leftSpin(speed);
      }break;
    case MOTION_RIGHT_SPIN://右旋
      {
          rightSpin(speed);
      }break;
    case MOTION_BACKWARD://后
      {
          backWard(speed);
      }break;     
    case MOTION_DANCE_START:
      {  
          second = calendar.sec % 8;
          DJ_Dance_controlFlag = 1;
          switch(second)
          {
          case 0:
            leftSpin(speed);
            break;
          case 1:
            leftSpin(speed);
            break;
          case 2:
            leftSpin(speed);
            break;
          case 3:
            leftSpin(speed);
            break;
          case 4:
            stopMove(speed);
            break;
          case 5:
            rightSpin(speed);
            break;
          case 6:
            rightSpin(speed);
            break;
          case 7:
            rightSpin(speed);
            break;
          case 8:
            rightSpin(speed);
            break;
          case 9:
            stopMove(speed);
            break;
          default:
            //stopMove(speed);
            //Action_Play(0);
            break;
            
          }
      }
      break;
    case MOTION_DANCE_STOP:
      {
          DJ_Dance_controlFlag = 0;
          /////////////////////
          //DanceLoopflag = 1;
          GoalGestureFlag = 0;
          
          stopMove(speed);
          Action_Play(0);
          Eye_Action(1);
          Driection_Cmd = '\0';
          m = 0;
          Servo_Arg.DanceMoveNum = 0;
      }break;
    case MOTION_STOP://停
      {
          stopMove(speed);         
      }break;

    }
  }
  else
  {
    if((Obstacle_Weight[LEFT]||Obstacle_Weight[RIGHT]))
    {//如果不是前面的指令，须要决定避障策略
      //有一个小于60CM的就停止
      //driection = MOTION_STOP;
    }
    switch (driection)
    {
        case MOTION_FORWARD://前
          {
              forwardMove(speed);
          }break;
        case MOTION_LEFT_FORWARD://左前
          {
              forwardLeft(speed);
          }break;
        case MOTION_RIGHT_FORWARD://右前
          {
              forwardRight(speed);
          }break;
        case MOTION_STOP://停
          {
              stopMove(speed);
          }break; 
        case 'C'://停
          {
              Test_Action_Flag = 1;
              Driection_Cmd = '\0';
          }break; 
        case 'D'://停
          {
              Test_Action_Flag = 0;
              Driection_Cmd = '\0';
          }break; 
        default:
    //      {//错误指令会从else进来
    //          //stopMove(speed);
    //      }
          break;
    }
  }
  
}
void autoMotion(float speed)
{
    if(!(Obstacle_Weight[LEFT]||Obstacle_Weight[RIGHT]))//如果都为0，没有障碍物（或完取非才为1）
    {//直行
        GPIO_ResetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
        GPIO_SetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
        PWM_Set(0,speed+forwardSpeedOffset);
        PWM_Set(1,speed);
    }
    else if(Obstacle_Weight[LEFT]>Obstacle_Weight[RIGHT])
    {//左侧障碍物权重大，右转
//        GPIO_ResetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
//        GPIO_SetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
//        PWM_Set(0,0);
//        PWM_Set(1,12+Obstacle_Weight[LEFT]);
        GPIO_ResetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
        GPIO_SetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
        PWM_Set(0,12+Obstacle_Weight[RIGHT]+ forwardSpeedOffset);
        PWM_Set(1,0);
    }
    else if(Obstacle_Weight[LEFT]<Obstacle_Weight[RIGHT])
    {//右侧障碍物权重大，左转
//        GPIO_ResetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
//        GPIO_SetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
//        PWM_Set(0,12+Obstacle_Weight[RIGHT]);
//        PWM_Set(1,0);
        GPIO_ResetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
        GPIO_SetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
        PWM_Set(0,0);
        PWM_Set(1,12+Obstacle_Weight[LEFT]);
    }
    else if(Obstacle_Weight[LEFT]==Obstacle_Weight[RIGHT])//如果两侧都有，并且权重相等
    {//判断哪侧距离障碍物更远
      if(Distance_Sum[LEFT]>Distance_Sum[RIGHT])
      {//左侧总距离长，左转
//          Delay_ms(500);
//          GPIO_ResetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
//          GPIO_SetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
//          PWM_Set(0,speed);
//          PWM_Set(1,0);
            Delay_ms(500);
            GPIO_ResetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
            GPIO_SetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
            PWM_Set(0,0);
            PWM_Set(1,speed);
      }
      else if(Distance_Sum[RIGHT]>Distance_Sum[LEFT])
      {//右侧总距离长，右转
//          Delay_ms(500);
//          GPIO_ResetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
//          GPIO_SetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
//          PWM_Set(0,0);
//          PWM_Set(1,speed);
          Delay_ms(500);
          GPIO_ResetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
          GPIO_SetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
          PWM_Set(0,speed+forwardSpeedOffset);
          PWM_Set(1,0);
      }
      else
      {
          delay_ms(500);
          PWM_Set(0,0);
          PWM_Set(1,0);
      }
    }
    else
    {
    }
}
void backwardLeft(float speed)//左后
{
//    GPIO_SetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
//    GPIO_ResetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
//    PWM_Set(0,speed);
//    PWM_Set(1,speed/2);
    
    GPIO_SetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
    GPIO_ResetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
    PWM_Set(0,speed/2+backwardSpeedOffset);
    PWM_Set(1,speed);    
}
void backwardRight(float speed)//右后
{
//    GPIO_SetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
//    GPIO_ResetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
//    PWM_Set(0,speed/2);
//    PWM_Set(1,speed);
    
    GPIO_SetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
    GPIO_ResetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
    PWM_Set(0,speed+backwardSpeedOffset);
    PWM_Set(1,speed/2);
}
void leftSpin(float speed)//左旋转
{
//    GPIO_ResetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
//    GPIO_ResetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
//    PWM_Set(0,speed/2);
//    PWM_Set(1,speed/2);
    
    GPIO_SetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
    GPIO_SetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
    PWM_Set(0,speed/2+backwardSpeedOffset);
    PWM_Set(1,speed/2);    
}
void rightSpin(float speed)//右旋转
{
//    GPIO_SetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
//    GPIO_SetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
//    PWM_Set(0,speed/2);
//    PWM_Set(1,speed/2);
    
    GPIO_ResetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
    GPIO_ResetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
    PWM_Set(0,speed/2+forwardSpeedOffset);
    PWM_Set(1,speed/2);    
}
void backWard(float speed)//后退
{
    GPIO_SetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
    GPIO_ResetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
    PWM_Set(0,speed+backwardSpeedOffset);//三号FangFang 左轮慢一点儿，需要加1.8，左右轮才一致
    PWM_Set(1,speed);
    
    
}
void stopMove(float speed)//停止
{  
    PWM_Set(0,0);
    PWM_Set(1,0);

}
void forwardMove(float speed)//向前
{
    GPIO_ResetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
    GPIO_SetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
    PWM_Set(0,speed+forwardSpeedOffset);//三号FangFang 左轮慢一点儿，需要加1.5，左右轮才一致
    PWM_Set(1,speed);
    
    
}
void forwardLeft(float speed)//左前
{
//    GPIO_ResetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
//    GPIO_SetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
//    PWM_Set(0,speed);
//    PWM_Set(1,speed/2);
    
    GPIO_ResetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
    GPIO_SetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
    PWM_Set(0,speed/2+forwardSpeedOffset);
    PWM_Set(1,speed);    
}
void forwardRight(float speed)//右前
{
//    GPIO_ResetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
//    GPIO_SetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
//    PWM_Set(0,speed/2);
//    PWM_Set(1,speed);
    
    GPIO_ResetBits(BLDC_GPIO[0].FR_Port,BLDC_GPIO[0].FR_Pin);
    GPIO_SetBits(BLDC_GPIO[1].FR_Port,BLDC_GPIO[1].FR_Pin);
    PWM_Set(0,speed+forwardSpeedOffset);
    PWM_Set(1,speed/2);    
}