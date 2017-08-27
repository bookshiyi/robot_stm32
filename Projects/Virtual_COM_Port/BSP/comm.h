#ifndef __COMM_H
#define __COMM_H
/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： smartbox.h
*	@Brief   	： 智能柜头文件
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

#include "usart.h"


/*****************************		数据帧位置定义	********************/
#define HEAD					0x00
#define OBJ						0X01
#define CMD						0X02
#define PARAM1					0X03
#define PARAM2					0X04
#define PARAM3					0X05
#define PARAM4					0X06
#define PARAM5					0X07
#define PARAM6					0X08
#define PARAM7					0X09
#define PARAM8					0X0A
#define PARAM9					0X0B
#define PARAM10					0X0C
#define PARAM11					0X0D
#define PARAM12					0X0E
#define PARAM13					0X0F
#define PARAM14					0X10
#define PARAM15					0X11
#define PARAM16					0X12
#define PARAM17					0X13
#define PARAM18					0X14
#define PARAM19					0X15
#define PARAM20					0X16
#define PARAM21					0X17
#define PARAM22					0X18
#define PARAM23					0X19
#define PARAM24					0X1A
/*****************************		数据帧内容定义	********************/
//帧头
#define ROS_FRAME_HEAD			0XA5
//对象
#define BEEP_OBJ				0X01	//蜂鸣器对象
#define MOTION_OBJ				0x02	//运动对象
#define IMU_OBJ                 0x03    //IMU对象	
#define RESOLVE_OBJ				0X04	//姿态解算对象
#define DISTANCE_OBJ 			0X05	//测距对象
#define BAT_OBJ		 			0X06	//电池对象
#define IR_OBJ					0X07	//红外对象

#define DEBUG_OBJ	            0X09
#define SELF_CHECK_OBJ          0X0A
#define POS_OBJ                 0X10    //位置数据
//指令
#define WRITE_CMD				0X80		//写命令不需要返回
#define READ_CMD				0X81		//读取和返回是一对
#define RETURN_CMD				0X82
//返回参数		
#define OK_PARAM				0X01
#define ERROR_PARAM				0X02
//帧尾
#define ROS_FRAME_END			0XAA


void Bat_to_ROS();
void AHRS_Data_to_ROS();
void Resolving_Data_to_ROS();
void Motion_Data_Status_to_ROS();
void Distance_Data_to_ROS(u8 num);

void ROS_Data_Process(Element_Data_TypDef source);
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
#endif




