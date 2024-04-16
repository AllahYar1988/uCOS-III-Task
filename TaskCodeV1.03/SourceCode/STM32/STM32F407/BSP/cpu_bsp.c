

#define    CPU_BSP_MODULE
#include  <cpu_core.h>

#include  <bsp.h>



#define  CPU_REG_DEM_CR                       (*(CPU_REG32 *)0xE000EDFC)
#define  CPU_REG_DWT_CR                       (*(CPU_REG32 *)0xE0001000)
#define  CPU_REG_DWT_CYCCNT                   (*(CPU_REG32 *)0xE0001004)
#define  CPU_REG_DBGMCU_CR                    (*(CPU_REG32 *)0xE0042004)



#define  CPU_DBGMCU_CR_TRACE_IOEN_MASK                   0x10
#define  CPU_DBGMCU_CR_TRACE_MODE_ASYNC                  0x00
#define  CPU_DBGMCU_CR_TRACE_MODE_SYNC_01                0x40
#define  CPU_DBGMCU_CR_TRACE_MODE_SYNC_02                0x80
#define  CPU_DBGMCU_CR_TRACE_MODE_SYNC_04                0xC0
#define  CPU_DBGMCU_CR_TRACE_MODE_MASK                   0xC0

#define  CPU_BIT_DEM_CR_TRCENA                    DEF_BIT_24

#define  CPU_BIT_DWT_CR_CYCCNTENA                 DEF_BIT_00

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{
    CPU_INT32U  fclk_freq;


    fclk_freq = BSP_CPU_ClkFreq();

    CPU_REG_DEM_CR     |= (CPU_INT32U)CPU_BIT_DEM_CR_TRCENA;    /* Enable Cortex-M4's DWT CYCCNT reg.                   */
    CPU_REG_DWT_CYCCNT  = (CPU_INT32U)0u;
    CPU_REG_DWT_CR     |= (CPU_INT32U)CPU_BIT_DWT_CR_CYCCNTENA;

    CPU_TS_TmrFreqSet((CPU_TS_TMR_FREQ)fclk_freq);
}
#endif



#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    CPU_TS_TMR  ts_tmr_cnts;


    ts_tmr_cnts = (CPU_TS_TMR)CPU_REG_DWT_CYCCNT;

    return (ts_tmr_cnts);
}
#endif



#if (CPU_CFG_TS_32_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS32_to_uSec (CPU_TS32  ts_cnts)
{
    CPU_INT64U  ts_us;
    CPU_INT64U  fclk_freq;


    fclk_freq = BSP_CPU_ClkFreq();
    ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);

    return (ts_us);
}
#endif


#if (CPU_CFG_TS_64_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS64_to_uSec (CPU_TS64  ts_cnts)
{
    CPU_INT64U  ts_us;
    CPU_INT64U  fclk_freq;


    fclk_freq = BSP_CPU_ClkFreq();
    ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);

    return (ts_us);
}
#endif

