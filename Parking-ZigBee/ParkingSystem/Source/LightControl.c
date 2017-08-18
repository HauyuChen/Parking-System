/**************************************************
** 功能：停车场亮度信息采集、数码管更新
** 硬件：光照传感器BH1750（GY-30）、TM1650数码管
** 日期：2015-5-8
**************************************************/
#include <ioCC2530.h>

#include "LightControl.h"

#define uint unsigned int
#define uchar unsigned char

#define SlaveAddress 0x46

#define SCL P1_2
#define SDA P1_3

uchar ge,shi,bai,qian,wan;  //数据变量
uchar Light[5];             //亮度数据
unsigned char tab[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};  //数码管

void Delay_1u(unsigned int times) //延时us
{
  while(times--)
  {
    asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
    asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
    asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
    asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
    asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
    asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
    asm("nop"); asm("nop");
  }
}

void Delay_1ms(unsigned int times)  //延时ms
{
    unsigned int i;
    for(i=0; i<times; i++)       
        Delay_1u(1000);            
}

void WriteSDA1(void)  //SDA输出1,相当于51里面的SDA=1   
{
    P1DIR |= 0x08;
    SDA = 1;
}    
void WriteSDA0(void)  //SDA 输出0   
{
    P1DIR |= 0x08;
    SDA = 0;
}  
void WriteSCL1(void)  //SCL 输出1   
{
    P1DIR |= 0x04;
    SCL = 1;
}
void WriteSCL0(void)  //SCL 输出0   
{
    P1DIR |= 0x04;
    SCL = 0;
}  
void ReadSDA(void)  //这里设置SDA对应IO口DIR可以接收数据   
{
    P1DIR &= 0xF7;
}

/* 起始信号 */
void START()
{
    WriteSDA1();
    WriteSCL1();
    Delay_1u(5);
    WriteSDA0();
    Delay_1u(5);
    WriteSCL0();
    Delay_1u(5);
}

/* 停止信号 */
void STOP()
{
    WriteSDA0();
    WriteSCL1();
    Delay_1u(5);
    WriteSDA1();
    Delay_1u(5);
    WriteSCL0();
    Delay_1u(5);
}

/* 发送应答信号-入口参数:ack (0:ACK 1:NAK) */
void SendACK(uchar ack)
{
   if(ack)
   {
       WriteSDA1();
   }
   else
   {
       WriteSDA0();
   }  
     
       Delay_1u(5);
       WriteSCL1();   
       Delay_1u(5);
       WriteSCL0();
}

/* 接收应答信号 */
uchar BH1750_RecvACK()
{        
    //unsigned int i=10;
    ReadSDA();F0=SDA;
    WriteSCL1();
    Delay_1u(5);
    WriteSCL0();
    Delay_1u(5);
    P1DIR |= 0x08;
    return F0;
}

/* 发送0，在SCL为高电平时使SDA信号为低 */
void SEND_0_1(void)   /* SEND ACK */
{
    WriteSDA0();
    Delay_1u(5);
    WriteSCL1();
    Delay_1u(5);
    WriteSCL0();
    Delay_1u(5);
}

/* 发送1，在SCL为高电平时使SDA信号为高 */
void SEND_1_1(void)
{
    WriteSDA1();
    Delay_1u(5);
    WriteSCL1();
    Delay_1u(5);
    WriteSCL0();
    Delay_1u(5);
}

/* 向IIC总线发送一个字节数据 */
void BH1750_SendByte(uchar dat)
{
    char i;
    WriteSCL0();
    for(i=0;i<8;i++)
    {
        if((dat<<i)&0x80)
        {
            SEND_1_1();
        }
        else
        {
            SEND_0_1();
        }
    }
    BH1750_RecvACK();     
}

/* 从IIC总线接收一个字节数据 */
uchar BH1750_RecvByte()
{
    char b=0,i;    
    WriteSCL0();
    Delay_1u(5);
    for(i=0;i<8;i++)
    {   
        ReadSDA();
        WriteSCL1();
        Delay_1u(5);
        F0=SDA; //寄存器中的一位,用于存储SDA中的一位数据
        if(F0)
        {
            b=b<<1;
            b=b|0x01;
        }
        else
        {b=b<<1;}
        WriteSCL0();
        Delay_1u(5);
    }
    P1DIR |= 0x08;
    return b;
}

