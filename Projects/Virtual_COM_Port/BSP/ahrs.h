#ifndef __AHRS_H
#define __AHRS_H

//IMU传感器选择，只能二选一，
#define JY_901
//#define MPU9250  
//IMU传感器接口类型选择，只能二选一，
#define USART
//#define IIC  



#ifdef MPU9250
#define Data_Tpye       s16
#endif
#ifdef JY_901
	#ifdef IIC
		#define Data_Tpye       float
	#endif
	#ifdef USART
		#define Data_Tpye       s16
	#endif
#endif
  


/* ---- MPU9250 Reg In MPU9250 ---------------------------------------------- */
#define MPU9250_I2C_ADDR            ((u8)0xD0)
#define MPU9250_Device_ID           ((u8)0x71)  // In MPU9250

#define MPU9250_SELF_TEST_XG        ((u8)0x00)
#define MPU9250_SELF_TEST_YG        ((u8)0x01)
#define MPU9250_SELF_TEST_ZG        ((u8)0x02)
#define MPU9250_SELF_TEST_XA        ((u8)0x0D)
#define MPU9250_SELF_TEST_YA        ((u8)0x0E)
#define MPU9250_SELF_TEST_ZA        ((u8)0x0F)
#define MPU9250_XG_OFFSET_H         ((u8)0x13)
#define MPU9250_XG_OFFSET_L         ((u8)0x14)
#define MPU9250_YG_OFFSET_H         ((u8)0x15)
#define MPU9250_YG_OFFSET_L         ((u8)0x16)
#define MPU9250_ZG_OFFSET_H         ((u8)0x17)
#define MPU9250_ZG_OFFSET_L         ((u8)0x18)
#define MPU9250_SMPLRT_DIV          ((u8)0x19)
#define MPU9250_CONFIG              ((u8)0x1A)
#define MPU9250_GYRO_CONFIG         ((u8)0x1B)
#define MPU9250_ACCEL_CONFIG        ((u8)0x1C)
#define MPU9250_ACCEL_CONFIG_2      ((u8)0x1D)
#define MPU9250_LP_ACCEL_ODR        ((u8)0x1E)
#define MPU9250_MOT_THR             ((u8)0x1F)
#define MPU9250_FIFO_EN             ((u8)0x23)
#define MPU9250_I2C_MST_CTRL        ((u8)0x24)
#define MPU9250_I2C_SLV0_ADDR       ((u8)0x25)
#define MPU9250_I2C_SLV0_REG        ((u8)0x26)
#define MPU9250_I2C_SLV0_CTRL       ((u8)0x27)
#define MPU9250_I2C_SLV1_ADDR       ((u8)0x28)
#define MPU9250_I2C_SLV1_REG        ((u8)0x29)
#define MPU9250_I2C_SLV1_CTRL       ((u8)0x2A)
#define MPU9250_I2C_SLV2_ADDR       ((u8)0x2B)
#define MPU9250_I2C_SLV2_REG        ((u8)0x2C)
#define MPU9250_I2C_SLV2_CTRL       ((u8)0x2D)
#define MPU9250_I2C_SLV3_ADDR       ((u8)0x2E)
#define MPU9250_I2C_SLV3_REG        ((u8)0x2F)
#define MPU9250_I2C_SLV3_CTRL       ((u8)0x30)
#define MPU9250_I2C_SLV4_ADDR       ((u8)0x31)
#define MPU9250_I2C_SLV4_REG        ((u8)0x32)
#define MPU9250_I2C_SLV4_DO         ((u8)0x33)
#define MPU9250_I2C_SLV4_CTRL       ((u8)0x34)
#define MPU9250_I2C_SLV4_DI         ((u8)0x35)
#define MPU9250_I2C_MST_STATUS      ((u8)0x36)
#define MPU9250_INT_PIN_CFG         ((u8)0x37)
#define MPU9250_INT_ENABLE          ((u8)0x38)
#define MPU9250_INT_STATUS          ((u8)0x3A)
#define MPU9250_ACCEL_XOUT_H        ((u8)0x3B)
#define MPU9250_ACCEL_XOUT_L        ((u8)0x3C)
#define MPU9250_ACCEL_YOUT_H        ((u8)0x3D)
#define MPU9250_ACCEL_YOUT_L        ((u8)0x3E)
#define MPU9250_ACCEL_ZOUT_H        ((u8)0x3F)
#define MPU9250_ACCEL_ZOUT_L        ((u8)0x40)
#define MPU9250_TEMP_OUT_H          ((u8)0x41)
#define MPU9250_TEMP_OUT_L          ((u8)0x42)
#define MPU9250_GYRO_XOUT_H         ((u8)0x43)
#define MPU9250_GYRO_XOUT_L         ((u8)0x44)
#define MPU9250_GYRO_YOUT_H         ((u8)0x45)
#define MPU9250_GYRO_YOUT_L         ((u8)0x46)
#define MPU9250_GYRO_ZOUT_H         ((u8)0x47)
#define MPU9250_GYRO_ZOUT_L         ((u8)0x48)
#define MPU9250_EXT_SENS_DATA_00    ((u8)0x49)
#define MPU9250_EXT_SENS_DATA_01    ((u8)0x4A)
#define MPU9250_EXT_SENS_DATA_02    ((u8)0x4B)
#define MPU9250_EXT_SENS_DATA_03    ((u8)0x4C)
#define MPU9250_EXT_SENS_DATA_04    ((u8)0x4D)
#define MPU9250_EXT_SENS_DATA_05    ((u8)0x4E)
#define MPU9250_EXT_SENS_DATA_06    ((u8)0x4F)
#define MPU9250_EXT_SENS_DATA_07    ((u8)0x50)
#define MPU9250_EXT_SENS_DATA_08    ((u8)0x51)
#define MPU9250_EXT_SENS_DATA_09    ((u8)0x52)
#define MPU9250_EXT_SENS_DATA_10    ((u8)0x53)
#define MPU9250_EXT_SENS_DATA_11    ((u8)0x54)
#define MPU9250_EXT_SENS_DATA_12    ((u8)0x55)
#define MPU9250_EXT_SENS_DATA_13    ((u8)0x56)
#define MPU9250_EXT_SENS_DATA_14    ((u8)0x57)
#define MPU9250_EXT_SENS_DATA_15    ((u8)0x58)
#define MPU9250_EXT_SENS_DATA_16    ((u8)0x59)
#define MPU9250_EXT_SENS_DATA_17    ((u8)0x5A)
#define MPU9250_EXT_SENS_DATA_18    ((u8)0x5B)
#define MPU9250_EXT_SENS_DATA_19    ((u8)0x5C)
#define MPU9250_EXT_SENS_DATA_20    ((u8)0x5D)
#define MPU9250_EXT_SENS_DATA_21    ((u8)0x5E)
#define MPU9250_EXT_SENS_DATA_22    ((u8)0x5F)
#define MPU9250_EXT_SENS_DATA_23    ((u8)0x60)
#define MPU9250_I2C_SLV0_DO         ((u8)0x63)
#define MPU9250_I2C_SLV1_DO         ((u8)0x64)
#define MPU9250_I2C_SLV2_DO         ((u8)0x65)
#define MPU9250_I2C_SLV3_DO         ((u8)0x66)
#define MPU9250_I2C_MST_DELAY_CTRL  ((u8)0x67)
#define MPU9250_SIGNAL_PATH_RESET   ((u8)0x68)
#define MPU9250_MOT_DETECT_CTRL     ((u8)0x69)
#define MPU9250_USER_CTRL           ((u8)0x6A)
#define MPU9250_PWR_MGMT_1          ((u8)0x6B)
#define MPU9250_PWR_MGMT_2          ((u8)0x6C)
#define MPU9250_FIFO_COUNTH         ((u8)0x72)
#define MPU9250_FIFO_COUNTL         ((u8)0x73)
#define MPU9250_FIFO_R_W            ((u8)0x74)
#define MPU9250_WHO_AM_I            ((u8)0x75)	// ID = 0x71 In MPU9250
#define MPU9250_XA_OFFSET_H         ((u8)0x77)
#define MPU9250_XA_OFFSET_L         ((u8)0x78)
#define MPU9250_YA_OFFSET_H         ((u8)0x7A)
#define MPU9250_YA_OFFSET_L         ((u8)0x7B)
#define MPU9250_ZA_OFFSET_H         ((u8)0x7D)
#define MPU9250_ZA_OFFSET_L         ((u8)0x7E)

