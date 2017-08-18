/**************************************************
** 功能：车位状态采集、LED车位状态灯控制
** 硬件：红外光电传感器
** 日期：2015-5-8
**************************************************/
#include <ioCC2530.h>
#include "OnBoard.h"

#define uint unsigned int
#define uchar unsigned char

/* 相关IO口定义 */
#define CAR_001 P0_1    //红外传感器
#define LED_G_001 P0_0  //绿灯
#define LED_R_001 P0_7  //红灯

#define CAR_002 P0_2    //红外传感器
#define LED_G_002 P1_2  //绿灯
#define LED_R_002 P1_4  //红灯

#define CAR_003 P0_3    //红外传感器
#define LED_G_003 P1_3  //绿灯
#define LED_R_003 P1_7  //红灯

/* 车位变量 */
uchar S001,S002,S003=3;

/* 函数原型 */
void Delay_ms(uint Time);
void Parking_Status(void) ;  
void init(void);

/* 初始化 */
void init(void)
{
  CAR_001 = 0;
  LED_G_001 = 0;
  LED_R_001 = 0;
  CAR_002 = 0;
  LED_G_002 = 0;
  LED_R_002 = 0;
  CAR_003 = 0;
  LED_G_003 = 0;
  LED_R_003 = 0;

}

void Delay_ms(uint Time)  //延时ms
{
  while(Time--)
  {
      MicroWait(1);
  }
}

/* 车位状态 */
void Parking_Status()   
{
  init(); //初始化
  
  S001 = CAR_001==1?1:0;
  LED_G_001 = CAR_001==0?1:0;
  LED_R_001 = CAR_001==1?1:0;
  
  S002 = CAR_002==1?1:0;
  LED_G_002 = CAR_002==0?1:0;
  LED_R_002 = CAR_002==1?1:0;
  
  S003 = CAR_003==1?1:0;
  LED_G_003 = CAR_003==0?1:0;
  LED_R_003 = CAR_003==1?1:0;
}
