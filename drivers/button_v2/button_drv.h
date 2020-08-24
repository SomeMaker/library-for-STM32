/*
 * button_drv.h
 *
 *  Created on: 21 авг. 2020 г.
 *      Author: Alexey
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BUTTON_DRV_H_
#define BUTTON_DRV_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/**
 * @brief Количество кнопок
 */
#define BUTTONn                         (4)

/**
 * @brief Кнопка "Menu"
 */
#define BUTTON_1_PIN                   GPIO_PIN_0
#define BUTTON_1_GPIO_PORT             GPIOB
#define BUTTON_1_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUTTON_1_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOB_CLK_DISABLE()
#define BUTTON_1_ACTIVE_LEVEL          ((uint16_t)0)

/**
 * @brief Кнопка "Select"
 */
#define BUTTON_2_PIN                   GPIO_PIN_1
#define BUTTON_2_GPIO_PORT             GPIOB
#define BUTTON_2_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUTTON_2_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOB_CLK_DISABLE()
#define BUTTON_2_ACTIVE_LEVEL          ((uint16_t)0)

/**
 * @brief Кнопка "Up"
 */
#define BUTTON_3_PIN                   GPIO_PIN_10
#define BUTTON_3_GPIO_PORT             GPIOB
#define BUTTON_3_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUTTON_3_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOB_CLK_DISABLE()
#define BUTTON_3_ACTIVE_LEVEL          ((uint16_t)1)

/**
 * @brief Кнопка "Down"
 */
#define BUTTON_4_PIN                   GPIO_PIN_11
#define BUTTON_4_GPIO_PORT             GPIOB
#define BUTTON_4_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUTTON_4_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOB_CLK_DISABLE()
#define BUTTON_4_ACTIVE_LEVEL          ((uint16_t)1)

/* Private macros ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void        ButtonLowLevel_Init(uint16_t button);
void        ButtonLowLevel_DeInit(uint16_t button);
uint16_t    ButtonLowLevel_GetState(uint16_t button);

#ifdef __cplusplus
}
#endif
#endif /* BUTTON_DRV_H_ */

/*********************************END OF FILE**********************************/
