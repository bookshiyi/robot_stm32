#include "Sys_Conf.h"
#include "string.h"

Servo_Data_Frame_TypeDef Servo_Data_Frame_Tx;
Servo_Data_Frame_TypeDef Servo_Data_Frame_Rx;
Servo_Arg_TypeDef        Servo_Arg;
//Frame_Header*2，ID*1，Lenth*1，Cmd*1,Addr*1，Data*N，Check_Sum*1
u16 DanceMove[8][NUM_OF_SERVO+1];
//结构体指针形式作为实参
void Servo_Data_Send(Servo_Data_Frame_TypeDef *data_frame)
{
    u8 i;
	u8 tx_buf[32];
	u16 check_sum_temp = 0;
	tx_buf[FRAME_HEADER_H] = SERVO_FRAME_HEAD0;
	tx_buf[FRAME_HEADER_L] = SERVO_FRAME_HEAD1;
	tx_buf[SERVO_ID] 	   = data_frame->ID;
	tx_buf[DATA_LENTH]     = data_frame->Len;
	tx_buf[SERVO_CMD] 	   = data_frame->Cmd;
	for(i=0;i<data_frame->Len-2;i++)//参数转存
		tx_buf[i+5] = data_frame->Data[i];
	for(i=2;i<data_frame->Len+3;i++)//计算校验和
    	check_sum_temp += tx_buf[i];
  	tx_buf[data_frame->Len+3] = ~check_sum_temp;//添加校验和
	USART_Send_Assign(USART2,tx_buf,tx_buf[DATA_LENTH]+4);
}
//写一个字，两个字节
void Servo_Write_Word(u8 id,u8 add,u16 data)
{
	u8 i;
	u8 tx_buf[32];
	u16 check_sum_temp = 0;
	tx_buf[FRAME_HEADER_H] = SERVO_FRAME_HEAD0;
	tx_buf[FRAME_HEADER_L] = SERVO_FRAME_HEAD1;
	tx_buf[SERVO_ID] 	   = id;//形参
	tx_buf[DATA_LENTH]     = 5;
	tx_buf[SERVO_CMD] 	   = CMD_WRITE;
	tx_buf[5]			   = add;//形参
	tx_buf[6]			   = data;//形参
	tx_buf[7]			   = data>>8;
	for(i=2;i<8;i++)//计算校验和
    	check_sum_temp += tx_buf[i];
  	tx_buf[8] = ~check_sum_temp;//添加校验和
	USART_Send_Assign(USART2,tx_buf,9);
}
//写一个字节
void Servo_Write_Byte(u8 id,u8 add,u8 data)
{
	u8 i;
	u8 tx_buf[32];
	u16 check_sum_temp = 0;
	tx_buf[FRAME_HEADER_H] = SERVO_FRAME_HEAD0;
	tx_buf[FRAME_HEADER_L] = SERVO_FRAME_HEAD1;
	tx_buf[SERVO_ID] 	   = id;//形参
	tx_buf[DATA_LENTH]     = 4;
	tx_buf[SERVO_CMD] 	   = CMD_WRITE;
	tx_buf[5]			   = add;//形参
	tx_buf[6]			   = data;//形参
	for(i=2;i<7;i++)//计算校验和
    	check_sum_temp += tx_buf[i];
  	tx_buf[7] = ~check_sum_temp;//添加校验和
	USART_Send_Assign(USART2,tx_buf,8);
}
//读取一个字节
void Servo_Read_Byte(u8 id,u8 add)
{
	u8 i;
	u8 tx_buf[32];
	u16 check_sum_temp = 0;
	tx_buf[FRAME_HEADER_H] = SERVO_FRAME_HEAD0;
	tx_buf[FRAME_HEADER_L] = SERVO_FRAME_HEAD1;
	tx_buf[SERVO_ID] 	   = id;//形参
	tx_buf[DATA_LENTH]     = 4;
	tx_buf[SERVO_CMD] 	   = CMD_READ;
	tx_buf[5]			   = add;//形参
	tx_buf[6]			   = 0x01;//一个字节长度
	for(i=2;i<7;i++)//计算校验和
    	check_sum_temp += tx_buf[i];
  	tx_buf[7] = ~check_sum_temp;//添加校验和
	USART_Send_Assign(USART2,tx_buf,8);
}
//读取一个字
void Servo_Read_Word(u8 id,u8 add)
{
	u8 i;
	u8 tx_buf[32];
	u16 check_sum_temp = 0;
	tx_buf[FRAME_HEADER_H] = SERVO_FRAME_HEAD0;
	tx_buf[FRAME_HEADER_L] = SERVO_FRAME_HEAD1;
	tx_buf[SERVO_ID] 	   = id;//形参
	tx_buf[DATA_LENTH]     = 4;
	tx_buf[SERVO_CMD] 	   = CMD_READ;
	tx_buf[5]			   = add;//形参
	tx_buf[6]			   = 0x02;//一个字节长度
	for(i=2;i<7;i++)//计算校验和
    	check_sum_temp += tx_buf[i];
  	tx_buf[7] = ~check_sum_temp;//添加校验和
	USART_Send_Assign(USART2,tx_buf,8);
}
#define MX 1
//伺服电机参数复位  
void Servo_Reset(u8 id)
{//在MX系列舵机上容易引起重置ID的问题
	u8 i;
	u8 tx_buf[32];
	u16 check_sum_temp = 0;
	tx_buf[FRAME_HEADER_H] = SERVO_FRAME_HEAD0;
	tx_buf[FRAME_HEADER_L] = SERVO_FRAME_HEAD1;
	tx_buf[SERVO_ID] 	   = id;//形参
	tx_buf[DATA_LENTH]     = 0X02;
	tx_buf[SERVO_CMD] 	   = CMD_RESET;
	for(i=2;i<5;i++)//计算校验和
    	check_sum_temp += tx_buf[i];
  	tx_buf[5] = ~check_sum_temp;//添加校验和
	USART_Send_Assign(USART2,tx_buf,6);
}
void Servo_Pos_Read(u16 *array)
{
  u8 i;
  //读取所有位置,初始位置使用
  for(i=1;i<10;i++)
  {
    Servo_Read_Word(i,P_PRESENT_POSITION_L);//读取当前位置
	Delay_us(20);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//使能串口中断接收
        Delay_ms(10);
	USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);//关闭串口中断接收
	
    if(USART_Data.Servo.Flag)//接收到数据
      array[i] = (USART_Data.Servo.Data[6]<<8) |USART_Data.Servo.Data[5];
    else
      array[i] = 2048;
    USART_Data.Servo.Flag = FALSE;
  }
}
//力矩限制使能开关
void Servo_Torque_EN(bool flag)
{	
  u8 i;
  if(flag)
  {
	for(i=1;i<NUM_OF_SERVO+1;i++)
        {
          Servo_Write_Byte(i,P_TORQUE_ENABLE,1);
          Delay_ms(30);
        }
  }
  else
  {
	for(i=1;i<NUM_OF_SERVO+1;i++)
        {
          Servo_Write_Byte(i,P_TORQUE_ENABLE,0);
          Delay_ms(30);
        }
  }  
}
void Servo_Struct_Init()
{
  u8 i;
  Servo_Arg.Action_Flag = FALSE;
  Servo_Arg.Init_Pos_Flag = FALSE;
  Servo_Arg.Step_Num = 25;//30步，3秒钟执行完
  //第一台FANFAN的参数（A版）
//  Servo_Arg.Init_Pos[1] = 2274;
//  Servo_Arg.Init_Pos[2] = 2086;
//  Servo_Arg.Init_Pos[3] = 1999;
//  Servo_Arg.Init_Pos[4] = 1792;
//  Servo_Arg.Init_Pos[5] = 2000;
//  Servo_Arg.Init_Pos[6] = 1833;
//  Servo_Arg.Init_Pos[7] = 1850;
//  Servo_Arg.Init_Pos[8] = 1539;
//  Servo_Arg.Init_Pos[9] = 1754;
  //第二台FANFAN的参数（B版）
//  Servo_Arg.Init_Pos[1] = 3400;//2832;
//  Servo_Arg.Init_Pos[2] = 2053;//2048;
//  Servo_Arg.Init_Pos[3] = 1973;//2048;
//  Servo_Arg.Init_Pos[4] = 2012;//2000;
//  Servo_Arg.Init_Pos[5] = 1981;//2000;
//  Servo_Arg.Init_Pos[6] = 2112;//2118;
//  Servo_Arg.Init_Pos[7] = 2048;
//  Servo_Arg.Init_Pos[8] = 2048;
//  Servo_Arg.Init_Pos[9] = 2048;
    //第三台FANFAN的参数（无2）
//  Servo_Arg.Init_Pos[1] = 1850;
//  Servo_Arg.Init_Pos[2] = 2292;
//  Servo_Arg.Init_Pos[3] = 2263;
//  Servo_Arg.Init_Pos[4] = 2289;
//  Servo_Arg.Init_Pos[5] = 2335;
//  Servo_Arg.Init_Pos[6] = 2059;
//  Servo_Arg.Init_Pos[7] = 1968;
//  Servo_Arg.Init_Pos[8] = 2106;
//  Servo_Arg.Init_Pos[9] = 2092;
  //B 直流电机 FANFAN的参数（直1）
  Servo_Arg.Init_Pos[1] = 2342;
  Servo_Arg.Init_Pos[2] = 1772;
  Servo_Arg.Init_Pos[3] = 2216;
  Servo_Arg.Init_Pos[4] = 1761;
  Servo_Arg.Init_Pos[5] = 1786;
  Servo_Arg.Init_Pos[6] = 2038;
  Servo_Arg.Init_Pos[7] = 2123;
  Servo_Arg.Init_Pos[8] = 1474;
  Servo_Arg.Init_Pos[9] = 1746;  
  for(i=1;i<NUM_OF_SERVO+1;i++)
  {
    Servo_Arg.Goal_Pos[i] = Servo_Arg.Init_Pos[i];//清所有动作
    Servo_Arg.Pre_Pos[i] = Servo_Arg.Init_Pos[i];
    Servo_Arg.Step_Pos[i] = Servo_Arg.Init_Pos[i];
  }
}
void Servo_Init()
{
//伺服电机参数初始化
  u8 i;
  for(i=1;i<NUM_OF_SERVO+1;i++)
  {
    
#ifdef AX
	//柔顺边距
	Servo_Write_Word(i,P_CW_COMPLIANCE_MARGIN,0X0F0F);
	//柔性斜线
	Servo_Write_Word(i,P_CW_COMPLIANCE_SLOPE,0XFFFF);
	//运动速度设置
	Servo_Write_Word(i,P_GOAL_SPEED_L,0X0101);
#endif
#ifdef MX
        //P GAIN
        Servo_Write_Byte(i,P_P_GAIN,0X40);
        Delay_ms(10);
         //I GAIN
        Servo_Write_Byte(i,P_I_GAIN,0X01);
         //D GAIN
        //Servo_Write_Byte(i,P_D_GAIN,0x00);
        //运动速度设置
        Delay_ms(10);
        Servo_Write_Word(i,P_GOAL_SPEED_L,0x0035);//0x0022);//0x004a);
        //Servo_Write_Word(i,P_GOAL_SPEED_L,0x0022);//0x004a);
        Delay_ms(10);
        //加速度设置/////////////
        Servo_Write_Byte(i,73,0x00);
#endif        
  }
  Servo_Torque_EN(FALSE);
  
  	for(i=1;i<NUM_OF_SERVO+1;i++)
        {
          Servo_Write_Word(i,P_MAX_TORQUE_L,0x03ff);
          Delay_ms(10);
        }
	//Servo_Reset(i);
}
///////////应该设计每次开机后自动识别零位，不用手动矫正
//mx系列经过验证可以使用扭矩开关没问题

