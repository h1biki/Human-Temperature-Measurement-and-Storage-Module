#include "delay.h"
#include "sys.h"
#include "24cxx.h"
#include "OLED12864.h"
#include "ds18b20.h"
#include "Key.h"
#include "Timer2.h"
#include "rtc.h" 		

unsigned char Time_Sec_Con=0;

unsigned char SFM_Data[3]={0,0,0};

unsigned char SFM_View_Data[]="24:50:50";

unsigned int  NYR_Data[3]={0,0,0};

unsigned char NYR_View_Data[]="2016-05-03";

unsigned char Record_View_Temp[]="000.0C";

unsigned char View_Con=0;
//界面控制 0正常显示 1设定日期 2设定时间 3查询记录

unsigned char View_Change=0;

unsigned char View_Con_Change=1;

void User_GPIO_Init(void);

unsigned char Record_Adress=0;//记录当前储存数据的位置，也就是0-99的具体编号 每个编号在储存器里占据24个字节。此地址数据储存在第2500字节的位置

unsigned char Record_EN=0;//0不储存 1储存一次数据

unsigned char Check_Adress=0;//用来储存查看数据的地址

unsigned char Check_Num=1;//用来记录查看的条数

unsigned char Check_View_Data[]="000";

void Record_Data_Program(void)//储存实时的数据
{
	AT24CXX_Write(Record_Adress*24,Data_View_Temp,6);
	AT24CXX_Write(Record_Adress*24+6,NYR_View_Data,10);
	AT24CXX_Write(Record_Adress*24+16,SFM_View_Data,8);
	
	Record_Adress++;//储存位置++
	if(Record_Adress==100)
		Record_Adress=0;
	AT24CXX_WriteOneByte(2500,Record_Adress);
}

