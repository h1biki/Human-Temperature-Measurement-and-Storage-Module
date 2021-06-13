#ifndef __OLED12864_h__
#define	__OLED12864_h__

#include "sys.h"
#include "delay.h"

// ------------------------------------------------------------
// IO口模拟I2C通信
// SCL
// SDA
// ------------------------------------------------------------
#define OLED_SCL PBout(9)  //串行时钟
#define OLED_SDA PBout(8) //串行数据

#define high 1
#define low 0

#define	Brightness	0xCF 
#define X_WIDTH 	128
#define Y_WIDTH 	64

void OLED12864_GPIO_Init(void);
void OLED12864_delay(unsigned int z);
void OLED_IIC_Start(void);
void OLED_IIC_Stop(void);
void OLED_Write_IIC_Byte(unsigned char IIC_Byte);
void OLED_WrDat(unsigned char IIC_Data)	;
void OLED_WrCmd(unsigned char IIC_Command);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char bmp_dat);
void OLED_Init(void);
void OLED_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
	


#endif
