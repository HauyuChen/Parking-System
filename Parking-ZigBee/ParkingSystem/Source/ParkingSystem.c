/*********************************************************************
** 项目名称：新型停车场引导与防盗系统
** 功能概述：传感器数据采集：车位状态、亮度信息;
             GPRS模块数据传输：TCP传输、短信发送;
             LED控制：车位状态LED灯、LED数码管
** 相关说明：基于例程SampleApp修改
** 修改日期：2015年5月8日
*********************************************************************/
#include "OSAL.h"
#include "ZGlobals.h"
#include "AF.h"
#include "aps_groups.h"
#include "ZDApp.h"

#include "ParkingSystem.h"
#include "SampleAppHw.h"

#include "OnBoard.h"

#include "string.h"

/* HAL */
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"

/* 车位信息 */
#include "ParkingStatus.h"
#include "LightControl.h"

/* GPRS模块相关AT指令定义 */
uchar CGDCONT[27] = {"AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n"};  //配置APN
uchar ETCPIP[11] = {"AT%ETCPIP\r\n"};     //进入TCP/IP功能
uchar IPOPEN[38] = {"AT%IPOPEN=\"TCP\",\"119.146.68.41\",5000\r\n"};  //开启TCP连接
uchar IOMODE[13] = {"AT%IOMODE=0\r\n"};   //设置文本模式
uchar IPSEND[11] = {"AT%IPSEND=\""};      //发送数据，在后面补数据包
uchar IPCLOSE[12] = {"AT%IPCLOSE\r\n"};   //关闭连接
uint8 PHONE[12] = " ";  //手机号码
uchar MSG[80] = "0008A72260A876848F665DF279BB5F008F664F4DFF0C598267095F025E388BF781F475350058"; //短信模版

/* 相关变量定义 */
uint8 LX;                 //亮度值LX
uint8 MAU;                //照明灯手动控制模式
uchar LStatus[1];         //照明灯状态
uint8 PA,PB;              //A区车位数量，B区车位数量
uint PnumA[2]={0};        //LED指示屏：A区车位数量
uint PnumB[2]={0};        //LED指示屏：B区车位数量
uint8 GPRS_FLAG = 0;      //GPRS模块初始化AT指令顺序标志
uint8 SENDMSG_FLAG = 0;   //GPRS模块发送短信AT指令顺序标志

/* LED灯IO口定义 */
#define LED P1_4
#define BLOCKA P0_0
#define BLOCKB P0_7

/*********************************************************************
** 变量定义
*/
const cId_t SampleApp_ClusterList[SAMPLEAPP_MAX_CLUSTERS] =
{
  SAMPLEAPP_SEND_DATA_CAR_CLUSTERID,    //更新车位状态ID
  SAMPLEAPP_SEND_DATA_LIGHT_CLUSTERID,  //更新亮度信息ID
  SAMPLEAPP_LED_CONTROL_CLUSTERID       //照明灯控制ID
}; 

const SimpleDescriptionFormat_t SampleApp_SimpleDesc =
{
  SAMPLEAPP_ENDPOINT,              //  int Endpoint;
  SAMPLEAPP_PROFID,                //  uint16 AppProfId[2];
  SAMPLEAPP_DEVICEID,              //  uint16 AppDeviceId[2];
  SAMPLEAPP_DEVICE_VERSION,        //  int   AppDevVer:4;
  SAMPLEAPP_FLAGS,                 //  int   AppFlags:4;
  SAMPLEAPP_MAX_CLUSTERS,          //  uint8  AppNumInClusters;
  (cId_t *)SampleApp_ClusterList,  //  uint8 *pAppInClusterList;
  SAMPLEAPP_MAX_CLUSTERS,          //  uint8  AppNumInClusters;
  (cId_t *)SampleApp_ClusterList   //  uint8 *pAppInClusterList;
};

endPointDesc_t SampleApp_epDesc;

uint8 SampleApp_TaskID;   // Task ID for internal task/event processing
                          // This variable will be received when
                          // SampleApp_Init() is called.
devStates_t SampleApp_NwkState;

uint8 SampleApp_TransID;  // This is the unique message ID (counter)

afAddrType_t SampleApp_Periodic_DstAddr;
afAddrType_t SampleApp_Flash_DstAddr;
afAddrType_t Point_To_Point_DstAddr;

