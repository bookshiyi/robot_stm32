/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： Sy.c
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

void Distance_Data_to_ROS(u8 num)
{
  u8 send_buf[16];
  u8 i;
  
  for(i=0;i<num;i+=2)
  {
	  send_buf[i]  = (Ultrasonic_Distance[i/2]) >> 8;
	  send_buf[i+1] = (Ultrasonic_Distance[i/2]);
  }

  Bluetooth_Send_Data_to_ROS(DISTANCE_OBJ,WRITE_CMD,send_buf,num*2);
  USB_Serial_Send_Data_to_ROS(DISTANCE_OBJ,WRITE_CMD,send_buf,num*2);	
}
/*
***电池数据反馈示例：
0xA5 0x05 0x80
{Bat_Vol1    Bat_Vol2}			//Bat_Vol，u16类型			//Ang.z，s16类型
0xAA*/
void Bat_to_ROS()
{
    u8 send_buf[2];	
    u16 Bat_Voltage;
    Bat_Voltage = AD_to_Voltage();
	
    send_buf[0] = Bat_Voltage >> 8;
    send_buf[1] = Bat_Voltage;

    Bluetooth_Send_Data_to_ROS(BAT_OBJ,WRITE_CMD,send_buf,2);
	USB_Serial_Send_Data_to_ROS(BAT_OBJ,WRITE_CMD,send_buf,2);
}


