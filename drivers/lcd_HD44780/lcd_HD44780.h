/*
 * lcd_HD44780.h
 *
 *  Created on: 3 янв. 2020 г.
 *      Author: Alexey
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LCD_HD44780_H_
#define LCD_HD44780_H_

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported constants --------------------------------------------------------*/
//Register Select: H:Data Input L:Instruction Input
#define LCD_RS_PIN                  GPIO_PIN_0
#define LCD_RS_GPIO_PORT            GPIOA
#define LCD_RS_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()

//LCD_RW_PIN  H--Read L--Write
// не используется, всегда только записываем в дисплей, RW_PIN дисплея подключить к земле

//Enable Signal: H,H-L - сигнал синхронизации, данные записываюся при переходе с 1 на 0
#define LCD_E_PIN                   GPIO_PIN_1
#define LCD_E_GPIO_PORT             GPIOA
#define LCD_E_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()

//Data bus, пины шины данных должны принадлежать одному порту
#define LCD_DB0_PIN                  
#define LCD_DB1_PIN                  
#define LCD_DB2_PIN                  
#define LCD_DB3_PIN                  
#define LCD_DB4_PIN                  GPIO_PIN_3
#define LCD_DB5_PIN                  GPIO_PIN_5
#define LCD_DB6_PIN                  GPIO_PIN_4
#define LCD_DB7_PIN                  GPIO_PIN_8
#define LCD_DATA_GPIO_PORT           GPIOB
#define LCD_DATA_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()

//работа параллельной шины 8 или 4 бита
//#define LCD_DATA_BUS_8_BIT
#define LCD_DATA_BUS_4_BIT



//команды управления
#define LCD_CLEAR             0x01
#define LCD_RETURN_HOME       0x02
//Entry Mode Set направление сдвига курсора/дисплея
#define LCD_MOVE_RIGHT        0x06
#define LCD_MOVE_LEFT         0x04
#define LCD_SHIFT_DISPLAY     0x05
#define LCD_SHIFT_CURSOR      0x04
//Display ON/OFF Cursor ON/OFF Cursor Blink ON/OFF
#define LCD_DISPLAY_ON        0x0C
#define LCD_DISPLAY_OFF       0x08
#define LCD_CURSOR_ON         0x0A
#define LCD_CURSOR_OFF        0x08
#define LCD_CURSOR_BLINK_ON   0x09
#define LCD_CURSOR_BLINK_OFF  0x08
//Cursor or Display Shift
#define LCD_SHIFT_CURSOR_L    0x10
#define LCD_SHIFT_CURSOR_R    0x14
#define LCD_SHIFT_DISPLAY_L   0x18
#define LCD_SHIFT_DISPLAY_R   0x1C
//Function Set
#define LCD_BUS_8_BIT         0x30
#define LCD_BUS_4_BIT         0x20
#define LCD_1_LINE_5x8        0x20
#define LCD_1_LINE_5x11       0x24
#define LCD_2_LINE_5x8        0x28
//Set CGRAM Address
#define LCD_SET_CGRAM         0x40
//Set DDRAM Address
#define LCD_SET_DDRAM         0x80

/* Exported functions --------------------------------------------------------*/
void LCD_Init(void);

void LCD_Command(uint8_t cmd);
void LCD_Write(uint8_t data);
void LCD_String(const char *c);
void LCD_SetCursor(uint8_t col, uint8_t row);

#ifdef __cplusplus
}
#endif

#endif /* LCD_HD44780_H_ */

/*******************************  END OF FILE  ********************************/
