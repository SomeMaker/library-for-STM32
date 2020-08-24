/*
 * button_drv.c
 *
 *  Created on: 21 авг. 2020 г.
 *      Author: Alexey
 */

/* Includes ------------------------------------------------------------------*/
#include "button_drv.h"

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {
#if (BUTTONn < 1)
#error BUTTONn must be > 0
#endif
        BUTTON_1_GPIO_PORT,
#if (BUTTONn > 1)
        BUTTON_2_GPIO_PORT,
#endif
#if (BUTTONn > 2)
        BUTTON_3_GPIO_PORT,
#endif
#if (BUTTONn > 3)
        BUTTON_4_GPIO_PORT,
#endif
#if (BUTTONn > 4)
#error Add BUTTON_x_GPIO_PORT
#endif
};

/* Private constants ---------------------------------------------------------*/
const uint16_t BUTTON_PIN[BUTTONn] = {
#if (BUTTONn < 1)
#error BUTTONn must be > 0
#endif
        BUTTON_1_PIN,
#if (BUTTONn > 1)
        BUTTON_2_PIN,
#endif
#if (BUTTONn > 2)
        BUTTON_3_PIN,
#endif
#if (BUTTONn > 3)
        BUTTON_4_PIN,
#endif
#if (BUTTONn > 4)
#error Add BUTTON_x_PIN
#endif
};

const uint16_t BUTTON_ACTIVE_LEVEL[BUTTONn] = {
#if (BUTTONn < 1)
#error BUTTONn must be > 0
#endif
        BUTTON_1_ACTIVE_LEVEL,
#if (BUTTONn > 1)
        BUTTON_2_ACTIVE_LEVEL,
#endif
#if (BUTTONn > 2)
        BUTTON_3_ACTIVE_LEVEL,
#endif
#if (BUTTONn > 3)
        BUTTON_4_ACTIVE_LEVEL,
#endif
#if (BUTTONn > 4)
#error Add BUTTON_x_ACTIVE_LEVEL
#endif
};

/* Private macros ------------------------------------------------------------*/
/**
 * @brief Включение тактирования переферии
 */
#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)    do { if((__INDEX__) == 0) BUTTON_1_GPIO_CLK_ENABLE(); else\
                                                   if((__INDEX__) == 1) BUTTON_2_GPIO_CLK_ENABLE(); else\
                                                   if((__INDEX__) == 2) BUTTON_3_GPIO_CLK_ENABLE(); else\
                                                   BUTTON_4_GPIO_CLK_ENABLE(); } while(0)

/**
 * @brief Отключение тактирования переферии
 */
#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__)    (((__INDEX__) == 0) ? BUTTON_1_GPIO_CLK_DISABLE() :\
                                                ((__INDEX__) == 1) ? BUTTON_2_GPIO_CLK_DISABLE() :\
                                                ((__INDEX__) == 2) ? BUTTON_3_GPIO_CLK_DISABLE() :\
                                                        BUTTON_4_GPIO_CLK_DISABLE())

/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Конфигурирование кнопки
 * @param  Button: кнопка
 * @retval None
 */
void ButtonLowLevel_Init(uint16_t Button)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    if(Button >= BUTTONn) return;
    /* Enable the BUTTON clock */
    BUTTONx_GPIO_CLK_ENABLE(Button);

    /* Configure Button pin as input */
    GPIO_InitStruct.Pin = BUTTON_PIN[Button];
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    if (BUTTON_ACTIVE_LEVEL[Button])
    {
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    } else
    {
        GPIO_InitStruct.Pull = GPIO_PULLUP;
    }
    HAL_GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStruct);
}

/**
 * @brief  Деинициализация кнопки
 * @param  Button: кнопка
 * @retval None
 */
void ButtonLowLevel_DeInit(uint16_t Button)
{
    HAL_GPIO_DeInit(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

/**
  * @brief  Возвращает состояние кнопки
  * @param  Button: опрашиваемая кнопка
  * @retval 0 - кнопка НЕ нажата,
  *         1 - кнопка нажата
  */
uint16_t ButtonLowLevel_GetState(uint16_t button)
{
    return (((BUTTON_PORT[button]->IDR & BUTTON_PIN[button]) && BUTTON_PIN[button])
                        == BUTTON_ACTIVE_LEVEL[button]);
}

/*********************************END OF FILE**********************************/