int main(void)
{		
	delay_init();//初始化延迟
	NVIC_Configuration();//设定中断规则
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//初始化AFIO时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//使用SW调试模式，关闭JTAG模式
	
	OLED_Init();
	AT24CXX_Init();
	TIM2_Int_Init(100,7199);
	OLED_P8x16Str(0,0,"Check FM24C256");
	while(AT24CXX_Check());
	User_GPIO_Init();
	OLED_P8x16Str(0,0,"Check DS18B20 ");
	while(DS18B20_Init());
	Key_GPIO_Init();
	Key_Second_Function_Run=Key_Value_2|Key_Value_3|Key_Value_4|Key_Value_5|Key_Value_6;
	Key_Second_Function_Long=Key_Value_1;
	OLED_P8x16Str(0,0,"**20151646*zzx**");
	delay_ms(500);
	delay_ms(500);
	while(RTC_Init());
	
	//AT24CXX_WriteOneByte(2500,0);//
	
	Record_Adress=AT24CXX_ReadOneByte(2500);
	while(1)
	{
		if(View_Change)
		{
			View_Change=0;
			if(View_Con_Change)
			{
				View_Con_Change=0;
				OLED_Fill(0x00); //清屏
				switch(View_Con)
				{
					case 0:
					{
						OLED_P8x16Str(0,0,"Temp:           ");
						OLED_P8x16Str(0,2,"Data:           ");
						OLED_P8x16Str(0,4,"Time:           ");
						break;
					}
					case 1:
					{
						OLED_P8x16Str(0,0,"    Data Set    ");
						break;
					}
					case 2:
					{
						OLED_P8x16Str(0,0,"    Time Set    ");
						break;
					}
					case 3:
					{
						OLED_P8x16Str(0,0,"Check Num:      ");
						OLED_P8x16Str(0,2,"Temp:           ");
						OLED_P8x16Str(0,4,"Data:           ");
						OLED_P8x16Str(0,6,"Time:           ");
						break;
					}
				}
			}
			SFM_View_Data[0]=SFM_Data[0]/10+0x30;
			SFM_View_Data[1]=SFM_Data[0]%10+0x30;
			SFM_View_Data[2]=':';
			SFM_View_Data[3]=SFM_Data[1]/10+0x30;
			SFM_View_Data[4]=SFM_Data[1]%10+0x30;
			SFM_View_Data[5]=':';
			SFM_View_Data[6]=SFM_Data[2]/10+0x30;
			SFM_View_Data[7]=SFM_Data[2]%10+0x30;

			NYR_View_Data[0]=NYR_Data[0]/1000+0x30;
			NYR_View_Data[1]=NYR_Data[0]%1000/100+0x30;
			NYR_View_Data[2]=NYR_Data[0]%100/10+0x30;
			NYR_View_Data[3]=NYR_Data[0]%10+0x30;
      NYR_View_Data[4]='-';
			NYR_View_Data[5]=NYR_Data[1]%100/10+0x30;
			NYR_View_Data[6]=NYR_Data[1]%10+0x30;
			NYR_View_Data[7]='-';
			NYR_View_Data[8]=NYR_Data[2]%100/10+0x30;
			NYR_View_Data[9]=NYR_Data[2]%10+0x30;
			
			switch(View_Con)
			{
				case 0:
				{
					OLED_P8x16Str(40,0,Data_View_Temp);
					OLED_P8x16Str(40,2,NYR_View_Data);
					OLED_P8x16Str(40,4,SFM_View_Data);
					
					if(Record_EN==1)
				{
						Record_EN=0;
						Record_Data_Program();
						OLED_P8x16Str(0,6,"         ");
				}
					break;
				}
				case 1:
				{
					OLED_P8x16Str(24,2,NYR_View_Data);
					break;
				}
				case 2:
				{
					OLED_P8x16Str(32,2,SFM_View_Data);
					break;
				}
				case 3:
				{
					/**************************///得到查看数据的地址
					if(Record_Adress>=Check_Num)
						Check_Adress=Record_Adress-Check_Num;
					else
					{
						Check_Adress=100-Check_Num+Record_Adress;
					}
					/**********************************/
					Check_View_Data[0]=Check_Num/100+0x30;
					Check_View_Data[1]=Check_Num%100/10+0x30;
					Check_View_Data[2]=Check_Num%10+0x30;
					
					OLED_P8x16Str(104,0,Check_View_Data);
					
					AT24CXX_Read(Check_Adress*24,Record_View_Temp,6);
					AT24CXX_Read(Check_Adress*24+6,NYR_View_Data,10);
					AT24CXX_Read(Check_Adress*24+16,SFM_View_Data,8);
					
					OLED_P8x16Str(40,2,Record_View_Temp);
					OLED_P8x16Str(40,4,NYR_View_Data);
					OLED_P8x16Str(40,6,SFM_View_Data);
					
					break;
				}

			}
		}

		if(Key_Change)
		{
			Key_Change=0;
			View_Change=1;
			switch(Key_Value)
			{
				case 0x11:
				{
					if(View_Con==0)
					{
						View_Con=3;
					}
					else
					{
						View_Con=0;
					}
					View_Con_Change=1;
					break;
				}
				case 1:
				{
					if(View_Con<3)
					{
						View_Con++;
						View_Con_Change=1;
						if(View_Con==3)
						{
							View_Con=0;
							RTC_Set(NYR_Data[0],NYR_Data[1],NYR_Data[2],SFM_Data[0],SFM_Data[1],SFM_Data[2]);
						}
					}
					break;
				}
				case 2:
				{
					switch(View_Con)
					{
						case 1:
						{
							NYR_Data[0]++;
							if(NYR_Data[0]==2050)
								NYR_Data[0]=2015;
							break;
						}
						case 2:
						{
							SFM_Data[0]++;
							if(SFM_Data[0]==24)
								SFM_Data[0]=0;
							break;
						}
					}
					break;
				}
				case 3:
				{
					switch(View_Con)
					{
						case 1:
						{
							NYR_Data[1]++;
							if(NYR_Data[1]==13)
								NYR_Data[1]=1;
							break;
						}
						case 2:
						{
							SFM_Data[1]++;
							if(SFM_Data[1]==60)
								SFM_Data[1]=0;
							break;
						}
					}
					break;
				}
				case 4:
				{
					switch(View_Con)
					{
						case 1:
						{
							NYR_Data[2]++;
							if(NYR_Data[2]==31)
								NYR_Data[2]=1;
							break;
						}
						case 2:
						{
							SFM_Data[2]++;
							if(SFM_Data[2]==60)
								SFM_Data[2]=0;
							break;
						}
					}
					break;
				}
				case 5:
				{
					if(View_Con==0)
					{
						Record_EN=1;
						OLED_P8x16Str(0,6,"Recording");
					}
					if(View_Con==3)
					{
						if(Check_Num<100)
							Check_Num++;
					}
					break;
				}
				case 6:
				{
					if(Check_Num>1)
						Check_Num--;
					break;
				}
			}
		}
		if(Time_Sec_Con!=calendar.sec&&View_Con==0)
		{
			Time_Sec_Con=calendar.sec;
			View_Change=1;
			
			DS18B20_Dis_Temperature();
			SFM_Data[0]=calendar.hour;
			SFM_Data[1]=calendar.min;
			SFM_Data[2]=calendar.sec;

			NYR_Data[0]=calendar.w_year;
			NYR_Data[1]=calendar.w_month;
			NYR_Data[2]=calendar.w_date;
		}
	}
}

void TIM2_IRQHandler(void)   //TIM2中断
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM2更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志
		Key_Acquisition();
	}
}


void User_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	 
}






