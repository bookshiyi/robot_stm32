#ifndef __SW_IIC_H
#define __SW_IIC_H


//接口定义,移植代码时仅需要调整此处
#define IIC_SCL_RCC             RCC_APB2Periph_GPIOC
#define IIC_SCL_Port            GPIOC
#define IIC_SCL_Pin             GPIO_Pin_0
#define IIC_SDA_RCC             RCC_APB2Periph_GPIOC
#define IIC_SDA_Port            GPIOC
#define IIC_SDA_Pin             GPIO_Pin_2

//写操作
#define IIC_SCL_Write(x)        GPIO_WriteBit(IIC_SCL_Port,IIC_SCL_Pin,x)
#define IIC_SDA_Write(x)        GPIO_WriteBit(IIC_SDA_Port,IIC_SDA_Pin,x)

//读操作
#define IIC_SDA_Read()          GPIO_ReadInputDataBit(IIC_SDA_Port,IIC_SDA_Pin)

//IO方向设置
#define IIC_SDA_IN()            GPIO_ModeSet(IIC_SDA_Port,IIC_SDA_Pin,GPIO_Mode_IN_FLOATING)                      //{GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
#define IIC_SDA_OUT()           GPIO_ModeSet(IIC_SDA_Port,IIC_SDA_Pin,GPIO_Mode_Out_PP)                   //{GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}

//IIC所有操作函数
void IIC_GPIO_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号
void IIC_Write_OneByte(u8 SlaveAddress,u8 REG_Address,u8 REG_data);
u8 IIC_Read_OneByte(u8 SlaveAddress,u8 REG_Address);
u8 IIC_Read_Bytes(u8 dev, u8 reg, u8 length, u8 *data);
u8 IIC_Write_Bytes(u8 dev, u8 reg, u8 length, u8* data);

void GPIO_ModeSet(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIOMode_TypeDef GPIO_Mode);    //输入输出模式改变

#endif
