void AHRS_Data_to_ROS()
{//惯性单元数据反馈
  u8 send_buf[24];
  
  send_buf[0]  = (AHRS_Data.IMU.Acc.x) >> 8;
  send_buf[1]  = (AHRS_Data.IMU.Acc.x);
  send_buf[2]  = (AHRS_Data.IMU.Acc.y) >> 8;          
  send_buf[3]  = (AHRS_Data.IMU.Acc.y);   
  send_buf[4]  = (AHRS_Data.IMU.Acc.z) >> 8;
  send_buf[5]  = (AHRS_Data.IMU.Acc.z) ;
                    
  send_buf[6]  = (AHRS_Data.IMU.Gyro.x)>> 8;
  send_buf[7]  = (AHRS_Data.IMU.Gyro.x);
  send_buf[8]  = (AHRS_Data.IMU.Gyro.y)>> 8;
  send_buf[9]  = (AHRS_Data.IMU.Gyro.y); 
  send_buf[10] = (AHRS_Data.IMU.Gyro.z)>> 8;    
  send_buf[11] = (AHRS_Data.IMU.Gyro.z);                     
    /*        
  send_buf[12] = (AHRS_Data.IMU.Mag.x) >> 8;
  send_buf[13] = (AHRS_Data.IMU.Mag.x);
  send_buf[14] = (AHRS_Data.IMU.Mag.y) >> 8;                 
  send_buf[15] = (AHRS_Data.IMU.Mag.y);    
  send_buf[16] = (AHRS_Data.IMU.Mag.z) >> 8;
  send_buf[17] = (AHRS_Data.IMU.Mag.z);
  */
  Bluetooth_Send_Data_to_ROS(IMU_OBJ,WRITE_CMD,send_buf,12);/////////////////
  USB_Serial_Send_Data_to_ROS(IMU_OBJ,WRITE_CMD,send_buf,12);///////////
}
void Resolving_Data_to_ROS()
{//解算数据反馈
  u8 send_buf[24];
  
  send_buf[0]  = (AHRS_Data.IMU.Ang.x) >> 8;
  send_buf[1]  = (AHRS_Data.IMU.Ang.x);
  send_buf[2]  = (AHRS_Data.IMU.Ang.y) >> 8;          
  send_buf[3]  = (AHRS_Data.IMU.Ang.y);   
  send_buf[4]  = (AHRS_Data.IMU.Ang.z) >> 8;
  send_buf[5]  = (AHRS_Data.IMU.Ang.z) ;
  
  send_buf[6]  = (AHRS_Data.IMU.Quat.q0)>> 8;
  send_buf[7]  = (AHRS_Data.IMU.Quat.q0);
  send_buf[8]  = (AHRS_Data.IMU.Quat.q1)>> 8;
  send_buf[9]  = (AHRS_Data.IMU.Quat.q1); 
  send_buf[10] = (AHRS_Data.IMU.Quat.q2)>> 8;    
  send_buf[11] = (AHRS_Data.IMU.Quat.q2);                                     
  send_buf[12] = (AHRS_Data.IMU.Quat.q3) >> 8;
  send_buf[13] = (AHRS_Data.IMU.Quat.q3);
  
  Bluetooth_Send_Data_to_ROS(RESOLVE_OBJ,WRITE_CMD,send_buf,14);/////////////////
  USB_Serial_Send_Data_to_ROS(RESOLVE_OBJ,WRITE_CMD,send_buf,14);///////////
}
/*
0xA5 0x02 0x80
{Motor_Ang_Vel_Average[LEFT]1   Motor_Ang_Vel_Average2[LEFT]} 	//左轮RPM，s16类型
{Motor_Ang_Vel_Average[RIGHT]1  Motor_Ang_Vel_Average2[RIGHT]}  //右轮RPM，s16类型
{Pos[LEFT]1   Pos[LEFT]2  Pos[LEFT]3  Pos[LEFT]4} 				//左轮编码器,s32类型
{Pos[RIGHT]1  Pos[RIGHT]2 Pos[RIGHT]3 Pos[RIGHT]4}              //右轮编码器,s32类型
0xAA*/
void Motion_Data_Status_to_ROS()
{
  u8 send_buf[12];
  
  send_buf[0] = Motion_Data.Status.Motor_Ang_Vel_Average[LEFT]>>8;
  send_buf[1] = Motion_Data.Status.Motor_Ang_Vel_Average[LEFT];
  send_buf[2] = Motion_Data.Status.Motor_Ang_Vel_Average[RIGHT]>>8;
  send_buf[3] = Motion_Data.Status.Motor_Ang_Vel_Average[RIGHT];
           
  send_buf[4] = Motion_Data.Status.Pos[LEFT]>>24;
  send_buf[5] = Motion_Data.Status.Pos[LEFT]>>16;
  send_buf[6] = Motion_Data.Status.Pos[LEFT]>>8;
  send_buf[7] = Motion_Data.Status.Pos[LEFT];
           
  send_buf[8] = Motion_Data.Status.Pos[RIGHT]>>24;
  send_buf[9] = Motion_Data.Status.Pos[RIGHT]>>16;
  send_buf[10]= Motion_Data.Status.Pos[RIGHT]>>8;
  send_buf[11]= Motion_Data.Status.Pos[RIGHT];
  
  Bluetooth_Send_Data_to_ROS(MOTION_OBJ,WRITE_CMD,send_buf,12);
  USB_Serial_Send_Data_to_ROS(MOTION_OBJ,WRITE_CMD,send_buf,12);
}
void Motion_to_ROS_Demo()
{
  u8 send_buf[12];
  s16 test1 =-60;
  s16 test2 =-60;
  send_buf[0] = test1>>8;
  send_buf[1] = test1;
  send_buf[2] = test2>>8;
  send_buf[3] = test2;
           
  send_buf[4] = Motion_Data.Status.Pos[LEFT]>>24;
  send_buf[5] = Motion_Data.Status.Pos[LEFT]>>16;
  send_buf[6] = Motion_Data.Status.Pos[LEFT]>>8;
  send_buf[7] = Motion_Data.Status.Pos[LEFT];
           
  send_buf[8] = Motion_Data.Status.Pos[RIGHT]>>24;
  send_buf[9] = Motion_Data.Status.Pos[RIGHT]>>16;
  send_buf[10]= Motion_Data.Status.Pos[RIGHT]>>8;
  send_buf[11]= Motion_Data.Status.Pos[RIGHT];
  
  Bluetooth_Send_Data_to_ROS(MOTION_OBJ,WRITE_CMD,send_buf,12);
  USB_Serial_Send_Data_to_ROS(MOTION_OBJ,WRITE_CMD,send_buf,12);

}

bool Check_Frame(u8 *data)
{
	u8 i=0;
	for(i=0;i<32;i++)
	{
		if((data[i] == ROS_FRAME_END) && (data[i-1] == i+1))
		{
				return TRUE;	
		}
	}
	return FALSE;
}
void ROS_Data_Process(Element_Data_TypDef source)
{
  if(source.Flag == TRUE)
  {
	if(Check_Frame(source.Data))
	{
		switch (source.Data[OBJ])
		{
			case BEEP_OBJ:
			  Beep_Obj_Handler(&source.Data[PARAM1]);
			  //Bluetooth_Send_Return_to_ROS(BEEP_OBJ,OK_PARAM);
			  break;
			case MOTION_OBJ:
			  Motion_Control_Obj_Handler(&source.Data[PARAM1]);
			  break;
			default: break;
			}
			//清空标志位
			if(source.Descrip== USB_DESCRIP)
				USART_Data.USB.Flag=FALSE;
			if(source.Descrip== BT_DESCRIP)
				USART_Data.BT.Flag=FALSE;
	  }
  }
}


