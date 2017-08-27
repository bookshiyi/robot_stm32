
#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

void Bluetooth_Init();
void Bluetooth_Send_Return_to_ROS(u8 obj,u8 param);
void Bluetooth_Send_Data_to_ROS(u8 obj,u8 cmd,u8 *param,u8 len);
void Bluetooth_USART_Data_RX_Handler(u16 usart_data);
void Bluetooth_Data_Process();

#endif
