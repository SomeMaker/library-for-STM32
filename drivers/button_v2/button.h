/*
 * button.h
 *
 *  Created on: 24 авг. 2020 г.
 *      Author: Alexey
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BUTTON_H_
#define BUTTON_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/**
 * @brief Антидребезг: сколько раз считывать состояние кнопки
 */
#define BUT_ANTI_BOUNCE_CNT     6

/**
 * @brief Период опроса кнопок в мс
 * @note  0  - пользователь сам вызывает функцию Button_Work с нужной периодичностью
 *        >0 - период опроса
 */
#define BUT_POLL_PERIOD         0

/**
 * @brief Количество циклов опроса короткого нажатия.
 * @note  не учитывается антидребезг. BUT_CNT_POLL > 0
 */
#define BUT_CNT_POLL            1

/**
 * @brief Количество циклов опроса долгого нажатия
 * @note  не учитывается антидребезг. BUT_CNT_POLL_L > BUT_CNT_POLL or = 0
 *        Тмс = (BUT_CNT_POLL_L * BUT_POLL_PERIOD),
 *        если BUT_POLL_PERIOD = 0, то умнажаем на период вызова Button_Work
 */
#define BUT_CNT_POLL_L          150

/**
 * @brief Количество циклов опроса очень долгого нажатия
 * @note  не учитывается антидребезг. BUT_CNT_POLL_LL > BUT_CNT_POLL_L or = 0
 *        Тмс = (BUT_CNT_POLL_LL * BUT_POLL_PERIOD),
 *        если BUT_POLL_PERIOD = 0, то умнажаем на период вызова Button_Work
 */
#define BUT_CNT_POLL_LL         400

/**
 * @brief Количество циклов опроса, после которого начнет генерироваться
 *        событие "повтор"
 * @note  0  - событие "повтор" не генерируется
 */
#define BUT_REPEAT_CNT          60

/**
 * @brief Частота генерации события "повтор"
 * @note  Через каждые BUT_REPEAT_DELAY генерируется событие "повтор"
 *        имеет значение при BUT_REPEAT_CNT > 0
 */
#define BUT_REPEAT_DELAY        6

/* Private macros ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/**
 * @brief Перечисление имен кнопок
 */
enum Button_Name{
  BUTTON_MENU = 0,
  BUTTON_SELECT,
  BUTTON_UP,
  BUTTON_DOWN,
  BUTTON_END
};

/**
 * @brief Перечисление идентификаторов функций обратного вызова
 */
enum Button_CallbackIDTypeDef {
    BUTTON_PRESSED_SHORT_CB_ID = 0x00U, /*!< Идентификатор функции обратного вызова по короткому нажатию */
    BUTTON_PRESSED_LONG_CB_ID = 0x01U, /*!< Идентификатор функции обратного вызова по длительному нажатию */
    BUTTON_PRESSED_LONG_LONG_CB_ID = 0x02U, /*!< Идентификатор функции обратного вызова по очень долгому нажатию */
    BUTTON_PRESSED_REPEAT_CB_ID = 0x03U, /*!< Идентификатор функции обратного вызова по повтору (удержание кнопки) */
    BUTTON_RELEASED_SHORT_CB_ID = 0x04U, /*!< Идентификатор функции обратного вызова по отпусканию после короткого нажатия */
    BUTTON_RELEASED_LONG_CB_ID = 0x05U, /*!< Идентификатор функции обратного вызова по отпусканию после долгого нажатия */
    BUTTON_RELEASED_LONG_LONG_CB_ID = 0x06U /*!< Идентификатор функции обратного вызова по отпусканию после очень долгого нажатия*/
};

/**
 * @brief  Определени функции обратного вызова опроса кнопки
 */
typedef  void (*pButton_CallbackTypeDef)(void);  /*!< Указатель на функцию обратного вызова опроса кнопки */


/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Рабочие функции кнопок *****************************************************/
void Button_Init(void);
void Button_Work(void);

/* Управление функциями обратного вызова кнопок ********************************/
uint_fast8_t Button_RegisterCallback(enum Button_Name Name, enum Button_CallbackIDTypeDef CallbackID,
                                        pButton_CallbackTypeDef pCallback);
uint_fast8_t Button_UnRegisterCallback(enum Button_Name Name, enum Button_CallbackIDTypeDef CallbackID);
void Button_ResetCallback(enum Button_Name Name);

#ifdef __cplusplus
}
#endif
#endif /* BUTTON_H_ */
