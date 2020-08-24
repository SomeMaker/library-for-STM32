/*
 * button.c
 *
 *  Created on: 24 авг. 2020 г.
 *      Author: Alexey
 */

/* Includes ------------------------------------------------------------------*/
#include "button_drv.h"
#include "button.h"

/* Проверка конфигурации модуля button.h*/
#if (BUT_ANTI_BOUNCE_CNT < 1)
#error BUT_ANTI_BOUNCE_CNT must be > 0
#endif
#if (BUT_CNT_POLL < 1)
#error BUT_CNT_POLL must be > 0
#endif
#if ((BUT_CNT_POLL_L != 0) && (BUT_CNT_POLL_L <= BUT_CNT_POLL))
#error BUT_CNT_POLL_L must be > BUT_CNT_POLL, or = 0
#endif
#if ((BUT_CNT_POLL_LL != 0) && (BUT_CNT_POLL_LL <= BUT_CNT_POLL_L))
#error BUT_CNT_POLL_LL must be > BUT_CNT_POLL_L, or = 0
#endif

/* Private types -------------------------------------------------------------*/
/**
  * @brief  Определение структуры кнопки
  */
struct button_t {
    uint32_t state; /*!< Опросы кнопки (накопление со сдвигом) */
    uint32_t hold; /*!< Удержание кнопки (количество опросов подряд ) */

    void (*Pressed_short)(void); /*!< Функция обратного вызова по короткому нажатию */
    void (*Pressed_long)(void); /*!< Функция обратного вызова по длительному нажатию */
    void (*Pressed_longlong)(void); /*!< Функция обратного вызова по очень долгому нажатию */
    void (*Pressed_repeat)(void); /*!< Функция обратного вызова по повтору (удержание кнопки) */
    void (*Released_short)(void); /*!< Функция обратного вызова по отпусканию после короткого нажатия */
    void (*Released_long)(void); /*!< Функция обратного вызова по отпусканию после долгого нажатия */
    void (*Released_longlong)(void); /*!< Функция обратного вызова по отпусканию после очень долгого нажатия */
};

/* Private variables ---------------------------------------------------------*/
struct button_t Button[BUTTON_END]; /*!< Кнопки */

/* Private macros ------------------------------------------------------------*/
#define BTN_PRESED (~(-1L << BUT_ANTI_BOUNCE_CNT)) /*!< Значащие биты при опросе кнопок */

/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Инициализация кнопок
 * @retval None
 */
void Button_Init(void)
{
    for (int i = 0; i < BUTTON_END; ++i)
    {
        ButtonLowLevel_Init(i);
        Button_ResetCallback(i);
        Button[i].state = 0;
        Button[i].hold = 0;
    }
}

/**
 * @brief  Рабочая функция кнопок
 * @retval None
 */
void Button_Work(void)
{
#if (BUT_POLL_PERIOD > 0)
    static uint32_t time_polling = 0;
    if(HAL_GetTick() - time_polling < BUT_POLL_PERIOD) return;
    time_polling = HAL_GetTick();
#endif

    for (int i = 0; i < BUTTON_END; ++i)
    {
        //накопление опроса кнопки для последующего програмного антидребезга
        Button[i].state = (Button[i].state << 1) | ButtonLowLevel_GetState(i);

        //состояние кнопки с учетом антидребезга
        if ((Button[i].state & BTN_PRESED) == BTN_PRESED)
        {
            if (Button[i].hold < 0xFFFF) // защита от переполнения
                Button[i].hold++;       // подсчет длительности нажатия
            switch (Button[i].hold) {
#if (BUT_CNT_POLL_LL > 0)
            case BUT_CNT_POLL_LL:
                if (Button[i].Pressed_longlong != NULL)
                {
                    Button[i].Pressed_longlong();
                }
                break;
#endif
#if (BUT_CNT_POLL_L > 0)
            case BUT_CNT_POLL_L:
                if (Button[i].Pressed_long != NULL)
                {
                    Button[i].Pressed_long();
                }
                break;
#endif
            case BUT_CNT_POLL:
                if (Button[i].Pressed_short != NULL)
                {
                    Button[i].Pressed_short();
                }
                break;
            default:
#if (BUT_REPEAT_CNT > 0)
                if (Button[i].hold > BUT_REPEAT_CNT)
                {
                    if ((Button[i].hold - BUT_REPEAT_CNT) % BUT_REPEAT_DELAY)
                    {
                        if (Button[i].Pressed_repeat != NULL)
                        {
                            Button[i].Pressed_repeat();
                        }
                    }
                }
#endif
                break;
            }

        } else
        {
#if (BUT_CNT_POLL_LL > 0)
            if (Button[i].hold >= BUT_CNT_POLL_LL)
            {
                if (Button[i].Released_longlong != NULL)
                {
                    Button[i].Released_longlong();
                }
            } else
#endif
#if (BUT_CNT_POLL_L > 0)
            if (Button[i].hold >= BUT_CNT_POLL_L)
            {
                if (Button[i].Released_long != NULL)
                {
                    Button[i].Released_long();
                }
            } else
#endif
            if (Button[i].hold >= BUT_CNT_POLL)
            {
                if (Button[i].Released_short != NULL)
                {
                    Button[i].Released_short();
                }
            }
            Button[i].hold = 0;
        }
    }
}

