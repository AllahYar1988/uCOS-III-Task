

#define   BSP_MODULE
#include  <bsp.h>
#include  <bsp_os.h>

#include  <stm32f4xx_hal.h>



#define  BSP_BIT_RCC_PLLCFGR_PLLM               25u
#define  BSP_BIT_RCC_PLLCFGR_PLLN              336u
#define  BSP_BIT_RCC_PLLCFGR_PLLP                2u
#define  BSP_BIT_RCC_PLLCFGR_PLLQ                7u


#define  BSP_GPIOG_LED1                        DEF_BIT_06
#define  BSP_GPIOG_LED2                        DEF_BIT_08
#define  BSP_GPIOI_LED3                        DEF_BIT_09
#define  BSP_GPIOC_LED4                        DEF_BIT_07


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






#define  BSP_REG_DEM_CR                       (*(CPU_REG32 *)0xE000EDFC)
#define  BSP_REG_DWT_CR                       (*(CPU_REG32 *)0xE0001000)
#define  BSP_REG_DWT_CYCCNT                   (*(CPU_REG32 *)0xE0001004)
#define  BSP_REG_DBGMCU_CR                    (*(CPU_REG32 *)0xE0042004)



#define  BSP_DBGMCU_CR_TRACE_IOEN_MASK                   0x10
#define  BSP_DBGMCU_CR_TRACE_MODE_ASYNC                  0x00
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_01                0x40
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_02                0x80
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_04                0xC0
#define  BSP_DBGMCU_CR_TRACE_MODE_MASK                   0xC0

#define  BSP_BIT_DEM_CR_TRCENA                    DEF_BIT_24

#define  BSP_BIT_DWT_CR_CYCCNTENA                 DEF_BIT_00



static  void  BSP_GPIO_Init        (void);



void  BSP_Init (void)
{
    RCC_OscInitTypeDef  RCC_OscInitStruct;
    RCC_ClkInitTypeDef  RCC_ClkInitStruct;

    BSP_IntInit();

    HAL_RCC_DeInit();

    __HAL_RCC_PWR_CLK_ENABLE();                                 /* Enable Power Control clock.                          */
                                                                /* See Note 3.                                          */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

                                                                /* PLLCLK    = HSE * (PLLN / PLLM)      = 336MHz.       */
                                                                /* SYSCLK    = PLLCLK / PLLP            = 168MHz.       */
                                                                /* OTG_FSCLK = PLLCLK / PLLQ            =  48MHz.       */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM       = BSP_BIT_RCC_PLLCFGR_PLLM;
    RCC_OscInitStruct.PLL.PLLN       = BSP_BIT_RCC_PLLCFGR_PLLN;
    RCC_OscInitStruct.PLL.PLLP       = BSP_BIT_RCC_PLLCFGR_PLLP;
    RCC_OscInitStruct.PLL.PLLQ       = BSP_BIT_RCC_PLLCFGR_PLLQ;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);


    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_SYSCLK |
                                       RCC_CLOCKTYPE_HCLK   |
                                       RCC_CLOCKTYPE_PCLK1  |
                                       RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;          /* HCLK    = AHBCLK  = PLL / AHBPRES(1) = 168MHz.       */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;           /* APB1CLK = AHBCLK  / APB1DIV(4)       = 42MHz (max).  */
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;           /* APB2CLK = AHBCLK  / APB2DIV(2)       = 84MHz.        */
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

                                                                /* STM32F405x/407x/415x/417x Revision Z devices: ...... */
    if (HAL_GetREVID() == 0x1001)                               /* ....prefetch is supported                            */
    {
      __HAL_FLASH_PREFETCH_BUFFER_ENABLE();                     /* Enable the Flash prefetch                            */
    }

    BSP_GPIO_Init();                                             /* Init LEDs.                                           */

#ifdef TRACE_EN                                                 /* See project / compiler preprocessor options.         */
    BSP_CPU_REG_DBGMCU_CR |=  BSP_DBGMCU_CR_TRACE_IOEN_MASK;    /* Enable tracing (see Note #2).                        */
    BSP_CPU_REG_DBGMCU_CR &= ~BSP_DBGMCU_CR_TRACE_MODE_MASK;    /* Clr trace mode sel bits.                             */
    BSP_CPU_REG_DBGMCU_CR |=  BSP_DBGMCU_CR_TRACE_MODE_SYNC_04; /* Cfg trace mode to synch 4-bit.                       */
#endif
}



CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    CPU_INT32U  hclk_freq;


    hclk_freq = HAL_RCC_GetHCLKFreq();
    return (hclk_freq);
}




HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    HAL_NVIC_SetPriorityGrouping(0);

    if (OSRunning > 0u) {                                       /*Check if multi-tasking has started.                   */
        BSP_Tick_Init();
    }

    return (HAL_OK);
}


void BSP_Tick_Init (void)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;

    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */

#if (OS_VERSION >= 30000u)
    cnts  = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;       /* Determine nbr SysTick increments.                    */
#else
    cnts  = cpu_clk_freq / (CPU_INT32U)OS_TICKS_PER_SEC;        /* Determine nbr SysTick increments.                    */
#endif

    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).              */
}




static void  BSP_GPIO_Init()
{
     GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(uBuzzer_GPIO_Port, uBuzzer_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LED1_Pin|LED2_Pin|LED3_Pin|LED4_Pin
                          |Master_Select_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : uBuzzer_Pin */
  GPIO_InitStruct.Pin = uBuzzer_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = 0;
  HAL_GPIO_Init(uBuzzer_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED1_Pin LED2_Pin LED3_Pin LED4_Pin
                           Master_Select_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|LED3_Pin|LED4_Pin
                          |Master_Select_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = 0;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}







