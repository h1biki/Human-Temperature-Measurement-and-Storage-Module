#include "Key.h"

unsigned char Key_Value=0;
unsigned char Key_Change=0;
unsigned char Key_Second_Function_Run;
unsigned char Key_Second_Function_Long;
unsigned char Key_Trigger_Time=100;//�趨���������ڶ�������Ҫ��ʱ�䣬100����1��
unsigned char Key_Space_Time=10;//�趨���ٱ仯������ÿ�α仯���ʱ�䣬20����0.2��

void Key_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//��ʼ�� ��������
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA,ENABLE);//ʹ��ʱ��

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //���óɸ�������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //���óɸ�������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
}

unsigned char Key_Pin_Read()
{
	unsigned char Reture_Data;
	Reture_Data=0xff;
	if(KEY1==0)
		Reture_Data=Reture_Data&0xFE;
	if(KEY2==0)
		Reture_Data=Reture_Data&0xFD;
	if(KEY3==0)
		Reture_Data=Reture_Data&0xFB;
	if(KEY4==0)
		Reture_Data=Reture_Data&0xF7;
	
	if(KEY5==0)
		Reture_Data=Reture_Data&0xEF;
	if(KEY6==0)
		Reture_Data=Reture_Data&0xDF;
//	if(KEY7==0)
//		Reture_Data=Reture_Data&0xBF;
//	if(KEY8==0)
//		Reture_Data=Reture_Data&0x7F;
	
	return(Reture_Data);
}

void Key_Acquisition()
{
 static unsigned char Key_Con=0;//���ڿ����������������״̬��
 static unsigned char Key_Keep;//���ڼ�¼�������ŵ�ֵ
 static unsigned char Key_Count;//���ڰ��������еı�Ҫ�ļ�������
 static unsigned char Key_Function_Con;//��¼���°�����ֵ�����ڵڶ����ܵĿ�����Ƚ�
 switch (Key_Con)
  {
   case 0:
      {
		   if( Key_Pin_Read()!=0xff)
		   {
				Key_Con=1;
				Key_Count=0;
			 }			  
		   break;
		  }
   case 1:
   		{
		   if( Key_Pin_Read()!=0xff)
			 {
			  switch (Key_Pin_Read()) //�Ե�Ƭ���ܽž���ɼ����жϰ��µİ�������һ��
			   {
			   	case 0xFE:Key_Keep=1;Key_Function_Con=Key_Value_1;break;
					case 0xFD:Key_Keep=2;Key_Function_Con=Key_Value_2;break;
					case 0xFB:Key_Keep=3;Key_Function_Con=Key_Value_3;break;
					case 0xF7:Key_Keep=4;Key_Function_Con=Key_Value_4;break;
					case 0xEF:Key_Keep=5;Key_Function_Con=Key_Value_5;break;
					case 0xDF:Key_Keep=6;Key_Function_Con=Key_Value_6;break;
					case 0xBF:Key_Keep=7;Key_Function_Con=Key_Value_7;break;
					case 0x7F:Key_Keep=8;Key_Function_Con=Key_Value_8;break;
			   }
			  Key_Con=2;			 
			 }
			else
			  Key_Con=0;
			break;
		 }
   case 2:
		  {
		   if( Key_Pin_Read()==0XFF) //���������������������������������������ذ���ֵ
			 {
			  Key_Con=0;
			  Key_Count=0;
			  Key_Change=1;	 //Key_Change��ֵ������һ���������а����������
			  Key_Value=Key_Keep;
			 }
		   else
			 {
			  if(Key_Second_Function(Key_Function_Con)!=0)
			   {
			   	if(Key_Count<Key_Trigger_Time)  //�����ڶ�������Ҫ���µ�ʱ���趨��  100����1��
				  Key_Count++;
				else
				  {
				   Key_Count=0;
				   Key_Con=3;
				  }
			   }
			 }
		   break;
		  }
   case 3:
   		  {
		   if(Key_Pin_Read()==0XFF)
		    {
			  Key_Con=0;
			  Key_Count=0;
			}
		   else
		    {
			 if(Key_Second_Function(Key_Function_Con)==1) //��������ڶ������ǿ��ٕ�Ӧ��
			  {
			  	if(Key_Count<Key_Space_Time)  //��������������Ҫ���µ�ʱ���趨��  20����0.2��
				  Key_Count++;
				else
				  {
				   Key_Count=0;
				   Key_Change=1;	 //Key_Change��ֵ������һ���������а����������
			       Key_Value=Key_Keep;
				  }
			  } 
			 else //��������ڶ������ǳ������ܡ�
			  {
			   if(Key_Count==0)
			    {
				   Key_Change=1;	 //Key_Change��ֵ������һ���������а����������
			       Key_Value=Key_Keep+0x10;
				   Key_Count=1;
				}
			  }
			}
		   break;
		  }
  }
}

unsigned char Key_Second_Function(unsigned char Key_Down)
{
 unsigned char Second_Function_Con;
 Second_Function_Con=Key_Down&Key_Second_Function_Run;
 if(Second_Function_Con!=0)
  return(1);   //������µİ����Ѿ��趨Ϊ���ٱ仯������1
 else
  {
   Second_Function_Con=Key_Down&Key_Second_Function_Long;
   if(Second_Function_Con!=0)
     return(2);	//������µİ����Ѿ��趨Ϊ�����仯������2
   else
     return(0);	//������µİ���û���趨�ڶ����ܣ�����0
  }
}
