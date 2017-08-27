/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： Sys_Conf.h.c
*	@Brief   	： 
*	@Author  	： admin@oarap.org
*	@Hardware	： STM32F103RCT6
*	@Date		： 2017-03
*	@Description    ： 系统基础配置文件
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYS_CONF_H
#define __SYS_CONF_H
	
//官方库
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "stm32_it.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h" 
#include "math.h"
//驱动库
//#include "iap.h"
#include "Blade.h"
#include "adc.h"
#include "usb_type.h"
#include "comm.h"
//#include "ccd.h"
//#include "hmi.h"
#include "gpio.h"
#include "rtc.h"
//#include "dht11.h"
//#include "lock.h"
#include "usart.h"
//#include "i2c_sw.h"
//#include "24C02.h"
#include "systick.h"
//#include "syn6288.h"
//#include "sim900a.h"
//#include "lcd12864.h"
//#include "Dynamixel.h"
#include "exti.h"
#include "str.h"
#include "timer.h"
#include "flash.h"
#include "rmds.h"
#include "ultrasonic.h"
#include "bluetooth.h"
#include "wireless.h"
//#include "pack.h"
#include "motion.h"
#include "usb_serial.h"
//#include "odom.h" 

#include "sw_iic.h"             //IIC的驱动程序
#include "ahrs.h"                //IMU传感器单元，同时支持裸MPU9250和JY-901





	

#define BIT0	0x0001
#define BIT1	0x0002
#define BIT2	0x0004
#define BIT3	0x0008
#define BIT4	0x0010
#define BIT5	0x0020
#define BIT6	0x0040
#define BIT7	0x0080
#define BIT8	0x0100
#define BIT9	0x0200
#define BIT10	0x0400
#define BIT11	0x0800
#define BIT12	0x1000
#define BIT13	0x2000
#define BIT14	0x4000
#define BIT15	0x8000
 
    










#endif 