void Action_Play(u8 group)
{
  u8 i;
  //TIM_Cmd(TIM5, DISABLE);
  for(i=1;i<NUM_OF_SERVO+1;i++)//重置目标位置
    Servo_Arg.Goal_Pos[i] = Servo_Arg.Init_Pos[i];
  switch(group)
  {//执行指定组的动作
  case 0x01:
    {//向左看介绍
      Servo_Arg.Step_Num = 20;//30;
      Servo_Arg.Goal_Pos[1] = Servo_Arg.Init_Pos[1]+300;
      
      Servo_Arg.Goal_Pos[2] = Servo_Arg.Init_Pos[2]+700;
      Servo_Arg.Goal_Pos[3] = Servo_Arg.Init_Pos[3]-700;
      
      Servo_Arg.Goal_Pos[4] = Servo_Arg.Init_Pos[4]-500;
      
      Servo_Arg.Goal_Pos[6] = Servo_Arg.Init_Pos[6]-400;
      Servo_Arg.Goal_Pos[7] = Servo_Arg.Init_Pos[7]+400;
      
      Servo_Arg.Goal_Pos[8] = Servo_Arg.Init_Pos[8]+900;
      Servo_Arg.Goal_Pos[9] = Servo_Arg.Init_Pos[9]-400;
    }break;
  case 0x02:
    {//向右看介绍
      Servo_Arg.Step_Num = 20;//30;
      Servo_Arg.Goal_Pos[1] = Servo_Arg.Init_Pos[1]-300;
      
      Servo_Arg.Goal_Pos[2] = Servo_Arg.Init_Pos[2]+700;
      Servo_Arg.Goal_Pos[3] = Servo_Arg.Init_Pos[3]-700;
      
      Servo_Arg.Goal_Pos[5] = Servo_Arg.Init_Pos[5]-500;
      
      Servo_Arg.Goal_Pos[6] = Servo_Arg.Init_Pos[6]-400;
      Servo_Arg.Goal_Pos[7] = Servo_Arg.Init_Pos[7]+400;
      
      Servo_Arg.Goal_Pos[8] = Servo_Arg.Init_Pos[8]+400;
      Servo_Arg.Goal_Pos[9] = Servo_Arg.Init_Pos[9]-900;
    }break;
  case 0x03:
    {//运动1
      Servo_Arg.Step_Num = 20;//25;
      Servo_Arg.Goal_Pos[2] = Servo_Arg.Init_Pos[2]+1000;
      Servo_Arg.Goal_Pos[3] = Servo_Arg.Init_Pos[3]-1200;
      Servo_Arg.Goal_Pos[6] = Servo_Arg.Init_Pos[6]+400;
      Servo_Arg.Goal_Pos[7] = Servo_Arg.Init_Pos[7]+400;
    }break;
  case 0x04:
    {//运动2
      Servo_Arg.Step_Num = 20;//25;
      Servo_Arg.Goal_Pos[2] = Servo_Arg.Init_Pos[2]+1200;
      Servo_Arg.Goal_Pos[3] = Servo_Arg.Init_Pos[3]-1000;
      Servo_Arg.Goal_Pos[6] = Servo_Arg.Init_Pos[6]-400;
      Servo_Arg.Goal_Pos[7] = Servo_Arg.Init_Pos[7]-400;
    }break;
  case 0x05:
    {//萌
      Servo_Arg.Step_Num = 15;//25;
      Servo_Arg.Goal_Pos[4] = Servo_Arg.Init_Pos[4]-320;
      Servo_Arg.Goal_Pos[5] = Servo_Arg.Init_Pos[5]-320;
    }break;  
  case 0x06:
    {//打招呼1
      Servo_Arg.Step_Num = 23;//30;
      Servo_Arg.Goal_Pos[1] = Servo_Arg.Init_Pos[1]-150;
      
      Servo_Arg.Goal_Pos[3] = Servo_Arg.Init_Pos[3]-1200;
      Servo_Arg.Goal_Pos[5] = Servo_Arg.Init_Pos[5]-200;
      Servo_Arg.Goal_Pos[7] = Servo_Arg.Init_Pos[7]+700;
      Servo_Arg.Goal_Pos[9] = Servo_Arg.Init_Pos[9]+900;
    }break;  
  case 0x07:
    {//打招呼2
      Servo_Arg.Step_Num = 23;//30;
      Servo_Arg.Goal_Pos[1] = Servo_Arg.Init_Pos[1]+150;
      
      Servo_Arg.Goal_Pos[2] = Servo_Arg.Init_Pos[2]+1200;
      Servo_Arg.Goal_Pos[4] = Servo_Arg.Init_Pos[4]-200;
      Servo_Arg.Goal_Pos[6] = Servo_Arg.Init_Pos[6]-700;
      Servo_Arg.Goal_Pos[8] = Servo_Arg.Init_Pos[8]-900;
    }break;  
  case 0x08:
    {//欢迎
      Servo_Arg.Step_Num = 22;//30;
      Servo_Arg.Goal_Pos[2] = Servo_Arg.Init_Pos[2]+600;
      Servo_Arg.Goal_Pos[3] = Servo_Arg.Init_Pos[3]-600;
      
      Servo_Arg.Goal_Pos[4] = Servo_Arg.Init_Pos[4]-400;
      Servo_Arg.Goal_Pos[5] = Servo_Arg.Init_Pos[5]-400;
      
      Servo_Arg.Goal_Pos[6] = Servo_Arg.Init_Pos[6]-500;
      Servo_Arg.Goal_Pos[7] = Servo_Arg.Init_Pos[7]+500;
      
      Servo_Arg.Goal_Pos[8] = Servo_Arg.Init_Pos[8]+600;
      Servo_Arg.Goal_Pos[9] = Servo_Arg.Init_Pos[9]-600;
    }break; 
  case 0x09:
    {//握手
      Servo_Arg.Step_Num = 15;//20;
      Servo_Arg.Goal_Pos[1] = Servo_Arg.Init_Pos[1]-150;
      
      Servo_Arg.Goal_Pos[3] = Servo_Arg.Init_Pos[3]-400;
      Servo_Arg.Goal_Pos[7] = Servo_Arg.Init_Pos[7]+400;
    }break;  
  case 0x0A:
    {//加油
      Servo_Arg.Step_Num = 20;//35;
      Servo_Arg.Goal_Pos[6] = Servo_Arg.Init_Pos[6]-800;
      Servo_Arg.Goal_Pos[7] = Servo_Arg.Init_Pos[7]+800;
      
      Servo_Arg.Goal_Pos[8] = Servo_Arg.Init_Pos[8]+900;
      Servo_Arg.Goal_Pos[9] = Servo_Arg.Init_Pos[9]-900;
      
      Servo_Arg.Goal_Pos[2] = Servo_Arg.Init_Pos[2]+800;
      Servo_Arg.Goal_Pos[3] = Servo_Arg.Init_Pos[3]-800;
      
      Servo_Arg.Goal_Pos[4] = Servo_Arg.Init_Pos[4]-200;
      Servo_Arg.Goal_Pos[5] = Servo_Arg.Init_Pos[5]-200;
      
    }break; 
  case 0x0B:
    {//看书
      Servo_Arg.Step_Num = 22;//35;
      Servo_Arg.Goal_Pos[1] = Servo_Arg.Init_Pos[1]+300;
      Servo_Arg.Goal_Pos[2] = Servo_Arg.Init_Pos[2]+1500;
      Servo_Arg.Goal_Pos[4] = Servo_Arg.Init_Pos[4]-400;
      Servo_Arg.Goal_Pos[6] = Servo_Arg.Init_Pos[6]-800;
      Servo_Arg.Goal_Pos[8] = Servo_Arg.Init_Pos[8]+900;
    }break; 
  case 0x0C:
    {//拥抱
      Servo_Arg.Step_Num = 22;//30;
      Servo_Arg.Goal_Pos[2] = Servo_Arg.Init_Pos[2]+500;
      Servo_Arg.Goal_Pos[3] = Servo_Arg.Init_Pos[3]-500;
      
      Servo_Arg.Goal_Pos[4] = Servo_Arg.Init_Pos[4]-200;
      Servo_Arg.Goal_Pos[5] = Servo_Arg.Init_Pos[5]-200;
      
      Servo_Arg.Goal_Pos[6] = Servo_Arg.Init_Pos[6]-600;
      Servo_Arg.Goal_Pos[7] = Servo_Arg.Init_Pos[7]+600;
      
      Servo_Arg.Goal_Pos[8] = Servo_Arg.Init_Pos[8]-200;
      Servo_Arg.Goal_Pos[9] = Servo_Arg.Init_Pos[9]+200;
    }break; 
  case 0x0D:
    {//右后，左前
      Servo_Arg.Step_Num = 15;//25;
      Servo_Arg.Goal_Pos[1] = Servo_Arg.Init_Pos[1]+150;
      
      Servo_Arg.Goal_Pos[6] = Servo_Arg.Init_Pos[6]-400;
      Servo_Arg.Goal_Pos[7] = Servo_Arg.Init_Pos[7]-400;
    }break;  
  case 0x0E:
    {//左后，右前
      Servo_Arg.Step_Num = 15;//25;
      Servo_Arg.Goal_Pos[1] = Servo_Arg.Init_Pos[1]-150;
      
      Servo_Arg.Goal_Pos[6] = Servo_Arg.Init_Pos[6]+400;
      Servo_Arg.Goal_Pos[7] = Servo_Arg.Init_Pos[7]+400;
    }break;  
  case 0x0F:
    {//可爱
      Servo_Arg.Step_Num = 18;//25;
      
      Servo_Arg.Goal_Pos[4] = Servo_Arg.Init_Pos[4]-200;
      Servo_Arg.Goal_Pos[5] = Servo_Arg.Init_Pos[5]-200;
      
      Servo_Arg.Goal_Pos[6] = Servo_Arg.Init_Pos[6]+400;
      Servo_Arg.Goal_Pos[7] = Servo_Arg.Init_Pos[7]-400;
      
      Servo_Arg.Goal_Pos[8] = Servo_Arg.Init_Pos[8]-200;
      Servo_Arg.Goal_Pos[9] = Servo_Arg.Init_Pos[9]+200;
    }break;  
    
  case 0x10:
    {//右看
      Servo_Arg.Step_Num = 15;//15;
      Servo_Arg.Goal_Pos[1] = Servo_Arg.Init_Pos[1]-250;
    }break;  
  case 0x11:
    {//左看
      Servo_Arg.Step_Num = 15;//15;
      Servo_Arg.Goal_Pos[1] = Servo_Arg.Init_Pos[1]+250;
    }break;  
  case 0x12:
    {//伸左手
      Servo_Arg.Step_Num = 20;//25;
      Servo_Arg.Goal_Pos[1] = Servo_Arg.Init_Pos[1]+200;
      
      Servo_Arg.Goal_Pos[2] = Servo_Arg.Init_Pos[2]+600;
      Servo_Arg.Goal_Pos[6] = Servo_Arg.Init_Pos[6]-600;
      Servo_Arg.Goal_Pos[8] = Servo_Arg.Init_Pos[8]+900;
    }break; 
  case 0x13:
    {//伸右手
      Servo_Arg.Step_Num = 20;//25;
      Servo_Arg.Goal_Pos[1] = Servo_Arg.Init_Pos[1]-200;
      
      Servo_Arg.Goal_Pos[3] = Servo_Arg.Init_Pos[3]-600;
      Servo_Arg.Goal_Pos[7] = Servo_Arg.Init_Pos[7]+600;
      Servo_Arg.Goal_Pos[9] = Servo_Arg.Init_Pos[9]-900;
    }break; 
    
  case 0x14:
    {//嫌弃1
      Servo_Arg.Step_Num = 15;//20;
      Servo_Arg.Goal_Pos[1] = Servo_Arg.Init_Pos[1]+250;
      
      Servo_Arg.Goal_Pos[5] = Servo_Arg.Init_Pos[5]-200;
      Servo_Arg.Goal_Pos[7] = Servo_Arg.Init_Pos[7]-400;
      Servo_Arg.Goal_Pos[9] = Servo_Arg.Init_Pos[9]+300;
    }break; 
  case 0x15:
    {//嫌弃2
      Servo_Arg.Step_Num = 15;//20;
      Servo_Arg.Goal_Pos[1] = Servo_Arg.Init_Pos[1]-250;
      
      Servo_Arg.Goal_Pos[4] = Servo_Arg.Init_Pos[4]-200;
      Servo_Arg.Goal_Pos[6] = Servo_Arg.Init_Pos[6]+400;
      Servo_Arg.Goal_Pos[8] = Servo_Arg.Init_Pos[8]-500;
    }break; 
   case 0x16:
    {//欢迎1
      Servo_Arg.Step_Num = 20;//20;
      Servo_Arg.Goal_Pos[1] = Servo_Arg.Init_Pos[1]+250; 
      
      Servo_Arg.Goal_Pos[2] = Servo_Arg.Init_Pos[2]+500;
      Servo_Arg.Goal_Pos[4] = Servo_Arg.Init_Pos[4]-300;
      Servo_Arg.Goal_Pos[6] = Servo_Arg.Init_Pos[6]-300;
      Servo_Arg.Goal_Pos[8] = Servo_Arg.Init_Pos[8]+500;
    }break; 
       case 0x17:
    {//欢迎2
      Servo_Arg.Step_Num = 20;//20;
      Servo_Arg.Goal_Pos[1] = Servo_Arg.Init_Pos[1]-250;   
      
      Servo_Arg.Goal_Pos[3] = Servo_Arg.Init_Pos[3]-500;
      Servo_Arg.Goal_Pos[5] = Servo_Arg.Init_Pos[5]-300;
      Servo_Arg.Goal_Pos[7] = Servo_Arg.Init_Pos[7]+300;
      Servo_Arg.Goal_Pos[9] = Servo_Arg.Init_Pos[9]-500;
    }break; 
  case 0x23:
    {
//        Servo_Arg.Step_Num = 25;//25;
//        Servo_Arg.Goal_Pos[1] = Servo_Arg.Init_Pos[1]+500;//头
//        Servo_Arg.Step_Num = 20;//30;
//        Servo_Arg.Goal_Pos[2] = Servo_Arg.Init_Pos[2]+1000;//肘
          Servo_Arg.Step_Num = 20;//30;
          Servo_Arg.Goal_Pos[6] = Servo_Arg.Init_Pos[6]-1000;//肘
    }break;
  default ://回到初始位置
    {  
      Servo_Arg.Step_Num = 20;//20;
    }break;
    
  }
  if(!group)
  {
      Servo_Arg.Init_Pos_Flag = TRUE;
      Step_Counter = 1;
  }
  else
  {
      Servo_Arg.Action_Flag = TRUE;  
  }      
  //TIM_Cmd(TIM5,ENABLE);
}