/**
 * @brief  Регистрация пользовательской функции обратного вызова
 * @param  Name: имя кнопки из перечисления enum Button_Name
 * @param  CallbackID: идентификатор функции обратного вызова
 *           @arg @ref BUTTON_PRESSED_SHORT_CB_ID Идентификатор функции обратного вызова по короткому нажатию
 *           @arg @ref BUTTON_PRESSED_LONG_CB_ID Идентификатор функции обратного вызова по длительному нажатию
 *           @arg @ref BUTTON_PRESSED_LONG_LONG_CB_ID Идентификатор функции обратного вызова по очень долгому нажатию
 *           @arg @ref BUTTON_PRESSED_REPEAT_CB_ID Идентификатор функции обратного вызова по повтору (удержание кнопки)
 *           @arg @ref BUTTON_RELEASED_SHORT_CB_ID Идентификатор функции обратного вызова по отпусканию после короткого нажатия
 *           @arg @ref BUTTON_RELEASED_LONG_CB_ID Идентификатор функции обратного вызова по отпусканию после долгого нажатия
 *           @arg @ref BUTTON_RELEASED_LONG_LONG_CB_ID Идентификатор функции обратного вызова по отпусканию после очень долгого нажатия
 * @param pCallback указатель на функцию обратного вызова
 * @retval status: 0 - успешно
 *                !0 - ошибка
 */
uint_fast8_t Button_RegisterCallback(enum Button_Name Name,
                                     enum Button_CallbackIDTypeDef CallbackID,
                                     pButton_CallbackTypeDef pCallback)
{
    uint_fast8_t status = 0;

    if (pCallback == NULL)
    {
        return 1;
    }
    struct button_t *hbut = &Button[Name];
    switch (CallbackID) {
    case BUTTON_PRESSED_SHORT_CB_ID:
        hbut->Pressed_short = pCallback;
        break;

    case BUTTON_PRESSED_LONG_CB_ID:
        hbut->Pressed_long = pCallback;
        break;

    case BUTTON_PRESSED_LONG_LONG_CB_ID:
        hbut->Pressed_longlong = pCallback;
        break;

    case BUTTON_PRESSED_REPEAT_CB_ID:
        hbut->Pressed_repeat = pCallback;
        break;

    case BUTTON_RELEASED_SHORT_CB_ID:
        hbut->Released_short = pCallback;
        break;

    case BUTTON_RELEASED_LONG_CB_ID:
        hbut->Released_long = pCallback;
        break;

    case BUTTON_RELEASED_LONG_LONG_CB_ID:
        hbut->Released_longlong = pCallback;
        break;

    default:
        /* Return error status */
        status = 1;
        break;
    }

    return status;
}

/**
 * @brief  Сброс регистрации пользовательской функции обратного вызова
 * @param  Name: имя кнопки из перечисления enum Button_Name
 * @param  CallbackID: идентификатор функции обратного вызова
 *           @arg @ref BUTTON_PRESSED_SHORT_CB_ID Идентификатор функции обратного вызова по короткому нажатию
 *           @arg @ref BUTTON_PRESSED_LONG_CB_ID Идентификатор функции обратного вызова по длительному нажатию
 *           @arg @ref BUTTON_PRESSED_LONG_LONG_CB_ID Идентификатор функции обратного вызова по очень долгому нажатию
 *           @arg @ref BUTTON_PRESSED_REPEAT_CB_ID Идентификатор функции обратного вызова по повтору (удержание кнопки)
 *           @arg @ref BUTTON_RELEASED_SHORT_CB_ID Идентификатор функции обратного вызова по отпусканию после короткого нажатия
 *           @arg @ref BUTTON_RELEASED_LONG_CB_ID Идентификатор функции обратного вызова по отпусканию после долгого нажатия
 *           @arg @ref BUTTON_RELEASED_LONG_LONG_CB_ID Идентификатор функции обратного вызова по отпусканию после очень долгого нажатия
 * @retval status: 0 - успешно
 *                !0 - ошибка
 */
uint_fast8_t Button_UnRegisterCallback(enum Button_Name Name, enum Button_CallbackIDTypeDef CallbackID)
{
    uint_fast8_t status = 0;
    struct button_t *hbut = &Button[Name];
    switch (CallbackID) {
    case BUTTON_PRESSED_SHORT_CB_ID:
        hbut->Pressed_short = NULL;
        break;

    case BUTTON_PRESSED_LONG_CB_ID:
        hbut->Pressed_long = NULL;
        break;

    case BUTTON_PRESSED_LONG_LONG_CB_ID:
        hbut->Pressed_longlong = NULL;
        break;

    case BUTTON_PRESSED_REPEAT_CB_ID:
        hbut->Pressed_repeat = NULL;
        break;

    case BUTTON_RELEASED_SHORT_CB_ID:
        hbut->Released_short = NULL;
        break;

    case BUTTON_RELEASED_LONG_CB_ID:
        hbut->Released_long = NULL;
        break;

    case BUTTON_RELEASED_LONG_LONG_CB_ID:
        hbut->Released_longlong = NULL;
        break;

    default:
        /* Return error status */
        status = 1;
        break;
    }

    return status;
}

/**
 * @brief  Сброс всех зарегистрированных функций обратного вызова
 * @param  Name: имя кнопки из перечисления enum Button_Name
 * @retval None
 */
void Button_ResetCallback(enum Button_Name Name)
{
    struct button_t *hbut = &Button[Name];
    hbut->Pressed_short = NULL;
    hbut->Pressed_long = NULL;
    hbut->Pressed_longlong = NULL;
    hbut->Pressed_repeat = NULL;
    hbut->Released_short = NULL;
    hbut->Released_long = NULL;
    hbut->Released_longlong = NULL;
}

/*********************************END OF FILE**********************************/
