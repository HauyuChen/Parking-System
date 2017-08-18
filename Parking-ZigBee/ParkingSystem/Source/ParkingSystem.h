#ifndef SAMPLEAPP_H
#define SAMPLEAPP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "ZComDef.h"

// These constants are only for example and should be changed to the
// device's needs
#define SAMPLEAPP_ENDPOINT           20

#define SAMPLEAPP_PROFID             0x0F08
#define SAMPLEAPP_DEVICEID           0x0001
#define SAMPLEAPP_DEVICE_VERSION     0
#define SAMPLEAPP_FLAGS              0

#define SAMPLEAPP_MAX_CLUSTERS          3
#define SAMPLEAPP_SEND_DATA_CAR_CLUSTERID  1    //车位状态ID
#define SAMPLEAPP_SEND_DATA_LIGHT_CLUSTERID  2  //亮度ID
#define SAMPLEAPP_LED_CONTROL_CLUSTERID  4      //照明灯控制ID
  
// Send Message Timeout
#define PARKINGSYSTEM_GPRS_TIMEOUT    5000        // Every 5 seconds
#define PARKINGSYSTEM_CAR_TIMEOUT     5000        // Every 5 seconds
#define PARKINGSYSTEM_LIGHT_TIMEOUT   10000       // Every 10 seconds
  
// Application Events (OSAL) - These are bit weighted definitions.
#define PARKINGSYSTEM_CAR_EVT         0x0001
#define PARKINGSYSTEM_LIGHT_EVT       0x0002
#define PARKINGSYSTEM_GPRS_EVT        0x0004
#define PARKINGSYSTEM_SEMDMSG_EVT     0x0008
  
// Group ID for Flash Command
#define SAMPLEAPP_FLASH_GROUP                  0x0001
  
// Flash Command Duration - in milliseconds
#define SAMPLEAPP_FLASH_DURATION               1000

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */

/*
 * Task Initialization for the Generic Application
 */
extern void SampleApp_Init( uint8 task_id );

/*
 * Task Event Processor for the Generic Application
 */
extern UINT16 SampleApp_ProcessEvent( uint8 task_id, uint16 events );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SAMPLEAPP_H */