aps_Group_t SampleApp_Group;

uint8 SampleAppFlashCounter = 0;


/*********************************************************************
** 函数原型
*/
void SampleApp_MessageMSGCB( afIncomingMSGPacket_t *pckt ); //消息处理事件
static void UART_CallBack (uint8 port,uint8 event);         //串口回调函数
void SampleApp_SendPointToPointMessage_Car(void);           //车位数据发送
void SampleApp_SendPointToPointMessage_Light(void);         //亮度数据发送
void LightYN(uint n);                                       //照明灯控制

/*********************************************************************
** 函数定义
*/
void SampleApp_Init( uint8 task_id )
{
  SampleApp_TaskID = task_id;
  SampleApp_NwkState = DEV_INIT;
  SampleApp_TransID = 0;

  /* 串口配置结构体 */
  halUARTCfg_t uartConfig;
  /* 串口初始化 */
  uartConfig.configured = TRUE;
  uartConfig.baudRate   = HAL_UART_BR_9600;  //波特率设置为9600
  uartConfig.flowControl  = FALSE;
  uartConfig.flowControlThreshold = 64;
  uartConfig.rx.maxBufSize        = 128;
  uartConfig.tx.maxBufSize        = 128;
  uartConfig.idleTimeout          = 6;
  uartConfig.intEnable            = TRUE;
  uartConfig.callBackFunc = UART_CallBack;    //串口回调函数UART_CallBack
  HalUARTOpen(0,&uartConfig);                 //启动串口
  
  /* LED灯初始化 */
  P1DIR |= 0x16;
  LED = 0;
  BLOCKA = 0;
  BLOCKB = 0;

#if defined ( BUILD_ALL_DEVICES )
  if ( readCoordinatorJumper() )
    zgDeviceLogicalType = ZG_DEVICETYPE_COORDINATOR;
  else
    zgDeviceLogicalType = ZG_DEVICETYPE_ROUTER;
#endif // BUILD_ALL_DEVICES

#if defined ( HOLD_AUTO_START )
  ZDOInitDevice(0);
#endif

  /* 组播通讯定义 */
  SampleApp_Flash_DstAddr.addrMode = (afAddrMode_t)afAddrGroup;
  SampleApp_Flash_DstAddr.endPoint = SAMPLEAPP_ENDPOINT;
  SampleApp_Flash_DstAddr.addr.shortAddr = SAMPLEAPP_FLASH_GROUP;

  /* 点对点通讯定义 */
  Point_To_Point_DstAddr.addrMode = (afAddrMode_t)Addr16Bit; //点播
  Point_To_Point_DstAddr.endPoint = SAMPLEAPP_ENDPOINT;
  Point_To_Point_DstAddr.addr.shortAddr = 0x0000;           //发给协调器
  
  SampleApp_epDesc.endPoint = SAMPLEAPP_ENDPOINT;
  SampleApp_epDesc.task_id = &SampleApp_TaskID;
  SampleApp_epDesc.simpleDesc
            = (SimpleDescriptionFormat_t *)&SampleApp_SimpleDesc;
  SampleApp_epDesc.latencyReq = noLatencyReqs;

  afRegister( &SampleApp_epDesc );

  RegisterForKeys( SampleApp_TaskID );

  SampleApp_Group.ID = 0x0001;
  osal_memcpy( SampleApp_Group.name, "Group 1", 7  );
  aps_AddGroup( SAMPLEAPP_ENDPOINT, &SampleApp_Group );
}

