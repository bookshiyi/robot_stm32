/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： bluetooth.c
*	@Brief   	： 
*	@Author  	： admin@oarap.org
*	@Hardware	： STM32F103RCT6
*	@Date		： 2017-03
*	@Description    ： 蓝牙通讯单元
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
extern void USB_SendBuf(uint8_t *_pTxBuf, uint8_t _ucLen);//,发送数据的接口函数，通过ENDP1发送数据到上位机

/*******************************************************************************
	发送返回命令给上位机
********************************************************************************/
void USB_Serial_Send_Return_to_ROS(u8 obj,u8 param)
{
	u8 send_buf[6];
	
	send_buf[HEAD]   = ROS_FRAME_HEAD;
	send_buf[OBJ]    = obj;
	send_buf[CMD]    = RETURN_CMD;
	send_buf[PARAM1] = param;
	
	send_buf[PARAM1+1] 	 = 0x06;	//数据帧总长度
	send_buf[PARAM1+2] 	 = ROS_FRAME_END;
	USB_SendBuf(send_buf,6);
}
/*******************************************************************************
	发送数据给上位机
********************************************************************************/
void USB_Serial_Send_Data_to_ROS(u8 obj,u8 cmd,u8 *param,u8 len)
{
	u8 send_buf[32];
	u8 i;
	send_buf[HEAD]   = ROS_FRAME_HEAD;
	send_buf[OBJ]    = obj;
	send_buf[CMD]    = cmd;
	for(i=0;i<len;i++)
	{
	  send_buf[PARAM1+i] = param[i];
	}

	send_buf[len+3] 	 = len+5;
	send_buf[len+4] 	 = ROS_FRAME_END;
	USB_SendBuf(send_buf,len+5);
}
void debug_serial_to_ROS()
{
  u8 send_buf[1];
  static u8 i=0;
  send_buf[0]=i++;
  USB_Serial_Send_Data_to_ROS(DEBUG_OBJ,WRITE_CMD,send_buf,1);///////////
}

void debug_jy_901_to_mini_imu()
{
  u8 send_buf[12],sum=0,i=0;
  
  send_buf[0] = 0X55;
  send_buf[1] = 0X53;
  send_buf[2]  = (AHRS_Data.IMU.Ang.x) ;
  send_buf[3]  = (AHRS_Data.IMU.Ang.x)>> 8;
  send_buf[4]  = (AHRS_Data.IMU.Ang.y) ;          
  send_buf[5]  = (AHRS_Data.IMU.Ang.y)>> 8;   
  send_buf[6]  = (AHRS_Data.IMU.Ang.z) ;
  send_buf[7]  = (AHRS_Data.IMU.Ang.z)>> 8 ;
  send_buf[8]  = (AHRS_Data.IMU.Temp);
  send_buf[9]  = (AHRS_Data.IMU.Temp) >>8;
  
  for(i=0;i<10;i++)
  {
  	sum +=send_buf[i];
  }
  send_buf[10]  = sum ;
  
	USB_SendBuf(send_buf,11);
}
/*

	u8 check_sum=0;
	for(i=0;i<len+3;i++)
	{
		check_sum += send_buf[i];
	}
	send_buf[len+3] 	 = check_sum;


*/

