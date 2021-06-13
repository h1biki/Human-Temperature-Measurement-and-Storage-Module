#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK MiniSTM32开发板
//DS18B20驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/12
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//IO方向设置
#define DS18B20_IO_IN()  {GPIOB->CRH&=0XFFF0FFFF;GPIOB->CRH|=8<<16;}
#define DS18B20_IO_OUT() {GPIOB->CRH&=0XFFF0FFFF;GPIOB->CRH|=3<<16;}
////IO操作函数											   
#define	DS18B20_DQ_OUT PBout(12) //数据端口	
#define	DS18B20_DQ_IN  PBin(12)  //数据端口	

extern short Temperature_Data;
extern unsigned char Data_View_Temp[];
extern char  DS18B20_Alert_L;	 //18B20自身带的EEPROM中，报警的低温度值与高温度值
extern char  DS18B20_Alert_H;
//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在   
u8 DS18B20_Init(void);			


short DS18B20_Get_Temp(void);	//获取温度
void DS18B20_Start(void);		//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);		//读出一个字节
u8 DS18B20_Read_Bit(void);		//读出一个位
u8 DS18B20_Check(void);			//检测是否存在DS18B20
void DS18B20_Rst(void);			//复位DS18B20    

/*
每隔至少100ms运行一次此函数
运行完毕后，温度数值储存在Temperature_Data，范围为-550到1250
液晶显示的BCD码值，储存在Data_View_Temp[7]数组当中
*/
void DS18B20_Dis_Temperature(void);

/*
运行之后吧
DS18B20_Alert_L
DS18B20_Alert_H
两个数据储存在eeprom当中
*/
void DS18B20_Alert_Set(void);
#endif