uint16 SampleApp_ProcessEvent( uint8 task_id, uint16 events )
{
  afIncomingMSGPacket_t *MSGpkt;
  (void)task_id;  // Intentionally unreferenced parameter

  /* 系统事件 */
  if ( events & SYS_EVENT_MSG )
  {
    MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( SampleApp_TaskID );
    while ( MSGpkt )
    {
      switch ( MSGpkt->hdr.event )
      {
        /* 接收到空中消息 */
        case AF_INCOMING_MSG_CMD: 
          SampleApp_MessageMSGCB( MSGpkt );   //消息处理事件
          break;
        
        /* 设备状态改变 */
        case ZDO_STATE_CHANGE:
          SampleApp_NwkState = (devStates_t)(MSGpkt->hdr.status);
          
          if ( SampleApp_NwkState == DEV_ZB_COORD ) //如果是ZigBee协调器
          {
            osal_start_timerEx( SampleApp_TaskID,
                              PARKINGSYSTEM_GPRS_EVT,
                              PARKINGSYSTEM_GPRS_TIMEOUT );
          }
          else if ( (SampleApp_NwkState == DEV_ROUTER)
              || (SampleApp_NwkState == DEV_END_DEVICE) ) //如果是ZigBee路由器或者ZigBee终端
          {
            //执行车位检测功能（根据节点功能选择是否编译）
            osal_start_timerEx( SampleApp_TaskID,
                              PARKINGSYSTEM_CAR_EVT,
                              PARKINGSYSTEM_GPRS_TIMEOUT );
            
            //执行亮度检测功能（根据节点功能选择是否编译）
            osal_start_timerEx( SampleApp_TaskID,
                              PARKINGSYSTEM_LIGHT_EVT,
                              PARKINGSYSTEM_LIGHT_TIMEOUT );
          }
          else
          {
            // Device is no longer in the network
          }
          break;

        default:
          break;
      }

      osal_msg_deallocate( (uint8 *)MSGpkt );

      MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( SampleApp_TaskID );
    }
    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);
  }
  
  /* 车位相关事件 */
  if ( events & PARKINGSYSTEM_CAR_EVT )
  {  
    Parking_Status();  //车位状态检测
    
    SampleApp_SendPointToPointMessage_Car();  //发送车位状态
    
    //每5秒定时监测车位并发送数据
    osal_start_timerEx( SampleApp_TaskID, PARKINGSYSTEM_CAR_EVT,
        (PARKINGSYSTEM_CAR_TIMEOUT + (osal_rand() & 0x00FF)) );
    // return unprocessed events
    return (events ^ PARKINGSYSTEM_CAR_EVT);
  }
  
  /* 亮度相关事件 */
  if ( events & PARKINGSYSTEM_LIGHT_EVT )
  {  
    Brightness();  //亮度信息监测

    SampleApp_SendPointToPointMessage_Light();  //发送亮度信息
    
    //每10秒定时监测亮度信息并发送数据
    osal_start_timerEx( SampleApp_TaskID, PARKINGSYSTEM_LIGHT_EVT,
        (PARKINGSYSTEM_LIGHT_TIMEOUT + (osal_rand() & 0x00FF)) );
    // return unprocessed events
    return (events ^ PARKINGSYSTEM_LIGHT_EVT);
  }
  
  /* GPRS模块事件：初始化TCP连接 */
  if ( events & PARKINGSYSTEM_GPRS_EVT )
  {  
    /* 每5秒发送一条AT指令 */
    switch(GPRS_FLAG)
    {
      case 0:         //AT指令（关闭TCP链接）：AT%IPCLOSE
        HalUARTWrite(0,IPCLOSE,sizeof(IPCLOSE));
        GPRS_FLAG++;  //AT指令执行序号加1，下次执行下一条AT指令
        osal_start_timerEx( SampleApp_TaskID, PARKINGSYSTEM_GPRS_EVT,
        (PARKINGSYSTEM_GPRS_TIMEOUT + (osal_rand() & 0x00FF)) );
      break;
      case 1:         //AT指令（配置APN）：AT+CGDCONT=1,”IP”,”CMNET”
        HalUARTWrite(0,CGDCONT,sizeof(CGDCONT));
        GPRS_FLAG++;  //AT指令执行序号加1，下次执行下一条AT指令
        osal_start_timerEx( SampleApp_TaskID, PARKINGSYSTEM_GPRS_EVT,
        (PARKINGSYSTEM_GPRS_TIMEOUT + (osal_rand() & 0x00FF)) );
      break;
      case 2:         //AT指令（进入tcpip 功能）：AT%ETCPIP
        HalUARTWrite(0,ETCPIP,sizeof(ETCPIP));
        GPRS_FLAG++;  //AT指令执行序号加1，下次执行下一条AT指令
        osal_start_timerEx( SampleApp_TaskID, PARKINGSYSTEM_GPRS_EVT,
        (PARKINGSYSTEM_GPRS_TIMEOUT + (osal_rand() & 0x00FF)) );
      break;
      case 3:         //AT指令（打开一条tcp链接）：AT%IPOPEN=”TCP”,”119.146.68.41”,5000
        HalUARTWrite(0,IPOPEN,sizeof(IPOPEN));
        GPRS_FLAG++;  //AT指令执行序号加1，下次执行下一条AT指令
        osal_start_timerEx( SampleApp_TaskID, PARKINGSYSTEM_GPRS_EVT,
        (PARKINGSYSTEM_GPRS_TIMEOUT + (osal_rand() & 0x00FF)) );
      break;
      case 4:         //AT指令（IOMODE设置）：AT%IOMODE=0
        HalUARTWrite(0,IOMODE,sizeof(IOMODE));
        GPRS_FLAG++;  //AT指令执行序号加1，下次执行下一条AT指令
        osal_start_timerEx( SampleApp_TaskID, PARKINGSYSTEM_GPRS_EVT,
        (PARKINGSYSTEM_GPRS_TIMEOUT + (osal_rand() & 0x00FF)) );
      break;
      case 5:         //AT指令（发送数据到tcp 对端）：AT%IPSEND=”Zigbee”
        HalUARTWrite(0,IPSEND,sizeof(IPSEND));
        HalUARTWrite(0,"Zigbee",6);   //标志
        HalUARTWrite(0,"\"\r\n",3);
        GPRS_FLAG++;  //AT指令执行序号加1，下次执行下一条AT指令
        osal_start_timerEx( SampleApp_TaskID, PARKINGSYSTEM_GPRS_EVT,
        (PARKINGSYSTEM_GPRS_TIMEOUT + (osal_rand() & 0x00FF)) );
      break;
      case 6:         //AT指令（发送数据到tcp 对端）：AT%IPSEND=”5226”
        HalUARTWrite(0,IPSEND,sizeof(IPSEND));
        HalUARTWrite(0,"5226",4);   
        HalUARTWrite(0,"\"\r\n",3);
        GPRS_FLAG++;  //AT指令执行序号加1，下次执行下一条AT指令
        osal_start_timerEx( SampleApp_TaskID, PARKINGSYSTEM_GPRS_EVT,
        (PARKINGSYSTEM_GPRS_TIMEOUT + (osal_rand() & 0x00FF)) );
      break;
      case 7:         //AT指令（选择模式）：AT+CMGF=0
        HalUARTWrite(0,"AT+CMGF=0\r\n",11);
        GPRS_FLAG=0;  //AT指令执行序号置0
      break;
      default:
        osal_start_timerEx( SampleApp_TaskID, PARKINGSYSTEM_GPRS_EVT,
        (PARKINGSYSTEM_GPRS_TIMEOUT + (osal_rand() & 0x00FF)) );
      break;
    }
    // return unprocessed events
    return (events ^ PARKINGSYSTEM_GPRS_EVT);
  }

  /* GPRS模块事件：发送短信 */
  if ( events & PARKINGSYSTEM_SEMDMSG_EVT )
  { 
    /* 每5秒发送一条AT指令 */
    switch(SENDMSG_FLAG)
    {
      
      case 0:             //AT指令（设置短信长度）：AT+CMGS=49
        HalUARTWrite(0,"AT+CMGS=49\r\n",12);
        SENDMSG_FLAG++;   //AT指令执行序号加1，下次执行下一条AT指令
        osal_start_timerEx( SampleApp_TaskID, PARKINGSYSTEM_SEMDMSG_EVT,
        (200 + (osal_rand() & 0x00FF)) );
      break;
      case 1:             //AT指令（短信内容）：0011000D9168+PHONE+MSG
        HalUARTWrite(0,"0011000D9168",12);
        HalUARTWrite(0,PHONE,12);     //用户手机号码
        strcat(MSG,"\x1A");           //短信内容
        HalUARTWrite(0,MSG,sizeof(MSG));
        SENDMSG_FLAG=0;   //AT指令执行序号置0
      break;
      default:
        osal_start_timerEx( SampleApp_TaskID, PARKINGSYSTEM_SEMDMSG_EVT,
        (PARKINGSYSTEM_GPRS_TIMEOUT + (osal_rand() & 0x00FF)) );
      break;
    } 
    // return unprocessed events
    return (events ^ PARKINGSYSTEM_SEMDMSG_EVT);
  }

  // Discard unknown events
  return 0;
}

