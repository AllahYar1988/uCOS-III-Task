

#include <cpu.h>

#ifndef  APP_CFG_MODULE_PRESENT
#define  APP_CFG_MODULE_PRESENT




/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define  APP_CFG_TASK_START_PRIO                2u
#define  APP_CFG_TASK_LEDC_PRIO                 3u




#define  APP_CFG_TASK_START_STK_SIZE            256u
#define  APP_CFG_TASK_LEDC_STK_SIZE             512u


                                                 




#define uBuzzer_Pin GPIO_PIN_12
#define uBuzzer_GPIO_Port GPIOE
#define LED1_Pin GPIO_PIN_8
#define LED1_GPIO_Port GPIOD
#define LED2_Pin GPIO_PIN_9
#define LED2_GPIO_Port GPIOD
#define LED3_Pin GPIO_PIN_10
#define LED3_GPIO_Port GPIOD
#define LED4_Pin GPIO_PIN_11
#define LED4_GPIO_Port GPIOD
#define Master_Select_Pin GPIO_PIN_15
#define Master_Select_GPIO_Port GPIOD
#define uMaster_TX_Pin GPIO_PIN_6
#define uMaster_TX_GPIO_Port GPIOC
#define uMaster_RX_Pin GPIO_PIN_7
#define uMaster_RX_GPIO_Port GPIOC
#define  SERIAL_CFG_MAX_NBR_IF                  2u              /* See Note #1.                                         */
#define  SERIAL_CFG_RD_BUF_EN                   DEF_ENABLED     /* See Note #2.                                         */
#define  SERIAL_CFG_WR_BUF_EN                   DEF_ENABLED     /* See Note #3.                                         */
#define  SERIAL_CFG_ARG_CHK_EXT_EN              DEF_DISABLED    /* See Note #4.                                         */
#define  SERIAL_CFG_TX_DESC_NBR                 1u              /* See Note #5.                                         */






#endif