void Dance_Move(u8 comboNum, u8 group)
{
  u8 i,j;
  //TIM_Cmd(TIM5, DISABLE);
  for (i=1; i<=comboNum;i++)
  {
    for(j=1;j<NUM_OF_SERVO+1;j++)//重置目标位置
    {
      DanceMove[i-1][j] = Servo_Arg.Init_Pos[j];
    }
  }
  switch(group)
  {//执行指定组的动作
       case 0x18:
    {//向左跳舞
      Servo_Arg.Step_Num = 20;//20;
      Servo_Arg.Goal_Pos[1] = Servo_Arg.Init_Pos[1]+300;
      
      Servo_Arg.Goal_Pos[4] = Servo_Arg.Init_Pos[4]-400;
      Servo_Arg.Goal_Pos[5] = Servo_Arg.Init_Pos[5]-400;
    }break; 
       case 0x19:
    {//向右跳舞
      Servo_Arg.Step_Num = 20;//20;
      Servo_Arg.Goal_Pos[1] = Servo_Arg.Init_Pos[1]-300;
      
      Servo_Arg.Goal_Pos[4] = Servo_Arg.Init_Pos[4]-400;
      Servo_Arg.Goal_Pos[5] = Servo_Arg.Init_Pos[5]-400;
    }break; 
       case 0x1A:
    {//向左跳舞
      Servo_Arg.Step_Num = 20;//20;
      Servo_Arg.Goal_Pos[1] = Servo_Arg.Init_Pos[1]+300;
      
      Servo_Arg.Goal_Pos[6] = Servo_Arg.Init_Pos[6]-400;//左臂向前
      Servo_Arg.Goal_Pos[7] = Servo_Arg.Init_Pos[7]-400;//右臂向后
    }break; 
       case 0x1B:
    {//向右跳舞
      Servo_Arg.Step_Num = 20;//20;
      Servo_Arg.Goal_Pos[1] = Servo_Arg.Init_Pos[1]-300;
      
      Servo_Arg.Goal_Pos[6] = Servo_Arg.Init_Pos[6]+400;//左臂向后
      Servo_Arg.Goal_Pos[7] = Servo_Arg.Init_Pos[7]+400;//右臂向前
    }break; 
       case 0x1C:
    {//舞蹈动作分解1
      Servo_Arg.Step_Num = 25;//20;
      Servo_Arg.Goal_Pos[2] = Servo_Arg.Init_Pos[2]+900;//肘
      Servo_Arg.Goal_Pos[3] = Servo_Arg.Init_Pos[3]-900;//肘
      Servo_Arg.Goal_Pos[8] = Servo_Arg.Init_Pos[8]-800;//腕
      Servo_Arg.Goal_Pos[9] = Servo_Arg.Init_Pos[9]+800;//腕
      Servo_Arg.Goal_Pos[6] = Servo_Arg.Init_Pos[6]-300;//前后摆臂
      Servo_Arg.Goal_Pos[7] = Servo_Arg.Init_Pos[7]+300;//前后摆臂
    }break; 
       case 0x1D:
    {//舞蹈动作分解2
      Servo_Arg.Step_Num = 25;//20;
      DanceMove[1][2] = Servo_Arg.Init_Pos[2]+150;//肘
      DanceMove[1][3] = Servo_Arg.Init_Pos[3]-150;//肘
      DanceMove[1][4] = Servo_Arg.Init_Pos[4]-800;//肘
      DanceMove[1][5] = Servo_Arg.Init_Pos[5]-800;//肘
      DanceMove[1][8] = Servo_Arg.Init_Pos[8]-200;//前后摆臂
      DanceMove[1][9] = Servo_Arg.Init_Pos[9]+200;//前后摆臂
      //DanceMove[0][1] = Servo_Arg.Init_Pos[1]+250;//肘
      DanceMove[0][2] = Servo_Arg.Init_Pos[2]+1000;//肘
      DanceMove[0][6] = Servo_Arg.Init_Pos[6]-600;//前后摆臂
      DanceMove[0][3] = Servo_Arg.Init_Pos[3]-1000;//肘
      DanceMove[0][7] = Servo_Arg.Init_Pos[7]+600;//前后摆臂
      DanceMove[0][8] = Servo_Arg.Init_Pos[8]-600;//前后摆臂
      DanceMove[0][9] = Servo_Arg.Init_Pos[9]+600;//前后摆臂
    }break; 
       case 0x1E:
    {//舞蹈动作分解3
      Servo_Arg.Step_Num = 28;
//      Servo_Arg.Dance_Step_Num[1] = 30;
//      Servo_Arg.Dance_Step_Num[2] = 15;
//      Servo_Arg.Dance_Step_Num[3] = 30;
      DanceMove[0][2] = Servo_Arg.Init_Pos[2]+300;//肘
      DanceMove[0][3] = Servo_Arg.Init_Pos[3]-300;//肘
      DanceMove[0][5] = Servo_Arg.Init_Pos[5]-100;//肘
      DanceMove[0][4] = Servo_Arg.Init_Pos[4]-100;//肘
      
      DanceMove[3][1] = Servo_Arg.Init_Pos[1]+250;//前后摆臂
      DanceMove[3][3] = Servo_Arg.Init_Pos[3]-281;//前后摆臂
      DanceMove[3][5] = Servo_Arg.Init_Pos[5]-373;//肘
      DanceMove[3][7] = Servo_Arg.Init_Pos[7]-498;//肘
      DanceMove[3][9] = Servo_Arg.Init_Pos[9]+504;//肘
      DanceMove[3][2] = Servo_Arg.Init_Pos[2]+883;//肘
      DanceMove[3][4] = Servo_Arg.Init_Pos[4]-526;//肘
      DanceMove[3][6] = Servo_Arg.Init_Pos[6]-1047;//肘
      DanceMove[3][8] = Servo_Arg.Init_Pos[8]+50;//肘、
      
      DanceMove[2][2] = Servo_Arg.Init_Pos[2]+300;//肘
      DanceMove[2][3] = Servo_Arg.Init_Pos[3]-300;//肘
      DanceMove[2][5] = Servo_Arg.Init_Pos[5]-100;//肘
      DanceMove[2][4] = Servo_Arg.Init_Pos[4]-100;//肘
      
      DanceMove[1][1] = Servo_Arg.Init_Pos[1]-250;//前后摆臂
      DanceMove[1][3] = Servo_Arg.Init_Pos[3]-883;//肘
      DanceMove[1][5] = Servo_Arg.Init_Pos[5]-526;//肘
      DanceMove[1][7] = Servo_Arg.Init_Pos[7]+1047;//肘
      DanceMove[1][9] = Servo_Arg.Init_Pos[9]-50;//肘、
      DanceMove[1][2] = Servo_Arg.Init_Pos[2]+281;//前后摆臂
      DanceMove[1][4] = Servo_Arg.Init_Pos[4]-373;//肘
      DanceMove[1][6] = Servo_Arg.Init_Pos[6]+498;//肘
      DanceMove[1][8] = Servo_Arg.Init_Pos[8]-504;//肘
    }break; 
       case 0x20:
    {//舞蹈动作分解1
      Servo_Arg.Step_Num = 15;//20;

      DanceMove[1][1] = Servo_Arg.Init_Pos[1]-250;//肘
      DanceMove[1][3] = Servo_Arg.Init_Pos[3]-400;//肘
      DanceMove[1][4] = Servo_Arg.Init_Pos[4]-400;//肘
      DanceMove[1][7] = Servo_Arg.Init_Pos[7]+400;//前后摆臂
      
      DanceMove[0][1] = Servo_Arg.Init_Pos[1]+250;//肘
      DanceMove[0][2] = Servo_Arg.Init_Pos[2]+400;//肘
      DanceMove[0][5] = Servo_Arg.Init_Pos[5]-400;//肘
      DanceMove[0][6] = Servo_Arg.Init_Pos[6]-400;//前后摆臂
    }break; 
       case 0x21:
    {//舞蹈动作画圈
      Servo_Arg.Step_Num = 20;
//      Servo_Arg.Dance_Step_Num[1] = 25;
//      Servo_Arg.Dance_Step_Num[2] = 15;
//      Servo_Arg.Dance_Step_Num[3] = 25;

      DanceMove[0][5] = Servo_Arg.Init_Pos[5]-300;//肘
      DanceMove[0][4] = Servo_Arg.Init_Pos[4]-300;//肘
      DanceMove[0][3] = Servo_Arg.Init_Pos[3]-150;//肘
      DanceMove[0][2] = Servo_Arg.Init_Pos[2]+150;//肘
      
      DanceMove[3][6] = Servo_Arg.Init_Pos[6]+400;//前后摆臂
      DanceMove[3][7] = Servo_Arg.Init_Pos[7]+400;//前后摆臂
      DanceMove[3][1] = Servo_Arg.Init_Pos[1]-250;//肘
      DanceMove[3][3] = Servo_Arg.Init_Pos[3]-400;//肘
      DanceMove[3][2] = Servo_Arg.Init_Pos[2]+400;//肘
      DanceMove[3][8] = Servo_Arg.Init_Pos[8]-400;//肘
      DanceMove[3][9] = Servo_Arg.Init_Pos[9]-400;//肘
      
      DanceMove[2][5] = Servo_Arg.Init_Pos[5]-300;//肘
      DanceMove[2][4] = Servo_Arg.Init_Pos[4]-300;//肘
      DanceMove[2][3] = Servo_Arg.Init_Pos[3]-150;//肘
      DanceMove[2][2] = Servo_Arg.Init_Pos[2]+150;//肘
      
      DanceMove[1][8] = Servo_Arg.Init_Pos[8]+400;//前后摆臂
      DanceMove[1][9] = Servo_Arg.Init_Pos[9]+400;//前后摆臂
      DanceMove[1][6] = Servo_Arg.Init_Pos[6]-400;//前后摆臂
      DanceMove[1][7] = Servo_Arg.Init_Pos[7]-400;//前后摆臂
      DanceMove[1][1] = Servo_Arg.Init_Pos[1]+250;//肘
      DanceMove[1][3] = Servo_Arg.Init_Pos[3]-400;//肘
      DanceMove[1][2] = Servo_Arg.Init_Pos[2]+400;//肘

    }break; 
       case 0x22://测试
    {//舞蹈动作分解3
      Servo_Arg.Step_Num = 20;//20;
      DanceMove[0][2] = Servo_Arg.Init_Pos[2]+1100;//肘
      DanceMove[0][4] = Servo_Arg.Init_Pos[4]-400;//肘
      DanceMove[0][5] = Servo_Arg.Init_Pos[5]-400;//肘
      DanceMove[0][3] = Servo_Arg.Init_Pos[3]-1100;//肘
      DanceMove[0][6] = Servo_Arg.Init_Pos[6]-600;//前后摆臂
      DanceMove[0][7] = Servo_Arg.Init_Pos[7]+600;//前后摆臂

    }break; 
  case 0x23:
    {
//        Servo_Arg.Step_Num = 25;//25;
//        Servo_Arg.Goal_Pos[1] = Servo_Arg.Init_Pos[1]+500;//头
//        Servo_Arg.Step_Num = 20;//30;
//        Servo_Arg.Goal_Pos[2] = Servo_Arg.Init_Pos[2]+1000;//肘
          Servo_Arg.Step_Num = 20;//30;
          Servo_Arg.Goal_Pos[6] = Servo_Arg.Init_Pos[6]-1000;//肘
    }break;
  default ://回到初始位置
    {  
      Servo_Arg.Step_Num = 20;//20;
    }break;
    
  }
  Servo_Arg.Dance_Flag = TRUE;        
}