uchar Read_BH1750()
{
    uchar t0,t1,t;
    START();
    BH1750_SendByte(SlaveAddress);      //发送设备地址+写信号
    if(F0)
    {
        START();                        //起始信号
        BH1750_SendByte(SlaveAddress);  //发送设备地址+写信号
    }
    BH1750_SendByte(0x00);              // 断电
    if(F0)
    {
        BH1750_SendByte(0x00);      
    }
    STOP();
    Delay_1ms(30);

    START();                            //起始信号
    BH1750_SendByte(SlaveAddress);      //发送设备地址+写信号
    if(F0)
    {
        START();                        //起始信号
        BH1750_SendByte(SlaveAddress);  //发送设备地址+写信号
    }
    BH1750_SendByte(0x01);              // 上电
    if(F0)
    {
        BH1750_SendByte(0x01);      
    }
    STOP();

    START();                            //起始信号
    BH1750_SendByte(SlaveAddress);      //发送设备地址+写信号
    if(F0)
    {
        START();                        //起始信号
        BH1750_SendByte(SlaveAddress);  //发送设备地址+写信号
    }
    BH1750_SendByte(0x10);              // H- resolution mode
    if(F0)
    {
        BH1750_SendByte(0x10);      
    }
    STOP();
    Delay_1ms(180);
   
    START();                            //起始信号
    BH1750_SendByte(SlaveAddress+1);    //发送设备地址+读信号
    if(F0)
    {
        START();                        //起始信号
        BH1750_SendByte(SlaveAddress+1);//发送设备地址+写信号
    }
   
    t0= BH1750_RecvByte();              //BUF[0]存储0x32地址中的数据
    SendACK(0);                         //最后一个数据需要回NOACK

    t1= BH1750_RecvByte();
    SendACK(1);                         //回应ACK
      
    STOP();                             //停止信号
    t=((t0<<8)+t1)/1.2;
    Delay_1ms(5);
    return t;
}

void Single_Write_BH1750(uchar REG_Address)
{
    START();                         //起始信号
    BH1750_SendByte(SlaveAddress);   //发送设备地址+写信号
    BH1750_SendByte(REG_Address);    //内部寄存器地址，
    STOP();                          //发送停止信号
}

void Init_BH1750()
{
    BH1750_SendByte(0x01);           //初始化
}

/* 字符转换 */
void conversion(uint temp_data)  //数据转换出 个，十，百，千，万
{  
  wan=temp_data/10000+0x30 ;
  temp_data=temp_data%10000;     //取余运算
  qian=temp_data/1000+0x30 ;
  temp_data=temp_data%1000;      //取余运算
  bai=temp_data/100+0x30   ;
  temp_data=temp_data%100;       //取余运算
  shi=temp_data/10+0x30    ;
  temp_data=temp_data%10;        //取余运算
  ge=temp_data+0x30;        
  Light[0]=(unsigned char)wan;
  Light[1]=(unsigned char)qian;
  Light[2]=(unsigned char)bai;
  Light[3]=(unsigned char)shi;
  Light[4]=(unsigned char)ge;
}

/* 光度数据采集 */
void Brightness()
{
  Delay_1ms(100);
  Init_BH1750();
  float lx;
  lx = Read_BH1750();
  conversion(lx);    
  Delay_1ms(1000);
}

void TM1650_Write(unsigned char	DATA)	//写数据函数
{
  unsigned char i;
  Delay_1u(1);
  WriteSCL0();
  for(i=0;i<8;i++)
  {
    if(DATA&0X80)
            WriteSDA1();
    else
            WriteSDA0();
    DATA<<=1;
    WriteSCL0();
    Delay_1u(1);
    WriteSCL1();
    Delay_1u(1);
    WriteSCL0();
    Delay_1u(1);
  }	
}

void Write_DATA(unsigned char add,unsigned char DATA)   //指定地址写入数据
{
  START();
  TM1650_Write(add);
  SendACK(0);
  TM1650_Write(DATA);
  SendACK(0);
  STOP();
}

/* 更新数码管 */
void num_led(uint num[])
{
  Write_DATA(0x48,0x01);
  Write_DATA(0x68+0,tab[(10)%16]|0x80); //第一位
  Write_DATA(0x68+2,tab[(0)%16]);       //第二位
  Write_DATA(0x68+4,tab[(num[0])%16]);  //第三位
  Write_DATA(0x68+6,tab[(num[1])%16]);  //第四位
  Delay_1ms(500);
}