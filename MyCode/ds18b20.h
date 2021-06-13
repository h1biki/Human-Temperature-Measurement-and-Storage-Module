#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK MiniSTM32������
//DS18B20��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//IO��������
#define DS18B20_IO_IN()  {GPIOB->CRH&=0XFFF0FFFF;GPIOB->CRH|=8<<16;}
#define DS18B20_IO_OUT() {GPIOB->CRH&=0XFFF0FFFF;GPIOB->CRH|=3<<16;}
////IO��������											   
#define	DS18B20_DQ_OUT PBout(12) //���ݶ˿�	
#define	DS18B20_DQ_IN  PBin(12)  //���ݶ˿�	

extern short Temperature_Data;
extern unsigned char Data_View_Temp[];
extern char  DS18B20_Alert_L;	 //18B20�������EEPROM�У������ĵ��¶�ֵ����¶�ֵ
extern char  DS18B20_Alert_H;
//��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���
//����1:������
//����0:����   
u8 DS18B20_Init(void);			


short DS18B20_Get_Temp(void);	//��ȡ�¶�
void DS18B20_Start(void);		//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);		//����һ���ֽ�
u8 DS18B20_Read_Bit(void);		//����һ��λ
u8 DS18B20_Check(void);			//����Ƿ����DS18B20
void DS18B20_Rst(void);			//��λDS18B20    

/*
ÿ������100ms����һ�δ˺���
������Ϻ��¶���ֵ������Temperature_Data����ΧΪ-550��1250
Һ����ʾ��BCD��ֵ��������Data_View_Temp[7]���鵱��
*/
void DS18B20_Dis_Temperature(void);

/*
����֮���
DS18B20_Alert_L
DS18B20_Alert_H
�������ݴ�����eeprom����
*/
void DS18B20_Alert_Set(void);
#endif















