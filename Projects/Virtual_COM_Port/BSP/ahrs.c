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
*	Rev1.1
*		Date：2017-04-15
*		Author：Blade
*		Modification：没有在结构体中包含温度，而每一帧数据都会传回来温度，使用的memcopy，所以导致地址溢出
*
*--------------------Copyright (C) oarap.org 2016------------------------------
*----------------------All rights reserved-------------------------------------
*
********************          一名工匠的遗憾与骄傲          ********************/
#include "Sys_Conf.h"

AHRS_Data_TypDef AHRS_Data;


float AccToAngle(float ax,float ay,float az,u8 dir)
{
  float temp;
  float res=0;
  switch (dir)
  {
  case 0:
    temp=ax/sqrt((ay*ay+az*az));
    res=atan(temp);
    break;
  case 1:
    temp=ay/sqrt((ax*ax+az*az));
    res=atan(temp);
    break;
  case 2:
    temp=sqrt((ax*ax+ay*ay))/az;
    res=atan(temp);
    break;
  }
  if(az>=0)
    return res*180/3.14;//弧度转换成角度
  else
  {
    res=res*180/3.14;
    if(res>0)
      return 180-res;
    else
      return -180-res;
    
  }
}
void ShortToChar(short sData,unsigned char cData[])
{
  cData[0]=sData&0xff;
  cData[1]=sData>>8;
}
short CharToShort(unsigned char cData[])
{
  return ((short)cData[1]<<8)|cData[0];
}


