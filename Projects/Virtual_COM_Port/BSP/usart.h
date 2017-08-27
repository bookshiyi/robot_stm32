#ifndef __USART_H
#define __USART_H

#include "stm32f10x_conf.h"
#include "usb_type.h"//使用枚举的BOOL型

#define USART1_Baud		PC_Baud		
#define USART2_Baud		AHRS_Baud	
#define USART3_Baud		RMDS_Baud	
#define UART4_Baud		BT_Baud		
#define UART5_Baud		RESERVE_Baud

#define PC_Baud				9600
#define AHRS_Baud			115200
#define RMDS_Baud			115200
#define BT_Baud				38400
#define RESERVE_Baud		9600

#define PC_USART			USART1
#define AHRS_USART          USART2
#define RMDS_USART          USART3
#define BT_USART            UART4
#define RESERVE_USART       UART5

#define AHRS_DESCRIP		0X01
#define RMDS_DESCRIP		0X02
#define BT_DESCRIP			0X03
#define USB_DESCRIP			0X04
/*****************************
STM32F103C8T6
USART1_TX -- PA9  (PB6)
USART1_RX -- PA10 (PB7)

USART2_TX -- PA2
USART2_RX -- PA3

USART3_TX -- PB10
USART3_RX -- PB11
******************************/

/*****************************
STM32F103RBT6
USART1_TX -- PA9  (PB6)
USART1_RX -- PA10 (PB7)

USART2_TX -- PA2 
USART2_RX -- PA3 

USART3_TX -- PB10 (PC10)
USART3_RX -- PB11 (PC11)
******************************/



typedef struct //二级结构体
{
  bool  Flag;
  u8    Data[32];
  u8    Pointer;
  u8 	Descrip;		//描述
}Element_Data_TypDef;//元素定义
typedef struct
{
	Element_Data_TypDef USB;	//上位机
	Element_Data_TypDef BT;	//蓝牙
	Element_Data_TypDef RMDS;	//485
	Element_Data_TypDef AHRS;	
}USART_Data_TypDef;

extern USART_Data_TypDef USART_Data;//定义USART数据的结构体


void USART1_Config(void);
//void USART1_IRQHandler(void);
void USART2_Config(void);
void USART2_IRQHandler(void);
void USART3_Config(void);
void USART3_IRQHandler(void);
void UART4_Config(void);
void UART4_IRQHandler(void);
void UART5_Config(void);
void UART5_IRQHandler(void);
void NVIC_Configuration(void);

void USART_Data_Struct_Init();
void USART_Send(USART_TypeDef* USARTx,u16 data);//USART发送函数
void USART_Send_Assign(USART_TypeDef* USARTx,u8 *data,u8 Data_Len);
void USART_Send_Str(USART_TypeDef* USARTx,u8 *str);
void USART_NVIC_Configuration();
#endif
