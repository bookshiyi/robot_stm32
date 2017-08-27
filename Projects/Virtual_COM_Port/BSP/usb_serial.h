
#ifndef __USB_SERIAL_H
#define __USB_SERIAL_H

void USB_Serial_Send_Return_to_ROS(u8 obj,u8 param);
void USB_Serial_Send_Data_to_ROS(u8 obj,u8 cmd,u8 *param,u8 len);

void debug_jy_901_to_mini_imu();
void debug_serial_to_ROS();
#endif
