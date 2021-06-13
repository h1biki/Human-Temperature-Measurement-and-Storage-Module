/***************************************************************

按键程序，主程序中调用Key_Pin_Read(void);
定时中断函数中调用Key_Acquisition();

引脚变化后需要改变引脚定义宏定义与初始化函数,根据按键数量调整Key_Pin_Read(void)
***************************************************************/

#ifndef __Key_H
#define __Key_H	 
#include "sys.h"

/********设定按键第二功能所用宏定义***************/
#define Key_Value_1  0X01
#define Key_Value_2	 0X02
#define Key_Value_3	 0X04
#define Key_Value_4	 0X08
#define Key_Value_5	 0X10
#define Key_Value_6	 0X20
#define Key_Value_7	 0X40
#define Key_Value_8	 0X80

/*********定义按键引脚读取宏定义*********************************/
#define KEY1  PAin(3) //读取按键1
#define KEY2  PAin(4)//读取按键2
#define KEY3  PAin(5)//读取按键3
#define KEY4  PAin(6)
#define KEY5  PAin(7)
#define KEY6  PBin(0)


extern unsigned char Key_Change;  //默认值是0，代表没有按键按下。只能被按键识别程序赋值，赋值后被主程序识别。被赋值为1，代表有按键按下。
extern unsigned char Key_Value;	  //一般按键命令为1-8。长按的命令暤回时0x11-0x18
extern unsigned char Key_Second_Function_Run;  //定义哪一个管脚的第二功能是快速变化。例：Key_Second_Function_Run=Key_Value_1|Key_Value_2;设定第1和2按键为长按快速变化。
extern unsigned char Key_Second_Function_Long; //定义哪一个管脚的第二功能是长按变化。例：Key_Second_Function_Long=Key_Value_3;设定第1和2按键为长按变化。

unsigned char Key_Pin_Read(void);//根据定义的引脚，读取每个引脚的数值，并返回数值
void Key_GPIO_Init(void);//初始化按键IO脚，在主函数中调用
void Key_Acquisition(void);	//按键识别程序，用在定时中断中，没10MS左右运行一次
unsigned char Key_Second_Function(unsigned char Key_Down);	//用在Key_Acquisition()内，对按下的按键进行第二功能判断。



#endif