/* ---- AK8963 Reg In MPU9250 ----------------------------------------------- */
#define AK8963_I2C_ADDR             ((u8)0x18)
#define AK8963_Device_ID            ((u8)0x48)

// Read-only Reg
#define AK8963_WIA                  ((u8)0x00)
#define AK8963_INFO                 ((u8)0x01)
#define AK8963_ST1                  ((u8)0x02)
#define AK8963_HXL                  ((u8)0x03)
#define AK8963_HXH                  ((u8)0x04)
#define AK8963_HYL                  ((u8)0x05)
#define AK8963_HYH                  ((u8)0x06)
#define AK8963_HZL                  ((u8)0x07)
#define AK8963_HZH                  ((u8)0x08)
#define AK8963_ST2                  ((u8)0x09)
// Write/Read Reg
#define AK8963_CNTL1                ((u8)0x0A)
#define AK8963_CNTL2                ((u8)0x0B)
#define AK8963_ASTC                 ((u8)0x0C)
#define AK8963_TS1                  ((u8)0x0D)
#define AK8963_TS2                  ((u8)0x0E)
#define AK8963_I2CDIS               ((u8)0x0F)
// Read-only Reg ( ROM )
#define AK8963_ASAX                 ((u8)0x10)
#define AK8963_ASAY                 ((u8)0x11)
#define AK8963_ASAZ                 ((u8)0x12)



