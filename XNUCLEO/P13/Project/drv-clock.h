/*drv-clk.h
* configura o clock do STM32Fxx para 72MHz com XTAL *
* baseados nos exemplos da std library da ST        *
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_CLOCK_H
#define __DRV_CLOCK_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line corresponding to the desired System clock (SYSCLK)
   frequency (after reset the HSI is used as SYSCLK source) */
//#define SYSCLK_HSE
//#define SYSCLK_FREQ_24MHz

#if !defined STM32F10X_LD_VL && !defined STM32F10X_MD_VL && !defined STM32F10X_HD_VL
  //#define SYSCLK_FREQ_36MHz
  //#define SYSCLK_FREQ_48MHz
  //#define SYSCLK_FREQ_56MHz
  #define SYSCLK_FREQ_72MHz
#endif

void SetSysClock(void);
#ifdef SYSCLK_HSE
  void SetSysClockToHSE(void);
#elif defined SYSCLK_FREQ_24MHz
  void SetSysClockTo24(void);
#elif defined SYSCLK_FREQ_36MHz
  void SetSysClockTo36(void);
#elif defined SYSCLK_FREQ_48MHz
  void SetSysClockTo48(void);
#elif defined SYSCLK_FREQ_56MHz
  void SetSysClockTo56(void);  
#elif defined SYSCLK_FREQ_72MHz
  void SetSysClockTo72(void);
#endif

#endif  /*__DRV_CLOCK_H */
