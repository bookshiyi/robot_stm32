
#ifndef __WIRELESS_H
#define __WIRELESS_H

//无线遥控
#define NUM_OF_WIRELESS_GPIO 2	//无线遥控的GPIO个数
typedef struct
{
  	uint32_t 		RCC_APB;
	GPIO_TypeDef*	PORT;
	uint16_t 		PIN;
	uint8_t 		GPIO_PortSourceGPIO;
	uint8_t 		GPIO_PinSource;
	uint32_t 		EXTI_Line;
	uint8_t 		EXTI_IRQn;
}Wireless_GPIO_TypeDef;
extern Wireless_GPIO_TypeDef Wireless_GPIO[NUM_OF_WIRELESS_GPIO];

void Wireless_GPIO_Struct_Init(void);
void Wireless_GPIO_Config();
void Wireless_EXTI_Init(void);	//无线遥控的EXTI初始化
#endif