/*------------------------	MPU9250-IIC部分			--------------------------*/
#ifdef IIC
/*******************************************************************************
	MPU9250的IIC初始化
********************************************************************************/
void MPU9250_IIC_Init(void)
{
  IIC_Write_OneByte(MPU9250_I2C_ADDR,MPU9250_PWR_MGMT_1,0x00);			//唤醒mpu9250
  IIC_Write_OneByte(MPU9250_I2C_ADDR,MPU9250_CONFIG,0x06);    			//低通滤波5hz
  IIC_Write_OneByte(MPU9250_I2C_ADDR,MPU9250_GYRO_CONFIG,0x18);			//不自检，2000deg/s
  IIC_Write_OneByte(MPU9250_I2C_ADDR,MPU9250_ACCEL_CONFIG,0x00);			//(0x00 +-2g;)  ( 0x08 +-4g;)  (0x10 +-8g;)  (0x18 +-16g)
  IIC_Write_OneByte(MPU9250_I2C_ADDR,MPU9250_INT_PIN_CFG,0x02);
  IIC_Write_OneByte(MPU9250_I2C_ADDR,MPU9250_USER_CTRL,0x00);				//使能I2C
  
  IIC_Write_OneByte(AK8963_I2C_ADDR,AK8963_CNTL1,0x10);
  Delay_ms(50);
  IIC_Write_OneByte(AK8963_I2C_ADDR,AK8963_CNTL1,0x11);					//单测量模式+16位精度
}
/*******************************************************************************
	MPU9250通过IIC读取数据
********************************************************************************/
void MPU9250_IIC_Data_Read(void)
{
  s16 buf[20];
  u8 i;
  /*------------------------------------------------------------------------------------------------------------------------------------*/	
  //读取加速度、陀螺仪传感器
  IIC_Start();                  //起始信号
  IIC_Send_Byte(MPU9250_I2C_ADDR);  //发送设备地址+写信号
  IIC_Wait_Ack();	   
  IIC_Send_Byte(MPU9250_ACCEL_XOUT_H);   //发送存储单元地址，从0开始
  IIC_Wait_Ack();	   
  IIC_Start();                  //起始信号
  IIC_Send_Byte(MPU9250_I2C_ADDR+1);//发送设备地址+读信号
  IIC_Wait_Ack();
  for(i=0;i<14;i++)
  {
    if(i==13)buf[i]=IIC_Read_Byte(0);//读取一个字节,不继续再读,发送NACK  
    else buf[i]=IIC_Read_Byte(1);	//读取一个字节,继续读,发送ACK 
  }
  IIC_Stop();					//产生一个停止条件
  
  /*-----------------------------------------------------------------------------------------------------------------------------------*/	
  //读取地磁传感器
  IIC_Start();                  //起始信号
  IIC_Send_Byte(AK8963_I2C_ADDR);  //发送设备地址+写信号
  IIC_Wait_Ack();	   
  IIC_Send_Byte(AK8963_HXL);   //发送存储单元地址，从0开始
  IIC_Wait_Ack();	   
  IIC_Start();                  //起始信号
  IIC_Send_Byte(AK8963_I2C_ADDR+1);//发送设备地址+读信号
  IIC_Wait_Ack();
  for(i=14;i<20;i++)
  {
    if(i==19)buf[i]=IIC_Read_Byte(0);//读取一个字节,不继续再读,发送NACK  
    else buf[i]=IIC_Read_Byte(1);	//读取一个字节,继续读,发送ACK 
  }
  IIC_Stop();					//产生一个停止条件
  
  IIC_Write_OneByte(AK8963_I2C_ADDR,AK8963_CNTL1,0x11);//每读一次数据，ak8963会自动进入powerdown模式,这里需要重新设定为单测量模式
  /*-------------------------------------------------------------------------------------------------------------------------------------*/		
  //加速度
  AHRS_Data.IMU.Acc.x = ((buf[0]<<8)|buf[1]);
  AHRS_Data.IMU.Acc.y = ((buf[2]<<8)|buf[3]);
  AHRS_Data.IMU.Acc.z = ((buf[4]<<8)|buf[5]);                           
  //温度                          
  AHRS_Data.IMU.Temp  = ((buf[6]<<8)|buf[7]);
  //陀螺仪     
  AHRS_Data.IMU.Gyro.x= ((buf[8]<<8) |buf[9]);
  AHRS_Data.IMU.Gyro.y= ((buf[10]<<8)|buf[11]);
  AHRS_Data.IMU.Gyro.z= ((buf[12]<<8)|buf[13]);             
  //地磁传感器                      
  AHRS_Data.IMU.Mag.x = ((buf[15]<<8)|buf[14]);
  AHRS_Data.IMU.Mag.y = ((buf[17]<<8)|buf[16]);
  AHRS_Data.IMU.Mag.z = ((buf[19]<<8)|buf[18]);
}
/*******************************************************************************
	MPU9250数据发送到上位机 字符串形式
********************************************************************************/
void MPU9250_Data_to_PC()
{
  MPU9250_IIC_Data_Read();//采集数据大约1.5ms
  /*printf("acc: %d,%d,%d\ntemp: %d\ngyro: %d,%d,%d\nmag: %d,%d,%d\n\n",
         AHRS_Data.IMU_Data.Acc_x,AHRS_Data.IMU_Data.Acc_y,AHRS_Data.IMU_Data.Acc_z,
         AHRS_Data.IMU_Data.Temp,
         AHRS_Data.IMU_Data.Gyro_x,AHRS_Data.IMU_Data.Gyro_y,AHRS_Data.IMU_Data.Gyro_z,
         AHRS_Data.IMU_Data.Mag_x,AHRS_Data.IMU_Data.Mag_y,AHRS_Data.IMU_Data.Mag_z);*/
  
}

/*------------------------	JY-901-IIC部分			--------------------------*/
/*******************************************************************************
	JY-901的IIC初始化程序
********************************************************************************/
void JY_901_IIC_Init()
{
  
}
/*******************************************************************************
	JY-901的IIC数据读取
********************************************************************************/
void JY_901_IIC_Data_Read()
{
  unsigned char chrTemp[30];
  
  IIC_Read_Bytes(0x50, AX, 24,&chrTemp[0]);
  
  AHRS_Data.IMU.Acc.x  = (float)CharToShort(&chrTemp[0])/32768*16;
  AHRS_Data.IMU.Acc.y  = (float)CharToShort(&chrTemp[2])/32768*16;
  AHRS_Data.IMU.Acc.z  = (float)CharToShort(&chrTemp[4])/32768*16;
  AHRS_Data.IMU.Gyro.x = (float)CharToShort(&chrTemp[6])/32768*2000;
  AHRS_Data.IMU.Gyro.y = (float)CharToShort(&chrTemp[8])/32768*2000;
  AHRS_Data.IMU.Gyro.z = (float)CharToShort(&chrTemp[10])/32768*2000;
  AHRS_Data.IMU.Mag.x  = CharToShort(&chrTemp[12]);
  AHRS_Data.IMU.Mag.y  = CharToShort(&chrTemp[14]);
  AHRS_Data.IMU.Mag.z  = CharToShort(&chrTemp[16]);
  AHRS_Data.IMU.Ang.x  = (float)CharToShort(&chrTemp[18])/32768*180;
  AHRS_Data.IMU.Ang.y  = (float)CharToShort(&chrTemp[20])/32768*180;
  AHRS_Data.IMU.Ang.z  = (float)CharToShort(&chrTemp[22])/32768*180;
}
#endif