//void TIM5_IRQHandler(void)   //TIM5中断
//{	
//  u8 static Step_Counter = 1;
//  u8 i;
//  if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
//  {	
//    if(Servo_Arg.Action_Flag)
//    {//执行动作
//      for(i=1;i<NUM_OF_SERVO+1;i++)
//      {//计算每个舵机下一步需要到达的位置
//        if(Servo_Arg.Goal_Pos[i] > Servo_Arg.Pre_Pos[i])
//          Servo_Arg.Step_Pos[i] = Servo_Arg.Pre_Pos[i] + ((Servo_Arg.Goal_Pos[i] - Servo_Arg.Pre_Pos[i]) * Step_Counter / Servo_Arg.Step_Num);
//        else
//          Servo_Arg.Step_Pos[i] = Servo_Arg.Pre_Pos[i] - ((Servo_Arg.Pre_Pos[i] - Servo_Arg.Goal_Pos[i]) * Step_Counter / Servo_Arg.Step_Num);   
//      }
//      for(i=1;i<NUM_OF_SERVO+1;i++)
//      {
//        Servo_Write_Word(i,P_GOAL_POSITION_L,Servo_Arg.Step_Pos[i]);
//        Delay_ms(6);
//      }    
//      Step_Counter++;
//      if(Step_Counter >= Servo_Arg.Step_Num)
//      {//说明完成了一次动作
//        for(i=1;i<NUM_OF_SERVO+1;i++)
//          Servo_Arg.Pre_Pos[i] = Servo_Arg.Goal_Pos[i];//更新当前位置
//        Step_Counter = 1;
//        Servo_Arg.Action_Flag = FALSE;
//        Servo_Arg.Init_Pos_Flag = TRUE;//准备返回
//      }
//      else
//      {
//      }
//    }
//    else if(Servo_Arg.Init_Pos_Flag)
//    {//返回初始位置
//      for(i=1;i<NUM_OF_SERVO+1;i++)
//      {//计算每个舵机下一步需要到达的位置
//        if(Servo_Arg.Init_Pos[i] > Servo_Arg.Pre_Pos[i])
//          Servo_Arg.Step_Pos[i] = Servo_Arg.Pre_Pos[i] + ((Servo_Arg.Init_Pos[i] - Servo_Arg.Pre_Pos[i]) * Step_Counter / Servo_Arg.Step_Num);
//        else
//          Servo_Arg.Step_Pos[i] = Servo_Arg.Pre_Pos[i] - ((Servo_Arg.Pre_Pos[i] - Servo_Arg.Init_Pos[i]) * Step_Counter / Servo_Arg.Step_Num);   
//
//      }
//      for(i=1;i<NUM_OF_SERVO+1;i++)
//      {
//        Servo_Write_Word(i,P_GOAL_POSITION_L,Servo_Arg.Step_Pos[i]);
//        Delay_ms(6);
//      }    
//      Step_Counter++;
//      if(Step_Counter >= Servo_Arg.Step_Num)
//      {//说明回到了初始位置
//        Step_Counter = 1;
//        Servo_Arg.Init_Pos_Flag = FALSE;//返回完成
//        Servo_Torque_EN(FALSE);//关闭扭矩输出
//        for(i=1;i<NUM_OF_SERVO+1;i++)
//          Servo_Arg.Pre_Pos[i] = Servo_Arg.Init_Pos[i];//更新当前位置
//         USB_Send_Return_to_PC(ACTION_OBJ,OK_ARG);//返回信息
//         
//         //返回初始状态表情
//         Eye_Action(0);
//      }
//    }
//    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 	
//  }
//  TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
//}
