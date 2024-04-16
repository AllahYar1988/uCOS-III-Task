
#include  <stm32f4xx_hal.h>

#include  <cpu.h>
#include  <os.h>
#include  <os_app_hooks.h>

#include  <app_cfg.h>
#include  <bsp.h>



/**************************************************************/
static  OS_TCB       AppTaskStartTCB;
static  CPU_STK      AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

static  OS_TCB       FirstLEDControlTCB;
static  CPU_STK      FirstLEDControlStk[APP_CFG_TASK_LEDC_STK_SIZE];

static  OS_TCB        SecondLEDControlTCB;
static  CPU_STK      SecondLEDControlStk[APP_CFG_TASK_LEDC_STK_SIZE];


/******************************************************/
static  void  AppTaskStart (void  *p_arg);
static  void  FirstLEDControl   (void  *p_arg);                                       
static  void  SecondLEDControl   (void  *p_arg);                   


/*************** Main Function ***********************/
int main(void)
{
    OS_ERR   err;


    HAL_Init();                                                                                         
                 
    BSP_IntDisAll();                                            
    OSInit(&err);                                               
    App_OS_SetAllHooks();

    OSTaskCreate(&AppTaskStartTCB,                             
                 "App Task Start",
                  AppTaskStart,
                  0u,
                  APP_CFG_TASK_START_PRIO,
                 &AppTaskStartStk[0u],
                  AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],
                  APP_CFG_TASK_START_STK_SIZE,
                  0u,
                  0u,
                  0u,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &err);

    OSStart(&err);                                             

    while (1) {                                           
        
    }
}


/*************** App Start Task ***********************/

static  void  AppTaskStart (void *p_arg)
{
    OS_ERR      err;

   (void)p_arg;

    BSP_Init();                                                
    CPU_Init();                                                


                                                               
    OSTaskCreate(&FirstLEDControlTCB,
                 "First LED Control",
                  FirstLEDControl,
                  0,
                  APP_CFG_TASK_LEDC_PRIO,
                 &FirstLEDControlStk[0],
                  FirstLEDControlStk[APP_CFG_TASK_LEDC_STK_SIZE / 10u],
                  APP_CFG_TASK_LEDC_STK_SIZE,
                  0u,
                  0u,
                  0,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 &err);

    OSTaskCreate(&SecondLEDControlTCB,
                 "Second LED Control",
                  SecondLEDControl,
                  0,
                  APP_CFG_TASK_LEDC_PRIO,
                 &SecondLEDControlStk[0],
                  SecondLEDControlStk[APP_CFG_TASK_LEDC_STK_SIZE / 10u],
                  APP_CFG_TASK_LEDC_STK_SIZE,
                  0u,
                  0u,
                  0,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 &err);

   

    while (1) {                                          

            HAL_GPIO_TogglePin(LED4_GPIO_Port,LED4_Pin);
             OSTimeDlyHMSM(0u, 0u, 0u, 100u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
    }
}




/* First LED Control Task*/

void  FirstLEDControl (void  *p_arg)
{
      OS_ERR       err;


    while (1) 
       {
      
         HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
         OSTimeDlyHMSM(0u, 0u, 0u, 200u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err); 
        }
}


/* Second LED Control Task*/

void  SecondLEDControl (void  *p_arg)
{
  OS_ERR       err;


    while (1) {
    
        HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);


        OSTimeDlyHMSM(0u, 0u, 0u, 500u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);

 
    }
}

