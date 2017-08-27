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


//模式设置函数，设置端口的输入和输出方向
void GPIO_ModeSet(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIOMode_TypeDef GPIO_Mode)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;//选择要用的GPIO引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode; //设置引脚模式为推免输出模式						 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置引脚速度为50MHZ         
  GPIO_Init(GPIOx, &GPIO_InitStructure);//调用库函数，初始化GPIO
}

//初始化IIC
void IIC_GPIO_Init(void)
{					     
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(IIC_SCL_RCC | IIC_SDA_RCC, ENABLE );	
  
  GPIO_InitStructure.GPIO_Pin = IIC_SCL_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(IIC_SCL_Port, &GPIO_InitStructure);
  GPIO_SetBits(IIC_SCL_Port,IIC_SCL_Pin); 	
  
  GPIO_InitStructure.GPIO_Pin = IIC_SDA_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(IIC_SDA_Port, &GPIO_InitStructure);
  GPIO_SetBits(IIC_SDA_Port,IIC_SDA_Pin); 
  
}
//产生IIC起始信号
void IIC_Start(void)
{
  IIC_SDA_OUT();     //sda线输出
  IIC_SDA_Write(Bit_SET);	  	  
  IIC_SCL_Write(Bit_SET);
  Delay_us(4);
  IIC_SDA_Write(Bit_RESET);//START:when CLK is high,DATA change form high to low 
  Delay_us(4);
  IIC_SCL_Write(Bit_RESET);//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
  IIC_SDA_OUT();//sda线输出
  IIC_SCL_Write(Bit_RESET);
  IIC_SDA_Write(Bit_RESET);//STOP:when CLK is high DATA change form low to high
  Delay_us(4);
  IIC_SCL_Write(Bit_SET); 
  IIC_SDA_Write(Bit_SET);//发送I2C总线结束信号
  Delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
  u8 ucErrTime=0;
  IIC_SDA_IN();      //SDA设置为输入  
  IIC_SDA_Write(Bit_SET);Delay_us(1);	   
  IIC_SCL_Write(Bit_SET);Delay_us(1);	 
  while(IIC_SDA_Read())
  {
    ucErrTime++;
    if(ucErrTime>250)
    {
      IIC_Stop();
      return 1;
    }
  }
  IIC_SCL_Write(Bit_RESET);//时钟输出0 	   
  return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
  IIC_SCL_Write(Bit_RESET);
  IIC_SDA_OUT();
  IIC_SDA_Write(Bit_RESET);
  Delay_us(2);
  IIC_SCL_Write(Bit_SET);
  Delay_us(2);
  IIC_SCL_Write(Bit_RESET);
}
//不产生ACK应答		    
void IIC_NAck(void)
{
  IIC_SCL_Write(Bit_RESET);
  IIC_SDA_OUT();
  IIC_SDA_Write(Bit_SET);
  Delay_us(2);
  IIC_SCL_Write(Bit_SET);
  Delay_us(2);
  IIC_SCL_Write(Bit_RESET);
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
  u8 t;   
  IIC_SDA_OUT(); 	    
  IIC_SCL_Write(Bit_RESET);//拉低时钟开始数据传输
  for(t=0;t<8;t++)
  {              
    if(((txd&0x80)>>7))
      IIC_SDA_Write(Bit_SET);
    else
      IIC_SDA_Write(Bit_RESET);
    txd<<=1; 	  
    Delay_us(2);   //对TEA5767这三个延时都是必须的
    IIC_SCL_Write(Bit_SET);
    Delay_us(2); 
    IIC_SCL_Write(Bit_RESET);	
    Delay_us(2);
  }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
  unsigned char i,receive=0;
  IIC_SDA_IN();//SDA设置为输入
  for(i=0;i<8;i++ )
  {
    IIC_SCL_Write(Bit_RESET); 
    Delay_us(2);
    IIC_SCL_Write(Bit_SET);
    receive<<=1;
    if(IIC_SDA_Read())receive++;   
    Delay_us(1); 
  }					 
  if (!ack)
    IIC_NAck();//发送nACK
  else
    IIC_Ack(); //发送ACK   
  return receive;
}


/*------------------------      MPU9250         ------------------------*/
//**************************************
//向I2C设备写入一个字节数据
//**************************************
void IIC_Write_OneByte(u8 SlaveAddress,u8 REG_Address,u8 REG_data)
{
  IIC_Start();                   //起始信号
  IIC_Send_Byte(SlaveAddress);   //发送设备地址+写信号
  IIC_Wait_Ack();	   
  IIC_Send_Byte(REG_Address);    //内部寄存器地址
  IIC_Wait_Ack(); 	 										  		   
  IIC_Send_Byte(REG_data);       //内部寄存器数据
  IIC_Wait_Ack(); 	 										  		   
  IIC_Stop();                    //发送停止信号
}
//**************************************
//从I2C设备读取一个字节数据
//**************************************
u8 IIC_Read_OneByte(u8 SlaveAddress,u8 REG_Address)
{
  u8 REG_data;
  IIC_Start();                  //起始信号
  IIC_Send_Byte(SlaveAddress);  //发送设备地址+写信号
  REG_data=IIC_Wait_Ack();	   
  IIC_Send_Byte(REG_Address);   //发送存储单元地址，从0开始
  REG_data=IIC_Wait_Ack();	   
  IIC_Start();                  //起始信号
  IIC_Send_Byte(SlaveAddress+1);//发送设备地址+读信号
  REG_data=IIC_Wait_Ack();	   
  REG_data=IIC_Read_Byte(0);		//读取一个字节,不继续再读,发送NAK,读出寄存器数据
  IIC_Stop();	                  //停止信号
  return REG_data;
}


/*------------------------      JY_901         ------------------------*/
/**************************实现函数********************************************
*函数原型:		u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data)
*功　　能:	    读取指定设备 指定寄存器的 length个值
输入	dev  目标设备地址
reg	  寄存器地址
length 要读的字节数
*data  读出的数据将要存放的指针
返回   读出来的字节数量
*******************************************************************************/ 
u8 IIC_Read_Bytes(u8 dev, u8 reg, u8 length, u8 *data)
{
  u8 count = 0;
  
  IIC_Start();
  IIC_Send_Byte((dev<<1));	   //发送写命令
  IIC_Wait_Ack();
  IIC_Send_Byte(reg);               //发送地址
  IIC_Wait_Ack();	  
  IIC_Start();
  IIC_Send_Byte(((dev<<1)+1));       //进入接收模式	
  IIC_Wait_Ack();
  
  for(count=0;count<length;count++){
    
    if(count!=length-1)
      data[count]=IIC_Read_Byte(1);  //带ACK的读数据
    else  
      data[count]=IIC_Read_Byte(0);	 //最后一个字节NACK
  }
  IIC_Stop();//产生一个停止条件
  return count;
}

/**************************实现函数********************************************
*函数原型:		u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data)
*功　　能:	    将多个字节写入指定设备 指定寄存器
输入	dev  目标设备地址
		reg	  寄存器地址
		length 要写的字节数
		*data  将要写的数据的首地址
返回   返回是否成功
*******************************************************************************/ 
u8 IIC_Write_Bytes(u8 dev, u8 reg, u8 length, u8* data)
{
  
 	u8 count = 0;
	IIC_Start();
	IIC_Send_Byte(dev<<1);	   //发送写命令
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   //发送地址
	IIC_Wait_Ack();	  
	for(count=0;count<length;count++){
		IIC_Send_Byte(data[count]); 
		IIC_Wait_Ack(); 
 }
	IIC_Stop();//产生一个停止条件

    return 1; //status == 0;
	
}










