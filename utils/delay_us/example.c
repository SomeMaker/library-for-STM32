// пример использования микросекундной задержки 

#include "delay_us.h"

int main(void)
{
  DWT_Init(); // инициализация отладочного таймера 

  while (1)
  {
    Delay_us(4); //задержка 4 мкс
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
    Delay_us(16); //задержка 16 мкс
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
    Delay_us(48); //задержка 48 мкс
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
  }
}