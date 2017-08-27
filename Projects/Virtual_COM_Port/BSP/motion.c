/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： ahrs.c
*	@Brief   	： 
*	@Author  	： admin@oarap.org
*	@Hardware	： STM32F103RCT6
*	@Date		： 2017-03
*	@Description    ： 惯性单元MPU9250+JY901 的ahrs
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

Motion_Data_Typdef Motion_Data;


/*******************************************************************************
	运动数据初始化
********************************************************************************/
void Motion_Data_Init()
{//运动数据初始化
  	Motion_Data.Control.Update_Flag = FALSE;
	Motion_Data.Control.Enable_Flag = FALSE;
	Motion_Data.Control.PWM_Limit = 5000;
}
/*******************************************************************************
	上位机指令超时检测（在RTC中断中被调用）
********************************************************************************/
void Motion_Control_Data_Time_Out_Check_Handler()
{
  if (!Motion_Data.Control.Update_Flag)//1s时间内没收到数据则停止运动、
  {
    Motion_Data.Control.Motor_Ang_Vel[LEFT]  = 0;
    Motion_Data.Control.Motor_Ang_Vel[RIGHT] = 0;
  }
  Motion_Data.Control.Update_Flag = FALSE;
}
/*******************************************************************************
	运动控制对象的处理程序（在ros发来数据后可能被调用）
********************************************************************************/
void Motion_Control_Obj_Handler(u8 *param)
{
	Motion_Data.Control.Motor_Ang_Vel[LEFT]  =  (param[0]<<8 | param[1]);
	Motion_Data.Control.Motor_Ang_Vel[RIGHT] =  (param[2]<<8 | param[3]);
	/*Motion_Data.Control.Motor_Ang_Vel[LEFT] /= 100;
	Motion_Data.Control.Motor_Ang_Vel[RIGHT] /=  100;*/
	Motion_Data.Control.Update_Flag = TRUE;
}
/*******************************************************************************
	运动控制数据的发送（发送给驱动器）
********************************************************************************/
void Motion_Control_Data_Send()
{ //运动控制数据发送（包括计算） 
  //if(Motion_Data.Control.Enable_Flag)   
	  RMDS_Vel_Send_Arg(1,Motion_Data.Control.PWM_Limit,Motion_Data.Control.Motor_Ang_Vel[LEFT]);
	  Delay_ms(1);              
	  RMDS_Vel_Send_Arg(2,Motion_Data.Control.PWM_Limit,Motion_Data.Control.Motor_Ang_Vel[RIGHT]);
	  Delay_ms(1); 
	  
	  //防止溢出
	  if((abs_s32(Motion_Data.Status.Pos[LEFT])  >= MAX_POSITIVE_POS) || 
		 (abs_s32(Motion_Data.Status.Pos[RIGHT]) >= MAX_POSITIVE_POS))
	  {
		Motion_Data.Status.Motor_Ang_Vel[LEFT] [0] = 0;
		Motion_Data.Status.Motor_Ang_Vel[RIGHT][0] = 0;
		
		RMDS_Init(RMDS_VEL_MODE);
		Delay_ms(500);
	  }
}
/*******************************************************************************
	运动状态数据的读取
********************************************************************************/
void Motion_Status_Data_Read()
{//运动状态数据读取（包括计算）
  RMDS_Get_Status(1);
  Delay_ms(3);
  RMDS_Get_Status(2);
  Delay_ms(3);
}
/*******************************************************************************
	运动角速度的平均值计算
********************************************************************************/
void Motor_Ang_Vel_Average_Calculate()
{
  	u8 i;
	s32 buf=0;
	for(i=0;i<AVERAGE_NUM;i++)
	{
		buf +=Motion_Data.Status.Motor_Ang_Vel[LEFT][i];
	}
	Motion_Data.Status.Motor_Ang_Vel_Average[LEFT] =buf/(AVERAGE_NUM);
	buf =0;
	for(i=0;i<AVERAGE_NUM;i++)
	{
		buf +=Motion_Data.Status.Motor_Ang_Vel[RIGHT][i];
	}
	Motion_Data.Status.Motor_Ang_Vel_Average[RIGHT] =buf/(AVERAGE_NUM);  
	  
	  
}