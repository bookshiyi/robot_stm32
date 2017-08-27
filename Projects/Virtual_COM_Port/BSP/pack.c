/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： packet.c
*	@Brief   	： 
*	@Author  	： admin@oarap.org
*	@Hardware	： STM32F103RCT6
*	@Date		： 2017-03
*	@Description    ： 通信协议中打包和解包函数
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

USB_Data_Frame_TypDef USB_Data_Frame_Tx;//发送用
USB_Data_Frame_TypDef USB_Data_Frame_Rx;//接受用，在USART中断中存入USART_DATA中，在后台处理过程中用结构体分解USART数据

void USB_Send_Data_Frame(USB_Data_Frame_TypDef *data_frame)//发送数据帧
{
  u8 i;
  u8 buf[32];
  data_frame->Head = ROS_FRAME_HEAD;//初始化结构体
  data_frame->End  = ROS_FRAME_END;
  buf[0] = data_frame->Head;//帧头
  buf[1] = data_frame->Obj;//对象
  buf[2] = data_frame->Cmd;//指令
  for(i=0;i < data_frame->Arg_Lenth;i++)
  {
    buf[i+3] = data_frame->Arg[i];
  }
  buf[i+3] = data_frame->End;//帧尾
  USB_SendBuf(buf,i+4);//发送Buf中的前i+3个数据
}
bool USB_Rec_Data_Frame(USB_Data_Frame_TypDef *data_frame)
{//将接收到的转存到数据接受结构体中
  u8 i;
  if(USART_Data.USB.Pointer < 4)
  {
    return FALSE;//数据错误
  }
  else
  {
  }
  data_frame->Arg_Lenth = USART_Data.USB.Pointer-4;
  data_frame->End = USART_Data.USB.Data[USART_Data.USB.Pointer-1];//节省时间
  for(i=USART_Data.USB.Pointer-2;i>=3;i--)//从后往前把数据存到结构体中，节省时间
  {     
    data_frame->Arg[i-3] = USART_Data.USB.Data[i];
  }
  data_frame->Cmd		     = USART_Data.USB.Data[2];
  data_frame->Obj		     = USART_Data.USB.Data[1];
  data_frame->Head		     = USART_Data.USB.Data[0];
  
  if((data_frame->Head!=0XA5) || (data_frame->End!=0XAA))
  {
    return FALSE;//数据错误
  }
  else
  {
    return TRUE;
  }
}
void USB_Send_Return_to_PC(u8 obj,u8 arg)
{
  USB_Data_Frame_Tx.Obj    = obj;
  USB_Data_Frame_Tx.Cmd    = RETURN_CMD;
  USB_Data_Frame_Tx.Arg[0] = arg;
  USB_Data_Frame_Tx.Arg_Lenth = 1;
  USB_Send_Data_Frame(&USB_Data_Frame_Tx);
}
void USB_Send_Data_to_PC(u8 obj,u8 cmd,u8 *arg,u8 arg_len)
{
  u8 i;
  USB_Data_Frame_Tx.Obj    = obj;
  USB_Data_Frame_Tx.Cmd    = cmd;
  for(i=0;i<arg_len;i++)
  {
    USB_Data_Frame_Tx.Arg[i] = arg[i];
  }
  USB_Data_Frame_Tx.Arg_Lenth = arg_len;
  USB_Send_Data_Frame(&USB_Data_Frame_Tx);
}


///////////////////////////////////////
bool Data_Frame_Check(USB_Data_Frame_TypDef *data_frame)
{//将接收到的转存到数据接受结构体中
  u8 i;
  if(USART_Data.USB.Pointer < 4)
  {
    return FALSE;//数据错误
  }
  else
  {
  }
  data_frame->Arg_Lenth = USART_Data.USB.Pointer-4;
  data_frame->End = USART_Data.USB.Data[USART_Data.USB.Pointer-1];//节省时间
  for(i=USART_Data.USB.Pointer-2;i>=3;i--)//从后往前把数据存到结构体中，节省时间
  {     
    data_frame->Arg[i-3] = USART_Data.USB.Data[i];
  }
  data_frame->Cmd		     = USART_Data.USB.Data[2];
  data_frame->Obj		     = USART_Data.USB.Data[1];
  data_frame->Head		     = USART_Data.USB.Data[0];
  
  if((data_frame->Head!=0XA5) || (data_frame->End!=0XAA))
  {
    return FALSE;//数据错误
  }
  else
  {
    return TRUE;
  }
}