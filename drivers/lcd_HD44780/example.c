
#include "lcd_HD44780.h"

int main(void)
{
  ...
  DWT_Init();
  LCD_Init();
  
  LCD_String("Hello!");
  LCD_SetCursor(0,1);
  LCD_String("World");
  uint8_t i = 0;

  while (1)
  {
    HAL_Delay(850);
    LCD_Command(LCD_CLEAR);
    HAL_Delay(2);
    LCD_SetCursor(0,0);
    LCD_String("charCode");
    LCD_SetCursor(1,1);
    LCD_Write(i);
    LCD_Write(' ');
    LCD_Write(' ');
    LCD_Write('0');
    LCD_Write('x');
    LCD_Write(((i >> 4) < 0x0A)?(0x30 + (i >> 4)):(0x41 - 0x0A + (i >> 4)));
    LCD_Write(((i & 0x0F) < 0x0A)?(0x30 + (i & 0x0F)):(0x41 - 0x0A + (i & 0x0F)));
    i++;
  }
}
