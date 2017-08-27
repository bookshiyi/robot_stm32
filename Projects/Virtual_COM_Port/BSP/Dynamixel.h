#ifndef _DYNAMIXEL_H_
#define _Dynamixel_H_

//定义舵机结构体的数据帧头的内容
#define SERVO_FRAME_HEAD0	0xFF
#define SERVO_FRAME_HEAD1	0xFF

//指令包（由单片机发送）【发送缓存中使用】
#define FRAME_HEADER_H  0       //帧头高
#define FRAME_HEADER_L  1       
#define SERVO_ID        2       //舵机ID
#define DATA_LENTH      3       //数据长度(不包括帧头,ID,和自身所占，所以数据长度加4为帧长度）
#define SERVO_CMD       4       //***发给舵机的指令
//指令包（由单片机发送）【结构体中使用】
#define EEPROM_ADD      0      //地址
#define PARA_1       	1       //参数
#define PARA_2       	2       //参数
#define PARA_3       	3       //参数
#define PARA_4       	4       //参数
#define PARA_5       	5        //参数
#define PARA_6       	6        //参数
#define PARA_7       	7        //参数
#define PARA_8       	8        //参数
#define PARA_9       	9        //参数
#define PARA_10       	10       //参数
#define PARA_11       	11       //参数
#define PARA_12       	12       //参数
#define PARA_13       	13       //参数
#define PARA_14       	14       //参数
#define PARA_15       	15       //参数
#define PARA_16       	16       //参数
#define PARA_17       	17       //参数
#define PARA_18       	18       //参数
#define PARA_19       	19       //参数
#define PARA_20       	20       //参数


//定义反馈包（由舵机发送）的数据帧的格式
#define Frame_Header_H  0       //帧头高
#define Frame_Header_L  1       
#define Servo_ID        2       //舵机ID
#define Data_Lenth      3       //数据长度(不包括帧头）
#define Servo_Error     4       //***错误状态字节
//#define Parameter       5       //附加信息


//EEPROM区控制表的地址定义
#define P_MODEL_NUMBER_L 0
#define P_MODOEL_NUMBER_H 1
#define P_VERSION 2
#define P_ID 3
#define P_BAUD_RATE 4
#define P_RETURN_DELAY_TIME 5
#define P_CW_ANGLE_LIMIT_L 6
#define P_CW_ANGLE_LIMIT_H 7
#define P_CCW_ANGLE_LIMIT_L 8
#define P_CCW_ANGLE_LIMIT_H 9
#define P_SYSTEM_DATA2 10
#define P_LIMIT_TEMPERATURE 11
#define P_DOWN_LIMIT_VOLTAGE 12
#define P_UP_LIMIT_VOLTAGE 13
#define P_MAX_TORQUE_L 14
#define P_MAX_TORQUE_H 15
#define P_RETURN_LEVEL 16
#define P_ALARM_LED 17
#define P_ALARM_SHUTDOWN 18
#define P_OPERATING_MODE 19
#define P_DOWN_CALIBRATION_L 20
#define P_DOWN_CALIBRATION_H 21
#define P_UP_CALIBRATION_L 22
#define P_UP_CALIBRATION_H 23
//RAM区控制表的地址定义
#define P_TORQUE_ENABLE (24)
#define P_LED (25)

//AX
#define P_CW_COMPLIANCE_MARGIN (26)
#define P_CCW_COMPLIANCE_MARGIN (27)
#define P_CW_COMPLIANCE_SLOPE (28)
#define P_CCW_COMPLIANCE_SLOPE (29)
//MX
#define P_D_GAIN (26)
#define P_I_GAIN (27)
#define P_P_GAIN (28)

#define P_GOAL_POSITION_L (30)
#define P_GOAL_POSITION_H (31)
#define P_GOAL_SPEED_L (32)
#define P_GOAL_SPEED_H (33)
#define P_TORQUE_LIMIT_L (34)
#define P_TORQUE_LIMIT_H (35)
#define P_PRESENT_POSITION_L (36)
#define P_PRESENT_POSITION_H (37)
#define P_PRESENT_SPEED_L (38)
#define P_PRESENT_SPEED_H (39)
#define P_PRESENT_LOAD_L (40)
#define P_PRESENT_LOAD_H (41)
#define P_PRESENT_VOLTAGE (42)
#define P_PRESENT_TEMPERATURE (43)
#define P_REGISTERED_INSTRUCTION (44)
#define P_PAUSE_TIME (45)
#define P_MOVING (46)
#define P_LOCK (47)
#define P_PUNCH_L (48)
#define P_PUNCH_H (49)



//--- CMD ---
#define CMD_PING 0x01
#define CMD_READ 0x02
#define CMD_WRITE 0x03
#define CMD_REG_WRITE 0x04
#define CMD_ACTION 0x05
#define CMD_RESET 0x06
#define CMD_DIGITAL_RESET 0x07
#define CMD_SYSTEM_READ 0x0C
#define CMD_SYSTEM_WRITE 0x0D
#define CMD_SYNC_WRITE 0x83
#define CMD_SYNC_REG_WRITE 0x84
#define CLEAR_BUFFER gbRxBufferReadPointer = gbRxBufferWritePointer
#define DEFAULT_RETURN_PACKET_SIZE 6
#define BROADCASTING_ID 0xfe
#define TxD8 TxD81
#define RxD8 RxD81


#define NUM_OF_SERVO 9//九个舵机

//Frame_Header*2，ID*1，Lenth*1，Cmd*1,Addr*1，Data*N，Check_Sum*1
typedef struct
{//通信使用
    u8 Head[2];
    u8 ID;
    u8 Len;
    u8 Cmd;
    u8 Add;
    u8 Data[25];
    u8 Check;
}Servo_Data_Frame_TypeDef;

typedef struct
{//舵机参数定义
    bool Action_Flag;     //动作标志位
    bool Dance_Flag;
    bool Init_Pos_Flag;   //是否需要移向初始位置
    u8 DanceMoveNum;
    u8 Step_Num;
    //u8 Dance_Step_Num[6];
    u16 Init_Pos[NUM_OF_SERVO+1];//初始位置
    u16 Pre_Pos[NUM_OF_SERVO+1];//当前位置
    u16 Goal_Pos[NUM_OF_SERVO+1];//目标位置
    u16 Step_Pos[NUM_OF_SERVO+1];//下一步的位置
}Servo_Arg_TypeDef;

extern Servo_Data_Frame_TypeDef         Servo_Data_Frame_Tx;
extern Servo_Data_Frame_TypeDef         Servo_Data_Frame_Rx;
extern Servo_Arg_TypeDef                Servo_Arg;
extern u16 DanceMove[8][NUM_OF_SERVO+1];

void Servo_Init();
void Servo_Reset(u8 id);
void Servo_Torque_EN(bool flag);
void Servo_Write_Byte(u8 id,u8 add,u8 data);//更改为u8
void Servo_Write_Word(u8 id,u8 add,u16 data);
void Servo_Read_Byte(u8 id,u8 add);
void Servo_Read_Word(u8 id,u8 add);
void Servo_Data_Send(Servo_Data_Frame_TypeDef *data_frame);
void Servo_Struct_Init();

void Servo_Action(u8 id,u8 cmd);
//void Servo_Pos_Read(u8 id);
void Servo_Pos_Read(u16 *array);
void Action_Play(u8 group);
void Dance_Move(u8 comboNum, u8 group);

#endif