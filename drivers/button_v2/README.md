# Драйвер опроса кнопок

в файле button_drv.h подключите файлы библиотеки для вашего микроконтроллера, например #include "stm32f1xx_hal.h" для МК серии STM32F1xx

настройте количетсво используемых кнопок и их привязку к пинам микроконтроллера
```
#define BUTTONn                             ((uint16_t)4)

/**
 * @brief Кнопка "Menu"
 */
#define BUTTON_1_PIN                   GPIO_PIN_0
#define BUTTON_1_GPIO_PORT             GPIOB
#define BUTTON_1_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUTTON_1_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOB_CLK_DISABLE()
#define BUTTON_1_ACTIVE_LEVEL          ((uint16_t)0)
```
при увеличении количетсва кнопок больше 4-х, внесите изменения в файл button_drv.c 

далее в файле button.h настройте режим работы опроса кнопок. и для удобства переименуйте имена ваших кнопок
```
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
```

# example
```
void LED_Set()
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, RESET);
}

void LED_ReSet()
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, SET);
}

static void ButtonTask(void * pvParameters)
{
    Button_Init();
    Button_RegisterCallback(BUTTON_MENU, BUTTON_PRESSED_SHORT_CB_ID, LED_Set);
    Button_RegisterCallback(BUTTON_SELECT, BUTTON_PRESSED_SHORT_CB_ID, LED_ReSet);
    for(;;){
        osDelay(10); // опрос кнопки каждые 10 мс
        Button_Work();
    }
}
```