void SampleApp_MessageMSGCB( afIncomingMSGPacket_t *pkt )
{
  switch ( pkt->clusterId )
  {
    /* 车位信息更新 */
    case SAMPLEAPP_SEND_DATA_CAR_CLUSTERID: 
      HalUARTWrite(0,IPSEND,sizeof(IPSEND));  //AT指令，向TCP服务器发送数据
      if(pkt->cmd.Data[0]=='A')               //如果是A区
      {
        //因为本项目在每个区域只选取三个车位布置传感器，故其他车位由0或1模拟
        HalUARTWrite(0,"A1",2);               //第一个字符指示是哪个区域，这里是A区
        HalUARTWrite(0,&pkt->cmd.Data[1],2);  //A002、A003车位
        HalUARTWrite(0,"010000000000000",15); //其他车位用假数据模拟
        HalUARTWrite(0,&pkt->cmd.Data[3],1);  //A019车位
        HalUARTWrite(0,"000010000001",12);    //其他车位用假数据模拟
      }
      else if(pkt->cmd.Data[0]=='B')          //如果是B区
      {
        //因为本项目在每个区域只选取三个车位布置传感器，故其他车位由0或1模拟
        HalUARTWrite(0,"B10000100000000110",18);   //第一个字符指示是哪个区域，这里是B区
        HalUARTWrite(0,&pkt->cmd.Data[1],1);       //B018车位
        HalUARTWrite(0,"0000",4);                  //其他车位用假数据模拟
        HalUARTWrite(0,&pkt->cmd.Data[2],1);       //B023车位
        HalUARTWrite(0,"1000000001",10);           //其他车位用假数据模拟
        HalUARTWrite(0,&pkt->cmd.Data[3],1);       //B034车位
        HalUARTWrite(0,"00001000",8);              //其他车位用假数据模拟
      }
      HalUARTWrite(0,"\"\r\n",3);
     break;
   
   /* 亮度信息更新 */
   case SAMPLEAPP_SEND_DATA_LIGHT_CLUSTERID:
    //LStatus[0] = LED+48;
    //向服务器发送亮度信息、照明灯状态，如LIGHT00079,Status:1
    HalUARTWrite(0,IPSEND,sizeof(IPSEND));  //AT指令，向TCP服务器发送数据
    HalUARTWrite(0,"ZBLIGHT",7);   
    HalUARTWrite(0,&pkt->cmd.Data[0],5);    //亮度，LX值
    HalUARTWrite(0,"LX,",3);
    HalUARTWrite(0,"Status:",7);
    HalUARTWrite(0,LStatus,1);              //照明灯状态
    HalUARTWrite(0,"\"\r\n",3);
    
    /* 非手动模式下，根据LX值自动控制照明灯 */
    if(MAU==0)
    {
      LX = (pkt->cmd.Data[2]-48)*100+(pkt->cmd.Data[3]-48)*10+(pkt->cmd.Data[4]-48);
      if(LX>=10)  //亮度大于10LX时，不开灯
      {
        LightYN(0);
        LStatus[0] = '0';
      }
      else
      {
        LightYN(1);
        LStatus[0] = '1';
      }
    } 
   break;
   
   /* 照明灯控制 */
   case SAMPLEAPP_LED_CONTROL_CLUSTERID:
    if(pkt->cmd.Data[0]==1)
      LED = 1;  //开灯
    else
      LED = 0;  //关灯
   break;
  }
}

