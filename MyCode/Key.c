#include "Key.h"

unsigned char Key_Value=0;
unsigned char Key_Change=0;
unsigned char Key_Second_Function_Run;
unsigned char Key_Second_Function_Long;
unsigned char Key_Trigger_Time=100;//设定触暍按键第二功能需要的时间，100代表1秒
unsigned char Key_Space_Time=10;//设定快速变化按键，每次变化间隔时间，20代表0.2秒

void Key_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//初始化 上拉输入
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA,ENABLE);//使能时钟

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //设置成浮空输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //设置成浮空输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
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
 static unsigned char Key_Con=0;//用于控制整个按键程序的状态机
 static unsigned char Key_Keep;//用于记录按键引脚的值
 static unsigned char Key_Count;//用于按键程序中的必要的计数储存
 static unsigned char Key_Function_Con;//记录按下按键的值，用于第二功能的开启与比较
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
			  switch (Key_Pin_Read()) //对单片机管脚尽情采集，判断按下的按键是哪一个
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
		   if( Key_Pin_Read()==0XFF) //如果是正常按键，按键检测程序在这里结束。返回按键值
			 {
			  Key_Con=0;
			  Key_Count=0;
			  Key_Change=1;	 //Key_Change赋值，触发一次主程序中按键处理程序
			  Key_Value=Key_Keep;
			 }
		   else
			 {
			  if(Key_Second_Function(Key_Function_Con)!=0)
			   {
			   	if(Key_Count<Key_Trigger_Time)  //按键第二功能需要按下的时间设定。  100代表1秒
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
			 if(Key_Second_Function(Key_Function_Con)==1) //如果按键第二功能是快速暣应。
			  {
			  	if(Key_Count<Key_Space_Time)  //按键连续按键需要按下的时间设定。  20代表0.2秒
				  Key_Count++;
				else
				  {
				   Key_Count=0;
				   Key_Change=1;	 //Key_Change赋值，触发一次主程序中按键处理程序
			       Key_Value=Key_Keep;
				  }
			  } 
			 else //如果按键第二功能是长按功能。
			  {
			   if(Key_Count==0)
			    {
				   Key_Change=1;	 //Key_Change赋值，触发一次主程序中按键处理程序
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
  return(1);   //如果按下的按键已经设定为快速变化，返回1
 else
  {
   Second_Function_Con=Key_Down&Key_Second_Function_Long;
   if(Second_Function_Con!=0)
     return(2);	//如果按下的按键已经设定为长按变化，返回2
   else
     return(0);	//如果按下的按键没有设定第二功能，返回0
  }
}
