#ifndef __Ultrasonic_H
#define	__Ultrasonic_H

//2016-09-18 
//Blade
//修改GPIO的define，使用超声波模块仅需修改此处的管脚
//但需要注意更改RCC_APB2


#define WINDOW_UPPER_VALUE 45		    //滞回窗口的上限
#define WINDOW_FLOOR_VALUE 23 		    //滞回窗口的下限
//#define THRESHOLD_VALUE		60



#define NUM_OF_ULTRASONIC 8	//超声波传感器个数
typedef struct 
{
	GPIO_TypeDef* 	TRIG_PORT;
	uint16_t 		TRIG_PIN;
	GPIO_TypeDef* 	ECHO_PORT;
	uint16_t 		ECHO_PIN;
	uint8_t 		ECHO_GPIO_PortSourceGPIO;
	uint8_t 		ECHO_GPIO_PinSource;
	uint32_t 		ECHO_EXTI_Line;
	uint8_t 		ECHO_EXTI_IRQn;
}Ultrasonic_GPIO_TypeDef;

extern Ultrasonic_GPIO_TypeDef Ultrasonic_GPIO[NUM_OF_ULTRASONIC];

void Ultrasonic_GPIO_Struct_Init();
void Ultrasonic_GPIO_Configuration(void);               //对超声波模块初始化
void Obstacle_Scan();//障碍物检测



#endif /* __UltrasonicWave_H */

