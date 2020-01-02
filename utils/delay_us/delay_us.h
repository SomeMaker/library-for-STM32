/*!
\file  delay_us.h
\brief микросекундная задержка на базе отладочного таймера DWT
*/
#ifndef __DELAY_US_H__
#define __DELAY_US_H__

#ifdef __cplusplus
  extern "C" {
#endif

/******************************************************************************/
/* Подключение заголовочных файлов используемых модулей */
#include "main.h"

/******************************************************************************/
/* inline func */
__STATIC_INLINE void DWT_Init(void)
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // разрешаем использовать счётчик
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;            // запускаем счётчик
}

__STATIC_INLINE void Delay_us(uint32_t us)
{
  uint32_t start_tic = DWT->CYCCNT;
  uint32_t count_tic = us * (SystemCoreClock / 1000000U);
  while((DWT->CYCCNT - start_tic) < count_tic);
}

#ifdef __cplusplus
}
#endif
#endif //__DELAY_US_H__
/*******************************  END OF FILE  ********************************/
