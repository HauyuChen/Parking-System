#ifndef __LIGHTCONTROL_H__
#define __LIGHTCONTROL_H__

#define uchar unsigned char
#define uint unsigned int

/* 变量定义 */
extern uchar  ge,shi,bai,qian,wan;        //显示变量
extern uchar Light[5];

/* 函数原型 */
extern void Delay_1u(unsigned int times);
extern void Delay_1ms(unsigned int times);
extern void WriteSDA1(void);
extern void WriteSDA0(void);
extern void WriteSCL1(void);
extern void WriteSCL0(void);
extern void ReadSDA(void);
extern void START(void);
extern void STOP(void);
extern void conversion(uint temp_data);
extern void SendACK(uchar ack);
extern uchar BH1750_RecvACK(void);
extern void SEND_0_1(void);
extern void SEND_1_1(void);
extern void BH1750_SendByte(uchar dat);
extern uchar BH1750_RecvByte(void);
extern uchar Read_BH1750(void);
extern void Single_Write_BH1750(uchar REG_Address);
extern void Init_BH1750(void);
extern void Brightness(void);
extern void TM1650_Write(unsigned char	DATA);
extern void Write_DATA(unsigned char add,unsigned char DATA);
extern void num_led(uint num[]);


#endif