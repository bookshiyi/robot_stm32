/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： readme.txt
*	@Brief   	： 
*	@Author  	： admin@oarap.org
*	@Hardware	： STM32F103RCT6
*	@Date		： 2017-03
*	@Description    ： 系统的功能说明
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

/********************          串口部分          ********************/
USART1 和 USB-UART为上位机通信的端口
USART2 负责IMU	115200波特率		返回角度 经纬度 地速
USART3 负责485通信	115200波特率
UART4  负责蓝牙串口通信	38400波特率
UART5  冗余串口	
/********************          定时器          ********************/
TIM2负责超声波的通信计时
TIM4负责超声波传感器调度
TIM3的四个通道分别输出四路无刷电机的PWM波

Motion_Data	  接收到的来自上位机的目标数据
RMDS_Status	  电机运行参数部分
Odom_Data	  发送给上位机的反馈数据

ahrs motion odom中都有关于Vector3_Data_TypDef的结构体定义

/********************          通信协议          ********************/
0xAA最少是数组中的第[5]个
***格式：
0xA5 0x01 0x80 
...  		//参数	高位在前低位在后的模式
frame_len 0xAA
***蜂鸣器示例：
0xA5 0x01 0x80 
0x02 		//蜂鸣器响2ms
frame_len 0xAA
***运动【控制】示例：
0xA5 0x02 0x80 
{Motor_Ang_Vel[LEFT]1  Motor_Ang_Vel[LEFT]2} 		//左轮RPM，s16类型
{Motor_Ang_Vel[RIGHT]1  Motor_Ang_Vel[RIGHT]2}		//右轮RPM，s16类型
frame_len 0xAA
***运动数据【反馈】示例：
0xA5 0x02 0x80
{Motor_Ang_Vel_Average[LEFT]1   Motor_Ang_Vel_Average[LEFT]2} 	//左轮RPM，s16类型
{Motor_Ang_Vel_Average[RIGHT]1  Motor_Ang_Vel_Average[RIGHT]2}  //右轮RPM，s16类型
{Pos[LEFT]1   Pos[LEFT]2  Pos[LEFT]3  Pos[LEFT]4} 				//左轮编码器,s32类型
{Pos[RIGHT]1  Pos[RIGHT]2 Pos[RIGHT]3 Pos[RIGHT]4}              //右轮编码器,s32类型
frame_len 0xAA
***IMU数据反馈示例：
0xA5 0x03 0x80
{AHRS_Data.IMU.Acc.x1    AHRS_Data.IMU.Acc.x2}			//Acc.x，s16类型
{AHRS_Data.IMU.Acc.y1    AHRS_Data.IMU.Acc.y2}			//Acc.y，s16类型
{AHRS_Data.IMU.Acc.z1    AHRS_Data.IMU.Acc.z2}			//Acc.z，s16类型
{AHRS_Data.IMU.Gyro.x1   AHRS_Data.IMU.Gyro.x2}			//Gyro.x，s16类型
{AHRS_Data.IMU.Gyro.y1   AHRS_Data.IMU.Gyro.y2}			//Gyro.y，s16类型
{AHRS_Data.IMU.Gyro.z1   AHRS_Data.IMU.Gyro.z2}			//Gyro.z，s16类型
{AHRS_Data.IMU.Ang.x1   AHRS_Data.IMU.Ang.x2}			//Ang.x，s16类型
{AHRS_Data.IMU.Ang.y1   AHRS_Data.IMU.Ang.y2}			//Ang.y，s16类型
{AHRS_Data.IMU.Ang.z1   AHRS_Data.IMU.Ang.z2}			//Ang.z，s16类型
frame_len 0xAA
***电池数据反馈示例：
0xA5 0x05 0x80
{Bat_Vol1    Bat_Vol2}			//Bat_Vol，u16类型			//Ang.z，s16类型
frame_len 0xAA



ros中订阅cmd_vel话题，然后通过串口以int32类型Motor_Ang_Vel[LEFT] 和Motor_Ang_Vel[RIGHT]的形式（高位在前低位在后，传输过程中扩大1000倍数）发送至STM32

/********************          采样电阻          ********************/
采样电阻：9980R和910R
((x/4096)*3.3)/(910/(9980+910))=V	x*0.009641408

/********************          RMDS          ********************/
RMDS速度环，
P=0.01	D=0.15
0.03	0.2
0.08 	0.4
0.05	0.4