/*---------------------     AHRS 寄存器    JY_901   -------------------------------*/
#define SAVE 			0x00
#define CALSW 		0x01
#define RSW 			0x02
#define RRATE			0x03
#define BAUD 			0x04
#define AXOFFSET	0x05
#define AYOFFSET	0x06
#define AZOFFSET	0x07
#define GXOFFSET	0x08
#define GYOFFSET	0x09
#define GZOFFSET	0x0a
#define HXOFFSET	0x0b
#define HYOFFSET	0x0c
#define HZOFFSET	0x0d
#define D0MODE		0x0e
#define D1MODE		0x0f
#define D2MODE		0x10
#define D3MODE		0x11
#define D0PWMH		0x12
#define D1PWMH		0x13
#define D2PWMH		0x14
#define D3PWMH		0x15
#define D0PWMT		0x16
#define D1PWMT		0x17
#define D2PWMT		0x18
#define D3PWMT		0x19
#define IICADDR		0x1a
#define LEDOFF 		0x1b
#define GPSBAUD		0x1c

#define YYMM				0x30
#define DDHH				0x31
#define MMSS				0x32
#define MS					0x33
#define AX					0x34
#define AY					0x35
#define AZ					0x36
#define GX					0x37
#define GY					0x38
#define GZ					0x39
#define HX					0x3a
#define HY					0x3b
#define HZ					0x3c			
#define Roll				0x3d
#define Pitch				0x3e
#define Yaw					0x3f
#define TEMP				0x40
#define D0Status		0x41
#define D1Status		0x42
#define D2Status		0x43
#define D3Status		0x44
#define PressureL		0x45
#define PressureH		0x46
#define HeightL			0x47
#define HeightH			0x48
#define LonL				0x49
#define LonH				0x4a
#define LatL				0x4b
#define LatH				0x4c
#define GPSHeight   0x4d
#define GPSYAW      0x4e
#define GPSVL				0x4f
#define GPSVH				0x50

#define DIO_MODE_AIN 0
#define DIO_MODE_DIN 1
#define DIO_MODE_DOH 2
#define DIO_MODE_DOL 3
#define DIO_MODE_DOPWM 4
#define DIO_MODE_GPS 5			

typedef  struct
{
	s16 x;
	s16 y;
	s16 z;
} Vector3_Data_TypDef;
typedef  struct
{
	s16 q0;    //w
	s16 q1;    //x
	s16 q2;    //y
	s16 q3;    //z
}Quat_Data_TypDef;
//三维坐标结构体
typedef struct 
{
  Vector3_Data_TypDef Acc;
  Vector3_Data_TypDef Gyro;
  Vector3_Data_TypDef Mag;
  Vector3_Data_TypDef Ang;
  Data_Tpye Temp;
  Quat_Data_TypDef Quat;
}IMU_TypDef;
typedef struct 
{
  u32 Lon;
  u32 Lat;
  u16 GpsHight;
  u16 GpsYaw;
  s32 GpsVel;
}GPS_TypDef;
typedef struct 
{
  IMU_TypDef IMU;
  GPS_TypDef GPS;
}AHRS_Data_TypDef;
extern AHRS_Data_TypDef AHRS_Data;

float AccToAngle(float ax,float ay,float az,u8 dir);
void ShortToChar(short sData,unsigned char cData[]);
short CharToShort(unsigned char cData[]);

void MPU9250_IIC_Init();
void MPU9250_IIC_Data_Read();

void JY_901_IIC_Init();
void JY_901_IIC_Data_Read();
void JY_901_USART_Init();
void JY_901_USART_Data_Read();
void JY_901_USART_Data_RX_Handler(u16 usart_data);

void AHRS_Init();
void AHRS_Data_Read();
#endif