/******************************************************
** 函数：终端发送数据
** 簇ID：SAMPLEAPP_SEND_DATA_CAR_CLUSTERID
** 功能：将车位状态信息发送给ZigBee协调器
*/
void SampleApp_SendPointToPointMessage_Car( void )
{
  uint8 PD[4];
  /* 车位区域 */
  //PD[0]=0+65;   //A区车位
  PD[0]=0+66;     //B区车位
  /* 该区域车位状态(只模拟其中三个车位) */
  PD[1]=S001+48;
  PD[2]=S002+48;
  PD[3]=S003+48;
  
  if ( AF_DataRequest( &Point_To_Point_DstAddr,
                       &SampleApp_epDesc,
                       SAMPLEAPP_SEND_DATA_CAR_CLUSTERID,
                       4,
                       PD,  //车位数据
                       &SampleApp_TransID,
                       AF_DISCV_ROUTE,
                       AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
  {
  }
  else
  {
    // Error occurred in request to send.
  }
}

/******************************************************
** 函数：终端发送数据
** 簇ID：SAMPLEAPP_SEND_DATA_LIGHT_CLUSTERID
** 功能：将亮度信息发送给ZigBee协调器
*/
void SampleApp_SendPointToPointMessage_Light( void )
{
  if ( AF_DataRequest( &Point_To_Point_DstAddr,
                       &SampleApp_epDesc,
                       SAMPLEAPP_SEND_DATA_LIGHT_CLUSTERID,
                       5,
                       Light, //亮度数据
                       &SampleApp_TransID,
                       AF_DISCV_ROUTE,
                       AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
  {
  }
  else
  {
    // Error occurred in request to send.
  }
}

/*************************************
** 函数：灯光远程控制
** 功能：控制停车场内照明灯的开灭
*/
void LightYN(uint n)
{
  uint8 L[1];
  L[0] = n;

  if ( AF_DataRequest( &SampleApp_Flash_DstAddr,
                       &SampleApp_epDesc,
                       SAMPLEAPP_LED_CONTROL_CLUSTERID,
                       1,
                       L,   //开关灯指令
                       &SampleApp_TransID,
                       AF_DISCV_ROUTE,
                       AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
  {
  }
  else
  {
    // Error occurred in request to send.
  }
}

/******************************************************
** 函数：串口回调函数
** 功能：对串口发送过来的数据进行处理
*/
static void UART_CallBack (uint8 port,uint8 event)
{
  uint8 usartbuf[30] = " ";
  HalUARTRead(0,usartbuf,30);   //从串口中读取数据，存放在usartbuf中
  
  if(osal_memcmp(&usartbuf[11],"MANUAL",6))   //如果接收到MANUAL
  {
    MAU = 1;      //手动模式
  }
  else if(osal_memcmp(&usartbuf[12],"SELFCONTROL",11))   //如果接收到SELFCONTROL
  {
    MAU = 0;      //自动模式
  }
  else if(osal_memcmp(&usartbuf[11],"LIGHTON",7))        //如果接收到LIGHTON
  {
    LightYN(1);   //开灯
    LStatus[0] = '1';
  }
  else if(osal_memcmp(&usartbuf[11],"LIGHTOFF",8))       //如果接收到LIGHTOFF
  {
    LightYN(0);   //关灯
    LStatus[0] = '0';
  }
  else if(osal_memcmp(&usartbuf[11],"PA",2))   //如果接收到PA
  {
    PA = (usartbuf[13]-48)*10 + (usartbuf[14]-48);  //计算A区车位数量
    if(PA != 0)
    {
      BLOCKA = 1;         //A区车位空闲灯亮
      PnumA[0] = usartbuf[13]-48;
      PnumA[1] = usartbuf[14]-48;
      num_led(PnumA);     //更新车位数量LED数码管
    }
  }
  else if(osal_memcmp(&usartbuf[11],"PB",2))   //如果接收到PB
  {
    PB = (usartbuf[13]-48)*10 + (usartbuf[14]-48);  //计算B区车位数量
    if(PB != 0)
    {
      BLOCKA = 1;         //B区车位空闲灯亮
      PnumB[0] = usartbuf[13]-48;
      PnumB[1] = usartbuf[14]-48;
      //num_led(PnumB);   //更新车位数量LED数码管
    }
  }
  else if(osal_memcmp(&usartbuf[12],"SENDMSG",7))   //如果接收到SENDMSG+手机号  
  {
    /* 将接收到手机号进行处理，如原始号码15767977442，转换后：5167977744F2 */
    PHONE[0]=usartbuf[20];
    PHONE[1]=usartbuf[19];
    PHONE[2]=usartbuf[22];
    PHONE[3]=usartbuf[21];
    PHONE[4]=usartbuf[24];
    PHONE[5]=usartbuf[23];
    PHONE[6]=usartbuf[26];
    PHONE[7]=usartbuf[25];
    PHONE[8]=usartbuf[28];
    PHONE[9]=usartbuf[27];
    PHONE[10]= 'F';
    PHONE[11]=usartbuf[29];
    
    /* 发送短信给目标手机号 */
    osal_start_timerEx( SampleApp_TaskID, PARKINGSYSTEM_SEMDMSG_EVT,
        (0 + (osal_rand() & 0x00FF)) );              
  }
}
