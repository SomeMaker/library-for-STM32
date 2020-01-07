# Драйвер символьного LCD дисплея
тестировался на дисплеях WH0802, WH2004.  

в файле lcd_HD44780.h настройте пины управления LCD_RS_PIN, LCD_E_PIN и шину данных LCD_DB0_PIN...LCD_DB7_PIN (для 4-х битиной схемы подключения настраиваются только LCD_DB4_PIN...LCD_DB7_PIN) и раскомментируйте дефайн разрядности шины LCD_DATA_BUS_8_BIT или LCD_DATA_BUS_4_BIT

для работы драйвера необходима утилита микросекундной задержки https://github.com/SomeMaker/library-for-STM32/tree/master/utils/delay_us 

LCD_Init(); - настройка ножек микроконтроллера и инициализация дисплея

LCD_Command(LCD_CURSOR_ON); - отправка управляющей команды, список команд в файле lcd_HD44780.h (команды расположенные между комментариями могут быть отправлены одновременно, например: LCD_DISPLAY_ON | LCD_CURSOR_ON | LCD_CURSOR_BLINK_OFF)
LCD_Write('0'); - отправка данных в DDRAM или CGRAM, по умолчанию отправляются в DDRAM и отображаются на экране, выводит символ в текущей позиции
LCD_String("Hello!"); - печать строки. если передается массив символов то последний символ должен быть = 0, например используйте snprintf, он добавляет  к строке 0
LCD_SetCursor(uint8_t col, uint8_t row); - установка курсора в столбец col строку row

# example
```
#include "lcd_HD44780.h"

int main(void)
{
  ...
  DWT_Init();
  LCD_Init();
  
  LCD_String("Hello!");
  
  uint8_t i = 0;

  while (1)
  {
    HAL_Delay(1000);
    LCD_Command(LCD_CLEAR);
    HAL_Delay(2);
    LCD_SetCursor(0,0);
    LCD_Write(i++);
  }
}
```