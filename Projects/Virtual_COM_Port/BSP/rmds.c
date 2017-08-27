/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： RMDS.c
*	@Brief   	： 
*	@Author  	： admin@oarap.org
*	@Hardware	： STM32F103RCT6
*	@Date		： 2017-03
*	@Description    ： RMDS驱动器通信单元
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
RMDS_Frame_TypeDef RMDS_Frame_TX;

void RMDS_GPIO_Config()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RMDS_GPIO_RCC,ENABLE);
  GPIO_InitStructure.GPIO_Pin = RMDS_GPIO_PIN; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//普通推挽输出不可用	 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RMDS_GPIO_PORT , &GPIO_InitStructure);
  
  RMDS_RX_EN();														//使能485接收
}
void RMDS_Send_Frame(RMDS_Frame_TypeDef rmds_frame)
{
  u8 send_buf[10];
  u8 i;
  send_buf[0] = rmds_frame.Frame_Head;
  send_buf[1] = rmds_frame.RMDS_ID;
  for(i=0;i<8;i++)
	send_buf[i+2] = RMDS_Frame_TX.Data[i];   
  RMDS_TX_EN();														//使能485发送
  USART_Send_Assign(RMDS_USART,send_buf,10);
  Delay_us(2);///////////////////////////////////////////////
  RMDS_RX_EN();														//使能485接收
}
void RMDS_Reset(u16 rmds_id,u8 cmd)
{//发送命令,如果ID为0则广播该指令
  u8 i;
  for(i=0;i<8;i++)
	RMDS_Frame_TX.Data[i] = 0X55;
  RMDS_Frame_TX.Frame_Head = RMDS_FRAME_HEAD;
  RMDS_Frame_TX.RMDS_ID = (rmds_id<<4) | cmd;
  
  RMDS_Send_Frame(RMDS_Frame_TX);
}
void RMDS_Mode_Change_Send(u16 rmds_id,u8 mode)
{//改变模式
  u8 i;
  for(i=0;i<8;i++)
	RMDS_Frame_TX.Data[i] = 0X55;
  RMDS_Frame_TX.Frame_Head = RMDS_FRAME_HEAD;
  RMDS_Frame_TX.RMDS_ID = (rmds_id<<4) | 1;
  RMDS_Frame_TX.Data[0] = mode;
  
  RMDS_Send_Frame(RMDS_Frame_TX);
}
void RMDS_Vel_Pos_Send_Arg(u16 rmds_id,u16 pwm,u16 vel,s32 pos)//vel的单位是RPM
{//速度位置模式下发送参数
  RMDS_Frame_TX.Frame_Head = RMDS_FRAME_HEAD;
  RMDS_Frame_TX.RMDS_ID = (rmds_id<<4) | 6;//功能序号
  RMDS_Frame_TX.Data[0] = (u8)(pwm>>8);
  RMDS_Frame_TX.Data[1] = (u8)(pwm);
  RMDS_Frame_TX.Data[2] = (u8)(vel>>8);
  RMDS_Frame_TX.Data[3] = (u8)(vel);
  RMDS_Frame_TX.Data[4] = (u8)(pos>>24);
  RMDS_Frame_TX.Data[5] = (u8)(pos>>16);
  RMDS_Frame_TX.Data[6] = (u8)(pos>>8);
  RMDS_Frame_TX.Data[7] = (u8)(pos);
  
  RMDS_Send_Frame(RMDS_Frame_TX);
}
void RMDS_Vel_Send_Arg(u16 rmds_id,u16 pwm,u16 vel)//vel的单位是RPM
{//速度模式下发送参数
  RMDS_Frame_TX.Frame_Head = RMDS_FRAME_HEAD;
  RMDS_Frame_TX.RMDS_ID = (rmds_id<<4) | 4;//功能序号
  RMDS_Frame_TX.Data[0] = (u8)(pwm>>8);
  RMDS_Frame_TX.Data[1] = (u8)(pwm);
  RMDS_Frame_TX.Data[2] = (u8)(vel>>8);
  RMDS_Frame_TX.Data[3] = (u8)(vel);
  RMDS_Frame_TX.Data[4] = 0x55;
  RMDS_Frame_TX.Data[5] = 0x55;
  RMDS_Frame_TX.Data[6] = 0x55;
  RMDS_Frame_TX.Data[7] = 0x55;
  
  RMDS_Send_Frame(RMDS_Frame_TX);
}
void RMDS_Get_Status(u8 rmds_id)
{//获取电机的运行参数
  u8 i;
  for(i=0;i<8;i++)
	RMDS_Frame_TX.Data[i] = 0X55;
  RMDS_Frame_TX.Frame_Head = RMDS_FRAME_HEAD;
  RMDS_Frame_TX.RMDS_ID = (rmds_id<<4) | 0X0A;
  RMDS_Frame_TX.Data[0] = 0X01;

  RMDS_Send_Frame(RMDS_Frame_TX);
}
void RMDS_Ping_Device(u8 rmds_id)
{
  u8 i;
  for(i=0;i<8;i++)
	RMDS_Frame_TX.Data[i] = 0X55;
  RMDS_Frame_TX.Frame_Head = RMDS_FRAME_HEAD;
  RMDS_Frame_TX.RMDS_ID = (rmds_id<<4) | 0X0F;
  
  RMDS_Send_Frame(RMDS_Frame_TX);
}
void RMDS_Init(u8 mode)
{//初始化
  RMDS_GPIO_Config();
  RMDS_Reset(0,RMDS_RESET_CMD);
  Delay_ms(30);
  RMDS_Reset(0,RMDS_RESET_CMD);
  Delay_ms(30);
  RMDS_Reset(0,RMDS_RESET_CMD);
  Delay_ms(500);
  RMDS_Mode_Change_Send(0,mode);	
}
void RMDS_USART_Data_RX_Handler(u16 usart_data)
{
  u8 Device_ID;
  u8 i=0;/////////////////////////////////////////////////

  USART_Data.RMDS.Data[USART_Data.RMDS.Pointer]=usart_data;	
  USART_Data.RMDS.Pointer++;
  if(USART_Data.RMDS.Pointer>9)
  {//接受完一帧数据
	if((USART_Data.RMDS.Data[0] == 0x48) && (USART_Data.RMDS.Data[1] & 0x0B) == 0x0B)//说明是返回的参数数据
	{
	  Device_ID = USART_Data.RMDS.Data[1]>>4;
	  Motion_Data.Status.Cur[Device_ID-1]  = (USART_Data.RMDS.Data[2]<<8)|USART_Data.RMDS.Data[3];
	  Motion_Data.Status.Motor_Ang_Vel[Device_ID-1][0] = (USART_Data.RMDS.Data[4]<<8)|USART_Data.RMDS.Data[5];
	  Motion_Data.Status.Pos[Device_ID-1] = (USART_Data.RMDS.Data[6]<<24)|(USART_Data.RMDS.Data[7]<<16)|(USART_Data.RMDS.Data[8]<<8)|USART_Data.RMDS.Data[9];

	  for(i=AVERAGE_NUM-1;i>0;i--)//滑动滤波
	  	Motion_Data.Status.Motor_Ang_Vel[Device_ID-1][i] = Motion_Data.Status.Motor_Ang_Vel[Device_ID-1][i-1];
	}
	USART_Data.RMDS.Pointer=0;
  }
  if(USART_Data.RMDS.Data[0] != 0x48)
	USART_Data.RMDS.Pointer = 0;
}
