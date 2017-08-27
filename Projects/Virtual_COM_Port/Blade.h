#ifndef __BLADE_H
#define __BLADE_H
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

#include "Sys_Conf.h"
#include "usb_type.h"//使用枚举的BOOL型

extern u16 BIT[16];

#define LEFT 0
#define RIGHT 1
#define MAX     0
#define MIN     1


s32 abs_s32(s32 i);
void System_Init();
void Main_Process();




#endif




