#include "ds18b20.h"
#include "delay.h"	


short Temperature_Data=0;//��¼��ȡ�����¶����ݣ���λΪ0.1�ȣ��������ֵΪ205�����ʾ��⵽��Ϊ20.5��
unsigned char Data_View_Temp[]="000.0C";

char  DS18B20_Alert_L;	 //18B20�������EEPROM�У������ĵ��¶�ֵ����¶�ֵ
char  DS18B20_Alert_H;
/*

ÿ������100ms����һ�δ˺���
������Ϻ��¶���ֵ������Temperature_Data����ΧΪ-550��1250
Һ����ʾ��BCD��ֵ��������Data_View_Temp[7]���鵱��
*/
void DS18B20_Dis_Temperature(void)
{
	Temperature_Data=DS18B20_Get_Temp();
	if(Temperature_Data<0)
	{
		Temperature_Data=Temperature_Data*(-1);
		Data_View_Temp[0]='-';
	}
	else
	{
		Data_View_Temp[0]=Temperature_Data/1000+0x30;
		if(Data_View_Temp[0]==0x30)
			Data_View_Temp[0]=' ';
	}
	Data_View_Temp[1]=Temperature_Data%1000/100+0x30; //���¶�ת��Ϊ������ʾ������
  Data_View_Temp[2]=Temperature_Data%100/10+0x30;
  Data_View_Temp[3]='.';
  Data_View_Temp[4]=Temperature_Data%10+0x30;
  Data_View_Temp[5]='C';
	
	if((Data_View_Temp[0]==' ')&&(Data_View_Temp[1]==0x30))//�ж��¶���ʾ���ź��Ƿ���Ҫ��ȱ��ʾ��λ��ʮλ
		Data_View_Temp[1]=' ';
  if((Data_View_Temp[0]=='-')&&(Data_View_Temp[1]==0x30))
  {
		Data_View_Temp[1]='-';
		Data_View_Temp[0]=' ';
	}
}

//��λDS18B20
void DS18B20_Rst(void)	   
{                 
	DS18B20_IO_OUT(); //SET ����OUTPUT
  DS18B20_DQ_OUT=0; //����DQ
  delay_us(750);    //����750us
  DS18B20_DQ_OUT=1; //DQ=1 
	delay_us(15);     //15US
}
//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽DS18B20�Ĵ���
//����0:����
u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;
	DS18B20_IO_IN();//SET ���� INPUT	 
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}
//��DS18B20��ȡһ��λ
//����ֵ��1/0
u8 DS18B20_Read_Bit(void) 			 // read one bit
{
    u8 data;
	DS18B20_IO_OUT();//�������״̬
    DS18B20_DQ_OUT=0; 
	delay_us(2);
    DS18B20_DQ_OUT=1; 
	DS18B20_IO_IN();//���ó�����״̬
	delay_us(12);
	if(DS18B20_DQ_IN)data=1;
    else data=0;	 
    delay_us(50);           
    return data;
}
//��DS18B20��ȡһ���ֽ�
//����ֵ������������
u8 DS18B20_Read_Byte(void)    // ��ȡһ���ֽ�
{        
    u8 i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}
//дһ���ֽڵ�DS18B20
//dat��Ҫд����ֽ�
void DS18B20_Write_Byte(u8 dat)     
 {             
    u8 j;
    u8 testb;
	DS18B20_IO_OUT();//�趨���״̬
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            DS18B20_DQ_OUT=0;// Write 1
            delay_us(2);                            
            DS18B20_DQ_OUT=1;
            delay_us(60);             
        }
        else 
        {
            DS18B20_DQ_OUT=0;// Write 0
            delay_us(60);             
            DS18B20_DQ_OUT=1;
            delay_us(2);                          
        }
    }
}
//��ʼ�¶�ת��
void DS18B20_Start(void)// ds1820 start convert
{   						               
    DS18B20_Rst();	   
		DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
} 

//��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���
//����1:������
//����0:����    	 
u8 DS18B20_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��IO��ʱ�� 
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				//�������
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOB,GPIO_Pin_12);    //���1

	DS18B20_Rst();
	if(DS18B20_Check())
	{	
		return 1;
	}
	else
	{
		DS18B20_Rst();
		DS18B20_Check();
		DS18B20_Write_Byte(0xcc);// skip rom
		DS18B20_Write_Byte(0xbe);// convert	
		DS18B20_Read_Byte();
		DS18B20_Read_Byte();
		DS18B20_Alert_H=DS18B20_Read_Byte();//��ȡEEPROM����
		DS18B20_Alert_L=DS18B20_Read_Byte();
		DS18B20_Start();//�����¶�ת��
		return 0;
	}
}  

void DS18B20_Alert_Set(void)
{
	DS18B20_Rst();
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);
	DS18B20_Write_Byte(0x4e);
	DS18B20_Write_Byte(DS18B20_Alert_H);
	DS18B20_Write_Byte(DS18B20_Alert_L);

	DS18B20_Rst();
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);
	DS18B20_Write_Byte(0x48);
	delay_ms(10);
	DS18B20_Rst();
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);
	DS18B20_Write_Byte(0x44);

}


//��ds18b20�õ��¶�ֵ
//���ȣ�0.1C
//����ֵ���¶�ֵ ��-550~1250�� 
short DS18B20_Get_Temp(void)
{
    u8 temp;
    u8 TL,TH;
		short tem;
    DS18B20_Start();                    // ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0xbe);// convert	    
    TL=DS18B20_Read_Byte(); // LSB   
    TH=DS18B20_Read_Byte(); // MSB  
	    	  
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//�¶�Ϊ��  
    }else temp=1;//�¶�Ϊ��	  	  
    tem=TH; //��ø߰�λ
    tem<<=8;    
    tem+=TL;//��õװ�λ
    tem=(float)tem*0.625;//ת��     
	if(temp)return tem; //�����¶�ֵ
	else return -tem;    
} 
 
