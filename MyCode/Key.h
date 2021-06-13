/***************************************************************

���������������е���Key_Pin_Read(void);
��ʱ�жϺ����е���Key_Acquisition();

���ű仯����Ҫ�ı����Ŷ���궨�����ʼ������,���ݰ�����������Key_Pin_Read(void)
***************************************************************/

#ifndef __Key_H
#define __Key_H	 
#include "sys.h"

/********�趨�����ڶ��������ú궨��***************/
#define Key_Value_1  0X01
#define Key_Value_2	 0X02
#define Key_Value_3	 0X04
#define Key_Value_4	 0X08
#define Key_Value_5	 0X10
#define Key_Value_6	 0X20
#define Key_Value_7	 0X40
#define Key_Value_8	 0X80

/*********���尴�����Ŷ�ȡ�궨��*********************************/
#define KEY1  PAin(3) //��ȡ����1
#define KEY2  PAin(4)//��ȡ����2
#define KEY3  PAin(5)//��ȡ����3
#define KEY4  PAin(6)
#define KEY5  PAin(7)
#define KEY6  PBin(0)


extern unsigned char Key_Change;  //Ĭ��ֵ��0������û�а������¡�ֻ�ܱ�����ʶ�����ֵ����ֵ��������ʶ�𡣱���ֵΪ1�������а������¡�
extern unsigned char Key_Value;	  //һ�㰴������Ϊ1-8�������������ʱ0x11-0x18
extern unsigned char Key_Second_Function_Run;  //������һ���ܽŵĵڶ������ǿ��ٱ仯������Key_Second_Function_Run=Key_Value_1|Key_Value_2;�趨��1��2����Ϊ�������ٱ仯��
extern unsigned char Key_Second_Function_Long; //������һ���ܽŵĵڶ������ǳ����仯������Key_Second_Function_Long=Key_Value_3;�趨��1��2����Ϊ�����仯��

unsigned char Key_Pin_Read(void);//���ݶ�������ţ���ȡÿ�����ŵ���ֵ����������ֵ
void Key_GPIO_Init(void);//��ʼ������IO�ţ����������е���
void Key_Acquisition(void);	//����ʶ��������ڶ�ʱ�ж��У�û10MS��������һ��
unsigned char Key_Second_Function(unsigned char Key_Down);	//����Key_Acquisition()�ڣ��԰��µİ������еڶ������жϡ�



#endif
