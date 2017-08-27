#ifndef __MOTION_H
#define __MOTION_H
/*union Float_Data //union的作用为实现char数组和float之间的转换
{
    float f;
    u8 c[4];
	s32 i;
};*/	
#define AVERAGE_NUM	25

typedef struct
{
	bool Update_Flag;			//运动状态更新标志位
	bool Enable_Flag;			//运动使能标志位
	s16  Motor_Ang_Vel[2];		//转速控制的
    u16 PWM_Limit;
}Control_Data_TypDef;			//运动控制数据
typedef struct
{
	s16 Cur[2];
	s16 Motor_Ang_Vel[2][AVERAGE_NUM];
	s32 Pos[2];
	s16 Motor_Ang_Vel_Average[2];			//反馈的
	bool Feedback_Flag;
}Status_Data_TypDef;			//运动状态数据
typedef struct
{
	Control_Data_TypDef		Control;
	Status_Data_TypDef		Status;
}Motion_Data_Typdef;
extern Motion_Data_Typdef Motion_Data;
	

	
void Motion_Control_Data_Time_Out_Check_Handler();	//命令超时检测函数
void Motion_Control_Obj_Handler(u8 *param);
void Motion_Data_Init();
void Motion_Control_Data_Send();
void Motion_Status_Data_Read();
void Motor_Ang_Vel_Average_Calculate();
#endif

