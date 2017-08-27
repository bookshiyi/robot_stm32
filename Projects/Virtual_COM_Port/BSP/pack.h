
#ifndef __PACK_H
#define __PACK_H
typedef struct 
{
	u8 Head;		        //帧头
	u8 Obj;				//对象
	u8 Cmd;				//指令
	u8 Arg[28];			//参数
	u8 End;		                //帧尾
	
	u8 Arg_Lenth;		        //参数长度
}USB_Data_Frame_TypDef;

extern USB_Data_Frame_TypDef USB_Data_Frame_Tx;//发送用
extern USB_Data_Frame_TypDef USB_Data_Frame_Rx;//接受用，在USART中断中存入USART_DATA中，在后台处理过程中用结构体分解USART数据

void USB_Send_Data_Frame(USB_Data_Frame_TypDef *data_frame);
bool USB_Rec_Data_Frame(USB_Data_Frame_TypDef *data_frame);
void USB_Send_Return_to_PC(u8 obj,u8 arg);
void USB_Send_Data_to_PC(u8 obj,u8 cmd,u8 *arg,u8 arg_len);

extern void USB_SendBuf(uint8_t *_pTxBuf, uint8_t _ucLen);
extern void USB_Send_Str(uint8_t *_pTxBuf);
#endif
