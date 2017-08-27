
#ifndef _GPIO_H_
#define _GPIO_H_

#include "stm32f10x.h"


//·äÃùÆ÷
#define BUZZ_GPIO_RCC           RCC_APB2Periph_GPIOB
#define BUZZ_GPIO_PORT          GPIOB
#define BUZZ_GPIO_PIN      		GPIO_Pin_9
#define BUZZ_ONOFF(x)     		GPIO_WriteBit(BUZZ_GPIO_PORT,BUZZ_GPIO_PIN,x);

//LED
#define LED1_GPIO_RCC           RCC_APB2Periph_GPIOC
#define LED1_GPIO_PORT          GPIOC
#define LED1_GPIO_PIN      		GPIO_Pin_13
#define LED1_ONOFF(x)     		GPIO_WriteBit(LED1_GPIO_PORT,LED1_GPIO_PIN,x);

void LED_GPIO_Config();
void LED_Flash_Handler();
void LED_Toggle();
void Buzz_GPIO_Config();
void Beep(u8 num,u16 time);
void Beep_Obj_Handler(u8 *param);
#endif