/*------------------------	JY-901-USART部分			--------------------------*/
#ifdef USART
/*******************************************************************************
	JY-901的USART数据接收处理程序
********************************************************************************/
void JY_901_USART_Data_RX_Handler(u16 usart_data)
{
  u8 sum=0;
  u8 i=0;
	USART_Data.AHRS.Data[USART_Data.AHRS.Pointer++]=usart_data;
	if (USART_Data.AHRS.Data[0]!=0x55) //êy?Yí·2???￡??ò??D??aê??°?ò0x55êy?Yí·
	{
		USART_Data.AHRS.Pointer=0;
		return;
	}
	if (USART_Data.AHRS.Pointer<11) {return;}//êy?Y2??ú11??￡??ò·μ??
	else
	{
	  for(i=0;i<10;i++)
	  {
	  	sum += USART_Data.AHRS.Data[i];
	  }
	  if(sum == USART_Data.AHRS.Data[10])
	  {//数据校验正确
		switch(USART_Data.AHRS.Data[1])
		{
			//case 0x50:	memcpy(&AHRS_Data.stcTime				,&RxBuffer[2],6);break;//memcpy?a±àò??÷×?′?μ??ú′???±′oˉêy￡?Dèòyó?"string.h"￡????óê??o3???μ?×?·???±′μ?êy?Y12í?ì?à???￡?′ó??êμ??êy?Yμ??a???￡
			case 0x51:	memcpy(&AHRS_Data.IMU.Acc.x			,&USART_Data.AHRS.Data[2],6);break;
			case 0x52:	memcpy(&AHRS_Data.IMU.Gyro.x		,&USART_Data.AHRS.Data[2],6);break;
			case 0x54:	memcpy(&AHRS_Data.IMU.Mag.x			,&USART_Data.AHRS.Data[2],6);break;
			case 0x53:	memcpy(&AHRS_Data.IMU.Ang.x			,&USART_Data.AHRS.Data[2],8);break;
			//case 0x55:	memcpy(&AHRS_Data.stcDStatus			,&RxBuffer[2],8);break;
			//case 0x56:	memcpy(&AHRS_Data.stcPress				,&RxBuffer[2],8);break;
			case 0x57:	memcpy(&AHRS_Data.GPS.Lon			,&USART_Data.AHRS.Data[2],8);break;
			case 0x58:	memcpy(&AHRS_Data.GPS.GpsHight		,&USART_Data.AHRS.Data[2],8);break;
			case 0x59:	memcpy(&AHRS_Data.IMU.Quat.q0		,&USART_Data.AHRS.Data[2],8);break;
		}
	  }
		USART_Data.AHRS.Pointer=0;
	}
}
/*******************************************************************************
	JY-901的USART初始化程序
********************************************************************************/
void JY_901_USART_Init()
{
  
}
#endif

/*------------------------			对外接口			--------------------------*/

/*******************************************************************************
	AHRS单元初始化
********************************************************************************/
void AHRS_Init()
{
#ifdef MPU9250
	IIC_GPIO_Init();
	MPU9250_IIC_Init();
#endif
	
#ifdef JY_901
	#ifdef IIC
		JY_901_IIC_Init();
	#endif
	#ifdef USART
		JY_901_USART_Init();
	#endif
#endif
}
/*******************************************************************************
	AHRS数据读取，仅在IIC模式需要主动读取
********************************************************************************/
void AHRS_Data_Read()
{		
#ifdef MPU9250
	MPU9250_IIC_Data_Read();
#endif
	
#ifdef JY_901
	JY_901_IIC_Data_Read();
#endif
}






